#include <Windows.h>
#include <iso646.h>
#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#define VIVA_DEBUG_ENABLED 1
#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/viva.h>
#include "maze.h"
#include "maze_stack.h"

/// @brief main function
/// @param argc The number of arguments
/// @param argv The arguments
/// @param envp The environment variables
int main(const int argc, char **argv, const char **envp) {
	enum choice choice;
	void			 *data = nullptr;

	val parse_callback = parse_args(argc, &argv, &choice, &data);
	if (parse_callback != kOkStatus)
		return fprintf(stderr, "Maze: Argument parsing failed with error code %d\n", parse_callback), parse_callback;

	val init_callback = console_init();
	if (init_callback != kOkStatus)
		return fprintf(stderr, "Maze: Console initialization failed with error code %d\n", init_callback), init_callback;

	val maze_callback = maze_main(choice, data);
	if (maze_callback != kOkStatus)
		// don't return here, we need to restore the console
		fprintf(stderr, "Maze: Maze failed with error code %d\n", maze_callback);

	val restore_callback = console_restore();
	if (restore_callback != kOkStatus)
		return fprintf(stderr, "Maze: Console restore failed with error code %d\n", restore_callback), restore_callback;

	return 0;
}
