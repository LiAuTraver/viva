//! note: REPL.c is somewhat vibe-coded.


#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"

enum : unsigned {
	command_buffer_size		= 0x0400,
	io_buffer_size				= 0x0100,
	path_buffer_size			= 0x0800,
	max_cli_path_segments = 64u,
	max_cli_segment_len		= 27u,
};

static char *trim_whitespace(char *text) {
	size_t len = 0;

	if (!text)
		return nullptr;

	while (*text != '\0' && isspace((unsigned char)*text)) {
		text++;
	}

	len = strlen(text);
	while (len > 0 && isspace((unsigned char)text[len - 1])) {
		text[len - 1] = '\0';
		len--;
	}

	return text;
}
inline void print_command_error(const char *command, int rc) {
	printf("\033[31m%s: %s (%d)\033[0m\n", command, simfs_error_string(rc), rc);
}

inline int is_path_separator(char ch) { return ch == '/' || ch == '\\'; }
static int normalize_path(const char *raw_path, char *normalized_path, size_t normalized_size) {
	char		 segments[max_cli_path_segments][max_cli_segment_len + 1];
	size_t	 i						 = 0;
	uint32_t segment_count = 0;

	memset(segments, 0, sizeof(segments));

	if (!raw_path || !normalized_path || normalized_size < 2)
		return SIMFS_ERR_INVALID_ARGUMENT;

	while (raw_path[i] != '\0') {
		char		 segment[max_cli_segment_len + 1] = {0};
		uint32_t len															= 0;

		while (is_path_separator(raw_path[i])) {
			i++;
		}

		if (raw_path[i] == '\0')
			break;

		while (raw_path[i] != '\0' && !is_path_separator(raw_path[i])) {
			if (len >= max_cli_segment_len)
				return SIMFS_ERR_NAME_TOO_LONG;
			segment[len] = raw_path[i];
			len++;
			i++;
		}
		segment[len] = '\0';

		if (strcmp(segment, ".") == 0)
			continue;
		if (strcmp(segment, "..") == 0) {
			if (segment_count > 0)
				segment_count--;
			continue;
		}

		if (segment_count >= max_cli_path_segments)
			return SIMFS_ERR_PATH_TOO_DEEP;

		memcpy(segments[segment_count], segment, len + 1);
		segment_count++;
	}

	normalized_path[0] = '/';
	normalized_path[1] = '\0';

	for (i = 0; i < segment_count; ++i) {
		size_t current_len = strlen(normalized_path);
		size_t part_len		 = strlen(segments[i]);

		if (current_len + part_len + 2 > normalized_size)
			return SIMFS_ERR_INVALID_PATH;

		if (current_len > 1) {
			normalized_path[current_len]		 = '/';
			normalized_path[current_len + 1] = '\0';
			current_len++;
		}

		memcpy(normalized_path + current_len, segments[i], part_len + 1);
	}

	return SIMFS_OK;
}

static int resolve_path_from_cwd(const char *cwd, const char *input_path, char *resolved_path, size_t resolved_size) {
	char raw_path[path_buffer_size];
	int	 rc			 = SIMFS_OK;
	int	 written = 0;

	if (!cwd || !input_path || !resolved_path)
		return SIMFS_ERR_INVALID_ARGUMENT;
	if (*input_path == '\0')
		return SIMFS_ERR_INVALID_PATH;

	if (is_path_separator(input_path[0])) {
		written = snprintf(raw_path, sizeof(raw_path), "%s", input_path);
	} else if (strcmp(cwd, "/") == 0) {
		written = snprintf(raw_path, sizeof(raw_path), "/%s", input_path);
	} else {
		written = snprintf(raw_path, sizeof(raw_path), "%s/%s", cwd, input_path);
	}

	if (written < 0 || (size_t)written >= sizeof(raw_path))
		return SIMFS_ERR_INVALID_PATH;

	rc = normalize_path(raw_path, resolved_path, resolved_size);
	if (rc != SIMFS_OK)
		return rc;

	return SIMFS_OK;
}

static char *strip_matching_quotes(char *text) {
	size_t len = 0;

	if (!text)
		return nullptr;

	len = strlen(text);
	if (len < 2)
		return text;

	if ((text[0] == '"' && text[len - 1] == '"') || (text[0] == '\'' && text[len - 1] == '\'')) {
		text[len - 1] = '\0';
		return text + 1;
	}

	return text;
}

static void print_help(void) {
	printf("\033[36m");

	puts("commands:");
	puts("  help");
	puts("  ls [path]");
	puts("  cd <path>");
	puts("  mkdir <path>");
	puts("  touch <path>");
	puts("  rm <path>");
	puts("  cat <path>");
	puts("  echo <text> > <path>");
	puts("  dummy <path> <size[k|m]>");
	puts("  format");
	puts("  exit | quit");

	printf("\033[0m");
}

