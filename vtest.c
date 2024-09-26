#include "include/ancillarycat/vtest.h"
#include <string.h>
#include "include/ancillarycat/list.h"

// Macro usage
#define foo(...) VIVA__VFUNC(foo, ##__VA_ARGS__)

// Function definitions
int foo_0() {
	printf("Called foo_0\n");
	return 0;
}

int foo_1(const int a) {
	printf("Called foo_1 with %d\n", a);
	return 1;
}

int foo_2(const int a, const int b) {
	printf("Called foo_2 with %d and %d\n", a, b);
	return 2;
}

TEST_SETUP()
TEST(internal, variadic) {
	EXPECT_EQ(foo(), 0);
	EXPECT_EQ(foo(1), 1);
	EXPECT_EQ(foo(1, 2), 2);
}

TEST() {
	val maybe_list_head = List.create(0);
	EXPECT_EQ(maybe_list_head.has_value, true);
	var lh = maybe_list_head.value.ptr;
	List.push_back(lh, 1);
	List.push_back(lh, 2);
	EXPECT_EQ(List.size(lh), 3);
	List.reverse(lh);
	EXPECT_EQ(List.size(lh), 3);
	List.drop(lh);
	EXPECT_EQ(List.size(lh), 2);
	List.reverse(lh);
	EXPECT_EQ(List.size(lh), 2);
	EXPECT_EQ(List.end(lh)->data, 1);
	List.drop(lh);
	EXPECT_EQ(List.size(lh), 1);
	EXPECT_EQ(List.end(lh)->data, 1);
}

TEST(create_list) {
	val maybe_list_head = List.create(0);
	EXPECT_EQ(maybe_list_head.has_value, true);
	EXPECT_EQ(maybe_list_head.value.ptr, nullptr);
}

TEST(push_back) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_back(lh, 1);
	List.push_back(lh, 2);

	EXPECT_EQ(List.size(lh), 2);
	EXPECT_EQ(List.end(lh)->data, 2); // Check the last element
}

TEST(push_front) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_front(lh, 1);
	List.push_front(lh, 2);

	EXPECT_EQ(List.size(lh), 2);
	EXPECT_EQ(List.end(lh)->data, 1); // Check the last element
}

TEST(erase_element) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_back(lh, 1);
	List.push_back(lh, 2);
	List.push_back(lh, 3);

	List.erase_if(lh, 2);
	EXPECT_EQ(List.size(lh), 2);
	EXPECT_EQ(List.end(lh)->data, 3); // Check that 2 was erased
}

TEST(reverse_list) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_back(lh, 1);
	List.push_back(lh, 2);
	List.push_back(lh, 3);

	List.reverse(lh);
	EXPECT_EQ(List.size(lh), 3);
	EXPECT_EQ(List.end(lh)->data, 1); // First element should be 1 after reverse
}

TEST(drop_last) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_back(lh, 1);
	List.push_back(lh, 2);

	List.drop(lh);
	EXPECT_EQ(List.size(lh), 1);
	EXPECT_EQ(List.end(lh)->data, 1); // Last element should be 1
}

TEST(clear_list) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_back(lh, 1);
	List.push_back(lh, 2);

	List.clear(lh);
	EXPECT_EQ(List.size(lh), 0); // List should be empty
}

TEST(free_list) {
	val maybe_list_head = List.create(0);
	var lh = maybe_list_head.value.ptr;

	List.push_back(lh, 1);
	List.free(&lh);
	EXPECT_EQ(lh, nullptr); // List header should be NULL after free
}
