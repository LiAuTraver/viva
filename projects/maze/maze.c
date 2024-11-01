#include <iso646.h>
#include <locale.h>
#include <time.h>

#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/utils/cursor.h>
#include "maze.h"

status_t random_maze_init(struct maze *maze, const SHORT size, bool *square) {
	maze->data = alloc(wchar_t, size * size);
	maze->size = (COORD){size, size};

	for_each *(square + row * size + col) = 1;
	return kOkStatus;
}

status_t carve_path(const SHORT size, const COORD pos, bool *square) { // NOLINT(*-no-recursion)
	// Directions: right, down, left, up
	SHORT dx[] = {2, 0, -2, 0};
	SHORT dy[] = {0, 2, 0, -2};

	// Randomize directions
	for (int i = 0; i < 4; i++) {
		var r			= rand() % (4 - i);
		var tempX = dx[i];
		var tempY = dy[i];
		dx[i]			= dx[r + i];
		dy[i]			= dy[r + i];
		dx[r + i] = tempX;
		dy[r + i] = tempY;
	}

	for (int i = 0; i < 4; i++) {
		int nx = pos.X + dx[i];
		int ny = pos.Y + dy[i];

		// Check bounds and if the cell is a wall
		if (nx >= 0 and ny >= 0 and nx < size and ny < size and *(square + ny * size + nx)) {
			*(square + (pos.Y + dy[i] / 2) * size + pos.X + dx[i] / 2) = 0; // Carve a path

			*(square + ny * size + nx) = 0; // Set the new cell as a path
			carve_path(size, (COORD){nx, ny}, square); // Recur
		}
	}

	return kOkStatus;
}

void random_generate_maze(struct maze *maze, const SHORT size) {
	var square = alloc(bool, size * size);
	contract_assert(square && (bool)"Invalid square");

	random_maze_init(maze, size, square);

	// Randomly set entrance on the left side
	int entranceY										 = rand() % maze->size.Y;
	square[entranceY * maze->size.X] = 0; // Entrance at a random position on the left

	// Carve paths from the entrance
	carve_path(size, (COORD){1, entranceY}, square);

	while (true) {
		val e = randint(0, maze->size.Y); // Randomly set exit on the right side
		if (*(square + e * maze->size.Y + maze->size.X - 2))
			continue; // if it's a wall, continue
		*(square + e * maze->size.Y + maze->size.X - 1)																= 0;
		for_each_row if (row != e) * (square + row * maze->size.Y + maze->size.X - 1) = 1;
		break;
	}
	for_each at_maze(row, col) = *(square + row * maze->size.Y + col) ? wall_char : path_char;
}


status_t console_init() {
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
status_t console_restore() {
	Console.sleep(5000);
	system("pause");
	Console.restore(&Terminal);
	return kOkStatus;
}
void read_sample_file(struct maze *maze) {
	// read the maze from the file
	maze->size		 = (COORD){20, 20};
	maze->data		 = alloc(wchar_t, maze->size.X * maze->size.Y);
	errno					 = 0;
	smart var file = _wfopen(LR"(Z:\viva\projects\maze\maze.txt)", L"r");
	if (not file or errno) {
		wprintf(L"Error opening file: %d\n", errno);
		contract_assert(false);
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
void viva_maze_free_with_data_ptr(struct maze **maze) {
	contract_assert(*maze && (bool)"Invalid maze");

	free((*maze)->data);
	free(*maze);
}
int print_help_message(void) {
	fprintf(stderr, "\nMaze: at least one argument is required\n");
	fprintf(stderr, "Usage: maze --help                  show this help message\n");
	fprintf(stderr, "Usage: maze --example               show the example maze\n");
	fprintf(stderr, "Usage: maze --random                generate a random maze and solve it\n");
	fprintf(stderr, "Usage: maze --file [file_path]      read the maze from the file and solve it\n");
	return 1;
}
/// @brief Generate a random integer between min and max
/// @param min The minimum value
/// @param max The maximum value
/// @return A random integer larger than or equal min and less than max
/// @note The range is [min, max)
int randint(const int min, const int max) {
	contract_assert(min < max);
	return rand() % (max - min) + min;
}
