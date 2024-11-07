#pragma once
#include <iso646.h>
#include <stdlib.h>
#include <wchar.h>
#include_next <string.h>
#include "net/ancillarycat/viva/internal/compat.h"
#include "net/ancillarycat/viva/viva.h"

// Define the viva_cstd_wstring struct
struct viva_cstd_wstring {
	wchar_t *str;
	size_t	 size;
};
struct VIVA_WSTR_SINGLETON;

static struct viva_cstd_wstring		*viva_cstd_wstring_init(const wchar_t *);
static struct VIVA_WSTR_SINGLETON *viva_cstd_wstring_concat(struct viva_cstd_wstring *,
																														const struct viva_cstd_wstring *);
static struct VIVA_WSTR_SINGLETON *viva_cstd_wstring_push(struct viva_cstd_wstring *, const wchar_t *);
static wchar_t										*viva_cstd_wstring_find(const struct viva_cstd_wstring *, const wchar_t *);
static size_t											 viva_cstd_wstring_length(const struct viva_cstd_wstring *);

struct VIVA_WSTR_SINGLETON {
	typeof(viva_cstd_wstring_init)	 *init;
	typeof(viva_cstd_wstring_concat) *concat;
	typeof(viva_cstd_wstring_push)	 *push;
	typeof(viva_cstd_wstring_find)	 *find;
	typeof(viva_cstd_wstring_length) *length;
} // Define the WStr singleton
static WStr = {
	.init		= viva_cstd_wstring_init,
	.concat = viva_cstd_wstring_concat,
	.push		= viva_cstd_wstring_push,
	.find		= viva_cstd_wstring_find,
	.length = viva_cstd_wstring_length,
};

static inline struct viva_cstd_wstring *viva_cstd_wstring_init(const wchar_t *init_str) {
	struct viva_cstd_wstring *s = malloc(sizeof(struct viva_cstd_wstring));
	if (not s)
		return nullptr;
	s->size = wcslen(init_str);
	s->str	= (wchar_t *)malloc((s->size + 1) * sizeof(wchar_t));
	if (not s->str) {
		free(s);
		return nullptr;
	}
	wcscpy(s->str, init_str);
	return s;
}

static inline typeof(WStr) *viva_cstd_wstring_concat(struct viva_cstd_wstring				*dest,
																										 const struct viva_cstd_wstring *src) {
	val new_size = dest->size + src->size;
	val new_str	 = (wchar_t *)realloc(dest->str, (new_size + 1) * sizeof(wchar_t));
	if (not new_str)
		return nullptr;
	wcscat(new_str, src->str);
	dest->str	 = new_str;
	dest->size = new_size;
	return &WStr;
}

static inline typeof(WStr) *viva_cstd_wstring_push(struct viva_cstd_wstring *dest, const wchar_t *src) {
	val src_size = wcslen(src);
	val new_size = dest->size + src_size;
	val new_str	 = (wchar_t *)realloc(dest->str, (new_size + 1) * sizeof(wchar_t));
	if (not new_str)
		return nullptr;
	wcscat(new_str, src);
	dest->str	 = new_str;
	dest->size = new_size;
	return &WStr;
}

static inline wchar_t *viva_cstd_wstring_find(const struct viva_cstd_wstring *s, const wchar_t *substr) {
	return wcsstr(s->str, substr);
}

static inline size_t viva_cstd_wstring_length(const struct viva_cstd_wstring *s) { return s->size; }

// Typedefs for convenience
typedef struct viva_cstd_string	 string;
typedef struct viva_cstd_wstring wstring;
#undef VIVA_WSTR_SINGLETON
#define VIVA_WSTR_SINGLETON // nothing
