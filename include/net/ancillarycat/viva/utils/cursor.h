#pragma once
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

// Function to save cursor position
static void viva_ctsd_impl_cursor_save() { printf("\x1b[s"); }

// Function to restore cursor position
static void viva_cstd_impl_cursor_restore() { printf("\x1b[u"); }

// Function to move cursor to specific coordinate
// static  void _move_cursor(const short row, const short col) { printf("\x1b[%d;%dH", (row), (col)); }

// Function to hide cursor
static void viva_cstd_impl_cursor_hide() { printf("\x1b[?25l"); }

// Function to show cursor
static void viva_cstd_impl_cursor_show() { printf("\x1b[?25h"); }

// Function to move cursor backward by 1 character
static void viva_cstd_impl_cursor_move_backward() { printf("\x1b[1D"); }

// Function to move cursor down by 1 line (vertical tab)
static void viva_cstd_impl_cursor_vertical_tab() { printf("\x1b[1B"); }

// Function to clear the terminal
static void viva_cstd_impl_cursor_clear_terminal() { printf("\x1b[2J"); }

// Function to return to the start of the line
static void viva_cstd_impl_cursor_carriage_return() { printf("\x1b[1G"); }
// Function to get the current cursor position using Windows API
COORD viva_cstd_impl_cursor_get_cursor_position() {
	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_screen_buffer_info))
		return console_screen_buffer_info.dwCursorPosition;
	return (COORD){-1, -1};
}
// Function to move cursor to specific coordinate using Windows API
static void viva_cstd_impl_cursor_move_cursor_win(const short row, const short col) {
	COORD coord = viva_cstd_impl_cursor_get_cursor_position();
	coord.X += col;
	coord.Y += row;
	// VIVA_RUNTIME_REQUIRE(coord.X >= 0 && coord.Y >= 0 && (bool)("Invalid cursor position"));
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
static void viva_cstd_impl_cursor_putc(const wchar_t c) { putwchar(c); }

static void viva_cstd_impl_cursor_set_cursor_win(const COORD coord) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

static void viva_cstd_impl_cursor_putc_at_coords(const COORD coord, const wchar_t c) {
	viva_cstd_impl_cursor_set_cursor_win(coord);
	viva_cstd_impl_cursor_putc(c);
}

#pragma pack(push, 1)
/// @brief Cursor functions
struct {
	typeof(viva_ctsd_impl_cursor_save)								*tell;
	typeof(viva_cstd_impl_cursor_restore)							*rewind;
	typeof(viva_cstd_impl_cursor_move_cursor_win)			*move;
	typeof(viva_cstd_impl_cursor_hide)								*hide;
	typeof(viva_cstd_impl_cursor_show)								*show;
	typeof(viva_cstd_impl_cursor_move_backward)				*back;
	typeof(viva_cstd_impl_cursor_vertical_tab)				*next_line;
	typeof(viva_cstd_impl_cursor_clear_terminal)			*clear_terminal;
	typeof(viva_cstd_impl_cursor_carriage_return)			*carriage_return;
	typeof(viva_cstd_impl_cursor_putc_at_coords)			*putc_at;
	typeof(viva_cstd_impl_cursor_get_cursor_position) *get;
	typeof(viva_cstd_impl_cursor_putc)								*putc;
	typeof(viva_cstd_impl_cursor_set_cursor_win)			*set;
} static const cursor = {.tell						= viva_ctsd_impl_cursor_save,
												 .rewind					= viva_cstd_impl_cursor_restore,
												 .move						= viva_cstd_impl_cursor_move_cursor_win,
												 .hide						= viva_cstd_impl_cursor_hide,
												 .show						= viva_cstd_impl_cursor_show,
												 .back						= viva_cstd_impl_cursor_move_backward,
												 .next_line				= viva_cstd_impl_cursor_vertical_tab,
												 .clear_terminal	= viva_cstd_impl_cursor_clear_terminal,
												 .carriage_return = viva_cstd_impl_cursor_carriage_return,
												 .get							= viva_cstd_impl_cursor_get_cursor_position,
												 .putc_at					= viva_cstd_impl_cursor_putc_at_coords,
												 .putc						= viva_cstd_impl_cursor_putc,
												 .set							= viva_cstd_impl_cursor_set_cursor_win};
#pragma pack(pop)