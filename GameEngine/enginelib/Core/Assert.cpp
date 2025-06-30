
#include "Core/Log.h"

#include <Core/Assert.h>

namespace Assert
{
    void HandleAssert(const char* msg, const char* condition, const char* fname, const char* funct, long lineNumber)
    {
        spdlog::critical("Assert Failed: {}\n"
                         "Condition: {}\n"
                         "File: {}\n"
                         "Funtion: {}\n"
                         "Line: {}\n"
                         "Terminating program.\n",
                         msg, condition, fname, funct, lineNumber);
    }
} // namespace Assert
