#include <Windows.h>
#include <iso646.h>
#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#define VIVA_DEBUG_ENABLED 1
#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/viva.h>
#include <time.h>
#include "maze.h"
#include "maze_stack.h"

enum direction {
	kBack	 = 0,
	kLeft	 = 1,
	kRight = 2,
	kUp		 = 3,
	kDown	 = 4,
};

enum choice {
	kExample = 1,
	kRandom	 = 2,
	kFile		 = 3,
};

typeof(Cursor) step(const struct maze *maze, PCOORD const pos, const enum direction dir, maze_stack *stack) {
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

void perform_step(const struct maze *maze, COORD *current_pos_at_maze, maze_stack *stack) {
	val leftable	= check_direction(maze, *current_pos_at_maze, kLeft);
	val rightable = check_direction(maze, *current_pos_at_maze, kRight);
	val upable		= check_direction(maze, *current_pos_at_maze, kUp);
	val downable	= check_direction(maze, *current_pos_at_maze, kDown);

	val backable = not leftable and not rightable and not upable and not downable;
	if (backable)
		step(maze, current_pos_at_maze, kBack, stack);
	else
		while (true) {
			val dir = randint(kLeft, kDown + 1); // [min, max)
			if (dir == 1 and leftable) {
				step(maze, current_pos_at_maze, kLeft, stack);
				break;
			}
			if (dir == 2 and rightable) {
				step(maze, current_pos_at_maze, kRight, stack);
				break;
			}
			if (dir == 3 and upable) {
				step(maze, current_pos_at_maze, kUp, stack);
				break;
			}
			if (dir == 4 and downable) {
				step(maze, current_pos_at_maze, kDown, stack);
				break;
			}
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

/// @brief main function
/// @param argc The number of arguments
/// @param argv The arguments
/// @param choice The choice
/// @param data
status_t maze_main(const int argc, char **argv, enum choice choice, void *data) {
	contract_assert(argc > 1 and (bool) "The situation should be handled in the main function");
	contract_assert(strcmp(argv[1], "--help") != 0 and (bool) "The situation should be handled in the main function");

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
	if (not res.has_value) {
		fprint(stderr, "Fatal error:");
		fprintf(stderr, "%s", res.error.message);
		return kUnknownError;
	}

	Cursor.set(at_terminal(maze->size.Y, 0));
	val count = result_cast(res, size_t);
	print("Success! The path has been found. Total step is:");
	println(count);

	return kOkStatus;
}

/// @brief main function
/// @param argc The number of arguments
/// @param argv The arguments
/// @param envp The environment variables
int main(const int argc, char **argv, const char **envp) {
	if (argc == 1)
		return print_help_message();
	if (argc == 2 and strcmp(argv[1], "--help") == 0)
		return print_help_message();

	enum choice choice = kExample;
	smart var s_data = alloc(SHORT, 1);
	void			 *data	 = nullptr;
	if (argc == 2 and strcmp(argv[1], "--example") == 0)
		choice = kExample;
	else if ((argc == 2 or argc == 3) and strcmp(argv[1], "--random") == 0)
		choice = kRandom;
	else if (argc == 3 and strcmp(argv[1], "--file") == 0)
		choice = kFile;
	else
		fprintf(stderr, "Maze: Invalid arguments.\n      Use --help for more information.\n");

	if (choice == kRandom) {
		data = s_data;
		if (argc == 3) {
			errno						= 0;
			*(SHORT *)&data = strtol(argv[2], nullptr, 10);
			if (errno) {
				fprintf(stderr, "Maze: Invalid arguments.\n      Use --help for more information.\n");
				return 1;
			}
			if (*(SHORT *)&data < 15 or *(SHORT *)&data > 50) {
				fprintf(stderr, "Maze: Invalid arguments.\n      Use --help for more information.\n");
				return 1;
			}
		}
		if(argc == 2)
			*(SHORT *)&data = 30;
	}
	if (choice == kFile) {
		data = argv[2];
	}

	console_init();
	maze_main(argc, argv, choice, data);
	console_restore();

	return 0;
}
