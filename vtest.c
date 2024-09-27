#include "include/net/ancillarycat/viva/vtest.h"
#include <string.h>
#include "include/net/ancillarycat/viva/specialized_list.h"
#include "include/net/ancillarycat/viva/generic/list.h"

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
instantiate(int)
TEST_SETUP()
TEST(internal, variadic) {
	EXPECT_EQ(foo(), 0);
	EXPECT_EQ(foo(1), 1);
	EXPECT_EQ(foo(1, 2), 2);
}

TEST(list, basic) {
	val maybe_list_head = List_of_int.create_with(0);
	EXPECT_EQ(maybe_list_head.has_value, true);
	var lh = maybe_list_head.value.ptr;
	List_of_int.push_back(lh, 1);
	List_of_int.push_back(lh, 2);
	EXPECT_EQ(List_of_int.size(lh), 3);
	List_of_int.reverse(lh);
	EXPECT_EQ(List_of_int.size(lh), 3);
	List_of_int.drop(lh);
	EXPECT_EQ(List_of_int.size(lh), 2);
	List_of_int.reverse(lh);
	EXPECT_EQ(List_of_int.size(lh), 2);
	EXPECT_EQ(List_of_int.end(lh)->data, 2);
	List_of_int.drop(lh);
	EXPECT_EQ(List_of_int.size(lh), 1);
	EXPECT_EQ(List_of_int.end(lh)->data, 1);
}

TEST(list, create) {
	val maybe_list_head = List_of_int.create_with(0);
	EXPECT_EQ(maybe_list_head.has_value, true);
	EXPECT_EQ(List_of_int.begin(maybe_list_head.value.ptr)->data, 0);
}

TEST(list, push_back) {
	val maybe_list_head = List_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	List_of_int.push_back(lh, 1);
	List_of_int.push_back(lh, 2);

	EXPECT_EQ(List_of_int.size(lh), 3);
	EXPECT_EQ(List_of_int.end(lh)->data, 2); // Check the last element
}

TEST(list, push_front) {
	val maybe_list_head = List_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	List_of_int.push_front(lh, 1);
	List_of_int.push_front(lh, 2);

	EXPECT_EQ(List_of_int.size(lh), 3);
	EXPECT_EQ(List_of_int.end(lh)->data, 0); // Check the last element
}

TEST(list, erase) {
	val maybe_list_head = List_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	List_of_int.push_back(lh, 1);
	List_of_int.push_back(lh, 2);
	List_of_int.push_back(lh, 3);

	List_of_int.erase_if(lh, 2);
	EXPECT_EQ(List_of_int.size(lh), 3);
	EXPECT_NULL(List_of_int.find_first_if(lh, 2)); // Check if 2 is erased
}

TEST(list, drop) {
	val maybe_list_head = List_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	List_of_int.push_back(lh, 1);
	List_of_int.push_back(lh, 2);

	List_of_int.drop(lh);
	EXPECT_EQ(List_of_int.size(lh), 2);
	EXPECT_EQ(List_of_int.end(lh)->data, 1); // the last element should be 1
}

TEST(list, clear) {
	val maybe_list_head = List_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	List_of_int.push_back(lh, 1);
	List_of_int.push_back(lh, 2);

	List_of_int.clear(lh);
	EXPECT_EQ(List_of_int.size(lh), 0);
	EXPECT_NULL(List_of_int.begin(lh));
}

TEST(list, free) {
	val maybe_list_head = List_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	List_of_int.push_back(lh, 1);
	List_of_int.free(&lh);
	EXPECT_NULL(lh);
}
