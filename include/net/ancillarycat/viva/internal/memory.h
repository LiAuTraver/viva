#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include "predef.h"


//! @see
//! https://stackoverflow.com/questions/34574933/a-good-and-idiomatic-way-to-use-gcc-and-clang-attribute-cleanup-and-point
//! 	and
//! https://github.com/systemd/systemd/blob/5809f340fd7e5e6c76e229059c50d92e1f57e8d8/src/basic/alloc-util.h#L50-L54
static inline void VIVA_AUTO_FREE_PTR_FUNC(void *_p) { free(*((void **)_p)); }

/* __cleanup__ gives void** not void* so can't pass free directly */
// Automatically frees a value at the end of scope.
#if defined(__GNUC__) || defined(__clang__)
#define VIVA_AUTO_PTR_ __attribute__((__cleanup__(VIVA_AUTO_FREE_PTR_FUNC)))
#endif

#define VIVA_ALLOC_IMPL_2(_type_, _size_) (_type_ *)malloc(sizeof(_type_) * (_size_))
#define VIVA_ALLOC_IMPL_3(_type_of_elem_, _size_, _type_of_ptr_) (_type_of_ptr_)malloc(sizeof(_type_of_elem_) * (_size_))
#define VIVA_ALLOC_IMPL(...) VIVA__VFUNC(VIVA_ALLOC_IMPL, __VA_ARGS__)

#ifdef __cplusplus
}
#endif
