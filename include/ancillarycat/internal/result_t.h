#pragma once

#include "predef.h"
#include "fmt.h"
#include "contract.h"
#ifdef __cplusplus
extern "C" {
#endif
#pragma region result_t
struct VIVA_RESULT_T {
	bool has_value;

	union {
		struct {
			enum VIVA_STATUS_ENUM code;
			const char *message;
		} error;

		struct {
			void *ptr;
			char *type_t;
			unsigned long long size;
		} value;
	};
};
#pragma endregion

#pragma region result_t_macros
#define VIVA_RETURN_ERROR_TWO(codec, msg) \
	return (struct VIVA_RESULT_T) { \
		.has_value = false, \
		.error = { \
			.code = codec, \
			.message = msg, \
		}, \
	}
#define VIVA_RETURN_ERROR_ONE(codec) \
	return (struct VIVA_RESULT_T) { \
		.has_value = false, \
		.error = { \
			.code = codec, \
			.message = "Unknown error.", \
		}, \
	}
#define VIVA_RETURN_ERROR_IMPL_1(codec) VIVA_RETURN_ERROR_ONE(codec)
#define VIVA_RETURN_ERROR_IMPL_2(codec, msg) VIVA_RETURN_ERROR_TWO(codec, msg)
#define VIVA_RETURN_ERROR_IMPL(...) VIVA__VFUNC(VIVA_RETURN_ERROR_IMPL, __VA_ARGS__)

/// @note Please use `VIVA_RETURN_VALUE_PTR_TWO` instead.
#define VIVA_RETURN_VALUE_PTR_THREE(ptr_, type_t_, size_) \
	return (struct VIVA_RESULT_T){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = type_t_, \
			.size = size_, \
		}, \
	}
#define VIVA_RETURN_VALUE_PTR_TWO(ptr_, size_) VIVA_RETURN_VALUE_PTR_THREE(ptr_, VIVA_REFLEXPR(ptr_), size_)
// one arg can be used for custom types
#define VIVA_RETURN_VALUE_PTR_ONE(ptr_) \
	return (struct VIVA_RESULT_T){ \
		.has_value = true, \
		.value = { \
			.ptr = ptr_, \
			.type_t = nullptr, \
			.size = sizeof(*(ptr_)), \
		}, \
	}
#define VIVA_RETURN_VALUE_PTR_IMPL_1(ptr_) VIVA_RETURN_VALUE_PTR_ONE(ptr_)
#define VIVA_RETURN_VALUE_PTR_IMPL_2(ptr_, size_) VIVA_RETURN_VALUE_PTR_TWO(ptr_, size_)
#define VIVA_RETURN_VALUE_PTR_IMPL_3(ptr_, type_t_, size_) VIVA_RETURN_VALUE_PTR_THREE(ptr_, type_t_, size_)
#define VIVA_RETURN_VALUE_PTR_IMPL(...) VIVA__VFUNC(VIVA_RETURN_VALUE_PTR_IMPL, __VA_ARGS__)
#define VIVA_RETURN_VALUE_IMPL_1(val_) VIVA_RETURN_VALUE_PTR_ONE(&val_)
#define VIVA_RETURN_VALUE_IMPL_2(val_, size_) VIVA_RETURN_VALUE_PTR_TWO(&val_, size_)
#define VIVA_RETURN_VALUE_IMPL_3(val_, type_t_, size_) VIVA_RETURN_VALUE_PTR_THREE(&val_, type_t_, size_)
#define VIVA_RETURN_VALUE_IMPL(...) VIVA__VFUNC(VIVA_RETURN_VALUE_IMPL, __VA_ARGS__)
#define VIVA_RESULT_CAST_IMPL__(res_, any_) (*(typeof(any_)*)res_.value.ptr)
#if VIVA_DEBUG_ENABLED
// GCC extension: https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
#define VIVA_RESULT_CAST_IMPL_2(res_, any_) \
	({ \
		VIVA_RUNTIME_REQUIRE_IMPL(res_.has_value); \
		VIVA_RUNTIME_REQUIRE_IMPL(res_.value.ptr); \
		VIVA_RESULT_CAST_IMPL__(res_, any_); \
	})
// ^^^^^^ VIVA_RUNTIME_REQUIRE_IMPL(not strcmp(res_.value.type_t, VIVA_REFLEXPR_IMPL(&any_))); \ // fixme, strcmp not working
#else
#define VIVA_RESULT_CAST_IMPL_2(res_, any_) VIVA_RESULT_CAST_IMPL__(res_, any_)
#endif
#define VIVA_RESULT_CAST_IMPL_(_1, _2, _3, N, ...) VIVA_RESULT_CAST_IMPL_##N
#define VIVA_RESULT_CAST_IMPL(...) VIVA_RESULT_CAST_IMPL_(__VA_ARGS__, 3,2,1)(__VA_ARGS__)
#pragma endregion
#ifdef __cplusplus
}
#endif
