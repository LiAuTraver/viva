#include "include/net/ancillarycat/viva/vtest.h"
#include <string.h>
#include "include/net/ancillarycat/viva/specialized_list.h"
#include "include/net/ancillarycat/viva/generic/list.h"

// Macro usage
#define foo(...) VIVA__VFUNC(foo, ##__VA_ARGS__)
struct list_int_header;
struct list_int_node;

static _Ret_maybe_result(struct list_int_header *) _Ret_notnull_ viva_result_t viva_cstd_impl_list_create_int(
	_In_ const int data);

static _Ret_maybenull_ struct list_int_header *viva_cstd_impl_list_header_empty_int();

static _Ret_maybenull_ struct list_int_node *viva_cstd_impl_list_create_list_node_impl_int(_In_ const int data);

static _Ret_maybe_result(struct list_int_node *) _Ret_notnull_ viva_result_t viva_cstd_impl_list_create_list_node_int(
	_In_ const int data);

static size_t viva_cstd_impl_list_size_int(_In_ const struct list_int_header *const list_header);

static bool viva_cstd_impl_list_is_empty_int(_In_ const struct list_int_header *const list_header);

static viva_status_t viva_cstd_impl_list_push_front_int(_Inout_ struct list_int_header *const list_header,
																												_In_ const int data);

static viva_status_t viva_cstd_impl_list_push_back_int(_Inout_ struct list_int_header *const list_header,
																											 _In_ const int data);

static viva_status_t viva_cstd_impl_list_insert_after_int(_In_ struct list_int_node *const node, _In_ const int data);

static viva_status_t viva_cstd_impl_list_erase_if_int(_Inout_ struct list_int_header *const list_header,
																											_In_ const int data);

static viva_status_t viva_cstd_impl_list_drop_int(_In_ struct list_int_header *const list_header);

static viva_status_t viva_cstd_impl_list_pop_front_int(_Inout_ struct list_int_header *const list_header);

static void viva_cstd_impl_list_clear_int(_Inout_ struct list_int_header *const list_header);

static void viva_cstd_impl_list_free_int(_Inout_ void **const list_header);

static void viva_cstd_impl_list_unique_int(_Inout_ const struct list_int_header *const list_header);

static void viva_cstd_impl_list_reverse_int(_Inout_ struct list_int_header *const list_header);

static _Ret_maybenull_ struct list_int_node *viva_cstd_impl_list_end_int(
	_In_ const struct list_int_header *const list_header);

static _Ret_maybenull_ struct list_int_node *viva_cstd_impl_list_begin_int(
	_In_ const struct list_int_header *const list_header);

static _Ret_maybenull_ struct list_int_node *viva_cstd_impl_list_find_first_if_int(
	_In_ const struct list_int_header *const list_header, _In_ const int data);

struct list_int_node {
	int data;
	struct list_int_node *next;
};

struct list_int_header {
	struct list_int_node *head;
};

