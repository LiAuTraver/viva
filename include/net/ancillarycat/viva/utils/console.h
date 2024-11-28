#pragma once
#include <iso646.h>
#include <locale.h>
#include <time.h>
#include <windows.h>
#include "../internal/viva_internal_export.h"
#include "cursor.h"

// declaratioon
struct Terminal;
struct VIVA_CONSOLE_SINGLETON;
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_save_console_buffer(struct Terminal *);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_clear_console(const struct Terminal *);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_restore_console_buffer(const struct Terminal *);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_get_console_size(struct Terminal *);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_init(struct Terminal *);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_load(struct Terminal *);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_sleep_win(DWORD);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_set_cursor_state(const struct Terminal *, bool);
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_set_color(unsigned short, unsigned short);
// implementation
struct Terminal {
	HANDLE		 hOutputConsole;
	HANDLE		 hInputConsole;
	PCHAR_INFO buffer;
	COORD			 size;
	COORD			 cursor;
} static Terminal = {
	.hOutputConsole = nullptr,
	.hInputConsole	= nullptr,
	.buffer					= nullptr,
	.size						= (COORD){0, 0},
	.cursor					= (COORD){0, 0},
};

#pragma pack(push, 1)
struct VIVA_CONSOLE_SINGLETON {
	typeof(viva_cstd_impl_terminal_save_console_buffer)		 *save;
	typeof(viva_cstd_impl_terminal_clear_console)					 *clear;
	typeof(viva_cstd_impl_terminal_restore_console_buffer) *restore;
	typeof(viva_cstd_impl_terminal_get_console_size)			 *get;
	typeof(viva_cstd_impl_terminal_init)									 *init;
	typeof(viva_cstd_impl_terminal_load)									 *load;
	typeof(viva_cstd_impl_terminal_sleep_win)							 *sleep;
	typeof(viva_cstd_impl_terminal_set_cursor_state)			 *set_cursor_state;
	typeof(viva_cstd_impl_terminal_set_color)							 *set_color;
} static Console = {.save							= viva_cstd_impl_terminal_save_console_buffer,
										.clear						= viva_cstd_impl_terminal_clear_console,
										.restore					= viva_cstd_impl_terminal_restore_console_buffer,
										.get							= viva_cstd_impl_terminal_get_console_size,
										.init							= viva_cstd_impl_terminal_init,
										.load							= viva_cstd_impl_terminal_load,
										.sleep						= viva_cstd_impl_terminal_sleep_win,
										.set_cursor_state = viva_cstd_impl_terminal_set_cursor_state,
										.set_color				= viva_cstd_impl_terminal_set_color};
#pragma pack(pop)

static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_sleep_win(const DWORD milliseconds) {
	Sleep(milliseconds);
	return Console;
}
struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_set_cursor_state(const struct Terminal *terminal,
																																			 const bool							is_visible) {
	DWORD dwMode;
	if (not GetConsoleMode(terminal->hInputConsole, &dwMode)) {
		fprintf(stderr, "Failed to get console mode\n");
		return Console;
	}
	if (is_visible) {
		dwMode |= ENABLE_ECHO_INPUT;
		dwMode |= ENABLE_LINE_INPUT;
	} else {
		dwMode &= ~ENABLE_ECHO_INPUT;
		dwMode &= ~ENABLE_LINE_INPUT;
	}
	if (not SetConsoleMode(terminal->hInputConsole, dwMode))
		fprintf(stderr, "Failed to set console mode\n");
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_init(struct Terminal *terminal) {
	terminal->hOutputConsole = nullptr;
	terminal->buffer				 = nullptr;
	terminal->size					 = (COORD){0, 0};
	terminal->cursor				 = (COORD){0, 0};
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_load(struct Terminal *terminal) {
	terminal->hOutputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	terminal->hInputConsole	 = GetStdHandle(STD_INPUT_HANDLE);
	terminal->buffer				 = nullptr;
	viva_cstd_impl_terminal_get_console_size(terminal);
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_get_console_size(struct Terminal *terminal) {
	VIVA_RUNTIME_REQUIRE(terminal->hConsole != nullptr && (bool)("Invalid console handle"));

	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
	if (GetConsoleScreenBufferInfo(terminal->hOutputConsole, &console_screen_buffer_info))
		terminal->size = console_screen_buffer_info.dwSize;
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_save_console_buffer(struct Terminal *terminal) {
	VIVA_RUNTIME_REQUIRE(terminal->hOutputConsole != nullptr && (bool)("Invalid console handle"));
	VIVA_RUNTIME_REQUIRE(terminal->hInputConsole != nullptr && (bool)("Invalid console handle"));
	VIVA_RUNTIME_REQUIRE(terminal->size.X > 0 && terminal->size.Y > 0 && (bool)("Invalid console size"));

	SMALL_RECT readRegion = {0, 0, terminal->size.X - 1, terminal->size.Y - 1};
	terminal->buffer			= (CHAR_INFO *)malloc(terminal->size.X * terminal->size.Y * sizeof(CHAR_INFO));
	ReadConsoleOutput(terminal->hOutputConsole, terminal->buffer, terminal->size, (COORD){0, 0}, &readRegion);

	terminal->cursor = Cursor.get();
	Cursor.set((COORD){0, 0});
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_clear_console(const struct Terminal *terminal) {
	VIVA_RUNTIME_REQUIRE(terminal->hOutputConsole != nullptr && (bool)("Invalid console handle"));
	VIVA_RUNTIME_REQUIRE(terminal->size.X > 0 && terminal->size.Y > 0 && (bool)("Invalid console size"));

	DWORD written;
	// Clear the console with spaces
	FillConsoleOutputCharacter(terminal->hOutputConsole, ' ', terminal->size.X * terminal->size.Y, (COORD){0, 0},
														 &written);
	// Set the cursor to the top-left corner
	SetConsoleCursorPosition(terminal->hOutputConsole, (COORD){0, 0});
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON
viva_cstd_impl_terminal_restore_console_buffer(const struct Terminal *terminal) {
	VIVA_RUNTIME_REQUIRE(terminal->hOutputConsole != nullptr && (bool)("Invalid console handle"));
	VIVA_RUNTIME_REQUIRE(terminal->hInputConsole != nullptr && (bool)("Invalid console handle"));

	SMALL_RECT writeRegion = {0, 0, terminal->size.X - 1, terminal->size.Y - 1};
	if (terminal->buffer && terminal->hOutputConsole)
		WriteConsoleOutput(terminal->hOutputConsole, terminal->buffer, terminal->size, (COORD){0, 0}, &writeRegion);

	Cursor.set(terminal->cursor);
	return Console;
}
static inline struct VIVA_CONSOLE_SINGLETON viva_cstd_impl_terminal_set_color(const unsigned short foreground_color,
																																							const unsigned short background_color) {
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	if (not GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo))
		fprintf(stderr, "GetConsoleScreenBufferInfo error!\n");
	if (not SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), foreground_color | background_color))
		fprintf(stderr, "SetConsoleTextAttribute error!\n");
	return Console;
}

/// @brief Initializes the console
static inline status_t console_init() {
	setlocale(LC_ALL, "");
	srand(time(nullptr));
	// clang-format off
	Console
		.init (&Terminal)
		.load (&Terminal)
		.save (&Terminal)
		.clear(&Terminal);
	// clang-format on
	return kOkStatus;
}

/// @brief Restores the console
static inline status_t console_restore() {
	Console.sleep(1000);
	system("pause");
	Console.restore(&Terminal);
	return kOkStatus;
}
