#include <ctype.h>
#include <net/ancillarycat/viva/viva.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "treev.h"

void preorder_traversal(const Node *node) {
	if (node == NULL) {
		return;
	}
	fprintf(stdout, "%c", node->data);
	preorder_traversal(node->left);
	preorder_traversal(node->right);
}


int main() {
	println("Please input a string:");
	fflush(stdout);
	val chars = get_rec(Any.char_ptr_type, 100);
	trim_str(chars);
	println("Your input was:");
	fprintf(stdout, "%s\n", chars);
	fflush(stdout);
	println("Make your string as a binary tree:");
	smart var nodes = alloc(Node, strlen(chars));
	var				dep		= 0ll;
	build_tree(&nodes, &chars, &dep);
	println("Depth:");
	println(dep);
	println("Preorder traversal:");
	preorder_traversal(&nodes[0]);
}
