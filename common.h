#pragma once


#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

#if __STDC_VERSION__ < 201112L
#error "This library is required at least C11."
#elif __STDC_VERSION__ < 201710L
#define VIVA_HAS_C11
#elif __STDC_VERSION__ < 202300L
#define VIVA_HAS_C11
#define VIVA_HAS_C17
#else
#define VIVA_HAS_C11
#define VIVA_HAS_C17
#define VIVA_HAS_C23
#endif

#ifdef VIVA_HAS_C17
#include <stdbool.h>
#endif

#if ! defined(__bool_true_false_are_defined)
#define bool _Bool
#define true 1
#define false 0
#endif

#ifndef VIVA_HAS_C23
#include <iso646.h>
#endif

#ifdef  VIVA_HAS_C23
#define var auto
#define val const auto
#else
// #ifdef defined(__GNUC__) || defined(__clang__)
#define var __auto_type
#define val const __auto_type
// #else
// #error "currently only support GCC and Clang"
// #endif
#endif
#define atomic _Atomic
#define noreturn _Noreturn
#ifndef complex
#define complex _Complex
#endif
#define imaginary _Imaginary
#define generic(_T, ...) _Generic((_T), __VA_ARGS__)
#if ! defined (__cplusplus) && ! defined (VIVA_HAS_C23)
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

#ifdef __cplusplus
}
#endif
