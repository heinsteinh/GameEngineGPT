
#ifndef CORE_LOG_H
#define CORE_LOG_H

#include <Core/Base.h>
#include <memory>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/string_cast.hpp>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

/////////////////////////////////////////////////////////////////////////////////////////
bool GLLogCall(const char* functionName, const char* fileName, int line);

/////////////////////////////////////////////////////////////////////////////////////////
void GLClearError();

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    FTS_ASSERT(GLLogCall(#x, __FILE__, __LINE__))

namespace fts
{

    class Log
    {
    public:
        static void Init(const std::string& logFileName = "EngineDebug.log");

        static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }
        static std::shared_ptr<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }

        static std::string DumpStackTrace();
        static void Uninitialize();

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

} // namespace fts

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
    return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
    return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
    return os << glm::to_string(quaternion);
}

// Core log macros
#define FTS_CORE_TRACE(...) ::fts::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FTS_CORE_INFO(...)  ::fts::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FTS_CORE_WARN(...)  ::fts::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FTS_CORE_ERROR(...)                                                                   \
    {                                                                                         \
        ::fts::Log::GetCoreLogger()->error(__VA_ARGS__);                                      \
        ::fts::Log::GetClientLogger()->error("Location: \n{}", ::fts::Log::DumpStackTrace()); \
    }
#define FTS_CORE_CRITICAL(...)                                                                   \
    {                                                                                            \
        ::fts::Log::GetCoreLogger()->critical(__VA_ARGS__);                                      \
        ::fts::Log::GetClientLogger()->critical("Location: \n{}", ::fts::Log::DumpStackTrace()); \
    }

// Client log macros
#define LOG_TRACE(...) ::fts::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)  ::fts::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)  ::fts::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)                                                                        \
    {                                                                                         \
        ::fts::Log::GetClientLogger()->error(__VA_ARGS__);                                    \
        ::fts::Log::GetClientLogger()->error("Location: \n{}", ::fts::Log::DumpStackTrace()); \
    }
#define LOG_CRITICAL(...)                                                                        \
    {                                                                                            \
        ::fts::Log::GetClientLogger()->critical(__VA_ARGS__);                                    \
        ::fts::Log::GetClientLogger()->critical("Location: \n{}", ::fts::Log::DumpStackTrace()); \
    }

#endif
