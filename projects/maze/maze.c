#include <iso646.h>
#include <locale.h>
#include <time.h>

#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/utils/cursor.h>
#include "maze.h"
#include "maze_stack.h"

#pragma pack(push, 1)
struct maze {
	wchar_t *data;
	COORD		 size;
	COORD		 entrance;
	COORD		 exit;
};
enum direction {
	kBack	 = 0,
	kLeft	 = 1,
	kRight = 2,
	kUp		 = 3,
	kDown	 = 4,
};
#pragma pack(pop)

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
		var tempX = *(dx + i);
		var tempY = *(dy + i);
		*(dx + i) = *(dx + r + i);
		*(dy + i) = *(dy + r + i);
		*(dx + r + i) = tempX;
		*(dy + r + i) = tempY;
	}

	for (int i = 0; i < 4; i++) {
		val nx = pos.X + *(dx + i);
		val ny = pos.Y + *(dy + i);
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
status_t print_help_message() {
	fprintf(stderr, "\nMaze: at least one argument is required\n");
	fprintf(stderr, "Usage: maze --help                  show this help message\n");
	fprintf(stderr, "Usage: maze --example               show the example maze\n");
	fprintf(stderr, "Usage: maze --random                generate a random maze and solve it\n");
	fprintf(stderr, "Usage: maze --file [file_path]      read the maze from the file and solve it\n");
	return kOkStatus;
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

/// @brief Parse the arguments
status_t parse_args(const int argc, char **argv, enum choice *choice, void **data) {
	if (argc == 1)
		return print_help_message(), kInvalidArgument;
	if (argc == 2 and strcmp(argv[1], "--help") == 0)
		return print_help_message(), kOkStatus;

	if (argc == 2 and strcmp(argv[1], "--example") == 0)
		*choice = kExample;
	else if ((argc == 2 or argc == 3) and strcmp(argv[1], "--random") == 0)
		*choice = kRandom;
	else if (argc == 3 and strcmp(argv[1], "--file") == 0)
		*choice = kFile;
	else
		return fprintf(stderr, "Maze: Invalid arguments.\n      Use --help for more information.\n"), kInvalidArgument;

	/* smart */ var s_data = alloc(SHORT, 1);
	errno									 = 0;
	*data									 = s_data;
	if (*choice == kRandom) {
		if (argc == 3) {
			*(SHORT *)data = strtol(argv[2], nullptr, 10);
			if (errno)
				return fprintf(stderr, "Maze: Invalid arguments.\n      Use --help for more information.\n"), kInvalidArgument;
			if (*(SHORT *)data < 15 or *(SHORT *)data > 50)
				return fprintf(stderr, "Maze: Invalid data. The size should be between 15 and 50.\n"), kInvalidArgument;
		}
		if (argc == 2)
			*(SHORT *)data = 30;
	} else if (*choice == kFile)
		*data = argv[2];
	return kOkStatus;
}



status_t maze_main(const enum choice choice, void *data) {
	smart_maze var maze = alloc(struct maze, 1);
	maze->size					= (COORD){0, 0};
	maze->entrance			= (COORD){0, 0};
	maze->exit					= (COORD){0, 0};
	maze->data					= nullptr;

	if (choice == kExample)
		read_sample_file(maze);
	else if (choice == kRandom) {
		random_generate_maze(maze, *(SHORT *)&data);
	} else if (choice == kFile) {
		// todo: implement file maze
		contract_assert(false and (bool) "Not implemented yet");
	} else
		contract_assert(false and (bool) "Invalid choice");

	print_maze(maze);

	get_maze_entrance(maze);
	get_maze_exit(maze);

	val res = find_and_print_path(maze, 100);
	Cursor.set(at_terminal(maze->size.Y, 0));

	if (not res.has_value) {
		fprintf(stderr, "\033[31mFatal error: \033[0m");
		fprintf(stderr, "\033[31m%s\033[0m\n", res.error.message);
		return kUnknownError;
	}

	val count = result_cast(res, size_t);
	print("Success! The path has been found. Total step is:");
	println(count);

	return kOkStatus;
}
typeof(Cursor) step(const struct maze *maze, PCOORD const pos, const enum direction dir, struct maze_stack *stack) {
	at_maze(pos->Y, pos->X) = visited_char;
	switch (dir) {
	case kLeft:
		Cursor.putc_at(at_terminal(*pos), left_arrow)
			.putc_at(at_terminal(pos->Y, pos->X - 1), current_char)
			.set(at_terminal(pos->Y, pos->X - 1));
		pos->X -= 1;
		return Cursor;
	case kRight:
		Cursor.putc_at(at_terminal(*pos), right_arrow)
			.putc_at(at_terminal(pos->Y, pos->X + 1), current_char)
			.set(at_terminal(pos->Y, pos->X + 1));
		pos->X += 1;
		return Cursor;
	case kUp:
		Cursor.putc_at(at_terminal(*pos), up_arrow)
			.putc_at(at_terminal(pos->Y - 1, pos->X), current_char)
			.set(at_terminal(pos->Y - 1, pos->X));
		pos->Y -= 1;
		return Cursor;
	case kDown:
		Cursor.putc_at(at_terminal(*pos), down_arrow)
			.putc_at(at_terminal(pos->Y + 1, pos->X), current_char)
			.set(at_terminal(pos->Y + 1, pos->X));
		pos->Y += 1;
		return Cursor;
	default:
		Stack.pop(stack);
		val popped = Stack.pop(stack);
		Cursor.putc_at(at_terminal(*pos), visited_char).putc_at(at_terminal(popped), current_char).set(at_terminal(popped));
		pos->X = popped.X;
		pos->Y = popped.Y;
		return Cursor;
	}
}
bool check_direction(const struct maze *maze, const COORD pos, const enum direction dir) {
	switch (dir) {
	case kLeft:
		return pos.X > 0 and at_maze(pos.Y, pos.X - 1) == path_char;
	case kRight:
		return pos.X < maze->size.X - 1 and at_maze(pos.Y, pos.X + 1) == path_char;
	case kUp:
		return pos.Y > 0 and at_maze(pos.Y - 1, pos.X) == path_char;
	case kDown:
		return pos.Y < maze->size.Y - 1 and at_maze(pos.Y + 1, pos.X) == path_char;
	default:
		return false;
	}
}
bool is_exit(const struct maze *maze, const COORD pos) { return pos.X == maze->exit.X and pos.Y == maze->exit.Y; }
bool is_entrance(const struct maze *maze, const COORD pos) {
	return pos.X == maze->entrance.X and pos.Y == maze->entrance.Y;
}
typeof(Cursor) perform_step(const struct maze *maze, COORD *current_pos_at_maze, struct maze_stack *stack) {
	val leftable	= check_direction(maze, *current_pos_at_maze, kLeft);
	val rightable = check_direction(maze, *current_pos_at_maze, kRight);
	val upable		= check_direction(maze, *current_pos_at_maze, kUp);
	val downable	= check_direction(maze, *current_pos_at_maze, kDown);
	val backable	= not leftable and not rightable and not upable and not downable;

	if (backable)
		return step(maze, current_pos_at_maze, kBack, stack);

	while (true) {
		val dir = randint(kLeft, kDown + 1); // [min, max)
		if (dir == 1 and leftable)
			return step(maze, current_pos_at_maze, kLeft, stack);
		if (dir == 2 and rightable)
			return step(maze, current_pos_at_maze, kRight, stack);
		if (dir == 3 and upable)
			return step(maze, current_pos_at_maze, kUp, stack);
		if (dir == 4 and downable)
			return step(maze, current_pos_at_maze, kDown, stack);
	}
}

result_t find_and_print_path(const struct maze *maze, const DWORD interval) {

	smart_stack var stack = Stack.create(maze->size.X * maze->size.Y);
	Cursor.set(at_terminal(maze->entrance));
	COORD current_pos_at_maze = maze->entrance;
	var		counter							= alloc(size_t, 1);
	*counter									= 0;

	while (not is_exit(maze, current_pos_at_maze)) {

		Console.sleep(interval);
		Stack.push(stack, current_pos_at_maze);
		perform_step(maze, &current_pos_at_maze, stack);
		if (is_entrance(maze, current_pos_at_maze))
			return_error(kNotFound, "Path not found");
		(*counter)++;
		if (*counter > 1000)
			return_error(kNotFound, "Counter exceeded 1000. Will stop the program.");
	}
	return_value(*counter)
}