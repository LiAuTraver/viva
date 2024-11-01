#include <Windows.h>
#include <iso646.h>
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#define VIVA_DEBUG_ENABLED 1
#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/viva.h>
#include "maze.h"

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

	Cursor.set(at_terminal(maze.size.Y, 0));
}

int main() {
	init();
	maze_main();
	restore();
	return 0;
}
