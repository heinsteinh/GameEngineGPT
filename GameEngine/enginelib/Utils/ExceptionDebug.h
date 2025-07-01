#ifndef EXCEPTIONDEBUG_H
#define EXCEPTIONDEBUG_H

#include <Config.h>
#include <Core/Base.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Debug
{
    enum TextColor
    {
        White = 0,
        Red   = 31,
        Blue  = 36
    };

    template <typename... Args>
    std::string makeString(Args&&... args)
    {
        std::ostringstream stream;
        std::vector<int> tmp{0, ((void)(stream << args << " "), 0)...};

        return stream.str();
    }

    template <typename... Args>
    void print(Args&&... args)
    {
        std::cerr << makeString(std::forward<Args>(args)...) << std::endl;
    }
} // namespace Debug

#define FTS_DEBUG

#define _FILE_ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 \
                                                                                              : __FILE__)

#ifdef DEBUG_ENABLED
#define DEBUG(...)                                      \
    {                                                   \
        std::cerr << _FILE_ << ":" << __LINE__ << ": "; \
        Debug::print(__VA_ARGS__);                      \
        std::cerr.flush();                              \
    }
#else
#define DEBUG(...) \
    {              \
    }
#endif

namespace System
{
    class Exception : public std::exception
    {
    public:
        template <typename... Args>
        Exception(const uint16_t line, const std::string& filename, Args&&... args) noexcept
        {
            m_errorMsg = "at " + filename + ":" + std::to_string(line) + ": ";
            m_errorMsg += Debug::makeString(std::forward<Args>(args)...);
        }

        virtual const char* what() const noexcept
        {
            return m_errorMsg.c_str();
        }

    private:
        std::string m_errorMsg;
    };
} // namespace System

#define EXCEPTION(...) (System::Exception(__LINE__, _FILE_, __VA_ARGS__))

#endif // EXCEPTIONDEBUG_H
