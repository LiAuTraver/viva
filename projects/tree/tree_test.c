#include "tree.h"
#include <../include/net/ancillarycat/viva/forward_list.h>
#include <../include/net/ancillarycat/viva/viva.h>

TEST_SETUP()

TEST(simple){
	val str = alloc(char, 100);
	strcpy(str, "Hello, World!");
	trim_str(str);
	EXPECT_EQ(strcmp(str, "HelloWorld"), 0);
	var nodes = alloc(Node, strlen(str));
	var depth = 1ll;
	build_tree(&nodes, &str, &depth);
	EXPECT_EQ(depth,4);
	val is_right_str = alloc(bool, depth);
	*is_right_str = 0;
	display_tree(nodes,1,is_right_str);
}

TEST(no_alnum){
	val str = alloc(char, 100);
	strcpy(str, "!@#$%^&*()_+{}|:\"<>?`~");
	trim_str(str);
	EXPECT_EQ(strcmp(str, ""), 0);
	var nodes = alloc(Node, strlen(str));
	var depth = 1ll;
	build_tree(&nodes, &str, &depth);
	EXPECT_EQ(depth,1);
	val is_right_str = alloc(bool, depth);
	*is_right_str = 0;
	display_tree(nodes,1,is_right_str);
}

TEST(super_long) {
	val str = alloc(char, 100);
	strcpy(str, "`1234567890-=	qwertyuiop[]\asdfghjkl;'zxcvbnm,./~!@#$%^&*()_+	QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>?	");
	trim_str(str);
	EXPECT_EQ(strcmp(str, "1234567890qwertyuiopsdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"), 0);
	var nodes = alloc(Node, strlen(str));
	var depth = 1ll;
	build_tree(&nodes, &str, &depth);
	EXPECT_EQ(depth, 6);
	val is_right_str = alloc(bool, depth);
	*is_right_str = 0;
	display_tree(nodes, 1, is_right_str);
}