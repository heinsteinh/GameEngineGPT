#pragma once
#include <cstdlib>

// #include "Config.h"

//
// CURRENT FUNCTION NAME
//
#if defined(__GNUC__) || defined(__clang__)
#define __function__ __PRETTY_FUNCTION__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define __function__ __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define __function__ __func__
#elif defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)
#define __function__ __FUNCTION__
#else

#if defined _MSC_VER
#define __function__ __FUNCSIG__
#else
#define __function__ __func__
#endif
#endif

namespace Assert
{
    void HandleAssert(const char* message, const char* condition, const char* fileName, const char* funct, long lineNumber);
}

#ifdef FTS_DEBUG

#define FTS_ASSERT_MSG(x, msg)                                               \
    do                                                                       \
    {                                                                        \
        if(!(x))                                                             \
        {                                                                    \
            Assert::HandleAssert(msg, #x, __FILE__, __function__, __LINE__); \
            std::abort();                                                    \
        }                                                                    \
    } while(0)

#define FTS_ASSERT(x)                                                                        \
    do                                                                                       \
    {                                                                                        \
        if(!(x))                                                                             \
        {                                                                                    \
            Assert::HandleAssert("Assertion Failed!", #x, __FILE__, __function__, __LINE__); \
            std::abort();                                                                    \
        }                                                                                    \
    } while(0)

#define CHECK_NO_ENTRY() FTS_ASSERT_MSG(false, "You should not be here!");

#else

#define FTS_ASSERT_MSG(x, msg)              \
    do                                      \
    {                                       \
        (void)sizeof(x), (void)sizeof(msg); \
    } while(0)
#define FTS_ASSERT(x)    \
    do                   \
    {                    \
        (void)sizeof(x); \
    } while(0) // noop
#define CHECK_NO_ENTRY()

#endif

#ifndef _DEBUG
#define UNREACHABLE G_ASSERT(0)
#else
#ifdef _MSC_VER
#define UNREACHABLE __assume(0)
#else // GCC, Clang
#define UNREACHABLE __builtin_unreachable()
#endif
#endif