_Pragma("pack (push, 1)") struct {
	typeof(viva_cstd_impl_list_create_int) *create_with;
	typeof(viva_cstd_impl_list_header_empty_int) *empty;
	typeof(viva_cstd_impl_list_header_empty_int) *empty_list;
	typeof(viva_cstd_impl_list_create_list_node_int) *create_list_node;
	typeof(viva_cstd_impl_list_size_int) *size;
	typeof(viva_cstd_impl_list_is_empty_int) *is_empty;
	typeof(viva_cstd_impl_list_push_front_int) *push_front;
	typeof(viva_cstd_impl_list_push_back_int) *push_back;
	typeof(viva_cstd_impl_list_insert_after_int) *insert_after;
	typeof(viva_cstd_impl_list_erase_if_int) *erase_if;
	typeof(viva_cstd_impl_list_drop_int) *drop;
	typeof(viva_cstd_impl_list_pop_front_int) *pop_front;
	typeof(viva_cstd_impl_list_clear_int) *clear;
	typeof(viva_cstd_impl_list_free_int) *free;
	typeof(viva_cstd_impl_list_unique_int) *unique;
	typeof(viva_cstd_impl_list_reverse_int) *reverse;
	typeof(viva_cstd_impl_list_begin_int) *begin;
	typeof(viva_cstd_impl_list_end_int) *end;
	typeof(viva_cstd_impl_list_find_first_if_int) *find_first_if;
} static List_of_int = {
	.create_with = viva_cstd_impl_list_create_int, .empty = viva_cstd_impl_list_header_empty_int,
	.empty_list = viva_cstd_impl_list_header_empty_int, .create_list_node = viva_cstd_impl_list_create_list_node_int,
	.size = viva_cstd_impl_list_size_int, .is_empty = viva_cstd_impl_list_is_empty_int,
	.push_front = viva_cstd_impl_list_push_front_int, .push_back = viva_cstd_impl_list_push_back_int,
	.insert_after = viva_cstd_impl_list_insert_after_int, .erase_if = viva_cstd_impl_list_erase_if_int,
	.drop = viva_cstd_impl_list_drop_int, .pop_front = viva_cstd_impl_list_pop_front_int,
	.clear = viva_cstd_impl_list_clear_int, .free = viva_cstd_impl_list_free_int,
	.unique = viva_cstd_impl_list_unique_int, .reverse = viva_cstd_impl_list_reverse_int,
	.begin = viva_cstd_impl_list_begin_int, .end = viva_cstd_impl_list_end_int,
	.find_first_if = viva_cstd_impl_list_find_first_if_int
};

_Pragma("pack (pop)") inline struct list_int_node *viva_cstd_impl_list_begin_int(
	const struct list_int_header *const list_header) {
	VIVA_RUNTIME_REQUIRE(list_header && "List header is null.");
	return list_header->head;
}

inline struct list_int_node *viva_cstd_impl_list_find_first_if_int(const struct list_int_header *const list_header,
																																	 const int data) {
	VIVA_RUNTIME_REQUIRE(list_header && "List header is null.");
	for (var current = list_header->head; current; current = current->next) if (current->data == data) return current;
	return nullptr;
}

inline viva_result_t viva_cstd_impl_list_create_int(const int data) {
	auto maybe_node = viva_cstd_impl_list_create_list_node_impl_int(data);
	if (maybe_node == nullptr)
		VIVA_RETURN_ERROR(kBadAlloc, "Failed to allocate memory for list node.")
	val header = viva_cstd_impl_list_header_empty_int();
	if (header) {
		header->head = maybe_node;
		VIVA_RETURN_VALUE_PTR(header)
	}
	VIVA_RETURN_ERROR(kBadAlloc, "Failed to allocate memory for list header.")
}

inline viva_status_t viva_cstd_impl_list_push_back_int(struct list_int_header *const list_header, const int data) {
	val maybe_node = viva_cstd_impl_list_create_list_node_impl_int(data);
	if (maybe_node == nullptr) return kBadAlloc;
	val end_node = viva_cstd_impl_list_end_int(list_header);
	if (end_node == nullptr) {
		list_header->head = maybe_node;
		return kOkStatus;
	}
	end_node->next = maybe_node;
	return kOkStatus;
}

inline struct list_int_node *viva_cstd_impl_list_create_list_node_impl_int(const int data) {
	val node = (struct list_int_node *) alloc_fwd(sizeof(struct list_int_node));
	node->data = data;
	node->next = nullptr;
	return node;
}

inline viva_result_t viva_cstd_impl_list_create_list_node_int(const int data) {
	auto node = viva_cstd_impl_list_create_list_node_impl_int(data);
	if (node == nullptr)
		VIVA_RETURN_ERROR(kBadAlloc, "Failed to allocate memory for list node.")
	node->data = data;
	node->next = nullptr;
	VIVA_RETURN_VALUE(node)
}

inline size_t viva_cstd_impl_list_size_int(const struct list_int_header *const list_header) {
	size_t idx = 0;
	var cur = (struct list_int_node *) list_header->head;
	while (cur) {
		idx++;
		cur = cur->next;
	}
	return idx;
}

inline bool viva_cstd_impl_list_is_empty_int(const struct list_int_header *const list_header) {
	return list_header && list_header->head;
}

