#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elevator.h"

/// @brief main function
/// @param argc The number of arguments
/// @param argv The arguments
/// @param envp The environment variables
int main(const int argc, char **argv, const char **envp) {
	void			 *data = nullptr;
	//
	// val parse_callback = elevator_parse_args(argc, &argv, &choice, &data);
	// if (parse_callback != kOkStatus)
	// 	return fprintf(stderr, "Elevator: Argument parsing failed with error code %d\n", parse_callback), parse_callback;

	val init_callback = console_init();
	if (init_callback != kOkStatus)
		return fprintf(stderr, "Elevator: Console initialization failed with error code %d\n", init_callback),
					 init_callback;

	val elevator_callback = elevator_main();
	if (elevator_callback != kOkStatus)
		// don't return here, we need to restore the console
			fprintf(stderr, "Elevator: Elevator failed with error code %d\n", elevator_callback);

	val restore_callback = console_restore();
	if (restore_callback != kOkStatus)
		return fprintf(stderr, "Elevator: Console restore failed with error code %d\n", restore_callback), restore_callback;

	return 0;
}
