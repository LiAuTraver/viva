#include "../include/net/ancillarycat/viva/vtest.h"
#include <string.h>
#include "../include/net/ancillarycat/viva/generic/forward_list.h"

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
bool predicate_of_erase_if(void* data) {
	return *(int*)data == 2;
}

instantiate_forward_list_of(int)

TEST_SETUP()
TEST(internal, variadic_macro) {
	EXPECT_EQ(foo(), 0);
	EXPECT_EQ(foo(1), 1);
	EXPECT_EQ(foo(1, 2), 2);
}

TEST(forward_list, basic) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	EXPECT_EQ(maybe_list_head.has_value, true);
	var lh = maybe_list_head.value.ptr;
	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.push_back(lh, 2);
	EXPECT_EQ(Forward_list_of_int.size(lh), 3);
	Forward_list_of_int.reverse(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 3);
	Forward_list_of_int.drop(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 2);
	Forward_list_of_int.reverse(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 2);
	EXPECT_EQ(*(int*)Forward_list_of_int.end(lh)->data_ptr, 2);
	Forward_list_of_int.drop(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 1);
	EXPECT_EQ(*(int*)Forward_list_of_int.end(lh)->data_ptr, 1);
}

TEST(forward_list, create) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	EXPECT_EQ(maybe_list_head.has_value, true);
	EXPECT_EQ(*(int*)Forward_list_of_int.begin(maybe_list_head.value.ptr)->data_ptr, 0);
}

TEST(forward_list, push_back) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.push_back(lh, 2);

	EXPECT_EQ(Forward_list_of_int.size(lh), 3);
	EXPECT_EQ(*(int*)Forward_list_of_int.end(lh)->data_ptr, 2); // Check the last element
}

TEST(forward_list, push_front) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_front(lh, 1);
	Forward_list_of_int.push_front(lh, 2);

	EXPECT_EQ(Forward_list_of_int.size(lh), 3);
	EXPECT_EQ(*(int*)Forward_list_of_int.end(lh)->data_ptr, 0); // Check the last element
}

TEST(forward_list, erase) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.push_back(lh, 2);
	Forward_list_of_int.push_back(lh, 3);

	Forward_list_of_int.erase_if(lh, predicate_of_erase_if);
	EXPECT_EQ(Forward_list_of_int.size(lh), 3);
	EXPECT_NULL(Forward_list_of_int.find_first_if(lh, predicate_of_erase_if)); // Check if 2 is erased
}

TEST(forward_list, drop) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.push_back(lh, 2);

	Forward_list_of_int.drop(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 2);
	EXPECT_EQ(*(int*)Forward_list_of_int.end(lh)->data_ptr, 1); // the last element should be 1
}

TEST(forward_list, clear) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.push_back(lh, 2);

	Forward_list_of_int.clear(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 0);
	EXPECT_NULL(Forward_list_of_int.begin(lh));
}

TEST(forward_list, free) {
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.free(&lh);
	EXPECT_NULL(lh);
}

TEST(forward_list, unique){
	val maybe_list_head = Forward_list_of_int.create_with(0);
	var lh = maybe_list_head.value.ptr;

	Forward_list_of_int.push_back(lh, 2);
	Forward_list_of_int.push_back(lh, 1);
	Forward_list_of_int.push_back(lh, 5);
	Forward_list_of_int.push_back(lh, 2);
	Forward_list_of_int.push_back(lh, 3);
	Forward_list_of_int.push_back(lh, 1);

	Forward_list_of_int.unique(lh);
	EXPECT_EQ(Forward_list_of_int.size(lh), 5);
}
