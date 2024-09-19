//////////////////////////////////////////////////////////////////////////
/// @file viva.h
/// @brief A header file for C11, C17, and C23, providing
///					some useful macros and functions.
/// @author LiAuTrever
/// @copyright MIT 3.0
//////////////////////////////////////////////////////////////////////////
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
#pragma endregion

#pragma region reflexpr
// default triggers an error.
#define VIVA_REFLEXPR(x) generic((x), \
	void *: "void *", \
	bool: "bool", \
	bool*: "bool *", \
	char: "char", \
	char *: "char *", \
	signed char: "signed char", \
	unsigned char: "unsigned char", \
	short int: "short int", \
	unsigned short int: "unsigned short int", \
	int: "int", \
	int *: "int *", \
	unsigned int: "unsigned int", \
	unsigned int *: "unsigned int *", \
	long int: "long int", \
	long int *: "long int *", \
	unsigned long int: "unsigned long int", \
	unsigned long int *: "unsigned long int *", \
	long long int: "long long int", \
	long long int *: "long long int *", \
	unsigned long long int: "unsigned long long int", \
	unsigned long long int *: "unsigned long long int *", \
	float: "float", \
	float *: "float *", \
	double: "double", \
	double *: "double *", \
	long double: "long double", \
	long double *: "long double *", \
	default: "Unknown type. This macro only supports primitive types." \
)
#pragma endregion
/**
 *  @see http://www.robertgamble.net/2012/01/c11-generic-selections.html
 */
#pragma region print functions
#define VIVA_PRINTF_DEC_FORMAT_IMPL(x) generic((x), \
	void *: "%p", \
	bool: "%d", \
	char: "%c", \
	signed char: "%hhd", \
	unsigned char: "%hhu", \
	char *: "%s", \
	signed short: "%hd", \
	unsigned short: "%hu", \
	signed int: "%d", \
	unsigned int: "%u", \
	long int: "%ld", \
	unsigned long int: "%lu", \
	long long int: "%lld", \
	unsigned long long int: "%llu", \
	float: "%f", \
	double: "%f", \
	long double: "%Lf", \
	default: "Unknown type. This macro only supports primitive types." \
)
#define VIVA_PRINT(x) printf(VIVA_PRINTF_DEC_FORMAT(x), x)
#define VIVA_PRINTLN(x) printf(VIVA_PRINTF_DEC_FORMAT(x), x), printf("\n")
#define VIVA_AMBIGUOUS_ELSE_BLOCKER \
	switch (0) \
		case 0: \
		default:		// NOLINT
