/**
* @file specialized_list.h
 * @addtogroup viva_cstd
 * @brief A simple list implementation.
 * @note This is a simple list implementation, it is not thread-safe.
 * <br> This implementation uses
 * <a href="https://learn.microsoft.com/en-us/cpp/c-runtime-library/sal-annotations?view=msvc-170">sal annotation</a>
 * @author liautraver
 * @date 2024-09-26
 */
#pragma once
#include <stdlib.h>
#include "internal/viva_internal_export.h"

#define _Ret_maybe_result(expr) // nothing
#if __has_include(<sal.h>)
#include <sal.h>
#else
#define _In_
#define _Out_
#define _Inout_
#define _Ret_maybenull_
#define _Ret_maybe_result(expr) // nothing
#endif

/**
 * @brief The header of the list.
 * @param head The first node of the list.
 * @note The head itself is not a node and doesn't contain data.
 */
struct list_header;

/**
 * @brief The node of the list.
 * @param data The data stored in the node.
 * @param next The next node in the list.
 */
struct list_node;
/**
 * @brief Create a list.
 * @param data to be stored in the list.
 * @return A pointer to the list header, wrapped in a viva_result_t.
 */
static _Ret_maybe_result(struct list_header *) _Ret_notnull_
viva_result_t viva_cstd_impl_list_create(_In_ const int data);

/**
 * @brief Create an empty list header.
 * @return A pointer to the list header(maybe null).
 */
static _Ret_maybenull_
struct list_header *viva_cstd_impl_list_header_empty();

/**
 * @brief Create a list node.
 * @param data to be stored in the list node.
 * @return A pointer to the list node(maybe null).
 */
static _Ret_maybenull_ struct list_node *viva_cstd_impl_list_create_list_node_impl(_In_ const int data);

/**
 * @brief Create a list node.
 * @param data to be stored in the list node.
 * @return A pointer to the list node, wrapped in a viva_result_t.
 */
static _Ret_maybe_result(struct list_node *) _Ret_notnull_
viva_result_t viva_cstd_impl_list_create_list_node(_In_ const int data);

/**
 * @brief Get the size of the list.
 * @param list_header to be checked.
 * @return The size of the list.
 */
static size_t viva_cstd_impl_list_size(_In_ const struct list_header *const list_header);

/**
 * @brief Check if the list is empty.
 * @param list_header to be checked.
 * @return True if the list is empty, false otherwise.
 */
static bool viva_cstd_impl_list_is_empty(_In_ const struct list_header *const list_header);

/**
 * @brief Push an element to the front of the list.
 * @param list_header to be modified.
 * @param data to be stored in the list.
 * @return Status of the operation.
 */
static viva_status_t viva_cstd_impl_list_push_front(_Inout_ struct list_header *const list_header, _In_ const int data);

/**
 * @brief Push an element to the back of the list.
 * @param list_header to be modified.
 * @param data to be stored in the list.
 * @return Status of the operation.
 */
static viva_status_t viva_cstd_impl_list_push_back(_Inout_ struct list_header *const list_header, _In_ const int data);

/**
 * @brief Insert an element after a node.
 * @param node to be inserted after.
 * @param data to be stored in the list.
 * @return Status of the operation.
 */
static viva_status_t viva_cstd_impl_list_insert_after(_In_ struct list_node *const node, _In_ const int data);

/**
 * @brief Erase an element from the list.
 * @param list_header to be modified.
 * @param data to be erased from the list.
 * @return Status of the operation.
 */
static viva_status_t viva_cstd_impl_list_erase_if(_Inout_ struct list_header *const list_header, _In_ const int data);

/**
 * @brief Drop the last element from the list.
 * @param list_header to be modified.
 * @return Status of the operation.
 */
static viva_status_t viva_cstd_impl_list_drop(_In_ struct list_header *const list_header);

/**
 * @brief Pop the first element from the list.
 * @param list_header to be modified.
 * @return Status of the operation.
 */
static viva_status_t viva_cstd_impl_list_pop_front(_Inout_ struct list_header *const list_header);

