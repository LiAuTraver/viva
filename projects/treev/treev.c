#include <ctype.h>
#include <iso646.h>
#include <net/ancillarycat/viva/viva.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "treev.h"

constexpr val max_token_count	 = 16ull;
constexpr val max_input_length = 128ull;


int main() {
	var		input = alloc(char, max_input_length);
	Node *head	= nullptr;
	print_intro(max_token_count, max_input_length);
	while (fgets(input, max_input_length, stdin)) {
		*(input + strcspn(input, "\r\n")) = '\0';
		val tokens = alloc(char*,max_token_count);
		val		 token_count								= split_input(input, tokens, " ");
		if (not token_count) {
			print_error_cnt0();
			continue;
		}
		if (token_count == 1) {
			fprintf(stderr, "Error: No child found for '%s'\n", *tokens);
			fflush(stderr);
			continue;
		}

		Node *current;
		if (not head) {
			head					= alloc(Node, 1);
			current				= head;
			current->data = alloc(char, strlen(*tokens) + 1);
			strcpy(current->data, *tokens);
		} else if (current = current_node(head, *tokens), is_valid_node(head, tokens, token_count, current))
			continue;

		add_children(tokens, token_count, current);

		// debug print
		// printf("Current Node: %s\n", current->data);
		// for (var i = 0ull; i < current->child_count; ++i)
		// 	printf("Child Node: %s\n", (*(current->childs + i))->data);
	}

	println("The Child-table(root -> child1 -> child2 ...) is as follows:");
	print_tree(head);
	println();
	println("Preorder Traversal of the tree is as follows:");
	preorder_traversal(head);
	println();
	fflush(stdout);
	getchar();
}
