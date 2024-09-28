#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define _Ret_maybe_result(expr) // nothing
#define _Generic_usage
#if __has_include(<sal.h>)
#include <sal.h>
#else
#define _In_
#define _Out_
#define _Inout_
#define _Ret_maybenull_
#endif
#include <stdlib.h>
#include "../internal/viva_internal_export.h"

#pragma region declaration of forward_list
static __attribute__((always_inline)) inline void* viva_alloc(size_t size) { return malloc(size); }
struct viva_forward_list_header;
struct viva_forward_list_node;

static _Ret_maybenull_ _Generic_usage
void* viva_cstd_impl_forward_list_empty_header();

static _Generic_usage
size_t viva_cstd_impl_forward_list_size(_In_ void* header);

static _Generic_usage bool
viva_cstd_impl_forward_list_is_empty(_In_ void* header);

static _Generic_usage
viva_status_t viva_cstd_impl_forward_list_drop(_In_ void* header);

static _Generic_usage viva_status_t
viva_cstd_impl_forward_list_pop_front(_Inout_ void* header);

static _Generic_usage
void viva_cstd_impl_forward_list_clear(_Inout_ void* header);

static _Generic_usage void
viva_cstd_impl_forward_list_free(_Inout_ void** const header_ptr);

static _Generic_usage void
viva_cstd_impl_forward_list_reverse(_Inout_ void* header);

static _Ret_maybenull_ _Generic_usage
struct viva_forward_list_node* viva_cstd_impl_forward_list_end(_In_ void* header);

static _Ret_maybenull_ _Generic_usage
struct viva_forward_list_node* viva_cstd_impl_forward_list_begin(_In_ void* header);

static viva_status_t
viva_cstd_impl_forward_list_erase_if(_Inout_ void* header, _In_ bool (*predicate)(void*));

static _Ret_maybenull_
void* viva_cstd_impl_forward_list_find_first_if(_In_ void* header, _In_ bool (*predicate)(void*));

_Pragma("pack(push, 1)")

struct viva_forward_list_header {
	struct viva_forward_list_node* head;
};

struct viva_forward_list_node {
	struct viva_forward_list_node* next;
	void* data_ptr;
};

# define VIVA_CSTD_FORWARD_LIST_COMMON_MEMBER_() \
	typeof(viva_cstd_impl_forward_list_empty_header)* new_header; \
	typeof(viva_cstd_impl_forward_list_size)* size; \
	typeof(viva_cstd_impl_forward_list_is_empty)* is_empty; \
	typeof(viva_cstd_impl_forward_list_drop)* drop; \
	typeof(viva_cstd_impl_forward_list_pop_front)* pop_front; \
	typeof(viva_cstd_impl_forward_list_clear)* clear; \
	typeof(viva_cstd_impl_forward_list_free)* free; \
	typeof(viva_cstd_impl_forward_list_reverse)* reverse; \
	typeof(viva_cstd_impl_forward_list_end)* end; \
	typeof(viva_cstd_impl_forward_list_begin)* begin; \
	typeof(viva_cstd_impl_forward_list_erase_if)* erase_if; \
	typeof(viva_cstd_impl_forward_list_find_first_if)* find_first_if;
//////////////////// End of The Macro ////////////////////

#define VIVA_CSTD_FORWARD_LIST_COMMON_MEMBER_INIT() \
	.new_header = viva_cstd_impl_forward_list_empty_header, \
	.size = viva_cstd_impl_forward_list_size, \
	.is_empty = viva_cstd_impl_forward_list_is_empty, \
	.drop = viva_cstd_impl_forward_list_drop, \
	.pop_front = viva_cstd_impl_forward_list_pop_front, \
	.clear = viva_cstd_impl_forward_list_clear, \
	.free = viva_cstd_impl_forward_list_free, \
	.reverse = viva_cstd_impl_forward_list_reverse, \
	.end = viva_cstd_impl_forward_list_end, \
	.begin = viva_cstd_impl_forward_list_begin, \
	.erase_if = viva_cstd_impl_forward_list_erase_if, \
	.find_first_if = viva_cstd_impl_forward_list_find_first_if
