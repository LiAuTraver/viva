#pragma once
#include <net/ancillarycat/viva/viva.h>
#include <windows.h>

struct maze;
enum direction;
enum choice;
struct maze_stack;
#pragma pack(push, 1)
enum choice {
	kExample = 1,
	kRandom	 = 2,
	kFile		 = 3,
};
#pragma pack(pop)
static constexpr val wall_char		 = L'■';
static constexpr val path_char		 = L'□';
static constexpr val entrance_char = L'〓';
static constexpr val exit_char		 = L'〓';
static constexpr val current_char	 = L'㊣';
static constexpr val visited_char	 = L'☒';
static constexpr val left_arrow		 = L'←';
static constexpr val right_arrow	 = L'→';
static constexpr val up_arrow			 = L'↑';
static constexpr val down_arrow		 = L'↓';
// extern static constexpr wchar_t* example_maze[];
/// @brief main function
status_t maze_main(enum choice, void *) __attribute__((nonnull(2)));
/// @brief Initializes the console
status_t console_init();
/// @brief Restores the console
status_t console_restore();
int			 randint(int, int);
/// @brief Reads the maze from the file
status_t read_file(struct maze *, const char *) __attribute__((nonnull(1, 2)));
/// @brief Prints the maze
void print_maze(const struct maze *) __attribute__((nonnull(1)));
/// @brief Gets the maze entrance point
void get_maze_entrance(struct maze *) __attribute__((nonnull(1)));
/// @brief Gets the maze exit point
void get_maze_exit(struct maze *) __attribute__((nonnull(1)));
/// @brief Frees the maze stack, Helper function for smart pointer.
void viva_maze_free_with_data_ptr(struct maze **) __attribute__((nonnull(1)));
/// @brief Prints the help message
status_t print_help_message();
/// @brief Initializes the random maze
status_t random_maze_init(struct maze *, SHORT, bool *) __attribute__((nonnull(1, 3)));
/// @brief Carves the path
status_t carve_path(SHORT, COORD, bool *) __attribute__((nonnull(3)));
/// @brief Generates the random maze
void random_generate_maze(struct maze *, SHORT) __attribute__((nonnull(1)));
/// @brief Parses the arguments
status_t parse_args(int, char ***, enum choice *, void **) __attribute__((nonnull(2, 3, 4)));
/// @brief Finds and prints the path
result_t find_and_print_path(const struct maze *, DWORD) __attribute__((nonnull(1)));
/// @brief Steps in the maze
typeof(Cursor) step(const struct maze *, PCOORD, enum direction, struct maze_stack *) __attribute__((nonnull(1, 4)));
/// @brief Checks the direction
bool check_direction(const struct maze *, COORD, enum direction) __attribute__((nonnull(1)));
/// @brief Checks if the position is the exit
bool is_exit(const struct maze *, COORD) __attribute__((nonnull(1)));
/// @brief Checks if the position is the entrance
bool is_entrance(const struct maze *, COORD) __attribute__((nonnull(1)));
/// @brief Performs a step in the maze
typeof(Cursor) perform_step(const struct maze *, COORD *, struct maze_stack *) __attribute__((nonnull(1, 2, 3)));
/// @brief Reads the example maze
void read_example_maze(struct maze *, wchar_t **, size_t) __attribute__((nonnull(1, 2)));


// clang-format off
#define at_terminal_impl_1(maze_coord)																																								 \
		((COORD){(SHORT)((maze_coord).X) * 2, (SHORT)((maze_coord).Y)})
#define at_terminal_impl_2(maze_y, maze_x)                                                                             \
		((COORD){(SHORT)((maze_x) * 2), (SHORT)(maze_y)})
/// @brief Syntax sugar for the maze position conversion to terminal position: x * 2, y.
/// @note X first, Y second, yet in maze, Y first, X second.
#define at_terminal(...) 																																															 \
		VIVA__VFUNC(at_terminal_impl, __VA_ARGS__)
/// @brief The maze stack smart pointer
#define smart_stack __attribute__((__cleanup__(viva_maze_free_stack)))
/// @brief The maze smart pointer
#define smart_maze __attribute__((__cleanup__(viva_maze_free_with_data_ptr)))
/// @brief Syntax sugar for the maze position
#define at_maze(y, x) 																																																 \
		(*(((maze)->data) + (y) * (maze)->size.X + (x)))
/// @brief Syntax sugar for iterating over the maze rows
#define for_each_row														  																																		 \
		for (SHORT row = 0; row < (maze)->size.Y; row++)
/// @brief Syntax sugar for iterating over the maze columns
#define for_each_col 											    																																				 \
		for (SHORT col = 0; col < (maze)->size.X; col++)
/// @brief Syntax sugar for iterating over the maze
#define for_each                                                                                                       \
		for_each_row                                                                                                       \
				for_each_col
static wchar_t* example_maze[] = {
	LR"(1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1)",
	LR"(0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 1)",
	LR"(1 1 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 1 1 1)",
	LR"(1 0 0 1 0 1 0 1 0 1 1 1 1 1 1 0 1 1 0 1)",
	LR"(1 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 1)",
	LR"(1 0 0 1 0 1 0 1 1 1 0 1 0 1 1 1 1 1 0 1)",
	LR"(1 0 1 1 0 1 0 1 1 0 0 0 0 0 1 0 1 1 0 1)",
	LR"(1 0 0 0 0 1 0 1 1 1 1 1 0 1 1 0 1 1 0 1)",
	LR"(1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1)",
	LR"(1 1 1 1 0 1 0 1 1 1 0 1 0 1 1 0 0 1 0 1)",
	LR"(1 0 0 1 0 1 0 1 1 1 0 1 0 1 1 0 1 1 0 1)",
	LR"(1 0 0 1 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1)",
	LR"(1 0 1 1 0 0 1 1 1 1 1 1 0 1 0 0 1 0 0 1)",
	LR"(1 0 0 0 0 0 0 0 0 0 1 1 1 1 0 0 1 0 0 1)",
	LR"(1 0 1 1 0 0 1 1 1 1 1 1 1 1 0 0 1 0 0 1)",
	LR"(1 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1)",
	LR"(1 0 0 0 0 0 1 0 0 1 0 1 1 1 0 1 1 0 1 1)",
	LR"(1 0 0 0 0 0 1 0 0 0 1 0 1 1 0 1 1 0 1 1)",
	LR"(1 0 1 1 1 0 1 0 0 1 1 0 0 0 0 0 0 0 0 0)",
	LR"(1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1)",
};
// clang-format on