#ifdef __linux__
#if defined(_DEBUG) || defined(DEBUG) || defined(__DEBUG) || defined(__DEBUG__)
#define VIVA_RUNTIME_DEBUG_RAISE raise(SIGTRAP);
#else
#define VIVA_RUNTIME_DEBUG_RAISE
#endif
#elifdef _WIN32
#define VIVA_RUNTIME_DEBUG_RAISE __debugbreak();
#endif
#define VIVA_PRINT_ERROR_MSG_IMPL_SINGLE(x) \
		fprintf(stderr,"In file %s function %s  line %d:\nConstraints not satisfied:\n\tExpect %s to be true.\n", __FILE__,__func__,__LINE__,#x);
#define VIVA_PRINT_ERROR_MSG_IMPL_BINARY(x, y) \
		fprintf(stderr,"In file %s function %s  line %d:\nConstraints not satisfied:\n\tExpect %s == %s\n", __FILE__,__func__,__LINE__,#x, #y);
#define VIVA_PRINT_ERROR_MSG_IMPL_1(x) VIVA_PRINT_ERROR_MSG_IMPL_SINGLE(x)
#define VIVA_PRINT_ERROR_MSG_IMPL_2(x, y) VIVA_PRINT_ERROR_MSG_IMPL_BINARY(x, y)
#define VIVA_PRINT_ERROR_MSG(...) VIVA_PRINT_ERROR_MSG_IMPL(__VA_ARGS__, 2, 1)
// flush the stderr to make sure the error message to be shown before SIGTRAP
#define VIVA_PRINT_ERROR_MSG_IMPL(_1, _2, N, ...) \
 	do { \
		VIVA_PRINT_ERROR_MSG_IMPL_##N(_1, _2) \
		fflush(stderr); \
	} while(false);
#define VIVA_RUNTIME_REQUIRE_IMPL_EQUAL(x,y) \
	VIVA_AMBIGUOUS_ELSE_BLOCKER \
			if((x) == (y)); \
			else { \
				VIVA_PRINT_ERROR_MSG(x,y) \
				VIVA_RUNTIME_DEBUG_RAISE \
			}

#define VIVA_RUNTIME_REQUIRE_IMPL_SATISFY(x) \
	VIVA_AMBIGUOUS_ELSE_BLOCKER \
			if(x); \
			else { \
				VIVA_PRINT_ERROR_MSG(x) \
				VIVA_RUNTIME_DEBUG_RAISE \
			}

#define VIVA_RUNTIME_REQUIRE_IMPL_2(x, y) VIVA_RUNTIME_REQUIRE_IMPL_EQUAL(x, y)
#define VIVA_RUNTIME_REQUIRE_IMPL_1(x) VIVA_RUNTIME_REQUIRE_IMPL_SATISFY(x)

#define VIVA_RUNTIME_REQUIRE(...) VIVA_RUNTIME_REQUIRE_IMPL(__VA_ARGS__, 2, 1)
#define VIVA_RUNTIME_REQUIRE_IMPL(_1, _2, N, ...) VIVA_RUNTIME_REQUIRE_IMPL_##N(_1, _2)
#pragma endregion

#pragma region result_t
struct viva_result_t {
	bool has_value;

	union {
		struct {
			enum {
				kERR_UNKNOWN,
				kERR_BAD_ALLOC,
				kERR_BAD_CAST,
				kERR_BAD_TYPEID,
				kERR_BAD_FUNCTION,
			} code;

			const char *message;
		} error;

		struct {
			void *ptr;
			const char *type_t;
			unsigned long long size;
		} value;
	};
};

#define VIVA_RETURN_ERROR_TWO(codec, msg) \
	return (struct viva_result_t) { \
		.has_value = false, \
		.error = { \
			.code = codec, \
			.message = msg, \
		}, \
	};
#define VIVA_RETURN_ERROR_ONE(codec) \
	return (struct viva_result_t) { \
		.has_value = false, \
		.error = { \
			.code = codec, \
			.message = "Unknown error.", \
		}, \
	};
#define VIVA_RETURN_ERROR_1(codec) VIVA_RETURN_ERROR_ONE(codec)
#define VIVA_RETURN_ERROR_2(codec, msg) VIVA_RETURN_ERROR_TWO(codec, msg)
#define VIVA_RETURN_ERROR(...) VIVA_RETURN_ERROR_IMPL(__VA_ARGS__, 2, 1)(__VA_ARGS__)
#define VIVA_RETURN_ERROR_IMPL(_1, _2, N, ...) VIVA_RETURN_ERROR_##N


#define VIVA_RETURN_VALUE(...) VIVA_RETURN_VALUE_IMPL(__VA_ARGS__, 3, 2, 1)(__VA_ARGS__)
#define VIVA_RETURN_VALUE_IMPL(_1, _2, _3, N, ...) VIVA_RETURN_VALUE_##N

#define VIVA_RETURN_VALUE_1(ptr_)  \
	return (struct viva_result_t){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = nullptr, \
			.size = 0, \
		}, \
	};

#define VIVA_RETURN_VALUE_2(ptr_, size_) \
	return (struct viva_result_t){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = nullptr, \
			.size = size_, \
		}, \
	};

#define VIVA_RETURN_VALUE_3(ptr_, type_t_, size_) \
	return (struct viva_result_t){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = type_t_, \
			.size = size_, \
		}, \
	};
#pragma endregion
//////////////////////////////////////////////////////////////////////////
/// viva_export part
/// can be turned off by defining VIVA_EXPORT_OFF
/// if having conflicts with other libraries
/// or manually turn off some macros using `#undef`
//////////////////////////////////////////////////////////////////////////
#pragma region viva_export
#ifndef VIVA_EXPORT_OFF
typedef struct viva_result_t result_t;
#define VIVA_PRINTF_DEC_FORMAT(x) VIVA_PRINTF_DEC_FORMAT_IMPL(x)
#define print(x) printf(VIVA_PRINTF_DEC_FORMAT(x), x)
#define println(x) printf(VIVA_PRINTF_DEC_FORMAT(x), x), printf("\n")
#define reflexpr(x) VIVA_REFLEXPR(x)
#define return_error(code, msg) VIVA_RETURN_ERROR(code, msg)
#define return_value(...) VIVA_RETURN_VALUE(__VA_ARGS__)
#define contract_assert(...) VIVA_RUNTIME_REQUIRE(__VA_ARGS__)
#endif
#pragma endregion
#ifdef __cplusplus
}
#endif