inline viva_status_t viva_cstd_impl_list_push_front_int(struct list_int_header *const list_header, const int data) {
	if (list_header == nullptr) return kOutOfRange;
	val maybe_node = viva_cstd_impl_list_create_list_node_impl_int(data);
	if (maybe_node) {
		maybe_node->next = list_header->head;
		list_header->head = maybe_node;
		return kOkStatus;
	}
	return kBadAlloc;
}

inline viva_status_t viva_cstd_impl_list_insert_after_int(struct list_int_node *const node, const int data) {
	if (node == nullptr) return kOutOfRange;
	struct list_int_node *maybe_node = viva_cstd_impl_list_create_list_node_impl_int(data);
	if (maybe_node) {
		maybe_node->next = node->next;
		node->next = maybe_node;
		return kOkStatus;
	}
	return kBadAlloc;
}

inline viva_status_t viva_cstd_impl_list_erase_if_int(struct list_int_header *const list_header, const int data) {
	bool cnt = 0;
	if (list_header == nullptr) return kEmpty;
	if (list_header->head == nullptr) return kOutOfRange;
	var before = list_header->head;
	while (before->next) {
		if (before->next->data == data) {
			val next = before->next;
			before->next = before->next->next;
			free(next);
			cnt = 1;
		} else before = before->next;
	}
	if (before->data == data) {
		val next = before->next;
		free(before);
		list_header->head = next;
		cnt = 1;
	}
	return cnt ? kOkStatus : kNotFound;
}

inline viva_status_t viva_cstd_impl_list_drop_int(struct list_int_header *const list_header) {
	var maybe_node_before_end = list_header->head;
	if (maybe_node_before_end == nullptr) return kEmpty;
	if (maybe_node_before_end->next == nullptr) {
		free(maybe_node_before_end);
		list_header->head = nullptr;
		return kOkStatus;
	}
	while (maybe_node_before_end->next->next) { maybe_node_before_end = maybe_node_before_end->next; }
	free(maybe_node_before_end->next);
	maybe_node_before_end->next = nullptr;
	return kOkStatus;
}

inline viva_status_t viva_cstd_impl_list_pop_front_int(struct list_int_header *const list_header) {
	if (list_header == nullptr) { return kEmpty; }
	if (list_header->head == nullptr) { return kOutOfRange; }
	val next = list_header->head->next;
	free(list_header->head);
	list_header->head = next;
	return kOkStatus;
}

inline void viva_cstd_impl_list_clear_int(struct list_int_header *const list_header) {
	for (var current = list_header->head; current;) {
		val next = current->next;
		free(current);
		current = next;
	}
	list_header->head = nullptr;
}

inline void viva_cstd_impl_list_free_int(void **const list_header) {
	viva_cstd_impl_list_clear_int(*list_header);
	free(*list_header);
	*list_header = nullptr;
}

inline void viva_cstd_impl_list_unique_int(const struct list_int_header *const list_header) {
	for (var current = list_header->head; current;) for (var next = current->next; next;) if (current->data != next->data)
		current = next;
	else {
		val dup = next;
		current->next = next->next;
		free(dup);
	}
}

inline void viva_cstd_impl_list_reverse_int(struct list_int_header *const list_header) {
	switch (viva_cstd_impl_list_size_int(list_header)) {
		case 0: [[fallthrough]];
		case 1: return;
		case 2: default: break;
	}
	val original_end_node = viva_cstd_impl_list_end_int(list_header);
	struct list_int_node *current;
	while (list_header->head != original_end_node) {
		current = list_header->head;
		list_header->head = list_header->head->next;
		current->next = original_end_node->next;
		original_end_node->next = current;
	}
}

inline struct list_int_node *viva_cstd_impl_list_end_int(const struct list_int_header *const list_header) {
	if (list_header == nullptr) return nullptr;
	var current = list_header->head;
	if (current == nullptr) return current;
	while (current->next) current = current->next;
	return current;
}

inline struct list_int_header *viva_cstd_impl_list_header_empty_int() {
	val lh = (struct list_int_header *) alloc_fwd(sizeof(struct list_int_header));
	if (lh) return ( {
		lh->head = nullptr;
		lh;
	});
	VIVA_RUNTIME_REQUIRE(false, "Failed to allocate memory for list header.");
	__builtin_expect(false, 0);
	return nullptr;
};
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
