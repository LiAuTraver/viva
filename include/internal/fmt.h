#pragma once
#include "predef.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma region reflexpr
	// default triggers an error.
#define VIVA_REFLEXPR_IMPL(x) generic((x), \
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
#define VIVA_PRINT_IMPL(x) printf(VIVA_PRINTF_DEC_FORMAT_IMPL(x), x)
#define VIVA_FPRINT_IMPL(file, x) fprintf(file, VIVA_PRINTF_DEC_FORMAT_IMPL(x), x)
#define VIVA_PRINTLN_IMPL(x) printf(VIVA_PRINTF_DEC_FORMAT_IMPL(x), x), printf("\n")
#define VIVA_FPRINTLN_IMPL(file, x) fprintf(file, VIVA_PRINTF_DEC_FORMAT_IMPL(x), x), fprintf(file, "\n")
#pragma endregion

#ifdef __cplusplus
}
#endif