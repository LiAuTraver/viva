#pragma once
#include <net/ancillarycat/viva/utils/console.h>
#include <net/ancillarycat/viva/viva.h>
#include <windows.h>

#pragma pack(push, 1)
typedef struct viva_maze_stack {
	COORD *data;
	size_t capacity;
	size_t size;
} maze_stack;
#pragma pack(pop)

maze_stack *create_maze_stack(size_t);
maze_stack *push_maze_stack(maze_stack *, COORD);

COORD pop_maze_stack(maze_stack *);
COORD top_maze_stack(const maze_stack *);
void	viva_maze_free_stack(maze_stack **);

struct {
	typeof(create_maze_stack) *create;
	typeof(push_maze_stack)		*push;
	typeof(pop_maze_stack)		*pop;
	typeof(top_maze_stack)		*top;
} static const Stack = {
	.create = create_maze_stack,
	.push		= push_maze_stack,
	.pop		= pop_maze_stack,
	.top		= top_maze_stack,
};