static int ensure_disk_ready(const char *diskpath) {
	printf("\033[37mChecking disk '%s'...\033[0m\n", diskpath);
	int rc = simfs_ops.mount(diskpath);
	if (rc == SIMFS_OK)
		return SIMFS_OK;

	printf("\033[33mDisk not ready: %s (%d). Attempting to format...\033[0m\n", simfs_error_string(rc), rc);

	rc = simfs_ops.format(diskpath);
	if (rc != SIMFS_OK)
		return rc;
	rc = simfs_ops.mount(diskpath);
	if (rc != SIMFS_OK)
		return rc;

	return SIMFS_OK;
}

static int write_text_to_file(const char *path, const char *text) {
	int		 rc				= SIMFS_OK;
	int		 fd				= -1;
	size_t text_len = 0;

	if (!path || !text)
		return SIMFS_ERR_INVALID_ARGUMENT;

	fd = simfs_ops.open(path);
	if (fd >= 0) {
		rc = simfs_ops.close(fd);
		if (rc != SIMFS_OK)
			return rc;
		rc = simfs_ops.remove(path);
		if (rc != SIMFS_OK)
			return rc;
	} else if (fd != SIMFS_ERR_NOT_FOUND) {
		return fd;
	}

	rc = simfs_ops.touch(path);
	if (rc != SIMFS_OK)
		return rc;

	fd = simfs_ops.open(path);
	if (fd < 0)
		return fd;

	text_len = strlen(text);
	rc			 = simfs_ops.write(fd, text, (unsigned int)text_len);
	if (rc < 0) {
		simfs_ops.close(fd);
		return rc;
	}

	rc = simfs_ops.close(fd);
	if (rc != SIMFS_OK)
		return rc;

	return SIMFS_OK;
}

static int parse_size_bytes(const char *size_text, uint64_t *size_out) {
	char							*end				= nullptr;
	unsigned long long value			= 0;
	unsigned long long multiplier = 1;

	if (!size_text || !size_out)
		return SIMFS_ERR_INVALID_ARGUMENT;

	while (*size_text != '\0' && isspace((unsigned char)*size_text)) {
		size_text++;
	}
	if (*size_text == '\0')
		return SIMFS_ERR_INVALID_ARGUMENT;

	value = strtoull(size_text, &end, 10);
	if (end == size_text)
		return SIMFS_ERR_INVALID_ARGUMENT;

	while (*end != '\0' && isspace((unsigned char)*end)) {
		end++;
	}

	if (*end != '\0') {
		if ((end[0] == 'k' || end[0] == 'K') && end[1] == '\0') {
			multiplier = 1024ull;
		} else if ((end[0] == 'm' || end[0] == 'M') && end[1] == '\0') {
			multiplier = 1024ull * 1024ull;
		} else {
			return SIMFS_ERR_INVALID_ARGUMENT;
		}
	}

	if (value > (UINT64_MAX / multiplier))
		return SIMFS_ERR_INVALID_ARGUMENT;

	*size_out = (uint64_t)(value * multiplier);
	return SIMFS_OK;
}

static int create_dummy_file(const char *path, uint64_t size) {
	char		 block[io_buffer_size];
	uint64_t remaining = size;
	int			 rc				 = SIMFS_OK;
	int			 fd				 = -1;

	memset(block, 'x', sizeof(block));

	fd = simfs_ops.open(path);
	if (fd >= 0) {
		rc = simfs_ops.close(fd);
		if (rc != SIMFS_OK)
			return rc;
		rc = simfs_ops.remove(path);
		if (rc != SIMFS_OK)
			return rc;
	} else if (fd != SIMFS_ERR_NOT_FOUND) {
		return fd;
	}

	rc = simfs_ops.touch(path);
	if (rc != SIMFS_OK)
		return rc;

	fd = simfs_ops.open(path);
	if (fd < 0)
		return fd;

	while (remaining > 0) {
		uint32_t chunk	 = (remaining > sizeof(block)) ? (uint32_t)sizeof(block) : (uint32_t)remaining;
		int			 written = simfs_ops.write(fd, block, chunk);

		if (written < 0) {
			simfs_ops.close(fd);
			return written;
		}
		if (written != (int)chunk) {
			simfs_ops.close(fd);
			return SIMFS_ERR_NO_SPACE;
		}

		remaining -= chunk;
	}

	rc = simfs_ops.close(fd);
	if (rc != SIMFS_OK)
		return rc;

	return SIMFS_OK;
}

