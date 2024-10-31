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

#define at_maze(x, y)                                                                                                  \
	({                                                                                                                   \
		VIVA_RUNTIME_REQUIRE(x >= 0 && x < (maze)->size.X && y >= 0 && y < (maze)->size.Y);                                \
		(maze)->data[(x) * (maze)->size.Y + (y)]                                                                           \
	})
//
// void read_sample_maze(wchar_t *maze) {
// 	// read the maze from the file
// 	errno					 = 0;
// 	smart var file = _wfopen(LR"(Z:\viva\projects\maze\maze.txt)", L"r");
// 	if (file == nullptr) {
// 		wprintf(L"Error opening file: %d\n", errno);
// 		VIVA_RUNTIME_REQUIRE(false); // sigtrap
// 		exit(1);
// 	}
// 	for (int i = 0; i < 20; i++) {
// 		for (int j = 0; j < 20; j++) {
// 			char c;
// 			while ((c = fgetc(file)) != EOF && c != '0' && c != '1')
// 				;
// 			maze[coord(i, j)] = c == '1' ? wall_char : path_char;
// 		}
// 	}
// }
//
// void print_maze(const wchar_t *maze) {
// 	for (int i = 0; i < 20; i++) {
// 		for (int j = 0; j < 20; j++)
// 			wprintf(L"%c ", maze[coord(i, j)]);
// 		wprintf(L"\n");
// 	}
// }

void read_sample_file

int main() {
	// 	setlocale(LC_ALL, "");
	// 	cursor.clear_terminal();
	// 	println();
	// 	cursor.save();
	//
	// 	val start_pos = cursor.get();
	//
	// #define to_COORD(x, y) (COORD){.X = (start_pos.X + (x)), .Y = (start_pos.Y + (y))}
	//
	// 	val maze = (alloc(wchar_t, 400));
	// 	read_sample_maze(maze);
	// 	// print the maze
	// 	for (int i = 0; i < 20; i++) {
	// 		for (int j = 0; j < 20; j++) {
	// 			cursor.putc(maze[coord(i, j)]);
	// 			printf(" ");
	// 		}
	// 		printf("\n");
	// 	}
	//
	// 	val		end_pos			 = cursor.get();
	// 	COORD entrance_pos = start_pos;
	// 	COORD out_pos			 = end_pos;
	// 	for (int k = 0; k < 20; k++) {
	// 		if (maze[coord(k, 0)] == path_char) {
	// 			entrance_pos = to_COORD(k, 0);
	// 			break;
	// 		}
	// 	}
	//
	// 	for (int k = 0; k < 20; k++) {
	// 		if (maze[coord(k, 19)] == path_char) {
	// 			out_pos = to_COORD(k, 19);
	// 			break;
	// 		}
	// 	}
	//
	var init_frame = init();
	println("input...") getwchar();
	destroy(init_frame);
	return 0;
}
