#pragma once

#include <stdlib.h>
#include "viva.h"
#ifdef VIVA_TEST_FRAMEWORK_ENABLED
#include "vtest.h"
#endif
#define _Ret_maybe_result(expr) // nothing
#if __has_include(<sal.h>)
#include <sal.h>
#else
#define _In_
#define _Out_
#define _Inout_
#endif
struct list_node;

struct list_header;

static _Ret_maybe_result(struct list_header *) _Ret_notnull_
result_t viva_cstd_impl_list_create(_In_ const int);

static _Ret_maybenull_
struct list_header *viva_cstd_impl_list_header_empty();

static _Ret_maybenull_ struct list_node *viva_cstd_impl_list_create_list_node_impl(_In_ const int);

static _Ret_maybe_result(struct list_node *) _Ret_notnull_
result_t viva_cstd_impl_list_create_list_node(_In_ const int);

static size_t viva_cstd_impl_list_size(_In_ const struct list_header *const);

static bool viva_cstd_impl_list_is_empty(_In_ const struct list_header *const);

static status_t viva_cstd_impl_list_push_front(_Inout_ struct list_header *const list_header, _In_ const int);

static status_t viva_cstd_impl_list_push_back(_Inout_ struct list_header *const, _In_ const int);

static status_t viva_cstd_impl_list_insert_after(_In_ struct list_node *const, _In_ const int);

static status_t viva_cstd_impl_list_erase_if(_Inout_ struct list_header *const, _In_ const int);

static status_t viva_cstd_impl_list_drop(_In_ struct list_header *const);

static status_t viva_cstd_impl_list_pop_front(_Inout_ struct list_header *const);

static void viva_cstd_impl_list_clear(_Inout_ struct list_header *const);

static void viva_cstd_impl_list_free(_Inout_ struct list_header *const);

static void viva_cstd_impl_list_unique(_Inout_ const struct list_header *const);

static void viva_cstd_impl_list_reverse(_Inout_ struct list_header *const);

static _Ret_maybenull_
struct list_node *viva_cstd_impl_list_end(_In_ const struct list_header *const);

static void *alloc_fwd(_In_ const size_t);

struct list_node {
	int data;
	struct list_node *next;
};

struct list_header {
	struct list_node *head;
};


#pragma pack(push, 1)
struct {
	typeof(viva_cstd_impl_list_create) *create;
	typeof(viva_cstd_impl_list_header_empty) *empty;
	typeof(viva_cstd_impl_list_create_list_node) *create_list_node;
	typeof(viva_cstd_impl_list_size) *size;
	typeof(viva_cstd_impl_list_is_empty) *is_empty;
	typeof(viva_cstd_impl_list_push_front) *push_front;
	typeof(viva_cstd_impl_list_push_back) *push_back;
	typeof(viva_cstd_impl_list_insert_after) *insert_after;
	typeof(viva_cstd_impl_list_erase_if) *erase_if;
	typeof(viva_cstd_impl_list_drop) *drop;
	typeof(viva_cstd_impl_list_pop_front) *pop_front;
	typeof(viva_cstd_impl_list_clear) *clear;
	typeof(viva_cstd_impl_list_free) *free;
	typeof(viva_cstd_impl_list_unique) *unique;
	typeof(viva_cstd_impl_list_reverse) *reverse;
	typeof(viva_cstd_impl_list_end) *end;
} static List = {
	.create = viva_cstd_impl_list_create,
	.empty = viva_cstd_impl_list_header_empty,
	.create_list_node = viva_cstd_impl_list_create_list_node,
	.size = viva_cstd_impl_list_size,
	.is_empty = viva_cstd_impl_list_is_empty,
	.push_front = viva_cstd_impl_list_push_front,
	.push_back = viva_cstd_impl_list_push_back,
	.insert_after = viva_cstd_impl_list_insert_after,
	.erase_if = viva_cstd_impl_list_erase_if,
	.drop = viva_cstd_impl_list_drop,
	.pop_front = viva_cstd_impl_list_pop_front,
	.clear = viva_cstd_impl_list_clear,
	.free = viva_cstd_impl_list_free,
	.unique = viva_cstd_impl_list_unique,
	.reverse = viva_cstd_impl_list_reverse,
	.end = viva_cstd_impl_list_end
};
#pragma pack(pop)

inline void *alloc_fwd(_In_ const size_t size) {
	return malloc(size);
}

inline result_t viva_cstd_impl_list_create(const int data) {
	auto maybe_node = viva_cstd_impl_list_create_list_node_impl(data);
	if (maybe_node == nullptr)
		// node allocation failed
		return_error(
		kBadAlloc,
		"Failed to allocate memory for list node."
	)
	val header = viva_cstd_impl_list_header_empty();
	if (header) {
		header->head = maybe_node;
		return_value_ptr(
			header
		)
	}
	// header allocation failed
	return_error(
		kBadAlloc,
		"Failed to allocate memory for list header."
	)
}

inline status_t viva_cstd_impl_list_push_back(struct list_header *const list_header, const int data) {
	val maybe_node = viva_cstd_impl_list_create_list_node_impl(data);
	if (maybe_node == nullptr)
		return kBadAlloc;
	val end_node = viva_cstd_impl_list_end(list_header);
	if (end_node == nullptr) {
		list_header->head = maybe_node;
		return kOkStatus;
	}
	end_node->next = maybe_node;
	return kOkStatus;
}