/**
 * @brief Clear the list.
 * @param list_header will not be freed, the nodes will be freed.
 */
static void viva_cstd_impl_list_clear(_Inout_ struct list_header *const list_header);

/**
 * @brief Free the list.
 * @param list_header The header will be freed and set to nullptr.
 */
static void viva_cstd_impl_list_free(_Inout_ void **const list_header);

/**
 * @brief Remove duplicates from the list.
 * @param list_header to be modified.
 */
static void viva_cstd_impl_list_unique(_Inout_ const struct list_header *const list_header);

/**
 * @brief Reverse the list.
 * @param list_header to be reversed.
 */
static void viva_cstd_impl_list_reverse(_Inout_ struct list_header *const list_header);

/**
 * @brief Get the end of the list.
 * @param list_header to be checked.
 * @return The end of the list.
 */
static _Ret_maybenull_
struct list_node *viva_cstd_impl_list_end(_In_ const struct list_header *const list_header);

/**
 * @brief Get the first node of the list.
 * @param list_header to be checked.
 * @return The first node of the list.
 */
static _Ret_maybenull_
struct list_node *viva_cstd_impl_list_begin(_In_ const struct list_header *const list_header);

/**
 * @brief Find the first node that satisfies the condition.
 * @param list_header to be checked.
 * @param data to be found.
 * @warning list_header shall not be null.
 * @return The first node that satisfies the condition. returns nullptr if not found.
 */
static _Ret_maybenull_
struct list_node *viva_cstd_impl_list_find_first_if(_In_ const struct list_header *const list_header,
																										_In_ const int data);

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
	//! @copydoc viva_cstd_impl_list_create
	typeof(viva_cstd_impl_list_create) *create_with;
	//! @copydoc viva_cstd_impl_list_header_empty
	typeof(viva_cstd_impl_list_header_empty) *empty;
	//! @copydoc viva_cstd_impl_list_header_empty
	typeof(viva_cstd_impl_list_header_empty) *empty_list;
	//! @copydoc viva_cstd_impl_list_create_list_node
	typeof(viva_cstd_impl_list_create_list_node) *create_list_node;
	//! @copydoc viva_cstd_impl_list_size
	typeof(viva_cstd_impl_list_size) *size;
	//! @copydoc viva_cstd_impl_list_is_empty
	typeof(viva_cstd_impl_list_is_empty) *is_empty;
	//! @copydoc viva_cstd_impl_list_push_front
	typeof(viva_cstd_impl_list_push_front) *push_front;
	//! @copydoc viva_cstd_impl_list_push_back
	typeof(viva_cstd_impl_list_push_back) *push_back;
	//! @copydoc viva_cstd_impl_list_insert_after
	typeof(viva_cstd_impl_list_insert_after) *insert_after;
	//! @copydoc viva_cstd_impl_list_erase_if
	typeof(viva_cstd_impl_list_erase_if) *erase_if;
	//! @copydoc viva_cstd_impl_list_drop
	typeof(viva_cstd_impl_list_drop) *drop;
	//! @copydoc viva_cstd_impl_list_pop_front
	typeof(viva_cstd_impl_list_pop_front) *pop_front;
	//! @copydoc viva_cstd_impl_list_clear
	typeof(viva_cstd_impl_list_clear) *clear;
	//! @copydoc viva_cstd_impl_list_free
	typeof(viva_cstd_impl_list_free) *free;
	//! @copydoc viva_cstd_impl_list_unique
	typeof(viva_cstd_impl_list_unique) *unique;
	//! @copydoc viva_cstd_impl_list_reverse
	typeof(viva_cstd_impl_list_reverse) *reverse;
	//! @copydoc viva_cstd_impl_list_begin
	typeof(viva_cstd_impl_list_begin) *begin;
	//! @copydoc viva_cstd_impl_list_end
	typeof(viva_cstd_impl_list_end) *end;
	//! @copydoc viva_cstd_impl_list_find_first_if
	typeof(viva_cstd_impl_list_find_first_if) *find_first_if;
} static List = {
	.create_with = viva_cstd_impl_list_create,
	.empty = viva_cstd_impl_list_header_empty,
	.empty_list = viva_cstd_impl_list_header_empty,
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
	.begin = viva_cstd_impl_list_begin,
	.end = viva_cstd_impl_list_end,
	.find_first_if = viva_cstd_impl_list_find_first_if
};
#pragma pack(pop)

