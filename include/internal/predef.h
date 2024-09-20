#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#pragma region includes and macros
#include <stdint.h> // intmax_t, ptrdiff_t, size_t
#include <stdio.h> // printf, fprintf
#include <signal.h> // raise, SIGTRAP

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
// else do nothing, because C23 standardize the bool type and related keywords.
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
// seems GCC support these C23 features, but stdc version macro is still C17
// #define nullptr NULL
// #define constexpr const
// #define typeof __typeof__
// #define typeof_unqual __typeof_unqual__
#endif

#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG) || defined(__DEBUG__)
#define VIVA_DEBUG_ENABLED 1
#else
#define VIVA_DEBUG_ENABLED 0
#endif

/// @see https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
/// Example usage: @code
/// definition for FOO
/// #define FOO(...) VFUNC(FOO, __VA_ARGS__)
///
/// #define FOO2(x, y) ((x) + (y))
/// #define FOO3(x, y, z) ((x) + (y) + (z))
/// @endcode
#define VIVA__NARG__(...)  VIVA__NARG_I_(__VA_ARGS__,VIVA__RSEQ_N())
#define VIVA__NARG_I_(...) VIVA__ARG_N(__VA_ARGS__)
#define VIVA__ARG_N( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
	_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
	_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
	_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
	_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
	_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
	_61,_62,_63,N,...) N
#define VIVA__RSEQ_N() \
	63,62,61,60,                   \
	59,58,57,56,55,54,53,52,51,50, \
	49,48,47,46,45,44,43,42,41,40, \
	39,38,37,36,35,34,33,32,31,30, \
	29,28,27,26,25,24,23,22,21,20, \
	19,18,17,16,15,14,13,12,11,10, \
	9,8,7,6,5,4,3,2,1,0

// general definition for any function name
#define VIVA__VFUNC_IMPL_(name,underscore,n) name##underscore##n
#define VIVA__VFUNC_IMPL(name, n) VIVA__VFUNC_IMPL_(name,_, n)
#define VIVA__VFUNC(func, ...) VIVA__VFUNC_IMPL(func, VIVA__NARG__(__VA_ARGS__)) (__VA_ARGS__)

#pragma endregion
#ifdef __SIZEOF_INT128__
	// only in 64-bit machine
	typedef __int128_t int128_t;
	typedef __uint128_t uint128_t;
#endif
#define VIVA_TO_STRING_IMPL(x) #x
#define VIVA_TO_STRING(x) VIVA_TO_STRING_IMPL(x)
#define VIVA_CONCAT_IMPL(x, y) x##y
#define VIVA_CONCAT(x, y) VIVA_CONCAT_IMPL(x, y)
union VIVA_ANY {
	void* void_ptr_type;
	char* char_ptr_type;
	bool boolean_type;
	char char_type;
	signed char signed_char_type;
	unsigned char unsigned_char_type;
	short short_type;
	signed short signed_short_type;
	unsigned short unsigned_short_type;
	int int_type;
	signed int signed_int_type;
	unsigned int unsigned_int_type;
	long long_type;
	signed long signed_long_type;
	unsigned long unsigned_long_type;
	long long long_long_type;
	signed long long signed_long_long_type;
	unsigned long long unsigned_long_long_type;
	float float_type;
	double double_type;
	long double long_double_type;
#ifdef __SIZEOF_INT128__
	int128_t int128_type;
	uint128_t uint128_type;
#endif
}Any,any;
#ifdef __cplusplus
}
#endif
