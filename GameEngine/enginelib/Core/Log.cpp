// #include "ftspch.h"

#include "Log.h"

#include "Core/PlatformDetection.h"
#include "Utils/IncludeDef.h"
#include "Utils/IncludeGL.h"

#include <iostream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifdef WINDOWS
#include <DbgHelp.h>
#include <Windows.h>
#endif

namespace fts
{

    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<spdlog::logger> Log::s_ClientLogger;

    void Log::Init(const std::string& logFileName)
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFileName, true));

        logSinks[0]->set_pattern("%^[%T] [%^-%L-%$]%n : %v%$");
        logSinks[1]->set_pattern("[%T] [%^---%L---%$] %n: %v [%l]"); //

        s_CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);

#ifdef WINDOWS
        // Get the process handle
        HANDLE process = GetCurrentProcess();

        // Initialize the symbol handler for our process
        SymInitialize(process, NULL, TRUE);
        SymSetOptions(SYMOPT_LOAD_LINES);
#endif
    }

    std::string Log::DumpStackTrace()
    {
        std::stringstream ss;

        // Windows implementation taken from http://www.rioki.org/2017/01/09/windows_stacktrace.html

#ifdef WINDOWS

        // We will always assume x64, since that is our only supported project architecture
        DWORD machine = IMAGE_FILE_MACHINE_AMD64;

        // Get the process handle
        HANDLE process = GetCurrentProcess();
        // Get the current thread handle
        HANDLE thread = GetCurrentThread();
        // Prep our context info
        CONTEXT context      = {};
        context.ContextFlags = CONTEXT_FULL;
        // Capture our device context (registers and everything)
        RtlCaptureContext(&context);

        // Prepare our stack frame
        STACKFRAME frame       = {};
        frame.AddrPC.Offset    = context.Rip; // This is our instruction pointer in register RIP
        frame.AddrPC.Mode      = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rbp; // This is the address of our frame, stored in RBP
        frame.AddrFrame.Mode   = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp; // This is the address of the stack, stored in RSP
        frame.AddrStack.Mode   = AddrModeFlat;

        // Bypass this frame
        StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL);

        // Iterate over all frames in the stack
        while(StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
        {
            // The address of the function
            DWORD64 functionAddress;
            // Stores the output information
            std::string moduleName, functionName, file;
            // Stores the line
            unsigned int line = 0;

            // Extract the function address from the current frame
            functionAddress = frame.AddrPC.Offset;

            // Get the module that the address is part of
            DWORD moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);
            // Get the name of the module, store it in buffer
            char moduleBuff[MAX_PATH];
            if(moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduleBuff, MAX_PATH))
            {
                moduleName = moduleBuff;
            }

            // Prepare a buffer to hold the symbol and it's name
            char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
            PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
            symbol->SizeOfStruct    = (sizeof IMAGEHLP_SYMBOL) + 255;
            symbol->MaxNameLength   = 254; // one byte for null terminator

            // Get the symbol info for the function address, and extract the name
            if(SymGetSymFromAddr(process, functionAddress, NULL, symbol))
            {
                functionName = symbol->Name;
            }

            // Prepare a line info structure
            DWORD offset = 0;
            IMAGEHLP_LINE hLine;
            hLine.SizeOfStruct = sizeof(IMAGEHLP_LINE);

            // Get the file and line info for the function
            if(SymGetLineFromAddr(process, functionAddress, &offset, &hLine))
            {
                file = hLine.FileName;
                line = hLine.LineNumber;
            }

            // Append to the output line
            ss << "\t" << functionName << "@" << file << " line " << line << std::endl;
        }
#endif

        // Return the result
        return ss.str();
    }

    void Log::Uninitialize()
    {

#ifdef WINDOWS
        HANDLE process = GetCurrentProcess();
        SymCleanup(process);
#endif

        s_CoreLogger.reset();
        s_CoreLogger = nullptr;

        s_ClientLogger.reset();
        s_ClientLogger = nullptr;
        spdlog::shutdown();
    }

} // namespace fts
