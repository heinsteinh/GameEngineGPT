#pragma once

#include <chrono>

#define FTS_TIME_SCOPE const ::fts::ScopedTimer _ftsScopedTimer(FTS_FUNCSIG ":" FTS_XSTR(__LINE__));

namespace fts
{

    class Timer
    {
    public:
        Timer()
        {
            Reset();
        }

        inline void Reset()
        {
            m_Start = std::chrono::high_resolution_clock::now();
        }

        inline float Elapsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
        }

        inline float ElapsedMillis()
        {
            return Elapsed() * 1000.0f;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };

    /// A basic logging utility to time a certain scope.
    class ScopedTimer
    {
    public:
        ScopedTimer(std::string_view label)
            : m_label(label)
        {
        }

        ~ScopedTimer()
        {
            const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

            FTS_CORE_INFO("{}: {} ms", m_label, duration.count());
        }

    private:
        std::string m_label;

        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    };

} // namespace fts
