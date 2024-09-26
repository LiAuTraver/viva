#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "predef.h"
#ifdef __SIZEOF_INT128__
#define VIVA_GET_ARGUMENT_TYPE_FOR_SCANF_IMPL(any_type_value_) _Generic((any_type_value_), \
    void *: " %p", \
    bool: " %d", \
    char: " %c", \
    signed char: " %hhd", \
    unsigned char: " %hhu", \
    short: " %hd", \
    unsigned short: " %hu", \
    int: " %d", \
    unsigned int: " %u", \
    long: " %ld", \
    unsigned long: " %lu", \
    long long: " %lld", \
    unsigned long long: " %llu", \
    float: " %f", \
    double: " %lf", \
    long double: " %Lf", \
    int128_t: " %lld", \
    uint128_t: " %llu", \
    default: " " \
)
#else
#define VIVA_GET_ARGUMENT_TYPE_FOR_SCANF_IMPL(any_type_value_) _Generic((any_type_value_), \
		void *: " %p", \
		bool: " %d", \
		char: " %c", \
		signed char: " %hhd", \
		unsigned char: " %hhu", \
		short: " %hd", \
		unsigned short: " %hu", \
		int: " %d", \
		unsigned int: " %u", \
		long: " %ld", \
		unsigned long: " %lu", \
		long long: " %lld", \
		unsigned long long: " %llu", \
		float: " %f", \
		double: " %lf", \
		long double: " %Lf", \
		default: " " \
)
#endif

#define VIVA_IS_CHAR_PTR(any_type_value_) _Generic((any_type_value_), \
		char *: true, \
		default: false \
)

//! for any type except char *
#define VIVA_GET_FROM_STDIN_RAW_IMPL_1(any_type_value_) ({ \
    typeof(any_type_value_) _any_type_value_; \
		scanf(VIVA_GET_ARGUMENT_TYPE_FOR_SCANF_IMPL(_any_type_value_), &_any_type_value_), \
    _any_type_value_; \
})
//! for char *
#define VIVA_GET_FROM_STDIN_RAW_IMPL_2(_, str_size_) ({ \
		char str_[str_size_]; \
		fgets(str_, str_size_ - 1, stdin); \
		str_; \
})

#define VIVA_GET_FROM_STDIN_RAW_IMPL(...) VIVA__VFUNC(VIVA_GET_FROM_STDIN_RAW_IMPL, __VA_ARGS__)

#ifdef __cplusplus
}
#endif