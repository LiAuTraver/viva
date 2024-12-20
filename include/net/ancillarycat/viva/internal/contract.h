#pragma once
#include <signal.h>
#include <stdio.h>
#include "fmt.h"
#include "predef.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma region contracts

#define VIVA_AMBIGUOUS_ELSE_BLOCKER                                                                                    \
	switch (0)                                                                                                           \
	case 0:                                                                                                              \
	default:
#ifdef __linux__
#if VIVA_DEBUG_ENABLED
#define VIVA_RUNTIME_DEBUG_RAISE raise(SIGTRAP);
#else
#define VIVA_RUNTIME_DEBUG_RAISE (void *)0;
#endif
#elifdef _WIN32
#define VIVA_RUNTIME_DEBUG_RAISE VIVA_DEBUG_SIGTRAP();
#endif
#define VIVA_PRINT_ERROR_MSG_IMPL_SINGLE(x)                                                                            \
	fprintf(stderr,                                                                                                      \
					"in file \"%s\" function \'%s\' line %d:\nConstraints not satisfied:\n\tExpected the value of \'%s\' to be " \
					"true.\n",                                                                                                   \
					__FILE__, __func__, __LINE__, #x);
#define VIVA_PRINT_ERROR_MSG_IMPL_BINARY(x, y)                                                                         \
	fprintf(stderr,                                                                                                      \
					"in file \"%s\" function \'%s\' line %d:\nConstraints not satisfied:\n\tExpected: %s equals to %s;\nbut "    \
					"actually %s appears to be ",                                                                                \
					__FILE__, __func__, __LINE__, #x, #y, #x);                                                                   \
	VIVA_FPRINT_IMPL(stderr, x);                                                                                         \
	fprintf(stderr, ",\n         and %s appears to be ", #y);                                                            \
	VIVA_FPRINTLN_IMPL(stderr, y);
#define VIVA_PRINT_ERROR_MSG_IMPL_1(x) VIVA_PRINT_ERROR_MSG_IMPL_SINGLE(x)
#define VIVA_PRINT_ERROR_MSG_IMPL_2(x, y) VIVA_PRINT_ERROR_MSG_IMPL_BINARY(x, y)
// flush the stderr to make sure the error message to be shown before SIGTRAP was raised.
#define VIVA_PRINT_ERROR_MSG(...)                                                                                      \
	do {                                                                                                                 \
		VIVA_PRINT_ERROR_MSG_IMPL(__VA_ARGS__, 2, 1)(__VA_ARGS__) fflush(stderr);                                          \
		VIVA_RUNTIME_DEBUG_RAISE                                                                                           \
	} while (false);
#define VIVA_PRINT_ERROR_MSG_IMPL(_1, _2, N, ...) VIVA_PRINT_ERROR_MSG_IMPL_##N
#define VIVA_RUNTIME_REQUIRE_IMPL_EQUAL(x, y)                                                                          \
	VIVA_AMBIGUOUS_ELSE_BLOCKER                                                                                          \
	if ((x) == (y))                                                                                                      \
		;                                                                                                                  \
	else {                                                                                                               \
		VIVA_PRINT_ERROR_MSG(x, y)                                                                                         \
	}
#define VIVA_RUNTIME_REQUIRE_IMPL_SATISFY(x)                                                                           \
	VIVA_AMBIGUOUS_ELSE_BLOCKER                                                                                          \
	if (x)                                                                                                               \
		;                                                                                                                  \
	else {                                                                                                               \
		VIVA_PRINT_ERROR_MSG(x)                                                                                            \
	}
#define VIVA_RUNTIME_REQUIRE_IMPL_2(x, y) VIVA_RUNTIME_REQUIRE_IMPL_EQUAL(x, y)
#define VIVA_RUNTIME_REQUIRE_IMPL_1(x) VIVA_RUNTIME_REQUIRE_IMPL_SATISFY(x)
#if VIVA_DEBUG_ENABLED
#define VIVA_RUNTIME_REQUIRE_IMPL(...) VIVA__VFUNC(VIVA_RUNTIME_REQUIRE_IMPL, __VA_ARGS__)
#else
// if debug is turned off, do nothing.
#define VIVA_RUNTIME_REQUIRE_IMPL(...)
#endif
#pragma endregion
#ifdef __cplusplus
}
#endif
