#pragma once
#include "predef.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma region reflexpr
// default triggers an error.
#define VIVA_REFLEXPR_IMPL(x)                                                                                          \
	_Generic((x),                                                                                                        \
		void *: "void *",                                                                                                  \
		bool: "bool",                                                                                                      \
		bool *: "bool *",                                                                                                  \
		char: "char",                                                                                                      \
		char *: "char *",                                                                                                  \
		signed char: "signed char",                                                                                        \
		unsigned char: "unsigned char",                                                                                    \
		short int: "short int",                                                                                            \
		unsigned short int: "unsigned short int",                                                                          \
		int: "int",                                                                                                        \
		int *: "int *",                                                                                                    \
		unsigned int: "unsigned int",                                                                                      \
		unsigned int *: "unsigned int *",                                                                                  \
		long int: "long int",                                                                                              \
		long int *: "long int *",                                                                                          \
		unsigned long int: "unsigned long int",                                                                            \
		unsigned long int *: "unsigned long int *",                                                                        \
		long long int: "long long int",                                                                                    \
		long long int *: "long long int *",                                                                                \
		unsigned long long int: "unsigned long long int",                                                                  \
		unsigned long long int *: "unsigned long long int *",                                                              \
		float: "float",                                                                                                    \
		float *: "float *",                                                                                                \
		double: "double",                                                                                                  \
		double *: "double *",                                                                                              \
		long double: "long double",                                                                                        \
		long double *: "long double *",                                                                                    \
		default: "Unknown type. This macro only supports primitive types.")
#pragma endregion

#pragma region print functions
/**
 *  @brief Print a value to stdout.
 *  @see http://www.robertgamble.net/2012/01/c11-generic-selections.html
 *  @note has a trailing whitespace.
 */
#define VIVA_PRINTF_DEC_FORMAT_IMPL(x)                                                                                 \
	_Generic((x),                                                                                                        \
		void *: "%p ",                                                                                                     \
		bool: "%d ",                                                                                                       \
		char: "%c ",                                                                                                       \
		signed char: "%hhd ",                                                                                              \
		unsigned char: "%hhu ",                                                                                            \
		char *: "%s ",                                                                                                     \
		signed short: "%hd ",                                                                                              \
		unsigned short: "%hu ",                                                                                            \
		signed int: "%d ",                                                                                                 \
		unsigned int: "%u ",                                                                                               \
		long int: "%ld ",                                                                                                  \
		unsigned long int: "%lu ",                                                                                         \
		long long int: "%lld ",                                                                                            \
		unsigned long long int: "%llu ",                                                                                   \
		float: "%f ",                                                                                                      \
		double: "%f ",                                                                                                     \
		long double: "%Lf ",                                                                                               \
		default: "Unknown type. This macro only supports primitive types. ")
#define VIVA_FPRINT_IMPL(file, x) fprintf(file, VIVA_PRINTF_DEC_FORMAT_IMPL(x), x)

#define VIVA_FPRINTLN_IMPL_0() fprintf(stdout, "\n")
#define VIVA_FPRINTLN_IMPL_2(file, x) fprintf(file, VIVA_PRINTF_DEC_FORMAT_IMPL(x), (x)), fprintf(file, "\n")
#define VIVA_FPRINTLN_IMPL_1(x) VIVA_FPRINTLN_IMPL_2(stdout, (x))

#define VIVA_PRINT_IMPL(x) VIVA_FPRINT_IMPL(stdout, x)

// fixme: VIVA__VFUNC(VIVA_FPRINTLN_IMPL, __VA_ARGS__) failed to interpret __VA_ARGS__.
#define VIVA_FPRINTLN_IMPL(...)                                                                                        \
	VIVA_VFUNC_CONCAT(VIVA_FPRINTLN_IMPL, VIVA_VFUNC_ARG_COUNT(__VA_ARGS__))                                             \
	(__VA_ARGS__)
#pragma endregion

#ifdef __cplusplus
}
#endif
