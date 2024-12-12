#include <iso646.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "net/ancillarycat/viva/viva.h"

constexpr val max_size = 100;

void push_to_adjacency_list(int *adjacency_matrix, int *adjacency_list_len, int x, int y) {
	*(adjacency_matrix + x * max_size + ++(*(adjacency_list_len + x))) = y;
}

void push_to_stack(int *stack, int *top, int x) { *(stack + (++(*top))) = x; }

bool topological_sort(int n, int *adjacency_matrix, int *adjacency_list_len, int *stack, int *topological_order,
											int *in_degree, int *visited, int *top) {
	for (int i = 1; i <= n; ++i) {
		if (*(in_degree + i) == 0) {
			push_to_stack(stack, top, i);
		}
	}
	int count = 0;
	while (*top != 0) {
		int x														 = *(stack + (*top)--);
		*(topological_order + (++count)) = x;
		for (int i = 1; i <= *(adjacency_list_len + x); ++i) {
			int y = *(adjacency_matrix + x * max_size + i);
			if (!*(visited + y)) {
				if (--*(in_degree + y) == 0) {
					push_to_stack(stack, top, y);
					*(visited + y) = 1;
				}
			}
		}
	}
	return (count < n) ? false : true;
}

int main() {
	val n = get_rec(Any.int_type);
	val m = get_rec(Any.int_type);

	smart val adjacency_matrix	 = alloc(int, pow(max_size, 2));
	smart val adjacency_list_len = alloc(int, max_size);
	smart val stack							 = alloc(int, max_size);
	smart val topological_order	 = alloc(int, max_size);
	smart val in_degree					 = alloc(int, max_size);
	smart val visited						 = alloc(int, max_size);
	smart val incoming					 = alloc(int, max_size);
	int				top								 = 0;

	for (int i = 0; i < max_size; ++i) {
		*(adjacency_list_len + i) = 0;
		*(in_degree + i)					= 0;
		*(visited + i)						= 0;
	}

	for (int i = 1; i <= m; ++i) {
		val x = get_rec(Any.int_type);
		val y = get_rec(Any.int_type);
		push_to_adjacency_list(adjacency_matrix, adjacency_list_len, x, y);
		(*(in_degree + y))++;
	}

	if (not topological_sort(n, adjacency_matrix, adjacency_list_len, stack, topological_order, in_degree, visited, &top)) {
		println("Cycle detected: no topological order exists.");
		return 0;
	}
	println("Topological order:");
	print("(");
	print(*(topological_order + 1));
	for (int i = 2; i <= n; ++i)
		print(","), print(*(topological_order + i));
	print(")");
	println();
	return 0;
}
