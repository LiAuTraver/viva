#pragma once
#include "../specialized_list.h"
#define instantiate(concrete_type_) \
	struct list_##concrete_type_##_header; \
	struct list_##concrete_type_##_node; \
	static _Ret_maybe_result(struct list_##concrete_type_##_header *) _Ret_notnull_ \
	viva_result_t viva_cstd_impl_list_create_##concrete_type_(_In_ const concrete_type_ data); \
	static _Ret_maybenull_ struct list_##concrete_type_##_header *viva_cstd_impl_list_header_empty_##concrete_type_(); \
	static _Ret_maybenull_ struct list_##concrete_type_##_node *viva_cstd_impl_list_create_list_node_impl_##concrete_type_(_In_ const concrete_type_ data); \
	static _Ret_maybe_result(struct list_##concrete_type_##_node *) _Ret_notnull_ \
	viva_result_t viva_cstd_impl_list_create_list_node_##concrete_type_(_In_ const concrete_type_ data); \
	static size_t viva_cstd_impl_list_size_##concrete_type_(_In_ const struct list_##concrete_type_##_header *const list_header); \
	static bool viva_cstd_impl_list_is_empty_##concrete_type_(_In_ const struct list_##concrete_type_##_header *const list_header); \
	static viva_status_t viva_cstd_impl_list_push_front_##concrete_type_(_Inout_ struct list_##concrete_type_##_header *const list_header, _In_ const concrete_type_ data); \
	static viva_status_t viva_cstd_impl_list_push_back_##concrete_type_(_Inout_ struct list_##concrete_type_##_header *const list_header, _In_ const concrete_type_ data); \
	static viva_status_t viva_cstd_impl_list_insert_after_##concrete_type_(_In_ struct list_##concrete_type_##_node *const node, _In_ const concrete_type_ data); \
	static viva_status_t viva_cstd_impl_list_erase_if_##concrete_type_(_Inout_ struct list_##concrete_type_##_header *const list_header, _In_ const concrete_type_ data); \
	static viva_status_t viva_cstd_impl_list_drop_##concrete_type_(_In_ struct list_##concrete_type_##_header *const list_header); \
	static viva_status_t viva_cstd_impl_list_pop_front_##concrete_type_(_Inout_ struct list_##concrete_type_##_header *const list_header); \
	static void viva_cstd_impl_list_clear_##concrete_type_(_Inout_ struct list_##concrete_type_##_header *const list_header); \
	static void viva_cstd_impl_list_free_##concrete_type_(_Inout_ void **const list_header); \
	static void viva_cstd_impl_list_unique_##concrete_type_(_Inout_ const struct list_##concrete_type_##_header *const list_header); \
	static void viva_cstd_impl_list_reverse_##concrete_type_(_Inout_ struct list_##concrete_type_##_header *const list_header); \
	static _Ret_maybenull_ struct list_##concrete_type_##_node *viva_cstd_impl_list_end_##concrete_type_(_In_ const struct list_##concrete_type_##_header *const list_header); \
	static _Ret_maybenull_ struct list_##concrete_type_##_node *viva_cstd_impl_list_begin_##concrete_type_(_In_ const struct list_##concrete_type_##_header *const list_header); \
	static _Ret_maybenull_ struct list_##concrete_type_##_node *viva_cstd_impl_list_find_first_if_##concrete_type_(_In_ const struct list_##concrete_type_##_header *const list_header, _In_ const concrete_type_ data); \
	struct list_##concrete_type_##_node { \
		concrete_type_ data; \
		struct list_##concrete_type_##_node *next; \
	}; \
	struct list_##concrete_type_##_header { \
		struct list_##concrete_type_##_node *head; \
	}; \
	_Pragma("pack (push, 1)") \
	struct { \
		typeof(viva_cstd_impl_list_create_##concrete_type_) *create_with; \
		typeof(viva_cstd_impl_list_header_empty_##concrete_type_) *empty; \
		typeof(viva_cstd_impl_list_header_empty_##concrete_type_) *empty_list; \
		typeof(viva_cstd_impl_list_create_list_node_##concrete_type_) *create_list_node; \
		typeof(viva_cstd_impl_list_size_##concrete_type_) *size; \
		typeof(viva_cstd_impl_list_is_empty_##concrete_type_) *is_empty; \
		typeof(viva_cstd_impl_list_push_front_##concrete_type_) *push_front; \
		typeof(viva_cstd_impl_list_push_back_##concrete_type_) *push_back; \
		typeof(viva_cstd_impl_list_insert_after_##concrete_type_) *insert_after; \
		typeof(viva_cstd_impl_list_erase_if_##concrete_type_) *erase_if; \
		typeof(viva_cstd_impl_list_drop_##concrete_type_) *drop; \
		typeof(viva_cstd_impl_list_pop_front_##concrete_type_) *pop_front; \
		typeof(viva_cstd_impl_list_clear_##concrete_type_) *clear; \
		typeof(viva_cstd_impl_list_free_##concrete_type_) *free; \
		typeof(viva_cstd_impl_list_unique_##concrete_type_) *unique; \
		typeof(viva_cstd_impl_list_reverse_##concrete_type_) *reverse; \
		typeof(viva_cstd_impl_list_begin_##concrete_type_) *begin; \
		typeof(viva_cstd_impl_list_end_##concrete_type_) *end; \
		typeof(viva_cstd_impl_list_find_first_if_##concrete_type_) *find_first_if; \
	} static List_of_##concrete_type_ = { \
		.create_with = viva_cstd_impl_list_create_##concrete_type_, \
		.empty = viva_cstd_impl_list_header_empty_##concrete_type_, \
		.empty_list = viva_cstd_impl_list_header_empty_##concrete_type_, \
		.create_list_node = viva_cstd_impl_list_create_list_node_##concrete_type_, \
		.size = viva_cstd_impl_list_size_##concrete_type_, \
		.is_empty = viva_cstd_impl_list_is_empty_##concrete_type_, \
		.push_front = viva_cstd_impl_list_push_front_##concrete_type_, \
		.push_back = viva_cstd_impl_list_push_back_##concrete_type_, \
		.insert_after = viva_cstd_impl_list_insert_after_##concrete_type_, \
		.erase_if = viva_cstd_impl_list_erase_if_##concrete_type_, \
		.drop = viva_cstd_impl_list_drop_##concrete_type_, \
		.pop_front = viva_cstd_impl_list_pop_front_##concrete_type_, \
		.clear = viva_cstd_impl_list_clear_##concrete_type_, \
		.free = viva_cstd_impl_list_free_##concrete_type_, \
		.unique = viva_cstd_impl_list_unique_##concrete_type_, \
		.reverse = viva_cstd_impl_list_reverse_##concrete_type_, \
		.begin = viva_cstd_impl_list_begin_##concrete_type_, \
		.end = viva_cstd_impl_list_end_##concrete_type_, \
		.find_first_if = viva_cstd_impl_list_find_first_if_##concrete_type_ \
	}; \
	_Pragma ("pack (pop)") \
	inline struct list_##concrete_type_##_node *viva_cstd_impl_list_begin_##concrete_type_(const struct list_##concrete_type_##_header *const list_header) { \
		VIVA_RUNTIME_REQUIRE(list_header && "List header is null."); \
		return list_header->head; \
	} \
	inline struct list_##concrete_type_##_node *viva_cstd_impl_list_find_first_if_##concrete_type_(const struct list_##concrete_type_##_header \
																														 *const list_header, const concrete_type_ data) { \
		VIVA_RUNTIME_REQUIRE(list_header && "List header is null."); \
		for (var current = list_header->head; current; current = current->next) \
			if (current->data == data) \
				return current; \
		return nullptr; \
	} \
	inline viva_result_t viva_cstd_impl_list_create_##concrete_type_(const concrete_type_ data) { \
		auto maybe_node = viva_cstd_impl_list_create_list_node_impl_##concrete_type_(data); \
		if (maybe_node == nullptr) \
			VIVA_RETURN_ERROR( \
			kBadAlloc, \
			"Failed to allocate memory for list node." \
		) \
		val header = viva_cstd_impl_list_header_empty_##concrete_type_(); \
		if (header) { \
			header->head = maybe_node; \
			VIVA_RETURN_VALUE_PTR( \
				header \
			) \
		} \
		VIVA_RETURN_ERROR( \
			kBadAlloc, \
			"Failed to allocate memory for list header." \
		) \
	} \
	inline viva_status_t viva_cstd_impl_list_push_back_##concrete_type_(struct list_##concrete_type_##_header *const list_header, const concrete_type_ data) { \
		val maybe_node = viva_cstd_impl_list_create_list_node_impl_##concrete_type_(data); \
		if (maybe_node == nullptr) \
			return kBadAlloc; \
		val end_node = viva_cstd_impl_list_end_##concrete_type_(list_header); \
		if (end_node == nullptr) { \
			list_header->head = maybe_node; \
			return kOkStatus; \
		} \
		end_node->next = maybe_node; \
		return kOkStatus; \
	} \
	inline struct list_##concrete_type_##_node *viva_cstd_impl_list_create_list_node_impl_##concrete_type_(const concrete_type_ data) { \
		val node = (struct list_##concrete_type_##_node *) alloc_fwd(sizeof(struct list_##concrete_type_##_node)); \
		node->data = data; \
		node->next = nullptr; \
		return node; \
	} \
	inline viva_result_t viva_cstd_impl_list_create_list_node_##concrete_type_(const concrete_type_ data) { \
		auto node = viva_cstd_impl_list_create_list_node_impl_##concrete_type_(data); \
		if (node == nullptr) \
			VIVA_RETURN_ERROR( \
			kBadAlloc, \
			"Failed to allocate memory for list node." \
		) \
		node->data = data; \
		node->next = nullptr; \
		VIVA_RETURN_VALUE( \
			node \
		) \
	} \
	inline size_t viva_cstd_impl_list_size_##concrete_type_(const struct list_##concrete_type_##_header *const list_header) { \
		size_t idx = 0; \
		var cur = (struct list_##concrete_type_##_node *) list_header->head; \
		while (cur) { \
			idx++; \
			cur = cur->next; \
		} \
		return idx; \
	} \
	inline bool viva_cstd_impl_list_is_empty_##concrete_type_(const struct list_##concrete_type_##_header *const list_header) { \
		return list_header && list_header->head; \
	} \
	inline viva_status_t viva_cstd_impl_list_push_front_##concrete_type_(struct list_##concrete_type_##_header *const list_header, const concrete_type_ data) { \
		if (list_header == nullptr) \
			return kOutOfRange; \
		val maybe_node = viva_cstd_impl_list_create_list_node_impl_##concrete_type_(data); \
		if (maybe_node) { \
			maybe_node->next = list_header->head; \
			list_header->head = maybe_node; \
			return kOkStatus; \
		} \
		return kBadAlloc; \
	} \
	inline viva_status_t viva_cstd_impl_list_insert_after_##concrete_type_(struct list_##concrete_type_##_node *const node, const concrete_type_ data) { \
		if (node == nullptr) \
			return kOutOfRange; \
		struct list_##concrete_type_##_node *maybe_node = viva_cstd_impl_list_create_list_node_impl_##concrete_type_(data); \
		if (maybe_node) { \
			maybe_node->next = node->next; \
			node->next = maybe_node; \
			return kOkStatus; \
		} \
		return kBadAlloc; \
	} \
	inline viva_status_t viva_cstd_impl_list_erase_if_##concrete_type_(struct list_##concrete_type_##_header *const list_header, const concrete_type_ data) { \
		bool cnt = 0; \
		if (list_header == nullptr) \
			return kEmpty; \
		if (list_header->head == nullptr) \
			return kOutOfRange; \
		var before = list_header->head; \
		while (before->next) { \
			if (before->next->data == data){ \
				val next = before->next; \
				before->next = before->next->next; \
				free(next); \
				cnt = 1; \
			} else \
				before = before->next; \
		} \
		if (before->data == data) { \
			val next = before->next; \
			free(before); \
			list_header->head = next; \
			cnt = 1; \
		} \
		return cnt ? kOkStatus : kNotFound; \
	} \
	inline viva_status_t viva_cstd_impl_list_drop_##concrete_type_(struct list_##concrete_type_##_header *const list_header) { \
		var maybe_node_before_end = list_header->head; \
		if (maybe_node_before_end == nullptr) \
			return kEmpty; \
		if (maybe_node_before_end->next == nullptr) { \
			free(maybe_node_before_end); \
			list_header->head = nullptr; \
			return kOkStatus; \
		} \
		while (maybe_node_before_end->next->next) { \
			maybe_node_before_end = maybe_node_before_end->next; \
		} \
		free(maybe_node_before_end->next); \
		maybe_node_before_end->next = nullptr; \
		return kOkStatus; \
	} \
	inline viva_status_t viva_cstd_impl_list_pop_front_##concrete_type_(struct list_##concrete_type_##_header *const list_header) { \
		if (list_header == nullptr) { \
			return kEmpty; \
		} \
		if (list_header->head == nullptr) { \
			return kOutOfRange; \
		} \
		val next = list_header->head->next; \
		free(list_header->head); \
		list_header->head = next; \
		return kOkStatus; \
	} \
	inline void viva_cstd_impl_list_clear_##concrete_type_(struct list_##concrete_type_##_header *const list_header) { \
		for (var current = list_header->head; current;) { \
			val next = current->next; \
			free(current); \
			current = next; \
		} \
		list_header->head = nullptr; \
	} \
	inline void viva_cstd_impl_list_free_##concrete_type_(void **const list_header) { \
		viva_cstd_impl_list_clear_##concrete_type_(*list_header); \
		free(*list_header); \
		*list_header = nullptr; \
	} \
	inline void viva_cstd_impl_list_unique_##concrete_type_(const struct list_##concrete_type_##_header *const list_header) { \
		for (var current = list_header->head; current;) \
			for (var next = current->next; next;) \
				if (current->data != next->data) \
					current = next; \
				else { \
					val dup = next; \
					current->next = next->next; \
					free(dup); \
				} \
	} \
	inline void viva_cstd_impl_list_reverse_##concrete_type_(struct list_##concrete_type_##_header *const list_header) { \
		switch (viva_cstd_impl_list_size_##concrete_type_(list_header)) { \
			case 0: [[fallthrough]]; \
			case 1: \
				return; \
			case 2: \
			default: \
				break; \
		} \
		val original_end_node = viva_cstd_impl_list_end_##concrete_type_(list_header); \
		struct list_##concrete_type_##_node *current; \
		while (list_header->head != original_end_node) { \
			current = list_header->head; \
			list_header->head = list_header->head->next; \
			current->next = original_end_node->next; \
			original_end_node->next = current; \
		} \
	} \
	inline struct list_##concrete_type_##_node *viva_cstd_impl_list_end_##concrete_type_(const struct list_##concrete_type_##_header *const list_header) { \
		if (list_header == nullptr) \
			return nullptr; \
		var current = list_header->head; \
		if (current == nullptr) \
			return current; \
		while (current->next) \
			current = current->next; \
		return current; \
	} \
	inline struct list_##concrete_type_##_header *viva_cstd_impl_list_header_empty_##concrete_type_() { \
		val lh = (struct list_##concrete_type_##_header *) alloc_fwd(sizeof(struct list_##concrete_type_##_header)); \
		if (lh) \
			return ( { \
				lh->head = nullptr; \
				lh; \
			}); \
		VIVA_RUNTIME_REQUIRE(false, "Failed to allocate memory for list header."); \
		__builtin_expect(false, 0); \
		return nullptr; \
	}

