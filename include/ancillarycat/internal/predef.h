#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#pragma region includes and macros
#include <limits.h>
#include <stdint.h> // intmax_t, ptrdiff_t, size_t
#include <stdio.h> // printf, fprintf
#include <signal.h> // raise, SIGTRAP

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

#ifndef VIVA_HAS_C17
#include <stdbool.h>
#if ! defined(__bool_true_false_are_defined)
#define bool _Bool
#define true 1
#define false 0
#endif
// else do nothing, because C23 standardize the bool type and related keywords.
#endif

#ifndef VIVA_HAS_C23
#include <iso646.h>
#endif

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

#pragma region variadic macros
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
// use this if `VIVA_VFUNC` was recognized as `implicit function declaration` rather than macro.
#define VIVA__VFUNC_IMPL(name, n) VIVA__VFUNC_IMPL_(name,_, n)
// ^^^^^^ just write the code below: vvvvvv
#define VIVA__VFUNC(func, ...) VIVA__VFUNC_IMPL(func, VIVA__NARG__(__VA_ARGS__)) (__VA_ARGS__)

#pragma endregion

#pragma region primitive types
#ifdef __SIZEOF_INT128__
// only in 64-bit machine
typedef __int128_t int128_t;
typedef __uint128_t uint128_t;
#endif

// create a variadic concat macro using VIVA__VFUNC
#define VIVA_CONCAT_IMPL(...) VIVA__VFUNC(VIVA_CONCAT_IMPL, __VA_ARGS__)
#define VIVA_CONCAT_IMPL_1(a) #a
#define VIVA_CONCAT_IMPL_2(a, b) a##b
#define VIVA_CONCAT_IMPL_3(a, b, c) a##b##c
#define VIVA_CONCAT_IMPL_4(a, b, c, d) a##b##c##d
#define VIVA_CONCAT_IMPL_5(a, b, c, d, e) a##b##c##d##e
#define VIVA_CONCAT_IMPL_6(a, b, c, d, e, f) a##b##c##d##e##f
#define VIVA_CONCAT_IMPL_7(a, b, c, d, e, f, g) a##b##c##d##e##f##g
#define VIVA_CONCAT(...) VIVA_CONCAT_IMPL(__VA_ARGS__)
#define VIVA_STRINGIFY_IMPL(...) #__VA_ARGS__
#define VIVA_STRINGIFY(...) VIVA_STRINGIFY_IMPL(__VA_ARGS__)

#define VIVA_ANY_DEFAULT_VALUE_() \
	.void_ptr_type = nullptr, \
	.char_ptr_type = nullptr, \
	.boolean_type = false, \
	.char_type = '\0', \
	.signed_char_type = 0, \
	.unsigned_char_type = 0, \
	.short_type = 0, \
	.signed_short_type = 0, \
	.unsigned_short_type = 0, \
	.int_type = 0, \
	.signed_int_type = 0, \
	.unsigned_int_type = 0, \
	.long_type = 0, \
	.signed_long_type = 0, \
	.unsigned_long_type = 0, \
	.long_long_type = 0, \
	.signed_long_long_type = 0, \
	.unsigned_long_long_type = 0, \
	.float_type = 0.0f, \
	.double_type = 0.0, \
	.long_double_type = 0.0L,

#define VIVA_ANY_DEFAULT_VALUE_128() \
	.int128_type = 0, \
	.uint128_type = 0,

// reduce the size of the union, for `Any` and `any` will only be used for type lookup.
#pragma pack(push, 1)
union {
	const void *void_ptr_type;
	const char *char_ptr_type;
	const bool boolean_type;
	const char char_type;
	const signed char signed_char_type;
	const unsigned char unsigned_char_type;
	const short short_type;
	const signed short signed_short_type;
	const unsigned short unsigned_short_type;
	const int int_type;
	const signed int signed_int_type;
	const unsigned int unsigned_int_type;
	const long long_type;
	const signed long signed_long_type;
	const unsigned long unsigned_long_type;
	const long long long_long_type;
	const signed long long signed_long_long_type;
	const unsigned long long unsigned_long_long_type;
	const float float_type;
	const double double_type;
	const long double long_double_type;
#ifdef __SIZEOF_INT128__
	const int128_t int128_type;
	const uint128_t uint128_type;
#endif
} static Any = {
			VIVA_ANY_DEFAULT_VALUE_()
#ifdef __SIZEOF_INT128__
			VIVA_ANY_DEFAULT_VALUE_128()
#endif
		},
		any = {
			VIVA_ANY_DEFAULT_VALUE_()
#ifdef __SIZEOF_INT128__
			VIVA_ANY_DEFAULT_VALUE_128()
#endif
		};
#pragma pack(pop)

enum VIVA_STATUS_ENUM : unsigned char {
	kOkStatus = 0,
	kEmpty,
	kBadAlloc,
	kOverflow,
	kUnderflow,
	kInvalidArgument,
	kOutOfRange,
	kNotSupported,
	kAccessDenied,
	kNotFound,
	kAlreadyExists,
	kUnknownError = UCHAR_MAX - 1,
	kNotImplemented = UCHAR_MAX
};
#pragma endregion
#ifdef __cplusplus
}
#endif
