#pragma once

#define BIT(x) (1 << x)

#define HBIND_EVENT(fn)   std::bind(&fn, this, std::placeholders::_1)
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { \
    return this->fn(std::forward<decltype(args)>(args)...);          \
}

#if defined(_MSC_VER)
#define FTS_FUNCSIG __FUNCSIG__
#else
#define FTS_FUNCSIG __func__
#endif

#ifdef UNICODE
typedef wchar_t FTS_CHAR;
#define FTS_TOCHAR(x) L##x
#else
typedef char FTS_CHAR;
#define FTS_TOCHAR(x) x
#endif

#define FTS_XSTR(x) FTS_STR(x)
#define FTS_STR(x)  #x

#define FTS_NOT_IMPLEMENTED throw std::logic_error{FTS_FUNCSIG + std::string(" is not implemented")};

#ifndef FTS_THROW_EXCEPTION
#define FTS_THROW_EXCEPTION(Message) throw std::logic_error{FTS_FUNCSIG + std::string("Exception happen => ") + std::string(FTS_TOCHAR(Message))};
#endif

#ifndef _FTS_THROW_EXCEPTION
#define _FTS_THROW_EXCEPTION(ex)         throw ex(FTS_FUNCSIG, __FILE__, __LINE__)
#define _FTS_THROW_EXCEPTION_EX(ex, ...) throw ex(__VA_ARGS__, FTS_FUNCSIG, __FILE__, __LINE__)
#endif
