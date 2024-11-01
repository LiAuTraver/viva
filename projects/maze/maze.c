
#include <iso646.h>
#include <locale.h>

#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/utils/cursor.h>
#include "maze.h"

// clang-format off
#define at_maze(y, x) 																																																 \
		(*(((maze)->data) + (y) * (maze)->size.X + (x)))
#define for_each_row														  																																		 \
		for (SHORT row = 0; row < (maze)->size.Y; row++)
#define for_each_col 											    																																				 \
		for (SHORT col = 0; col < (maze)->size.X; col++)
#define for_each                                                                                                       \
		for_each_row                                                                                                       \
				for_each_col
// clang-format on

status_t init() {
	setlocale(LC_ALL, "");
	// clang-format off
	Console
		.init (&Terminal)
		.load (&Terminal)
		.save (&Terminal)
		.clear(&Terminal);
	// clang-format on
	return kOkStatus;
}
status_t restore() {
	Console
		.sleep(5000)
		// system("pause");
		// Console
		.restore(&Terminal);
	return kOkStatus;
}
void read_sample_file(struct maze *maze) {
	// read the maze from the file
	maze->size		 = (COORD){20, 20};
	maze->data		 = alloc(wchar_t, maze->size.X * maze->size.Y);
	errno					 = 0;
	smart var file = _wfopen(LR"(Z:\viva\projects\maze\maze.txt)", L"r");
	if (not file) {
		wprintf(L"Error opening file: %d\n", errno);
		VIVA_RUNTIME_REQUIRE(false); // sigtrap
		exit(1);
	}
	for_each {
		wchar_t c;
		while ((c = fgetwc(file)) != EOF && c != L'0' && c != L'1')
			;
		at_maze(row, col) = c == L'1' ? wall_char : path_char;
	}
}
void print_maze(const struct maze *maze) {
	for_each_row {
		for_each_col wprintf(L"%c ", at_maze(row, col));
		println();
	}
}
void get_maze_entrance(struct maze *maze) {
	for_each_row {
		if (at_maze(row, 0) == path_char) {
			maze->entrance = (COORD){0, row};
			break;
		}
	}
	Cursor.putc_at(at_terminal(maze->entrance), entrance_char);
}
void get_maze_exit(struct maze *maze) {
	for_each_row {
		if (at_maze(row, maze->size.X - 1) == path_char) {
			maze->exit = (COORD){(short)(maze->size.X - 1), row};
			break;
		}
	}
	Cursor.putc_at(at_terminal(maze->exit), exit_char);
}
