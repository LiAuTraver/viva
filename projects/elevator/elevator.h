#pragma once
#include <net/ancillarycat/viva/utils/console.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "string.h"

enum choice {
	kChoiceInvalid = 0,
	kChoiceElevator,
	kChoiceCount,
};

struct viva_cstd_terminal_rectangle;
struct VIVA_CSTD_TERMINAL_RECT;

status_t elevator_parse_args(int, char ***, enum choice *, void **);
status_t elevator_main(enum choice, void *);

static inline struct viva_cstd_terminal_rectangle *viva_cstd_terminal_rect_init(COORD, COORD, WORD);
static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_set_outline(const struct viva_cstd_terminal_rectangle *, wchar_t);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_draw(const struct viva_cstd_terminal_rectangle *);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_free(void *);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_fill(const struct viva_cstd_terminal_rectangle *,
																																					wchar_t);
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_fill_all(const struct viva_cstd_terminal_rectangle *,
wchar_t);

struct VIVA_CSTD_TERMINAL_RECT {
	typeof(viva_cstd_terminal_rect_init)				*init;
	typeof(viva_cstd_terminal_rect_set_outline) *set_outline;
	typeof(viva_cstd_terminal_rect_draw)				*draw;
	typeof(viva_cstd_terminal_rect_free)				*free;
	typeof(viva_cstd_terminal_rect_fill)				*fill;
	typeof(viva_cstd_terminal_rect_fill_all)	*fill_all;
} static const viva_rectangle = {
	.init				 = viva_cstd_terminal_rect_init,
	.set_outline = viva_cstd_terminal_rect_set_outline,
	.draw				 = viva_cstd_terminal_rect_draw,
	.free				 = viva_cstd_terminal_rect_free,
	.fill				 = viva_cstd_terminal_rect_fill,
	.fill_all		 = viva_cstd_terminal_rect_fill_all,
};
struct viva_cstd_terminal_rectangle {
	COORD			 size;
	COORD			 upper_left;
	PCHAR_INFO buffer;
	WORD			 attributes;
	HANDLE		 console;
};
static inline struct viva_cstd_terminal_rectangle *
viva_cstd_terminal_rect_init(const COORD size, const COORD upper_left, const WORD attributes) {
	val rect = alloc(struct viva_cstd_terminal_rectangle, 1);
	*rect = (struct viva_cstd_terminal_rectangle){
		.size				= size,
		.upper_left = upper_left,
		.attributes = attributes,
		.buffer			= malloc(size.X * size.Y * sizeof(CHAR_INFO)),
		.console		= GetStdHandle(STD_OUTPUT_HANDLE),
	};
	return rect;
}
static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_set_outline(const struct viva_cstd_terminal_rectangle *rect, const wchar_t outline_char) {
	for (int y = 0; y < rect->size.Y; y++)
		for (int x = 0; x < rect->size.X; x++) {
			val cell				 = rect->buffer + y * rect->size.X + x;
			cell->Attributes = rect->attributes;
			if (y == 0 or y == rect->size.Y - 1 or x == 0 or x == rect->size.X - 1)
				cell->Char.UnicodeChar = outline_char; // Set outline
			else
				cell->Char.UnicodeChar = L' '; // Interior empty space
		}
	return viva_rectangle;
}
static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_draw(const struct viva_cstd_terminal_rectangle *rect) {
	SMALL_RECT writeRegion = {
		.Left		= rect->upper_left.X,
		.Top		= rect->upper_left.Y,
		.Right	= rect->upper_left.X + rect->size.X - 1,
		.Bottom = rect->upper_left.Y + rect->size.Y - 1,
	};
	WriteConsoleOutput(rect->console, rect->buffer, rect->size, (COORD){0, 0}, &writeRegion);
	return viva_rectangle;
}
static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_free(void *rect) {
	free(((struct viva_cstd_terminal_rectangle *)(void**)rect)->buffer);
	free(*((void **)rect));
	return viva_rectangle;
}

static inline struct VIVA_CSTD_TERMINAL_RECT
viva_cstd_terminal_rect_fill(const struct viva_cstd_terminal_rectangle *rect, const wchar_t fill_char) {
	for (int y = 1; y < rect->size.Y - 1; y++)
		for (int x = 1; x < rect->size.X - 1; x++) {
			val cell							 = rect->buffer + y * rect->size.X + x;
			cell->Attributes			 = rect->attributes;
			cell->Char.UnicodeChar = fill_char;
		}
	return viva_rectangle;
}

static inline struct VIVA_CSTD_TERMINAL_RECT viva_cstd_terminal_rect_fill_all(const struct viva_cstd_terminal_rectangle *rect,
																																						const wchar_t fill_char) {
	for (int y = 0; y < rect->size.Y; y++)
		for (int x = 0; x < rect->size.X; x++) {
			val cell							 = rect->buffer + y * rect->size.X + x;
			cell->Attributes			 = rect->attributes;
			cell->Char.UnicodeChar = fill_char;
		}
	return viva_rectangle;
}


#undef VIVA_CSTD_TERMINAL_RECT
#define VIVA_CSTD_TERMINAL_RECT // nothing

typedef struct viva_cstd_terminal_rectangle rectangle;

#define auto_ptr_rect __attribute__((cleanup(viva_cstd_terminal_rect_free)))
