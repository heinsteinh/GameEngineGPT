/*
 * Copyright (c) 2012-2022 Daniele Bartolini et al.
 * License: https://github.com/crownengine/crown/blob/master/LICENSE
 */

// Adapted from Branimir Karadžić's platform.h (https://github.com/bkaradzic/bx)

#ifndef PLATFORMDETECTION_H
#define PLATFORMDETECTION_H

#define FTS_COMPILER_CLANG 0
#define FTS_COMPILER_GCC   0
#define FTS_COMPILER_MSVC  0

#define FTS_PLATFORM_ANDROID 0
#define FTS_PLATFORM_IOS     0
#define FTS_PLATFORM_LINUX   0
#define FTS_PLATFORM_OSX     0
#define FTS_PLATFORM_WINDOWS 0

#define FTS_CPU_ARM  0
#define FTS_CPU_JIT  0
#define FTS_CPU_MIPS 0
#define FTS_CPU_PPC  0
#define FTS_CPU_X86  0

#define FTS_ARCH_32BIT 0
#define FTS_ARCH_64BIT 0

#define FTS_CPU_ENDIAN_BIG    0
#define FTS_CPU_ENDIAN_LITTLE 0

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(_MSC_VER)
#undef FTS_COMPILER_MSVC
#define FTS_COMPILER_MSVC 1
#elif defined(__clang__) // clang defines __GNUC__
#undef FTS_COMPILER_CLANG
#define FTS_COMPILER_CLANG 1
#elif defined(__GNUC__)
#undef FTS_COMPILER_GCC
#define FTS_COMPILER_GCC 1
#else
#error "FTS_COMPILER_* is not defined!"
#endif

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_WIN32) || defined(_WIN64)
#undef FTS_PLATFORM_WINDOWS
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#if !defined(WINVER) && !defined(_WIN32_WINNT)
// Windows 7 and above
#define WINVER       0x0601
#define _WIN32_WINNT 0x0601
#endif
#define FTS_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#undef FTS_PLATFORM_ANDROID
#define FTS_PLATFORM_ANDROID 1
#elif defined(__linux__)
#undef FTS_PLATFORM_LINUX
#define FTS_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#undef FTS_PLATFORM_IOS
#define FTS_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#undef FTS_PLATFORM_OSX
#define FTS_PLATFORM_OSX 1
#else
#error "FTS_PLATFORM_* is not defined!"
#endif // if defined(_WIN32) || defined(_WIN64)

#define FTS_PLATFORM_POSIX (FTS_PLATFORM_ANDROID  \
                            || FTS_PLATFORM_IOS   \
                            || FTS_PLATFORM_LINUX \
                            || FTS_PLATFORM_OSX)

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__)
#undef FTS_CPU_ARM
#define FTS_CPU_ARM         1
#define FTS_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__) || defined(__mips_isa_rev) // defined(mips)
#undef FTS_CPU_MIPS
#define FTS_CPU_MIPS        1
#define FTS_CACHE_LINE_SIZE 64
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
#undef FTS_CPU_PPC
#define FTS_CPU_PPC         1
#define FTS_CACHE_LINE_SIZE 128
#elif defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#undef FTS_CPU_X86
#define FTS_CPU_X86         1
#define FTS_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
#undef FTS_CPU_JIT
#define FTS_CPU_JIT         1
#define FTS_CACHE_LINE_SIZE 64
#endif // if defined(__arm__)

#if defined(__x86_64__) || defined(__arm64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
#undef FTS_ARCH_64BIT
#define FTS_ARCH_64BIT 64
#else
#undef FTS_ARCH_32BIT
#define FTS_ARCH_32BIT 32
#endif

#if FTS_CPU_PPC
#undef FTS_CPU_ENDIAN_BIG
#define FTS_CPU_ENDIAN_BIG 1
#else
#undef FTS_CPU_ENDIAN_LITTLE
#define FTS_CPU_ENDIAN_LITTLE 1
#endif

#if FTS_COMPILER_GCC
#define FTS_COMPILER_NAME "GCC"
#elif FTS_COMPILER_MSVC
#define FTS_COMPILER_NAME "MSVC"
#endif

#if FTS_PLATFORM_ANDROID
#define FTS_PLATFORM_NAME "android"
#elif FTS_PLATFORM_IOS
#define FTS_PLATFORM_NAME "ios"
#elif FTS_PLATFORM_LINUX
#define FTS_PLATFORM_NAME "linux"
#elif FTS_PLATFORM_OSX
#define FTS_PLATFORM_NAME "osx"
#elif FTS_PLATFORM_WINDOWS
#define FTS_PLATFORM_NAME "windows"
#endif

#if FTS_CPU_ARM
#define FTS_CPU_NAME "ARM"
#elif FTS_CPU_MIPS
#define FTS_CPU_NAME "MIPS"
#elif FTS_CPU_PPC
#define FTS_CPU_NAME "PowerPC"
#elif FTS_CPU_JIT
#define FTS_CPU_NAME "JIT-VM"
#elif FTS_CPU_X86
#define FTS_CPU_NAME "x86"
#endif

#if FTS_ARCH_32BIT
#define FTS_ARCH_NAME "32-bit"
#elif FTS_ARCH_64BIT
#define FTS_ARCH_NAME "64-bit"
#endif

#endif
