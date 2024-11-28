#pragma once

#include <stdio.h>
#include <windows.h>
#include "net/ancillarycat/viva/internal/compat.h"
#include "net/ancillarycat/viva/viva.h"


struct viva_cstd_threading_thread_t {
	HANDLE handle;
	DWORD	 thread_id;
};

struct viva_cstd_threading_mutex_t {
	HANDLE handle;
};

typedef void (*viva_cstd_threading_thread_func_t)(void *);

static inline struct viva_cstd_threading_thread_t *
viva_cstd_threading_thread_create(const viva_cstd_threading_thread_func_t func, void *arg) {
	val thread		 = alloc(struct viva_cstd_threading_thread_t, 1);
	thread->handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, &thread->thread_id);
	return thread->handle ? thread : nullptr;
}

static inline int viva_cstd_threading_thread_join(const struct viva_cstd_threading_thread_t *thread) {
	if (WaitForSingleObject(thread->handle, INFINITE) != WAIT_OBJECT_0)
		return -1;
	return CloseHandle(thread->handle);
}

static inline int viva_cstd_threading_thread_detach(const struct viva_cstd_threading_thread_t *thread) {
	return CloseHandle(thread->handle);
}

static inline struct viva_cstd_threading_mutex_t *viva_cstd_threading_mutex_init() {
	val mutex			= alloc(struct viva_cstd_threading_mutex_t, 1);
	mutex->handle = CreateMutex(nullptr, FALSE, nullptr);
	return mutex->handle ? mutex : nullptr;
}

static inline int viva_cstd_threading_mutex_lock(const struct viva_cstd_threading_mutex_t *mutex) {
	return WaitForSingleObject(mutex->handle, INFINITE) == WAIT_OBJECT_0;
}

static inline int viva_cstd_threading_mutex_unlock(const struct viva_cstd_threading_mutex_t *mutex) {
	return ReleaseMutex(mutex->handle);
}

static inline int viva_cstd_threading_mutex_destroy(const struct viva_cstd_threading_mutex_t *mutex) {
	return CloseHandle(mutex->handle);
}

#pragma pack(push, 1)
struct {
	struct {
		typeof(viva_cstd_threading_thread_create) *create;
		typeof(viva_cstd_threading_thread_join)		*join;
		typeof(viva_cstd_threading_thread_detach) *detach;
	} Thread;
	struct {
		typeof(viva_cstd_threading_mutex_init)		*create;
		typeof(viva_cstd_threading_mutex_lock)		*lock;
		typeof(viva_cstd_threading_mutex_unlock)	*unlock;
		typeof(viva_cstd_threading_mutex_destroy) *destroy;
	} Mutex;
} static const Threading = {.Thread =
															{
																.create = viva_cstd_threading_thread_create,
																.join		= viva_cstd_threading_thread_join,
																.detach = viva_cstd_threading_thread_detach,
															},
														.Mutex = {
															.create	 = viva_cstd_threading_mutex_init,
															.lock		 = viva_cstd_threading_mutex_lock,
															.unlock	 = viva_cstd_threading_mutex_unlock,
															.destroy = viva_cstd_threading_mutex_destroy,
														}};
#pragma pack(pop)
typedef struct viva_cstd_threading_thread_t viva_thread_t;
typedef struct viva_cstd_threading_mutex_t	viva_mutex_t;