//////////////////// End of The Macro ////////////////////

struct viva_forward_list {
	VIVA_CSTD_FORWARD_LIST_COMMON_MEMBER_()
} static const Forward_list = {
	VIVA_CSTD_FORWARD_LIST_COMMON_MEMBER_INIT()
};

_Pragma("pack(pop)")
#pragma endregion

#pragma region implementaion
inline void* viva_cstd_impl_forward_list_empty_header() {
	val header = viva_alloc(sizeof(struct viva_forward_list_header));
	((struct viva_forward_list_header*)header)->head = nullptr;
	return header;
}

inline size_t viva_cstd_impl_forward_list_size(void* header) {
	size_t idx = 0;
	var current = ((struct viva_forward_list_header*)header)->head;
	while (current) {
		idx++;
		current = current->next;
	}
	return idx;
}

inline bool viva_cstd_impl_forward_list_is_empty(void* header) {
	return header && ((struct viva_forward_list_header*)header)->head;
}

inline viva_status_t viva_cstd_impl_forward_list_drop(void* header) {
	var maybe_node_before_end = ((struct viva_forward_list_header*)header)->head;
	if (maybe_node_before_end == nullptr)
		return kEmpty;
	if (maybe_node_before_end->next == nullptr) {
		free(maybe_node_before_end->data_ptr);
		free(maybe_node_before_end);
		((struct viva_forward_list_header*)header)->head = nullptr;
		return kOkStatus;
	}
	while (maybe_node_before_end->next->next) { maybe_node_before_end = maybe_node_before_end->next; }
	free(maybe_node_before_end->next->data_ptr);
	free(maybe_node_before_end->next);
	maybe_node_before_end->next = nullptr;
	return kOkStatus;
}

inline viva_status_t viva_cstd_impl_forward_list_pop_front(void* header) {
	if (header == nullptr)
		return kEmpty;
	if (((struct viva_forward_list_header*)header)->head == nullptr)
		return kOutOfRange;
	return ( {
		val next = ((struct viva_forward_list_header*)header)->head->next;
		free(((struct viva_forward_list_header*)header)->head->data_ptr);
		free(((struct viva_forward_list_header*)header)->head);
		((struct viva_forward_list_header*)header)->head = next;
		kOkStatus;
	});
}

inline void viva_cstd_impl_forward_list_clear(void* header) {
	for (var current = ((struct viva_forward_list_header*)header)->head; current;) {
		var next = current->next;
		free(current->data_ptr);
		free(current);
		current = next;
	}
	((struct viva_forward_list_header*)header)->head = nullptr;
}

inline void
viva_cstd_impl_forward_list_free(_Inout_ void** const header_ptr) {
	viva_cstd_impl_forward_list_clear(*header_ptr);
	free(*header_ptr);
	*header_ptr = nullptr;
}

inline _Generic_usage void
viva_cstd_impl_forward_list_reverse(_Inout_ void* header) {
	switch (viva_cstd_impl_forward_list_size(header)) {
	case 0:
	case 1:
		return;
	case 2:
		{
			val end_node = viva_cstd_impl_forward_list_end(header);
			val head = ((struct viva_forward_list_header*)header)->head;
			head->next = nullptr;
			end_node->next = head;
			((struct viva_forward_list_header*)header)->head = end_node;
		}
		return;
	default: break;
	}
	val original_end_node = viva_cstd_impl_forward_list_end(header);
	while (((struct viva_forward_list_header*)header)->head != original_end_node) {
		val current = ((struct viva_forward_list_header*)header)->head;
		((struct viva_forward_list_header*)header)->head = ((struct viva_forward_list_header*)header)->head->next;
		current->next = original_end_node->next;
		original_end_node->next = current;
	}
}

inline struct viva_forward_list_node* viva_cstd_impl_forward_list_end(void* header) {
	if (header == nullptr)
		return nullptr;
	var current = ((struct viva_forward_list_header*)header)->head;
	if (current == nullptr)
		return nullptr;
	while (current->next) { current = current->next; }
	return current;
}

