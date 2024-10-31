#include <Windows.h>
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#define VIVA_DEBUG_ENABLED 1
#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/viva.h>
#include "maze.h"

Terminal *init() {
	setlocale(LC_ALL, "");
	smart var terminal = alloc(Terminal, 1);
	// clang-format off
	Console
		.init (terminal)
		.load (terminal)
		.save (terminal)
		.clear(terminal);
	// clang-format on
	return terminal;
}
status_t destroy(const Terminal *terminal) {
	Console.restore(terminal);
	return kOkStatus;
}
// clang-format off
#define at_maze(y, x) 																																																 \
		(*(((maze)->data) + (y) * (maze)->size.X + (x)))
#define for_each_row														  																																		 \
		for (short row = 0; row < (maze)->size.Y; row++)
#define for_each_col 											    																																				 \
		for (short col = 0; col < (maze)->size.X; col++)
#define for_each                                                                                                       \
		for_each_row                                                                                                       \
			for_each_col
// since we put a space between each character, we need to multiply the width by 2, height inaffected
#define at_terminal_impl_1(maze_coord)																																								 \
		((COORD){(maze_coord).X * 2, (maze_coord).Y})
#define at_terminal_impl_2(maze_y, maze_x)                                                                             \
		((COORD){(maze_x) * 2, (maze_y)})
#define at_terminal(...) VIVA__VFUNC(at_terminal_impl, __VA_ARGS__)
// clang-format on
void read_sample_file(struct maze *maze) {
	// read the maze from the file
	maze->size		 = (COORD){20, 20};
	maze->data		 = alloc(wchar_t, maze->size.X * maze->size.Y);
	errno					 = 0;
	smart var file = _wfopen(LR"(Z:\viva\projects\maze\maze.txt)", L"r");
	if (file == nullptr) {
		wprintf(L"Error opening file: %d\n", errno);
		VIVA_RUNTIME_REQUIRE(false); // sigtrap
		exit(1);
	}
	for_each {
		char c;
		while ((c = fgetc(file)) != EOF && c != '0' && c != '1')
			;
		at_maze(row, col) = c == '1' ? wall_char : path_char;
	}
}

void print_maze(const struct maze *maze) {
	for_each_row {
		for_each_col wprintf(L"%c ", at_maze(row, col));
		println()
	}
}

void get_maze_entrance(struct maze *maze) {
	for_each_row {
		if (at_maze(row, 0) == path_char) {
			maze->entrance = (COORD){0,row};
			break;
		}
	}
	cursor.putc_at(at_terminal(maze->entrance), entrance_char);
}
void get_maze_exit(struct maze *maze) {
	for_each_row {
		if (at_maze(row, maze->size.X - 1) == path_char) {
			maze->exit = (COORD){(short)(maze->size.X - 1),row};
			break;
		}
	}
	cursor.putc_at(at_terminal(maze->exit), exit_char);
}
void maze_main() {
	static struct maze maze = {
		.size			= {0, 0},
		.entrance = {0, 0},
		.exit			= {0, 0},
		.data			= nullptr,
	};
	read_sample_file(&maze);
	print_maze(&maze);

	get_maze_entrance(&maze);
	get_maze_exit(&maze);
	cursor.set(maze.entrance);
}

int main() {
	var init_frame = init();

	maze_main();
	getchar();
	system("pause");

	return destroy(init_frame);
}