static int cat_file(const char *path) {
	char buffer[io_buffer_size + 1] = {0};
	int	 fd													= -1;
	int	 bytes_read									= 0;
	int	 rc													= SIMFS_OK;

	fd = simfs_ops.open(path);
	if (fd < 0)
		return fd;

	while ((bytes_read = simfs_ops.read(fd, buffer, (unsigned int)io_buffer_size)) > 0) {
		buffer[bytes_read] = '\0';
		fputs(buffer, stdout);
	}

	if (bytes_read < 0) {
		simfs_ops.close(fd);
		return bytes_read;
	}

	rc = simfs_ops.close(fd);
	if (rc != SIMFS_OK)
		return rc;

	putchar('\n');
	return SIMFS_OK;
}

static int handle_echo(char *line, const char *cwd) {
	char *payload = nullptr;
	char *path		= nullptr;
	char *redir		= nullptr;
	int		rc			= SIMFS_OK;
	char	resolved_path[path_buffer_size];

	payload = trim_whitespace(line + 4);
	redir		= strrchr(payload, '>');
	if (!redir)
		return SIMFS_ERR_INVALID_ARGUMENT;

	*redir = '\0';
	redir++;

	payload = trim_whitespace(payload);
	payload = strip_matching_quotes(payload);
	path		= trim_whitespace(redir);

	if (!path || *path == '\0')
		return SIMFS_ERR_INVALID_ARGUMENT;
	rc = resolve_path_from_cwd(cwd, path, resolved_path, sizeof(resolved_path));
	if (rc != SIMFS_OK)
		return rc;

	rc = write_text_to_file(resolved_path, payload);
	if (rc != SIMFS_OK)
		return rc;

	printf("wrote %zu bytes to %s\n", strlen(payload), resolved_path);
	return SIMFS_OK;
}

static int handle_dummy(const char *arg, const char *cwd) {
	char		 arg_copy[command_buffer_size];
	char		*path			 = nullptr;
	char		*size_text = nullptr;
	char		*cursor		 = nullptr;
	char		 resolved_path[path_buffer_size];
	uint64_t size		= 0;
	int			 rc			= SIMFS_OK;
	int			 copied = 0;

	if (!arg)
		return SIMFS_ERR_INVALID_ARGUMENT;

	copied = snprintf(arg_copy, sizeof(arg_copy), "%s", arg);
	if (copied < 0 || (size_t)copied >= sizeof(arg_copy))
		return SIMFS_ERR_INVALID_ARGUMENT;

	path = trim_whitespace(arg_copy);
	if (!path || *path == '\0')
		return SIMFS_ERR_INVALID_ARGUMENT;

	cursor = path;
	while (*cursor != '\0' && !isspace((unsigned char)*cursor)) {
		cursor++;
	}

	if (*cursor == '\0')
		return SIMFS_ERR_INVALID_ARGUMENT;

	*cursor = '\0';
	cursor++;
	size_text = trim_whitespace(cursor);
	if (!size_text || *size_text == '\0')
		return SIMFS_ERR_INVALID_ARGUMENT;

	rc = resolve_path_from_cwd(cwd, path, resolved_path, sizeof(resolved_path));
	if (rc != SIMFS_OK)
		return rc;
	rc = parse_size_bytes(size_text, &size);
	if (rc != SIMFS_OK)
		return rc;
	rc = create_dummy_file(resolved_path, size);
	if (rc != SIMFS_OK)
		return rc;

	printf("created %s (%llu bytes)\n", resolved_path, (unsigned long long)size);
	return SIMFS_OK;
}

