#pragma once
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

// clang-format off
struct VIVA_CURSOR_SINGLETON;
static inline struct VIVA_CURSOR_SINGLETON viva_ctsd_impl_cursor_save();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_restore();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_move_cursor_win(short row, short col);
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_hide();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_show();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_move_backward();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_vertical_tab();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_clear_terminal();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_carriage_return();
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_putc(wchar_t c);
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_set_cursor_win(COORD coord);
static inline struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_putc_at_coords(COORD coord, wchar_t c);
static inline 		 COORD 				   viva_cstd_impl_cursor_get_cursor_position();

#pragma pack(push, 1)
/// @brief Cursor functions
struct VIVA_CURSOR_SINGLETON {
	typeof(viva_ctsd_impl_cursor_save)						*tell;
	typeof(viva_cstd_impl_cursor_restore)					*rewind;
	typeof(viva_cstd_impl_cursor_move_cursor_win)			*move;
	typeof(viva_cstd_impl_cursor_hide)						*hide;
	typeof(viva_cstd_impl_cursor_show)						*show;
	typeof(viva_cstd_impl_cursor_move_backward)				*back;
	typeof(viva_cstd_impl_cursor_vertical_tab)				*next_line;
	typeof(viva_cstd_impl_cursor_clear_terminal)			*clear_terminal;
	typeof(viva_cstd_impl_cursor_carriage_return)			*carriage_return;
	typeof(viva_cstd_impl_cursor_putc_at_coords)			*putc_at;
	typeof(viva_cstd_impl_cursor_get_cursor_position) 		*get;
	typeof(viva_cstd_impl_cursor_putc)						*putc;
	typeof(viva_cstd_impl_cursor_set_cursor_win)			*set;
} static const Cursor = {.tell						= viva_ctsd_impl_cursor_save,
						 .rewind					= viva_cstd_impl_cursor_restore,
						 .move						= viva_cstd_impl_cursor_move_cursor_win,
						 .hide						= viva_cstd_impl_cursor_hide,
						 .show						= viva_cstd_impl_cursor_show,
						 .back						= viva_cstd_impl_cursor_move_backward,
						 .next_line					= viva_cstd_impl_cursor_vertical_tab,
						 .clear_terminal			= viva_cstd_impl_cursor_clear_terminal,
						 .carriage_return 			= viva_cstd_impl_cursor_carriage_return,
						 .get						= viva_cstd_impl_cursor_get_cursor_position,
						 .putc_at					= viva_cstd_impl_cursor_putc_at_coords,
						 .putc						= viva_cstd_impl_cursor_putc,
						 .set						= viva_cstd_impl_cursor_set_cursor_win};
// clang-format on
#pragma pack(pop)
inline static struct VIVA_CURSOR_SINGLETON viva_ctsd_impl_cursor_save() {
	printf("\x1b[s");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_restore() {
	printf("\x1b[u");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_hide() {
	printf("\x1b[?25l");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_show() {
	printf("\x1b[?25h");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_move_backward() {
	printf("\x1b[1D");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_vertical_tab() {
	printf("\x1b[1B");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_clear_terminal() {
	printf("\x1b[2J");
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_carriage_return() {
	printf("\x1b[1G");
	return Cursor;
}
static COORD viva_cstd_impl_cursor_get_cursor_position() {
	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_screen_buffer_info))
		return console_screen_buffer_info.dwCursorPosition;
	return (COORD){-1, -1};
}
inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_move_cursor_win(const short row, const short col) {
	COORD coord = viva_cstd_impl_cursor_get_cursor_position();
	coord.X += col;
	coord.Y += row;
	// VIVA_RUNTIME_REQUIRE(coord.X >= 0 && coord.Y >= 0 && (bool)("Invalid cursor position"));
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return Cursor;
}
inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_putc(const wchar_t c) {
	putwchar(c);
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_set_cursor_win(const COORD coord) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return Cursor;
}

inline static struct VIVA_CURSOR_SINGLETON viva_cstd_impl_cursor_putc_at_coords(const COORD coord, const wchar_t c) {
	viva_cstd_impl_cursor_set_cursor_win(coord);
	viva_cstd_impl_cursor_putc(c);
	return Cursor;
}

#undef VIVA_CURSOR_SINGLETON
#define VIVA_CURSOR_SINGLETON // nothing
