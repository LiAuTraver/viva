#pragma once
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "../viva.h"

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
	typeof(viva_cstd_wstring_init) *const		init;
	typeof(viva_cstd_wstring_concat) *const concat;
	typeof(viva_cstd_wstring_push) *const		push;
	typeof(viva_cstd_wstring_find) *const		find;
	typeof(viva_cstd_wstring_length) *const length;
} static WStr = {
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


typedef struct viva_cstd_string	 string;
typedef struct viva_cstd_wstring wstring;

#pragma GCC poison viva_cstd_string
#pragma GCC poison viva_cstd_wstring

#pragma GCC poison viva_cstd_wstring_init
#pragma GCC poison viva_cstd_wstring_concat
#pragma GCC poison viva_cstd_wstring_push
#pragma GCC poison viva_cstd_wstring_find
#pragma GCC poison viva_cstd_wstring_length
