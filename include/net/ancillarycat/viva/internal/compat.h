#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#pragma region includes and macros
#include <limits.h>
#include <stdint.h> // intmax_t, ptrdiff_t, size_t
#include <stdio.h> // printf, fprintf
#include <signal.h> // raise, SIGTRAP
#include <stdbool.h> // bool, true, false

#if __STDC_VERSION__ < 201112L
#error "This library is required at least C11."
#elif __STDC_VERSION__ < 201710L
#define VIVA_HAS_C11
// currently GCC 14 defines __STDC_VERSION__ as 202000L (some C23 support still not implemented)
#elif __STDC_VERSION__ < 202000L
#define VIVA_HAS_C11
#define VIVA_HAS_C17
#else
#define VIVA_HAS_C11
#define VIVA_HAS_C17
#define VIVA_HAS_C23
#endif

#ifndef VIVA_HAS_C23
#include <iso646.h>
#endif

#pragma push_macro("var")
#undef var
#pragma push_macro("val")
#undef val
#ifdef  VIVA_HAS_C23
// C23 changed the usage of `auto` keyword, make it more like C++'s.
#define var auto
#define val const auto
#else
#if defined(__GNUC__) || defined(__clang__)
// GNU and Clang support `__auto_type` as an extension before C23.
#define var __auto_type
#define val const __auto_type
#else
#error "currently only support GCC and Clang. MSVC doesn't support `__auto_type`, so most code here will not work."
#endif
#endif
#if ! defined (__cplusplus) && ! defined (VIVA_HAS_C23)
// C23 standardize the those keywords.
#define thread_local _Thread_local
#define static_assert _Static_assert
#define alignas _Alignas
#define alignof _Alignof
#define nothrow _Nothrow
#define nullptr NULL
#define constexpr const
#define typeof __typeof__
#define typeof_unqual __typeof_unqual__
#endif
#ifndef VIVA_DEBUG_ENABLED
#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG) || defined(__DEBUG__)
#define VIVA_DEBUG_ENABLED 1
#else
#define VIVA_DEBUG_ENABLED 0
#endif
#endif
#if defined(__i386__) || defined(__x86_64__)
#define VIVA_DEBUG_SIGTRAP() \
  __asm__ __volatile__("int {$}3":)
#elif defined(__arm__)
#define VIVA_DEBUG_SIGTRAP \
  __asm__ __volatile__("udf #0xfe")
#elif defined(__aarch64__)
#define VIVA_DEBUG_SIGTRAP \
  __asm__ __volatile__("brk #0xf000")
#else
#define VIVA_DEBUG_SIGTRAP \
  __asm__ __volatile__("unimplemented")
#endif
#pragma endregion
#ifdef __cplusplus
}
#endif