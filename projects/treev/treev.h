#pragma once
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct viva_node Node;

void	 preorder_traversal(Node *) __attribute__((nonnull(1)));
size_t split_input(char *, char **, const char *) __attribute__((nonnull(1, 2, 3)));
Node	*current_node(Node *, char *) __attribute__((nonnull(1, 2)));
void	 print_tree(Node *) __attribute__((nonnull(1)));
bool	 is_unique_node(Node *, char **, size_t) __attribute__((nonnull(1, 2)));
bool	 is_valid_node(Node *, char **, size_t, Node *) __attribute__((nonnull(1, 2, 4)));
void	 print_intro(size_t, size_t);
void	 print_error_cnt0(void);
void	 add_children(char **, size_t, Node *) __attribute__((nonnull(1, 3)));


typedef struct viva_node {
	char							*data;
	struct viva_node **childs;
	size_t						 child_count;
} Node;

inline size_t split_input(char *str, char **tokens, const char *delimitor) {
	errno							= 0;
	var		token_count = 0ull;
	char *token				= strtok(str, delimitor);

	while (token && not errno) {
		*(tokens + token_count++) = token;
		token											= strtok(nullptr, delimitor); // Get next token
	}

	if (errno)
		return 0;
	return token_count;
}

inline Node *current_node(Node *cur, char *tokens) {
	if (strcmp(cur->data, tokens) == 0)
		return cur;
	for (var i = 0ull; i < cur->child_count; ++i) {
		Node *node = current_node(*(cur->childs + i), tokens);
		if (node)
			return node;
	}
	return nullptr;
}

/// @brief print function, root --> child1 --> child2 ...
inline void print_tree(Node *root) {
	if (not root)
		return;
	printf("%s", root->data);

	if (root->child_count)
		for (var i = 0ull; i < root->child_count; ++i)
			printf(" --> %s", (*(root->childs + i))->data);
	else
		printf(" (no child)");

	putchar('\n');
	for (var i = 0ull; i < root->child_count; ++i)
		print_tree(*(root->childs + i));
}

inline bool is_unique_node(Node *head, char **tokens, const size_t token_count) {
	for (var i = 1ull; i < token_count; ++i) {
		if (current_node(head, *(tokens + i))) {
			fprintf(stderr, "Error: Duplicate Node '%s'; please input again.\n", *(tokens + i));
			fflush(stderr);
		}
	}
	return true;
}
inline void preorder_traversal(Node *root) {
	if (not root)
		return;
	printf("%s ", root->data);
	for (var i = 0ull; i < root->child_count; ++i)
		preorder_traversal(*(root->childs + i));
}

inline void print_intro(const size_t max_token_count, const size_t max_input_length) {
	println("Please input the tree in the following format: root child1 child2 ...");
	println("Enter Ctrl+D(Linux) or Ctrl+Z(Windows) to finish input.");
	printf("Note: the maximum number of children is %llu and the maximum length of each node is %llu.\n",
				 max_token_count - 1, max_input_length - 1);
}

inline bool is_valid_node(Node *head, char **tokens, const size_t token_count, Node *current) {
	if (not current) {
		fprintf(stderr, "Error: Root Node '%s' not found in existing tree\n", *tokens);
		fflush(stderr);
		return true;
	}
	if (current->child_count) {
		fprintf(stderr, "Error: Child Node of '%s' already exists. Ignoring\n", *tokens);
		fflush(stderr);
		return true;
	}
	return is_unique_node(head, tokens, token_count) ? false : true;
}


inline void print_error_cnt0(void) {
	if (errno) {
		fprintf(stderr, "Error: %s\n", strerror(errno)), fflush(stderr);
		errno = 0;
	} else {
		fprintf(stderr, "Error: No input found\n");
		fflush(stderr);
	}
}
inline void add_children(char **tokens, const size_t token_count, Node *current) {
	current->childs			 = alloc(Node *, token_count - 1);
	current->child_count = token_count - 1;

	for (var i = 1ull; i < token_count; ++i) {
		// create new node
		*(current->childs + i - 1)				 = alloc(Node, 1);
		(*(current->childs + i - 1))->data = alloc(char, strlen(*(tokens + i)) + 1);
		strcpy((*(current->childs + i - 1))->data, *(tokens + i));
		(*(current->childs + i - 1))->child_count = 0;
		(*(current->childs + i - 1))->childs			= nullptr;
	}
}
