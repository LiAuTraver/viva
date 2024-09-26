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

#define VIVA_IS_CHAR_PTR_IMPL(any_type_value_) _Generic((any_type_value_), \
		char *: true, \
		default: false \
)

//! for any type except char *
#define VIVA_GET_FROM_STDIN_RAW_IMPL_1(any_type_value_) ({ \
    typeof(any_type_value_) _any_type_value_; \
		scanf(VIVA_GET_ARGUMENT_TYPE_FOR_SCANF_IMPL(any_type_value_), &_any_type_value_), \
    _any_type_value_; \
})
//! for char *
#define VIVA_GET_FROM_STDIN_RAW_IMPL_2(_, str_size_) ({ \
		char str_[str_size_]; \
		fscanf(stdin, " "); \
		fgets(str_, str_size_ - 1, stdin); \
		str_; \
})

#define VIVA_GET_FROM_STDIN_RAW_IMPL(...) VIVA__VFUNC(VIVA_GET_FROM_STDIN_RAW_IMPL, __VA_ARGS__)


// Macro to check if the input is a valid character
#define VIVA_CHECK_CHAR_(any_type_value_, val_str) ({ \
    if (strlen(val_str) != 1) { \
        fprintf(stderr, "Error: invalid input. Please enter a valid character.\n"); \
        errno = 42; \
    } else { \
        any_type_value_ = (typeof(any_type_value_))val_str[0]; \
    } \
    any_type_value_; \
})

// Macro to get input from stdin and convert it to the appropriate type
#define VIVA_GET_FROM_STDIN_RECURSIVE_IMPL(any_type_value_) ({ \
    char *end_ptr; \
    typeof(any_type_value_) _any_type_value; \
    while (true) { \
        errno = 0; \
        char _any_type_value_str[128]; \
        if (fgets(_any_type_value_str, sizeof(_any_type_value_str), stdin) == nullptr) { \
            fprintf(stderr, "Error: failed to read input Please input data again.\n"); \
            continue; \
        } \
        _any_type_value_str[strcspn(_any_type_value_str, "\r\n")] = '\0'; \
        _any_type_value = _Generic((any_type_value_), \
            char: VIVA_CHECK_CHAR_(any_type_value_, _any_type_value_str), \
            signed char: VIVA_CHECK_CHAR_(any_type_value_, _any_type_value_str), \
            unsigned char: VIVA_CHECK_CHAR_(any_type_value_, _any_type_value_str), \
            int: strtol(_any_type_value_str, &end_ptr, 10), \
            unsigned int: strtoul(_any_type_value_str, &end_ptr, 10), \
            long: strtol(_any_type_value_str, &end_ptr, 10), \
            unsigned long: strtoul(_any_type_value_str, &end_ptr, 10), \
            long long: strtoll(_any_type_value_str, &end_ptr, 10), \
            unsigned long long: strtoull(_any_type_value_str, &end_ptr, 10), \
            float: strtof(_any_type_value_str, &end_ptr), \
            double: strtod(_any_type_value_str, &end_ptr), \
            long double: strtold(_any_type_value_str, &end_ptr), \
            default: 0 \
        ); \
        switch (errno) { \
        case 0: \
            break; \
        case 42: \
            continue; \
        case ERANGE: \
            fprintf(stderr, "Error: out of range. Please enter a valid number.\n"); \
            continue; \
        default: \
            fprintf(stderr, "Error: invalid input. Please enter a valid number.\n"); \
            continue; \
        } \
        if (end_ptr && *end_ptr != '\0') { \
            fprintf(stderr, "Error: invalid input. Please enter a valid number.\n"); \
            continue; \
        } \
        break; \
    } \
    _any_type_value; \
})

#ifdef __cplusplus
}
#endif