#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "compat.h"
#include "variadic.h"
#pragma region primitive types
#ifdef __SIZEOF_INT128__
// only in 64-bit machine
typedef __int128_t	int128_t;
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
// get compiler's counter
#if !defined(__COUNTER__) || __COUNTER__ <= 100ULL
#define VIVA_COUNTER __COUNTER__ // + 1000ULL // BUG, fixme
#else
#define VIVA_COUNTER __COUNTER__
#endif


#define VIVA_MAKE_LABEL_IMPL(_namespace, _prefix, _label) _namespace##_##_prefix##_##_label
#define VIVA_LABEL(_prefix, _label) VIVA_MAKE_LABEL_IMPL(viva_internal_label, _prefix, _label)

#pragma push_macro("Any")
#undef Any
#pragma push_macro("any")
#undef any
//! reduce the size of the union, for `Any` and `any` will only be used for type lookup.
#pragma pack(push, 1)
union {
	void							*void_ptr_type;
	char							*char_ptr_type;
	bool							 boolean_type;
	char							 char_type;
	signed char				 signed_char_type;
	unsigned char			 unsigned_char_type;
	short							 short_type;
	signed short			 signed_short_type;
	unsigned short		 unsigned_short_type;
	int								 int_type;
	signed int				 signed_int_type;
	unsigned int			 unsigned_int_type;
	long							 long_type;
	signed long				 signed_long_type;
	unsigned long			 unsigned_long_type;
	long long					 long_long_type;
	signed long long	 signed_long_long_type;
	unsigned long long unsigned_long_long_type;
	float							 float_type;
	double						 double_type;
	long double				 long_double_type;
#ifdef __SIZEOF_INT128__
	int128_t	int128_type;
	uint128_t uint128_type;
#endif
} static
#ifdef __GNUC__
const /// @note clang's typeof() works differently with GCC's typeof(), which preserves the const qualifier.
#endif
Any = {.void_ptr_type = nullptr}, any = {.void_ptr_type = nullptr};
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
	kUnknownError		= UCHAR_MAX - 1,
	kNotImplemented = UCHAR_MAX
};
#pragma endregion
#ifdef __cplusplus
}
#endif
