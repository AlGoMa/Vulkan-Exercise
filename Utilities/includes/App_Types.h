#pragma once

#ifdef _MQX
/* HACK : there is a conflict with string.h */
char * _strdup(const char *str);
#else
#include <string.h>
#endif

#if defined(__linux) || defined(_DIKERNEL) || defined(__QNX__) || defined(_MQX) || defined(INTEGRITY_POSIX_GH) || defined(INTEGRITY_WITHOUT_POSIX_GH)
#include <ctype.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  else
#    define DO_NOT_UNDEF_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  else
#    define DO_NOT_UNDEF_NOMINMAX
#  endif
#  include <windows.h>
#  ifndef DO_NOT_UNDEF_NOMINMAX
#    undef NOMINMAX
#  else
#    undef DO_NOT_UNDEF_NOMINMAX
#  endif
#  ifndef DO_NOT_UNDEF_LEAN_AND_MEAN
#    undef WIN32_LEAN_AND_MEAN
#  else
#    undef DO_NOT_UNDEF_LEAN_AND_MEAN
#  endif
#endif

#if defined(HAVE_STDINT_H)
typedef int8_t   t_IntS8;
typedef int16_t  t_IntS16;
typedef int32_t  t_IntS32;
typedef int64_t  t_IntS64;
typedef uint8_t  t_IntU8;
typedef uint16_t t_IntU16;
typedef uint32_t t_IntU32;
typedef uint64_t t_IntU64;
#elif __linux
/*
** we need to explicitely define if the type is signed or not: on ARM, a char is "unsigned" by default,
** at the opposite of x86 where a char is signed
*/
#ifndef _LP64
/* 32 bits */
typedef signed char        t_IntS8;
typedef signed short       t_IntS16;
typedef signed int         t_IntS32;
typedef signed long long   t_IntS64;
typedef unsigned char      t_IntU8;
typedef unsigned short     t_IntU16;
typedef unsigned int       t_IntU32;
typedef unsigned long long t_IntU64;
#else
/* 64 bits */
typedef signed char        t_IntS8;
typedef signed short       t_IntS16;
typedef signed int         t_IntS32;
typedef signed long        t_IntS64;
typedef unsigned char      t_IntU8;
typedef unsigned short     t_IntU16;
typedef unsigned int       t_IntU32;
typedef unsigned long      t_IntU64;
#endif
#elif _WIN32
typedef char               t_IntS8;
typedef short              t_IntS16;
typedef int                t_IntS32;
typedef long long          t_IntS64;
typedef unsigned char      t_IntU8;
typedef unsigned short     t_IntU16;
typedef unsigned int       t_IntU32;
typedef unsigned long long t_IntU64; 
#elif _WIN64
typedef char               t_IntS8;
typedef short              t_IntS16;
typedef int                t_IntS32;
typedef long long          t_IntS64;
typedef unsigned char      t_IntU8;
typedef unsigned short     t_IntU16;
typedef unsigned int       t_IntU32;
typedef unsigned long long t_IntU64;
#endif