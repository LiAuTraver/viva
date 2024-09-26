#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "variadic.h"
#include "compat.h"
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
