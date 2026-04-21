#pragma once
#include <stdint.h>

typedef enum : int {
	SIMFS_OK											= 0,
	SIMFS_ERR_INVALID_ARGUMENT		= -1,
	SIMFS_ERR_NOT_MOUNTED					= -2,
	SIMFS_ERR_IO									= -3,
	SIMFS_ERR_NOT_FOUND						= -4,
	SIMFS_ERR_ALREADY_EXISTS			= -5,
	SIMFS_ERR_NO_SPACE						= -6,
	SIMFS_ERR_NOT_A_DIRECTORY			= -7,
	SIMFS_ERR_IS_A_DIRECTORY			= -8,
	SIMFS_ERR_DIRECTORY_NOT_EMPTY = -9,
	SIMFS_ERR_TOO_MANY_OPEN_FILES = -10,
	SIMFS_ERR_BAD_FILE_DESCRIPTOR = -11,
	SIMFS_ERR_INVALID_PATH				= -12,
	SIMFS_ERR_NAME_TOO_LONG				= -13,
	SIMFS_ERR_PATH_TOO_DEEP				= -14,
	SIMFS_ERR_CORRUPTED_FS				= -15,
} simfs_error_t;
typedef int simfs_fd_t;
const char *simfs_error_string(int error_code);

simfs_error_t simfs_format(const char *disk_name);
simfs_error_t simfs_mount(const char *disk_name);

simfs_error_t simfs_touch(const char *name);
simfs_fd_t simfs_open(const char *name);
simfs_error_t simfs_close(int fd);
simfs_fd_t		simfs_read(int fd, void *buffer, uint32_t size);
simfs_fd_t		simfs_write(int fd, const void *buffer, uint32_t size);
simfs_error_t simfs_remove(const char *name);

simfs_error_t simfs_mkdir(const char *name);
simfs_error_t simfs_ls(void);
simfs_error_t simfs_ls_path(const char *path);
simfs_error_t simfs_cd(const char *path);

#define SIMFS_ERROR(msg) fprintf(stderr, "\033[31m" msg "\033[0m\n")
#define SIMFS_INFO(msg) fprintf(stdout, "\033[32m" msg "\033[0m\n")
