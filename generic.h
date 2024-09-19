#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"
#pragma region reflexpr
#define VIVA_REFLEXPR(x) generic((x), \
	bool: "bool", \
	char: "char", \
	signed char: "signed char", \
	unsigned char: "unsigned char", \
	short int: "short int", \
	unsigned short int: "unsigned short int", \
	int: "int", \
	unsigned int: "unsigned int", \
	long int: "long int", \
	unsigned long int: "unsigned long int", \
	long long int: "long long int", \
	unsigned long long int: "unsigned long long int", \
	float: "float", \
	double: "double", \
	long double: "long double", \
	char *: "char *", \
	void *: "void *", \
	int *: "int *", \
	default: "unknown type" \
)
#pragma endregion
/**
 *  @see http://www.robertgamble.net/2012/01/c11-generic-selections.html
 */
#pragma region print functions
#define VIVA_PRINTF_DEC_FORMAT_IMPL(x) generic((x), \
	char: "%c", \
	signed char: "%hhd", \
	unsigned char: "%hhu", \
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
	char *: "%s", \
	void *: "%p" \
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
	do{ \
		fprintf(stderr,"In file %s function %s  line %d:\nConstraints not satisfied:\n\tExpect %s to be true.\n", __FILE__,__func__,__LINE__,#x); \
		fflush(stderr); \
	} while(false);
#define VIVA_PRINT_ERROR_MSG_IMPL_BINARY(x, y) \
	do{ \
		fprintf(stderr,"In file %s function %s  line %d:\nConstraints not satisfied:\n\tExpect %s == %s\n", __FILE__,__func__,__LINE__,#x, #y); \
		fflush(stderr); \
	} while(false);
#define VIVA_PRINT_ERROR_MSG_IMPL_1(x) VIVA_PRINT_ERROR_MSG_IMPL_SINGLE(x)
#define VIVA_PRINT_ERROR_MSG_IMPL_2(x, y) VIVA_PRINT_ERROR_MSG_IMPL_BINARY(x, y)
#define VIVA_PRINT_ERROR_MSG(...) VIVA_PRINT_ERROR_MSG_IMPL(__VA_ARGS__, 2, 1)
#define VIVA_PRINT_ERROR_MSG_IMPL(_1, _2, N, ...) VIVA_PRINT_ERROR_MSG_IMPL_##N(_1, _2)
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
	return (result_t){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = nullptr, \
			.size = size_, \
		}, \
	};

#define VIVA_RETURN_VALUE_3(ptr_, type_t_, size_) \
	return (result_t){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = type_t_, \
			.size = size_, \
		}, \
	};
#pragma endregion
//////////////////////////////////////////////////////////////////////////
/// viva_export, can be turned off by defining VIVA_EXPORT_OFF
/// if having conflicts with other libraries
/// or manually turn off some macros using `#undef`
//////////////////////////////////////////////////////////////////////////
#pragma region viva_export
#ifndef VIVA_EXPORT_OFF
#define VIVA_PRINTF_DEC_FORMAT(x) VIVA_PRINTF_DEC_FORMAT_IMPL(x)
#define print(x) printf(VIVA_PRINTF_DEC_FORMAT(x), x)
#define println(x) printf(VIVA_PRINTF_DEC_FORMAT(x), x), printf("\n")
#define reflexpr(x) VIVA_REFLEXPR(x)
typedef struct viva_result_t result_t;
#define return_error(code, msg) VIVA_RETURN_ERROR(code, msg)
#define return_value(...) VIVA_RETURN_VALUE(__VA_ARGS__)
#define contract_assert(...) VIVA_RUNTIME_REQUIRE(__VA_ARGS__)
#endif
#pragma endregion
#ifdef __cplusplus
}
#endif
