#pragma once
#include <net/ancillarycat/viva/viva.h>
#include <windows.h>
struct maze {
	COORD		 size;
	COORD		 entrance;
	COORD		 exit;
	wchar_t *data;
};

constexpr val wall_char			 = L'■';
constexpr val path_char			 = L'□';
constexpr val entrance_char	 = L'〓';
constexpr val exit_char			 = L'〓';
constexpr val current_char	 = L'㊣';
constexpr val footprint_char = L'☒';
constexpr val left_arrow		 = L'←';
constexpr val right_arrow		 = L'→';
constexpr val up_arrow			 = L'↑';
constexpr val down_arrow		 = L'↓';

// clang-format off
// since we put a space between each character, we need to multiply the width by 2, height inaffected
#define at_terminal_impl_1(maze_coord)																																								 \
		((COORD){(SHORT)((maze_coord).X) * 2, (SHORT)((maze_coord).Y)})
#define at_terminal_impl_2(maze_y, maze_x)                                                                             \
		((COORD){(SHORT)((maze_x) * 2), (SHORT)(maze_y)})
#define at_terminal(...) 																																															 \
		VIVA__VFUNC(at_terminal_impl, __VA_ARGS__)
// clang-format on

status_t init();
status_t restore();

void read_sample_file(struct maze *maze);
void print_maze(const struct maze *maze);
void get_maze_entrance(struct maze *maze);
void get_maze_exit(struct maze *maze) ;