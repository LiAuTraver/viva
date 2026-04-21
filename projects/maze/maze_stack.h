#pragma once
#include <windows.h>

struct maze_stack {
	COORD *data;
	size_t capacity;
	size_t size;
};

struct maze_stack *create_maze_stack(size_t);
struct maze_stack *push_maze_stack(struct maze_stack *, COORD);
COORD							 pop_maze_stack(struct maze_stack *);
COORD							 top_maze_stack(const struct maze_stack *);
void							 viva_maze_free_stack(struct maze_stack **);

struct {
	typeof(create_maze_stack) *const create;
	typeof(push_maze_stack) *const	 push;
	typeof(pop_maze_stack) *const		 pop;
	typeof(top_maze_stack) *const		 top;
} static const Stack = {
	.create = create_maze_stack,
	.push		= push_maze_stack,
	.pop		= pop_maze_stack,
	.top		= top_maze_stack,
};
