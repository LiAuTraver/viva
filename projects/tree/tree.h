#pragma once
#include <ctype.h>
#include <iso646.h>
#include <locale.h>
#include <net/ancillarycat/viva/viva.h>

#ifndef ssize_t
#define ssize_t long long
#endif

typedef struct viva_node Node;
static constexpr auto		 left_corner = 0x231E;
static inline void			 trim_str(char *str) __attribute__((nonnull(1)));
static inline void			 display_tree(const Node *node, ssize_t depth, bool *is_right) __attribute__((nonnull(1, 3)));
static inline void			 build_tree(Node **nodes, char *const *str, ssize_t *depth) __attribute__((nonnull(1, 2, 3)));
static inline void			 terminal_init();

typedef struct viva_node {
	char							data;
	struct viva_node *left;
	struct viva_node *right;
} Node;

void display_tree(const Node *node, const ssize_t depth, bool *const is_right) { // NOLINT(misc-no-recursion)
	if (not node)
		return (void)fprintf(stdout, "(null)\n");

	// Print the indentation for the current node
	for (ssize_t i = 1ll; i < depth - 1; ++i)
		if (*(is_right + i))
			fprintf(stdout, "    ");
		else
			fprintf(stdout, "|   ");

	// If this isn't the root node, print the connecting line
	if (depth > 1)
		fprintf(stdout, "|---");

	// Print the current node's data
	fprintf(stdout, "%c\n", node->data);

	if (node->left)
		*(is_right + depth - 1 + 1) = false, display_tree(node->left, depth + 1, is_right);

	if (node->right)
		return ({
			if (node->right->left or node->right->right) {
				for (ssize_t i = 1ll; i < depth; ++i)
					if (*(is_right + i))
						fprintf(stdout, "    ");
					else
						fprintf(stdout, "|   ");
				fprintf(stdout, "|\n");
			}
			*(is_right + depth - 1 + 1) = true, display_tree(node->right, depth + 1, is_right);
		});
}

void trim_str(char *const str) {
	smart var new_str = alloc(char, strlen(str) + 1);
	var				cur_ptr = new_str;
	for (var i = 0ll; i < strlen(str); ++i)
		if (isalnum(*(str + i)))
			*(cur_ptr++) = *(str + i);
	*cur_ptr = '\0';
	memmove(str, new_str, strlen(new_str) + 1);
}

void terminal_init() {
	setlocale(LC_CTYPE, "");
	fflush(stdout);
}

void build_tree(Node **nodes, char *const *str, ssize_t *depth) {
	if (not strlen(*str))
		*(nodes + 0) = nullptr;
	// 2^dep <= strlen(chars) < 2^(dep+1)
	// clang-format off
	for (var i = 0; i < strlen(*str); ++i)
		(*nodes + i)->data = *(*str + i),
		(*nodes + i)->left = 2 * i + 1 < strlen(*str) ? *nodes + 2 * i + 1 : nullptr,
		(*nodes + i)->right = 2 * i + 2 < strlen(*str) ? *nodes + 2 * i + 2 : nullptr;
	// clang-format on
	while (1ll << (*depth) <= strlen(*str))
		++*depth;
}