int simfs(char const *const diskpath) {

	char command_line[command_buffer_size];
	char cwd[path_buffer_size] = "/";
	int	 rc										 = SIMFS_OK;

	rc = ensure_disk_ready(diskpath);
	if (rc != SIMFS_OK) {
		print_command_error("startup", rc);
		return EXIT_FAILURE;
	}

	SIMFS_INFO("simfs repl started");
	print_help();

	while (true) {
		char *line	 = nullptr;
		char *arg		 = nullptr;
		char *cursor = nullptr;
		char	resolved_path[path_buffer_size];
		char	target_path[path_buffer_size];

		printf("\033[37msimfs:%s> \033[0m", cwd);
		if (!fgets(command_line, sizeof(command_line), stdin))
			break;

		line = trim_whitespace(command_line);
		if (!line || *line == '\0')
			continue;

		if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0)
			break;

		if (strcmp(line, "help") == 0) {
			print_help();
			continue;
		}

		if (strcmp(line, "format") == 0) {
			rc = simfs_ops.format(diskpath);
			if (rc == SIMFS_OK)
				rc = simfs_ops.mount(diskpath);
			if (rc != SIMFS_OK)
				print_command_error("format", rc);
			else {
				snprintf(cwd, sizeof(cwd), "/");
				SIMFS_INFO("disk formatted and mounted");
			}
			continue;
		}

		if (strncmp(line, "echo", 4) == 0 && (line[4] == ' ' || line[4] == '\t')) {
			rc = handle_echo(line, cwd);
			if (rc == SIMFS_ERR_INVALID_ARGUMENT)
				SIMFS_INFO("usage: echo <text> > <path>");
			else if (rc != SIMFS_OK)
				print_command_error("echo", rc);
			continue;
		}

		cursor = line;
		while (*cursor != '\0' && !isspace((unsigned char)*cursor)) {
			cursor++;
		}

		if (*cursor != '\0') {
			*cursor = '\0';
			cursor++;
			arg = trim_whitespace(cursor);
		}

		if (strcmp(line, "ls") == 0) {
			if (!arg || *arg == '\0') {
				snprintf(target_path, sizeof(target_path), "%s", cwd);
			} else {
				rc = resolve_path_from_cwd(cwd, arg, target_path, sizeof(target_path));
			}

			if (rc != SIMFS_OK) {
				print_command_error("ls", rc);
				continue;
			}

			rc = simfs_ops.ls_path(target_path);
			if (rc < 0)
				print_command_error("ls", rc);
			continue;
		}

		if (strcmp(line, "cd") == 0) {
			if (!arg || *arg == '\0') {
				snprintf(cwd, sizeof(cwd), "/");
				continue;
			}

			rc = resolve_path_from_cwd(cwd, arg, target_path, sizeof(target_path));
			if (rc != SIMFS_OK) {
				print_command_error("cd", rc);
				continue;
			}

			rc = simfs_ops.cd(target_path);
			if (rc != SIMFS_OK) {
				print_command_error("cd", rc);
				continue;
			}

			snprintf(cwd, sizeof(cwd), "%s", target_path);
			continue;
		}

		if (strcmp(line, "mkdir") == 0) {
			if (!arg || *arg == '\0') {
				print_command_error("mkdir", SIMFS_ERR_INVALID_ARGUMENT);
				continue;
			}
			rc = resolve_path_from_cwd(cwd, arg, resolved_path, sizeof(resolved_path));
			if (rc != SIMFS_OK) {
				print_command_error("mkdir", rc);
				continue;
			}

			rc = simfs_ops.mkdir(resolved_path);
			if (rc != SIMFS_OK)
				print_command_error("mkdir", rc);
			continue;
		}

		if (strcmp(line, "touch") == 0) {
			if (!arg || *arg == '\0') {
				print_command_error("touch", SIMFS_ERR_INVALID_ARGUMENT);
				continue;
			}
			rc = resolve_path_from_cwd(cwd, arg, resolved_path, sizeof(resolved_path));
			if (rc != SIMFS_OK) {
				print_command_error("touch", rc);
				continue;
			}

			rc = simfs_ops.touch(resolved_path);
			if (rc != SIMFS_OK)
				print_command_error("touch", rc);
			continue;
		}

		if (strcmp(line, "rm") == 0) {
			if (!arg || *arg == '\0') {
				print_command_error("rm", SIMFS_ERR_INVALID_ARGUMENT);
				continue;
			}
			rc = resolve_path_from_cwd(cwd, arg, resolved_path, sizeof(resolved_path));
			if (rc != SIMFS_OK) {
				print_command_error("rm", rc);
				continue;
			}

			rc = simfs_ops.remove(resolved_path);
			if (rc != SIMFS_OK)
				print_command_error("rm", rc);
			continue;
		}

		if (strcmp(line, "cat") == 0) {
			if (!arg || *arg == '\0') {
				print_command_error("cat", SIMFS_ERR_INVALID_ARGUMENT);
				continue;
			}
			rc = resolve_path_from_cwd(cwd, arg, resolved_path, sizeof(resolved_path));
			if (rc != SIMFS_OK) {
				print_command_error("cat", rc);
				continue;
			}

			rc = cat_file(resolved_path);
			if (rc != SIMFS_OK)
				print_command_error("cat", rc);
			continue;
		}

		if (strcmp(line, "dummy") == 0) {
			rc = handle_dummy(arg, cwd);
			if (rc == SIMFS_ERR_INVALID_ARGUMENT)
				SIMFS_INFO("usage: dummy <path> <size[k|m]>");
			else if (rc != SIMFS_OK)
				print_command_error("dummy", rc);
			continue;
		}

		SIMFS_ERROR("unknown command; use 'help'");
	}

	SIMFS_INFO("bye");
	return EXIT_SUCCESS;
}