inline struct list_node *viva_cstd_impl_list_create_list_node_impl(const int data) {
	return alloc_fwd(sizeof(struct list_node));
}

inline result_t viva_cstd_impl_list_create_list_node(const int data) {
	auto node = viva_cstd_impl_list_create_list_node_impl(data);
	if (node == nullptr)
		return_error(
		kBadAlloc,
		"Failed to allocate memory for list node."
	)
	node->data = data;
	node->next = nullptr;
	return_value_ptr(
		node
	)
}

inline size_t viva_cstd_impl_list_size(const struct list_header *const list_header) {
	size_t idx = 0;
	var cur = (struct list_node *) list_header->head;
	while (cur) {
		idx++;
		cur = cur->next;
	}
	return idx;
}

inline bool viva_cstd_impl_list_is_empty(const struct list_header *const list_header) {
	return list_header && list_header->head;
}

inline status_t viva_cstd_impl_list_push_front(struct list_header *const list_header, const int data) {
	if (list_header == nullptr)
		return kOutOfRange;
	val maybe_node = viva_cstd_impl_list_create_list_node_impl(data);
	if (maybe_node) {
		maybe_node->next = list_header->head;
		list_header->head = maybe_node;
		return kOkStatus;
	}
	return kBadAlloc;
}

inline status_t viva_cstd_impl_list_insert_after(struct list_node *const node, const int data) {
	if (node == nullptr)
		return kOutOfRange;
	struct list_node *maybe_node = viva_cstd_impl_list_create_list_node_impl(data);
	if (maybe_node) {
		maybe_node->next = node->next;
		node->next = maybe_node;
		return kOkStatus;
	}
	return kBadAlloc;
}

inline status_t viva_cstd_impl_list_erase_if(struct list_header *const list_header, const int data) {
	bool cnt = 0;
	if (list_header == nullptr)
		return kEmpty;
	if (list_header->head == nullptr)
		return kOutOfRange;
	var before = list_header->head;
	while (before->next && before->next->data == data) {
		val next = before->next;
		before->next = before->next->next;
		free(next);
		cnt = 1;
	}
	if (before->data == data) {
		val next = before->next;
		free(before);
		list_header->head = next;
		cnt = 1;
	}
	return cnt ? kOkStatus : kNotFound;
}

inline status_t viva_cstd_impl_list_drop(struct list_header *const list_header) {
	var maybe_node_before_end = list_header->head;
	if (maybe_node_before_end == nullptr)
		return kEmpty;
	if (maybe_node_before_end->next == nullptr) {
		free(maybe_node_before_end);
		list_header->head = nullptr;
		return kOkStatus;
	}
	while (maybe_node_before_end->next->next) {
		maybe_node_before_end = maybe_node_before_end->next;
	}
	free(maybe_node_before_end->next);
	maybe_node_before_end->next = nullptr;
	return kOkStatus;
}

inline status_t viva_cstd_impl_list_pop_front(struct list_header *const list_header) {
	if (list_header == nullptr) {
		return kEmpty;
	}
	if (list_header->head == nullptr) {
		return kOutOfRange;
	}
	val next = list_header->head->next;
	free(list_header->head);
	list_header->head = next;
	return kOkStatus;
}

inline void viva_cstd_impl_list_clear(struct list_header *const list_header) {
	for (var current = list_header->head; current; /* nothing */) {
		val next = current->next;
		free(current);
		current = next;
	}
	list_header->head = nullptr;
}

inline void viva_cstd_impl_list_free(struct list_header *const list_header) {
	viva_cstd_impl_list_clear(list_header);
	val ptr = (struct list_header **) &list_header;
	free(list_header);
	*ptr = nullptr;
}

inline void viva_cstd_impl_list_unique(const struct list_header *const list_header) {
	for (var current = list_header->head; current; /* nothing */)
		for (var next = current->next; next; /* nothing */)
			if (current->data != next->data)
				current = next;
			else {
				val dup = next;
				current->next = next->next;
				free(dup);
			}
}

inline void viva_cstd_impl_list_reverse(struct list_header *const list_header) {
	switch (viva_cstd_impl_list_size(list_header)) {
		case 0: [[fallthrough]];
		case 1:
			return;
		case 2:
			// todo: impl a swap function
			return;
		case 3:
		default:
			break;
	}
	val original_end_node = viva_cstd_impl_list_end(list_header);
	var before_new_end = original_end_node;
	for (val current = list_header->head; list_header->head != original_end_node; before_new_end = current) {
		before_new_end->next = current;
		list_header->head = current->next;
		// current->next = original_end_node->next; // nullptr
	}
	// now the last node is `before_new_end`
	before_new_end->next = nullptr;
}

inline struct list_node *viva_cstd_impl_list_end(const struct list_header *const list_header) {
	if (list_header == nullptr)
		return nullptr;
	var current = list_header->head;
	if (current == nullptr)
        return current;
	while (current->next)
		current = current->next;
	return current;
}

inline struct list_header *viva_cstd_impl_list_header_empty() {
	val lh = (struct list_header *) alloc_fwd(sizeof(struct list_header));
	if (lh)
		return ( {
			lh->head = nullptr;
			lh;
		});
	contract_assert(false, "Failed to allocate memory for list header.");
	__builtin_expect(false, 0);
	return nullptr;
}
