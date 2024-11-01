#pragma once
#include <net/ancillarycat/viva/viva.h>
#include <windows.h>

#pragma pack(push, 1)
struct maze {
	wchar_t *data;
	COORD		 size;
	COORD		 entrance;
	COORD		 exit;
};
#pragma pack(pop)

constexpr val wall_char			= L'■';
constexpr val path_char			= L'□';
constexpr val entrance_char = L'〓';
constexpr val exit_char			= L'〓';
constexpr val current_char	= L'㊣';
constexpr val visited_char	= L'☒';
constexpr val left_arrow		= L'←';
constexpr val right_arrow		= L'→';
constexpr val up_arrow			= L'↑';
constexpr val down_arrow		= L'↓';

// clang-format off
// since we put a space between each character, we need to multiply the width by 2, height inaffected
#define at_terminal_impl_1(maze_coord)																																								 \
		((COORD){(SHORT)((maze_coord).X) * 2, (SHORT)((maze_coord).Y)})
#define at_terminal_impl_2(maze_y, maze_x)                                                                             \
		((COORD){(SHORT)((maze_x) * 2), (SHORT)(maze_y)})
#define at_terminal(...) 																																															 \
		VIVA__VFUNC(at_terminal_impl, __VA_ARGS__)
// clang-format on

status_t console_init();
status_t console_restore();
int			 randint(int, int);

void		 read_sample_file(struct maze *);
void		 print_maze(const struct maze *);
void		 get_maze_entrance(struct maze *);
void		 get_maze_exit(struct maze *);
void		 viva_maze_free_with_data_ptr(struct maze **);
int			 print_help_message();
status_t random_maze_init(struct maze *, SHORT,bool*);
status_t carve_path(SHORT size, COORD,bool *square);
void random_generate_maze(struct maze *, SHORT);

#define smart_stack __attribute__((__cleanup__(viva_maze_free_stack)))
#define smart_maze __attribute__((__cleanup__(viva_maze_free_with_data_ptr)))

// clang-format off
#define at_maze(y, x) 																																																 \
		(*(((maze)->data) + (y) * (maze)->size.X + (x)))
#define for_each_row														  																																		 \
		for (SHORT row = 0; row < (maze)->size.Y; row++)
#define for_each_col 											    																																				 \
		for (SHORT col = 0; col < (maze)->size.X; col++)
#define for_each                                                                                                       \
		for_each_row                                                                                                       \
				for_each_col
// clang-format on