inline struct viva_forward_list_node* viva_cstd_impl_forward_list_begin(void* header) {
	VIVA_RUNTIME_REQUIRE(header && "header is nullptr");
	return ((struct viva_forward_list_header*)header)->head;
}

inline viva_status_t viva_cstd_impl_forward_list_erase_if(void* header, bool (*predicate)(void*)) {
	bool cnt = false;
	if (header == nullptr)
		return kEmpty;
	if (((struct viva_forward_list_header*)header)->head == nullptr)
		return kOutOfRange;
	var before = ((struct viva_forward_list_header*)header)->head;
	while (before->next) {
		if (predicate(before->next->data_ptr)) {
			var next = before->next->next;
			free(before->next->data_ptr);
			free(before->next);
			before->next = next;
			cnt = true;
		}
		else { before = before->next; }
	}
	return cnt ? kOkStatus : kNotFound;
}

inline void* viva_cstd_impl_forward_list_find_first_if(void* header, bool (*predicate)(void*)) {
	if (header == nullptr)
		return nullptr;
	for (var current = ((struct viva_forward_list_header*)header)->head; current; current = current->next) {
		if (predicate(current->data_ptr))
			return current;
	}
	return nullptr;
}
#pragma endregion

#pragma region instantiating forward_list_of
//////////////////////////////////////////////////////////////
////////// Macro for instantiating forward_list_of ///////////
#define VIVA_CSTD_INSTANTIATE_FORWARD_LIST_OF_TYPE_IMPL(data_type_)  \
		static _Ret_maybenull_ struct viva_forward_list_node* \
		viva_cstd_impl_forward_list_create_forward_list_node_impl_of_##data_type_(_In_ data_type_ data); \
		static _Ret_maybe_result(struct viva_forward_list_node*) _Ret_notnull_ \
		viva_result_t viva_cstd_impl_forward_list_create_forward_list_node_of_##data_type_(_In_ const data_type_ data); \
		static viva_status_t viva_cstd_impl_forward_list_push_front_of_##data_type_(_Inout_ void* header, _In_ data_type_ data); \
		static viva_status_t viva_cstd_impl_forward_list_push_back_of_##data_type_(_Inout_ void* header, _In_ data_type_ data); \
		static viva_status_t viva_cstd_impl_forward_list_insert_after_of_##data_type_(_In_ void* header, _In_ void* node, _In_ data_type_ data); \
		static viva_status_t viva_cstd_impl_forward_list_unique_of_##data_type_(_Inout_ void* header); \
		\
		_Pragma("pack(push, 1)") \
		struct { \
			VIVA_CSTD_FORWARD_LIST_COMMON_MEMBER_() \
			typeof(viva_cstd_impl_forward_list_create_forward_list_node_of_##data_type_)* create_with; \
			typeof(viva_cstd_impl_forward_list_push_front_of_##data_type_)* push_front; \
			typeof(viva_cstd_impl_forward_list_push_back_of_##data_type_)* push_back; \
			typeof(viva_cstd_impl_forward_list_insert_after_of_##data_type_)* insert_after; \
			typeof(viva_cstd_impl_forward_list_unique_of_##data_type_)* unique; \
		} static const Forward_list_of_##data_type_ = { \
			VIVA_CSTD_FORWARD_LIST_COMMON_MEMBER_INIT(), \
			.create_with = viva_cstd_impl_forward_list_create_forward_list_node_of_##data_type_, \
			.push_front = viva_cstd_impl_forward_list_push_front_of_##data_type_, \
			.push_back = viva_cstd_impl_forward_list_push_back_of_##data_type_, \
			.insert_after = viva_cstd_impl_forward_list_insert_after_of_##data_type_, \
			.unique = viva_cstd_impl_forward_list_unique_of_##data_type_, \
		}; \
		_Pragma("pack(pop)") \
		\
		inline viva_result_t viva_cstd_impl_forward_list_create_forward_list_node_of_##data_type_(const data_type_ data) { \
			val node = viva_cstd_impl_forward_list_create_forward_list_node_impl_of_##data_type_(data); \
			if (node == nullptr) \
				VIVA_RETURN_ERROR( \
				kBadAlloc, \
				"Failed to allocate memory for forward_list_node") \
			val header = viva_alloc(sizeof(struct viva_forward_list_header)); \
			if (header == nullptr) { \
				free(node->data_ptr); \
				free(node); \
				VIVA_RETURN_ERROR( \
				kBadAlloc, \
				"Failed to allocate memory for forward_list_header") \
			} \
			((struct viva_forward_list_header*)header)->head = node; \
			VIVA_RETURN_VALUE_PTR( \
				header \
			) \
		} \
		inline viva_status_t viva_cstd_impl_forward_list_push_front_of_##data_type_(void* header, data_type_ data) { \
			if (header == nullptr) \
				return kEmpty; \
			val maybe_node = viva_cstd_impl_forward_list_create_forward_list_node_impl_of_##data_type_(data); \
			if (maybe_node == nullptr) \
				return kBadAlloc; \
			maybe_node->next = ((struct viva_forward_list_header*)header)->head; \
			((struct viva_forward_list_header*)header)->head = maybe_node; \
			return kOkStatus; \
		} \
		inline viva_status_t viva_cstd_impl_forward_list_push_back_of_##data_type_(void* header, data_type_ data) { \
			if (header == nullptr) \
				return kEmpty; \
			val maybe_node = viva_cstd_impl_forward_list_create_forward_list_node_impl_of_##data_type_(data); \
			if (maybe_node == nullptr) \
				return kBadAlloc; \
			val end_node = (struct viva_forward_list_node*)viva_cstd_impl_forward_list_end(header); \
			if (end_node == nullptr) { \
				((struct viva_forward_list_header*)header)->head = maybe_node; \
				return kOkStatus; \
			} \
			end_node->next = maybe_node; \
			return kOkStatus; \
		} \
		inline viva_status_t viva_cstd_impl_forward_list_insert_after_of_##data_type_(void* header, void* node, data_type_ data) { \
			if (header == nullptr) \
				return kEmpty; \
			if (node == nullptr) \
				return kOutOfRange; \
			val maybe_node = viva_cstd_impl_forward_list_create_forward_list_node_impl_of_##data_type_(data); \
			if (maybe_node == nullptr) \
				return kBadAlloc; \
			maybe_node->next = ((struct viva_forward_list_node*)node)->next; \
			((struct viva_forward_list_node*)node)->next = maybe_node; \
			return kOkStatus; \
		} \
		inline viva_status_t viva_cstd_impl_forward_list_unique_of_##data_type_(void* header) { \
			if (header == nullptr) \
				return kEmpty; \
			for (var current = ((struct viva_forward_list_header*)header)->head; current->next; current = current->next) { \
				for (var next = current; next->next;) { \
					if (*(data_type_*)current->data_ptr == *(data_type_*)next->next->data_ptr) { \
						var temp = next->next; \
						next->next = next->next->next; \
						free(temp->data_ptr); \
						free(temp); \
					} \
					else \
						next = next->next; \
				} \
			} \
			return kOkStatus; \
		} \
		inline struct viva_forward_list_node* viva_cstd_impl_forward_list_create_forward_list_node_impl_of_##data_type_(data_type_ data) { \
			val node = (struct viva_forward_list_node*)viva_alloc(sizeof(struct viva_forward_list_node)); \
			node->data_ptr = viva_alloc(sizeof(data_type_)); \
			*(data_type_*)node->data_ptr = data; \
			node->next = nullptr; \
			return node; \
		} \
//////////////////////////////////////////////// End of The Macro //////////////////////////////////////////////////////
#pragma endregion

#define VIVA_CSTD_INSTANTIATE_FORWARD_LIST_OF_TYPE(data_type_) \
	VIVA_CSTD_INSTANTIATE_FORWARD_LIST_OF_TYPE_IMPL(data_type_)

#ifndef VIVA_EXPORT_OFF
#define instantiate_forward_list_of(data_type_) \
	VIVA_CSTD_INSTANTIATE_FORWARD_LIST_OF_TYPE(data_type_)
#endif

#ifdef __cplusplus
}
#endif
