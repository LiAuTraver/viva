#include "maze_stack.h"

maze_stack *create_maze_stack(const size_t capacity) {
	contract_assert(capacity > 0);

	var stack_ptr = alloc(maze_stack, 1);
	contract_assert(stack_ptr);

	stack_ptr->data = alloc(COORD, capacity);
	contract_assert(stack_ptr->data);

	stack_ptr->capacity = capacity;
	stack_ptr->size			= 0;
	return stack_ptr;
}
maze_stack *push_maze_stack(maze_stack *stack, const COORD coord) {
	contract_assert(stack->size < stack->capacity && (bool)"Stack overflow");

	stack->data[stack->size++] = coord;
	return stack;
}
COORD pop_maze_stack(maze_stack *stack) {
	contract_assert(stack->size > 0 && (bool)"Stack underflow");

	return stack->data[--stack->size];
}
COORD top_maze_stack(const maze_stack *stack) {
	contract_assert(stack->size > 0 && (bool)"Stack is empty");

	return stack->data[stack->size - 1];
}
void viva_maze_free_stack(maze_stack **stack) {
	contract_assert(*stack && (bool)"Invalid stack");

	free((*stack)->data);
	free(*stack);
}