inline struct list_node *viva_cstd_impl_list_begin(const struct list_header *const list_header) {
	VIVA_RUNTIME_REQUIRE(list_header && "List header is null.");
	return list_header->head;
}

inline struct list_node *viva_cstd_impl_list_find_first_if(const struct list_header
																													 *const list_header, const int data) {
	VIVA_RUNTIME_REQUIRE(list_header && "List header is null.");
	for (var current = list_header->head; current; current = current->next)
		if (current->data == data)
			return current;
	return nullptr;
}

inline void *alloc_fwd(_In_ const size_t size) {
	return malloc(size);
}

inline viva_result_t viva_cstd_impl_list_create(const int data) {
	auto maybe_node = viva_cstd_impl_list_create_list_node_impl(data);
	if (maybe_node == nullptr)
		// node allocation failed
		VIVA_RETURN_ERROR(
		kBadAlloc,
		"Failed to allocate memory for list node."
	)
	val header = viva_cstd_impl_list_header_empty();
	if (header) {
		header->head = maybe_node;
		VIVA_RETURN_VALUE(
			header
		)
	}
	// header allocation failed
	VIVA_RETURN_ERROR(
		kBadAlloc,
		"Failed to allocate memory for list header."
	)
}

inline viva_status_t viva_cstd_impl_list_push_back(struct list_header *const list_header, const int data) {
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
	val node = (struct list_node *) alloc_fwd(sizeof(struct list_node));
	node->data = data;
	node->next = nullptr;
	return node;
}

inline viva_result_t viva_cstd_impl_list_create_list_node(const int data) {
	auto node = viva_cstd_impl_list_create_list_node_impl(data);
	if (node == nullptr)
		VIVA_RETURN_ERROR(
		kBadAlloc,
		"Failed to allocate memory for list node."
	)
	node->data = data;
	node->next = nullptr;
	VIVA_RETURN_VALUE(
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

inline viva_status_t viva_cstd_impl_list_push_front(struct list_header *const list_header, const int data) {
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

inline viva_status_t viva_cstd_impl_list_insert_after(struct list_node *const node, const int data) {
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

inline viva_status_t viva_cstd_impl_list_erase_if(struct list_header *const list_header, const int data) {
	bool cnt = 0;
	if (list_header == nullptr)
		return kEmpty;
	if (list_header->head == nullptr)
		return kOutOfRange;
	var before = list_header->head;
	while (before->next) {
		if (before->next->data == data){
			val next = before->next;
			before->next = before->next->next;
			free(next);
			cnt = 1;
		} else
			before = before->next;
	}
	if (before->data == data) {
		val next = before->next;
		free(before);
		list_header->head = next;
		cnt = 1;
	}
	return cnt ? kOkStatus : kNotFound;
}

inline viva_status_t viva_cstd_impl_list_drop(struct list_header *const list_header) {
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

inline viva_status_t viva_cstd_impl_list_pop_front(struct list_header *const list_header) {
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

inline void viva_cstd_impl_list_free(void **const list_header) {
	viva_cstd_impl_list_clear(*list_header);
	free(*list_header);
	*list_header = nullptr;
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
			// return;
		default:
			break;
	}
	val original_end_node = viva_cstd_impl_list_end(list_header);
	struct list_node *current;
	while (list_header->head != original_end_node) {
		current = list_header->head;
		list_header->head = list_header->head->next;
		current->next = original_end_node->next;
		original_end_node->next = current;
	}
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
	VIVA_RUNTIME_REQUIRE(false, "Failed to allocate memory for list header.");
	__builtin_expect(false, 0);
	return nullptr;
}

