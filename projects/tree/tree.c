#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <net/ancillarycat/viva/viva.h>
#include "tree.h"


int main() {
	println("Please input a string:");
	fflush(stdout);
	val chars = get_rec(Any.char_ptr_type, 100);
	trim_str(chars);
	println("Your input was:");
	fprintf(stdout, "%s\n", chars);
	println("Make your string as a binary tree:");
	smart var nodes = alloc(Node, strlen(chars));
	var				dep		= 0ll;
	build_tree(&nodes, &chars, &dep);
	println("Depth:");
	println(dep);
	val is_right_str = alloc(bool, dep);
	is_right_str[0]	 = 0;
	display_tree(nodes, 1, is_right_str);
	getchar();
}
