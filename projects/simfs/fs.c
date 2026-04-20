
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs_impl.h"

enum : uint32_t {
	disk_size_bytes			= 0xAu * 0x0400u * 0x0400u,
	block_size					= 0x1000u,
	max_inodes					= 0x0400u,
	num_direct_pointers = 0x0Au,
	max_filename_len		= 27u,
	max_open_files			= 0x0080u,
	max_path_segments		= 0x0040u,
	magic_ident					= 0xDEADBEEFu,
	invalid_inode				= ~0x0u,
	invalid_block				= ~0x0u,
	max_disk_blocks			= (disk_size_bytes / block_size),
	fs_rootdir					= 0x0u,
};

typedef enum : uint32_t { TYPE_FILE = 1, TYPE_DIR = 2 } file_type_t;
typedef enum : uint32_t { STAT_VACANT = 0, STAT_IN_USE = 1 } use_stat_t;

/// virtual disk:
///
/// | metadata | inode bitmap | block bitmap | inode table | data region |
typedef struct {
	uint32_t ident_marker;
	uint32_t total_blocks;
	uint32_t free_blocks;
	uint32_t total_inodes;
	uint32_t free_inodes;
	uint64_t disk_size_bytes;
	uint64_t inode_bitmap_offset;
	uint64_t block_bitmap_offset;
	uint64_t inode_table_offset;
	uint64_t data_region_offset;
} metadata_t;

typedef struct {
	uint32_t		size;
	uint32_t		direct_pointers[num_direct_pointers];
	use_stat_t	stat;
	file_type_t type;
} inode_t;

typedef struct {
	uint32_t inode_num;
	char		 filename[max_filename_len + 1];
} dir_entry_t;

enum : uint64_t {
	fixed_region = sizeof(metadata_t) + max_inodes + ((uint64_t)max_inodes * sizeof(inode_t)),
};
enum : uint32_t {
	dir_entries_per_block = (block_size / sizeof(dir_entry_t)),
	data_block_size				= fixed_region > disk_size_bytes ? 0 : ((disk_size_bytes - fixed_region) / (block_size + 1u))
};

static_assert((data_block_size && data_block_size <= max_disk_blocks), "not a valid specs...");
typedef struct {
	use_stat_t stat;
	uint32_t	 inode_num;
	uint32_t	 offset;
} simfs_file_descriptor_t;

typedef struct {
	uint32_t		block_num;
	uint32_t		entry_index;
	dir_entry_t entry;
} dir_lookup_t;

static FILE										*vdisk = nullptr;
static metadata_t							 metadata;
static uint8_t								 inode_bitmap[max_inodes];
static uint8_t								 block_bitmap[max_disk_blocks];
static simfs_file_descriptor_t open_files[max_open_files];

const char *simfs_error_string(int error_code) {
	switch (error_code) {
	case SIMFS_OK:
		return "ok";
	case SIMFS_ERR_INVALID_ARGUMENT:
		return "invalid argument";
	case SIMFS_ERR_NOT_MOUNTED:
		return "disk is not mounted";
	case SIMFS_ERR_IO:
		return "i/o error";
	case SIMFS_ERR_NOT_FOUND:
		return "not found";
	case SIMFS_ERR_ALREADY_EXISTS:
		return "already exists";
	case SIMFS_ERR_NO_SPACE:
		return "no space left";
	case SIMFS_ERR_NOT_A_DIRECTORY:
		return "not a directory";
	case SIMFS_ERR_IS_A_DIRECTORY:
		return "is a directory";
	case SIMFS_ERR_DIRECTORY_NOT_EMPTY:
		return "directory not empty";
	case SIMFS_ERR_TOO_MANY_OPEN_FILES:
		return "too many open files";
	case SIMFS_ERR_BAD_FILE_DESCRIPTOR:
		return "bad file descriptor";
	case SIMFS_ERR_INVALID_PATH:
		return "invalid path";
	case SIMFS_ERR_NAME_TOO_LONG:
		return "name too long";
	case SIMFS_ERR_PATH_TOO_DEEP:
		return "path too deep";
	case SIMFS_ERR_CORRUPTED_FS:
		return "filesystem metadata corrupted";
	default:
		return "unknown error";
	}
}

static void close_disk(void) {
	if (vdisk) {
		fflush(vdisk);
		fclose(vdisk);
		vdisk = nullptr;
	}
}

static simfs_error_t write_at(uint64_t offset, const void *buffer, size_t size) {
	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;
	if (!buffer)
		return SIMFS_ERR_INVALID_ARGUMENT;
	if (fseek(vdisk, (long)offset, SEEK_SET))
		return SIMFS_ERR_IO;
	if (fwrite(buffer, 1ull, size, vdisk) != size)
		return SIMFS_ERR_IO;
	return SIMFS_OK;
}

static simfs_error_t read_at(uint64_t offset, void *buffer, size_t size) {
	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;
	if (!buffer)
		return SIMFS_ERR_INVALID_ARGUMENT;
	if (fseek(vdisk, (long)offset, SEEK_SET) != 0)
		return SIMFS_ERR_IO;
	if (fread(buffer, 1ull, size, vdisk) != size)
		return SIMFS_ERR_IO;
	return SIMFS_OK;
}

static inline uint32_t min_u32(uint32_t a, uint32_t b) { return (a < b) ? a : b; }

static inline uint64_t inode_offset(uint32_t inode_index) {
	return metadata.inode_table_offset + ((uint64_t)inode_index * sizeof(inode_t));
}

static inline uint64_t data_block_offset(uint32_t block_index) {
	return metadata.data_region_offset + ((uint64_t)block_index * block_size);
}

static simfs_error_t flush_metadata(void) {
	auto rc = write_at(0, &metadata, sizeof(metadata));
	if (rc)
		return rc;
	rc = write_at(metadata.inode_bitmap_offset, inode_bitmap, metadata.total_inodes);
	if (rc)
		return rc;
	rc = write_at(metadata.block_bitmap_offset, block_bitmap, metadata.total_blocks);
	if (rc)
		return rc;
	fflush(vdisk);
	return SIMFS_OK;
}

static inline inode_t init_inode(file_type_t type, use_stat_t stat) {
	auto inode = (inode_t){
		.size = 0u,
		.type = type,
		.stat = stat,
	};
#pragma unroll
	for (auto i = 0u; i < num_direct_pointers; ++i) {
		inode.direct_pointers[i] = invalid_block;
	}
	return inode;
}

static inline int read_inode(uint32_t inode_index, inode_t *const inode) {
	return (inode_index >= metadata.total_inodes) ? SIMFS_ERR_INVALID_ARGUMENT
																								: read_at(inode_offset(inode_index), inode, sizeof(*inode));
}

static inline int write_inode(uint32_t inode_index, const inode_t *const inode) {

	return (inode_index >= metadata.total_inodes) ? SIMFS_ERR_INVALID_ARGUMENT
																								: write_at(inode_offset(inode_index), inode, sizeof(*inode));
}

static simfs_error_t allocate_inode(uint32_t *inode_index_out) {

	if (!inode_index_out)
		return SIMFS_ERR_INVALID_ARGUMENT;

	for (auto i = 0u; i < metadata.total_inodes; ++i) {
		if (inode_bitmap[i] == 0) {
			inode_bitmap[i] = 1;
			if (metadata.free_inodes > 0)
				metadata.free_inodes--;
			*inode_index_out = i;
			return SIMFS_OK;
		}
	}

	return SIMFS_ERR_NO_SPACE;
}

static void free_inode_slot(uint32_t inode_index) {
	if (inode_index < metadata.total_inodes && inode_bitmap[inode_index] != 0) {
		inode_bitmap[inode_index] = 0;
		metadata.free_inodes++;
	}
}

static simfs_error_t allocate_block(uint32_t *block_index_out) {

	if (!block_index_out)
		return SIMFS_ERR_INVALID_ARGUMENT;

	for (auto i = 0u; i < metadata.total_blocks; ++i) {
		if (block_bitmap[i] == 0) {
			block_bitmap[i] = 1;
			if (metadata.free_blocks > 0)
				metadata.free_blocks--;
			*block_index_out = i;
			return SIMFS_OK;
		}
	}

	return SIMFS_ERR_NO_SPACE;
}

static void free_block(uint32_t block_index) {
	if (block_index < metadata.total_blocks && block_bitmap[block_index] != 0) {
		block_bitmap[block_index] = 0;
		metadata.free_blocks++;
	}
}

static simfs_error_t init_directory_block(uint32_t block_index) {

	dir_entry_t entries[dir_entries_per_block];

	for (auto i = 0u; i < dir_entries_per_block; ++i) {
		entries[i].inode_num = invalid_inode;
		memset(entries[i].filename, 0, sizeof(entries[i].filename));
	}

	return write_at(data_block_offset(block_index), entries, sizeof(entries));
}

static simfs_error_t read_directory_block(uint32_t block_index, dir_entry_t entries[static dir_entries_per_block]) {
	return read_at(data_block_offset(block_index), entries, sizeof(dir_entry_t) * dir_entries_per_block);
}

static simfs_error_t write_directory_block(uint32_t					 block_index,
																					 const dir_entry_t entries[static dir_entries_per_block]) {
	return write_at(data_block_offset(block_index), entries, sizeof(dir_entry_t) * dir_entries_per_block);
}

static simfs_error_t load_root_inode(inode_t *root) {
	auto rc = read_inode(fs_rootdir, root);
	if (rc)
		return rc;
	if (root->stat == STAT_VACANT || root->type != TYPE_DIR)
		return SIMFS_ERR_CORRUPTED_FS;
	return SIMFS_OK;
}

static simfs_error_t is_valid_name(const char *name) {

	if (!name)
		return false;

	auto len = strlen(name);
	if (len == 0 || len > max_filename_len)
		return false;

	if (strchr(name, '/') || strchr(name, '\\'))
		return false;

	return true;
}

static simfs_error_t load_directory_inode(uint32_t inode_num, inode_t *dir_inode) {

	if (!dir_inode)
		return SIMFS_ERR_INVALID_ARGUMENT;

	auto rc = read_inode(inode_num, dir_inode);
	if (rc)
		return rc;
	if (dir_inode->stat == STAT_VACANT)
		return SIMFS_ERR_NOT_FOUND;
	if (dir_inode->type != TYPE_DIR)
		return SIMFS_ERR_NOT_A_DIRECTORY;
	return SIMFS_OK;
}

static simfs_error_t find_entry_in_directory(uint32_t dir_inode_num, const char *name, dir_lookup_t *lookup) {

	if (!is_valid_name(name))
		return SIMFS_ERR_INVALID_PATH;

	inode_t dir_inode;
	auto		rc = load_directory_inode(dir_inode_num, &dir_inode);
	if (rc)
		return rc;

	dir_entry_t entries[dir_entries_per_block];

	for (auto i = 0u; i < num_direct_pointers; ++i) {
		if (dir_inode.direct_pointers[i] == invalid_block)
			continue;

		rc = read_directory_block(dir_inode.direct_pointers[i], entries);
		if (rc)
			return rc;

		for (auto j = 0u; j < dir_entries_per_block; ++j) {
			if (entries[j].inode_num != invalid_inode &&
					strncmp(entries[j].filename, name, sizeof(entries[j].filename)) == 0) {
				if (lookup) {
					lookup->block_num		= dir_inode.direct_pointers[i];
					lookup->entry_index = j;
					lookup->entry				= entries[j];
				}
				return SIMFS_OK;
			}
		}
	}

	return SIMFS_ERR_NOT_FOUND;
}

static simfs_error_t add_entry_to_directory(uint32_t dir_inode_num, const char *name, uint32_t inode_num) {
	if (!is_valid_name(name))
		return SIMFS_ERR_INVALID_PATH;

	inode_t dir_inode;
	auto		rc = load_directory_inode(dir_inode_num, &dir_inode);
	if (rc)
		return rc;

	rc = find_entry_in_directory(dir_inode_num, name, nullptr);
	if (!rc)
		return SIMFS_ERR_ALREADY_EXISTS;
	if (rc != SIMFS_ERR_NOT_FOUND)
		return rc;

	auto				new_block = invalid_block;
	dir_entry_t entries[dir_entries_per_block];

	for (auto i = 0u; i < num_direct_pointers; ++i) {
		if (dir_inode.direct_pointers[i] == invalid_block) {
			rc = allocate_block(&new_block);
			if (rc)
				return rc;

			dir_inode.direct_pointers[i] = new_block;
			rc													 = init_directory_block(new_block);
			if (!rc)
				rc = write_inode(dir_inode_num, &dir_inode);
			if (rc) {
				dir_inode.direct_pointers[i] = invalid_block;
				free_block(new_block);
				return rc;
			}
		}

		rc = read_directory_block(dir_inode.direct_pointers[i], entries);
		if (rc)
			return rc;

		for (auto j = 0u; j < dir_entries_per_block; ++j) {
			if (entries[j].inode_num == invalid_inode) {
				entries[j].inode_num = inode_num;
				memset(entries[j].filename, 0, sizeof(entries[j].filename));
				strncpy(entries[j].filename, name, max_filename_len);

				rc = write_directory_block(dir_inode.direct_pointers[i], entries);
				if (rc)
					return rc;

				dir_inode.size += sizeof(dir_entry_t);
				rc = write_inode(dir_inode_num, &dir_inode);
				if (rc)
					return rc;

				return SIMFS_OK;
			}
		}
	}

	return SIMFS_ERR_NO_SPACE;
}

static simfs_error_t remove_entry_from_directory(uint32_t dir_inode_num, const dir_lookup_t *lookup) {

	if (!lookup)
		return SIMFS_ERR_INVALID_ARGUMENT;
	inode_t dir_inode;
	auto		rc = load_directory_inode(dir_inode_num, &dir_inode);
	if (rc)
		return rc;

	dir_entry_t entries[dir_entries_per_block];
	rc = read_directory_block(lookup->block_num, entries);
	if (rc)
		return rc;

	entries[lookup->entry_index].inode_num = invalid_inode;
	memset(entries[lookup->entry_index].filename, 0, sizeof(entries[lookup->entry_index].filename));

	rc = write_directory_block(lookup->block_num, entries);
	if (rc)
		return rc;

	if (dir_inode.size >= sizeof(dir_entry_t))
		dir_inode.size -= sizeof(dir_entry_t);
	else
		dir_inode.size = 0;

	return write_inode(dir_inode_num, &dir_inode);
}

static simfs_error_t parse_path_segments(const char *path,
																				 char				 segments[static max_path_segments][max_filename_len + 1],
																				 uint32_t		*segment_count_out) {

	if (!path || !segment_count_out)
		return SIMFS_ERR_INVALID_ARGUMENT;

	auto i		 = 0ull;
	auto count = 0u;
	while (path[i] != '\0') {
		auto len = 0u;

		while (path[i] == '/' || path[i] == '\\') {
			i++;
		}

		if (path[i] == '\0')
			break;
		if (count >= max_path_segments)
			return SIMFS_ERR_PATH_TOO_DEEP;

		while (path[i] != '\0' && path[i] != '/' && path[i] != '\\') {
			if (len >= max_filename_len)
				return SIMFS_ERR_NAME_TOO_LONG;
			segments[count][len] = path[i];
			len++;
			i++;
		}

		segments[count][len] = '\0';
		if (!is_valid_name(segments[count]))
			return SIMFS_ERR_INVALID_PATH;

		count++;
	}

	if (count == 0)
		return SIMFS_ERR_INVALID_PATH;

	*segment_count_out = count;
	return SIMFS_OK;
}

static simfs_error_t resolve_parent_and_name(const char *path, uint32_t *parent_inode_out,
																						 char name_out[static max_filename_len + 1]) {
	if (!parent_inode_out || !name_out)
		return SIMFS_ERR_INVALID_ARGUMENT;

	char segments[max_path_segments][max_filename_len + 1];
	auto segment_count = 0u;
	auto rc						 = parse_path_segments(path, segments, &segment_count);
	if (rc)
		return rc;

	auto parent_inode = fs_rootdir;
	for (auto i = 0u; i + 1 < segment_count; ++i) {
		dir_lookup_t lookup;
		inode_t			 next_dir;

		rc = find_entry_in_directory(parent_inode, segments[i], &lookup);
		if (rc)
			return rc;
		rc = load_directory_inode(lookup.entry.inode_num, &next_dir);
		if (rc)
			return rc;

		parent_inode = lookup.entry.inode_num;
	}

	memset(name_out, 0, max_filename_len + 1);
	strncpy(name_out, segments[segment_count - 1], max_filename_len);
	*parent_inode_out = parent_inode;
	return SIMFS_OK;
}

static simfs_error_t resolve_path(const char *path, uint32_t *parent_inode_out, dir_lookup_t *lookup_out) {

	if (!lookup_out)
		return SIMFS_ERR_INVALID_ARGUMENT;
	char segments[max_path_segments][max_filename_len + 1];
	auto segment_count = 0u;

	auto rc = parse_path_segments(path, segments, &segment_count);
	if (rc)
		return rc;

	auto parent_inode = fs_rootdir;
	for (auto i = 0u; i < segment_count; ++i) {
		dir_lookup_t lookup;

		rc = find_entry_in_directory(parent_inode, segments[i], &lookup);
		if (rc)
			return rc;

		if (i + 1 < segment_count) {
			inode_t next_dir;
			rc = load_directory_inode(lookup.entry.inode_num, &next_dir);
			if (rc)
				return rc;
			parent_inode = lookup.entry.inode_num;
			continue;
		}

		*lookup_out = lookup;
		if (parent_inode_out)
			*parent_inode_out = parent_inode;
		return SIMFS_OK;
	}

	return SIMFS_ERR_NOT_FOUND;
}

static bool is_root_path(const char *path) {

	if (!path)
		return false;

	for (auto i = 0ull; path[i] != '\0'; ++i) {
		if (path[i] != '/' && path[i] != '\\')
			return false;
	}

	return true;
}

static simfs_error_t resolve_directory_inode(const char *path, uint32_t *inode_out) {

	if (!path || !inode_out)
		return SIMFS_ERR_INVALID_ARGUMENT;

	if (is_root_path(path)) {
		*inode_out = fs_rootdir;
		return SIMFS_OK;
	}

	dir_lookup_t lookup;
	inode_t			 dir_inode;

	auto rc = resolve_path(path, nullptr, &lookup);
	if (rc)
		return rc;
	rc = load_directory_inode(lookup.entry.inode_num, &dir_inode);
	if (rc)
		return rc;

	*inode_out = lookup.entry.inode_num;
	return SIMFS_OK;
}

static simfs_error_t list_directory_inode(uint32_t dir_inode_num) {

	inode_t dir_inode;

	auto rc = load_directory_inode(dir_inode_num, &dir_inode);
	if (rc)
		return rc;

	dir_entry_t entries[dir_entries_per_block];
	auto				count = 0;
	for (auto i = 0u; i < num_direct_pointers; ++i) {
		if (dir_inode.direct_pointers[i] == invalid_block)
			continue;

		rc = read_directory_block(dir_inode.direct_pointers[i], entries);
		if (rc)
			return rc;

		for (auto j = 0u; j < dir_entries_per_block; ++j) {
			if (entries[j].inode_num != invalid_inode) {
				inode_t			item_inode;
				int					inode_loaded = 0;
				const char *kind				 = "\033[31m"
																	 "<UNK>"
																	 "\033[0m";

				memset(&item_inode, 0, sizeof(item_inode));
				if (!read_inode(entries[j].inode_num, &item_inode)) {
					inode_loaded = 1;
					if (item_inode.type == TYPE_DIR)
						kind = "\033[36m"
									 "<DIR>"
									 "\033[0m";
					else if (item_inode.type == TYPE_FILE)
						kind = "\033[32m"
									 "<FILE>"
									 "\033[0m";
				}

				printf("%s\t", kind);
				// if it's file, also print the size.
				if (inode_loaded && item_inode.type == TYPE_FILE) {
					printf("\033[37m%s\t%u bytes\033[0m\n", entries[j].filename, item_inode.size);
				} else {
					printf("\033[37m%s\033[0m\n", entries[j].filename);
				}
				count++;
			}
		}
	}

	return count;
}

static bool is_directory_empty(const inode_t *inode) {

	dir_entry_t entries[dir_entries_per_block];

	for (auto i = 0u; i < num_direct_pointers; ++i) {
		if (inode->direct_pointers[i] == invalid_block)
			continue;

		if (read_directory_block(inode->direct_pointers[i], entries) != 0)
			return false;

		for (auto j = 0u; j < dir_entries_per_block; ++j) {
			if (entries[j].inode_num != invalid_inode)
				return false;
		}
	}

	return true;
}

static simfs_error_t create_object(const char *name, file_type_t type) {

	char item_name[max_filename_len + 1];
	auto parent_inode = fs_rootdir;
	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;

	auto rc = resolve_parent_and_name(name, &parent_inode, item_name);
	if (rc)
		return rc;

	rc = find_entry_in_directory(parent_inode, item_name, nullptr);
	if (!rc)
		return SIMFS_ERR_ALREADY_EXISTS;
	if (rc != SIMFS_ERR_NOT_FOUND)
		return rc;

	auto inode_index = invalid_inode;
	rc							 = allocate_inode(&inode_index);
	if (rc)
		return rc;

	auto inode = init_inode(type, STAT_IN_USE);

	auto block_index = invalid_block;
	if (type == TYPE_DIR) {
		rc = allocate_block(&block_index);
		if (rc) {
			free_inode_slot(inode_index);
			return rc;
		}
		inode.direct_pointers[0] = block_index;
		rc											 = init_directory_block(block_index);
		if (rc) {
			free_block(block_index);
			free_inode_slot(inode_index);
			return rc;
		}
	}

	rc = write_inode(inode_index, &inode);
	if (rc) {
		if (block_index != invalid_block)
			free_block(block_index);
		free_inode_slot(inode_index);
		return rc;
	}

	rc = add_entry_to_directory(parent_inode, item_name, inode_index);
	if (rc) {
		auto cleared = init_inode(TYPE_FILE, STAT_VACANT);
		if (block_index != invalid_block)
			free_block(block_index);
		free_inode_slot(inode_index);
		write_inode(inode_index, &cleared);
		return rc;
	}

	rc = flush_metadata();
	if (rc)
		return rc;

	return SIMFS_OK;
}

///////////////////////////////////////////////////////////////////////////////

// Initialize the virtual disk and filesystem metadata.
simfs_error_t simfs_format(const char *disk_name) {

	if (!disk_name)
		return SIMFS_ERR_INVALID_ARGUMENT;

	close_disk();

	vdisk = fopen(disk_name, "wb+");
	if (!vdisk)
		return SIMFS_ERR_IO;

	memset(inode_bitmap, 0, sizeof(inode_bitmap));
	memset(block_bitmap, 0, sizeof(block_bitmap));
	memset(open_files, 0, sizeof(open_files));

	metadata = (metadata_t){
		.ident_marker				 = magic_ident,
		.total_blocks				 = data_block_size,
		.free_blocks				 = data_block_size,
		.total_inodes				 = max_inodes,
		.free_inodes				 = max_inodes,
		.disk_size_bytes		 = disk_size_bytes,
		.inode_bitmap_offset = sizeof(metadata_t),
		.block_bitmap_offset = sizeof(metadata_t) + max_inodes,
		.inode_table_offset	 = sizeof(metadata_t) + max_inodes + data_block_size,
		.data_region_offset	 = sizeof(metadata_t) + max_inodes + data_block_size + ((uint64_t)max_inodes * sizeof(inode_t)),
	};

	uint8_t zero_block[block_size] = {0};

	for (auto written = 0ull; written < disk_size_bytes;) {
		auto chunk = min_u32(block_size, (uint32_t)(disk_size_bytes - written));
		if (fwrite(zero_block, 1, chunk, vdisk) != chunk) {
			close_disk();
			return SIMFS_ERR_IO;
		}
		written += chunk;
	}

	auto rc = flush_metadata();
	if (rc) {
		close_disk();
		return rc;
	}

	for (auto i = 0u; i < max_inodes; ++i) {
		auto empty_inode = init_inode(TYPE_FILE, STAT_VACANT);
		rc							 = write_inode(i, &empty_inode);
		if (rc) {
			close_disk();
			return rc;
		}
	}

	inode_bitmap[fs_rootdir] = 1;
	block_bitmap[0]					 = 1;
	metadata.free_inodes--;
	metadata.free_blocks--;

	auto root_inode								= init_inode(TYPE_DIR, STAT_IN_USE);
	root_inode.direct_pointers[0] = 0;

	rc = write_inode(fs_rootdir, &root_inode);
	if (!rc)
		rc = init_directory_block(0);
	if (!rc)
		rc = flush_metadata();
	if (rc) {
		close_disk();
		return rc;
	}

	printf("\033[37mDisk formatted. Blocks: %u, Inodes: %u\033[0m\n", metadata.total_blocks, metadata.total_inodes);
	return SIMFS_OK;
}

// Mount an existing virtual disk by loading metadata and bitmaps.

simfs_error_t simfs_mount(const char *disk_name) {
	if (!disk_name)
		return SIMFS_ERR_INVALID_ARGUMENT;

	close_disk();
	vdisk = fopen(disk_name, "rb+");
	if (!vdisk) {
		if (errno == ENOENT)
			return SIMFS_ERR_NOT_FOUND;
		return SIMFS_ERR_IO;
	}

	auto rc = read_at(0, &metadata, sizeof(metadata));
	if (rc || metadata.ident_marker != magic_ident || metadata.total_inodes > max_inodes ||
			metadata.total_blocks > max_disk_blocks || metadata.disk_size_bytes != disk_size_bytes) {
		close_disk();
		if (rc)
			return rc;
		return SIMFS_ERR_CORRUPTED_FS;
	}

	memset(inode_bitmap, 0, sizeof(inode_bitmap));
	memset(block_bitmap, 0, sizeof(block_bitmap));
	rc = read_at(metadata.inode_bitmap_offset, inode_bitmap, metadata.total_inodes);
	if (!rc) {
		rc = read_at(metadata.block_bitmap_offset, block_bitmap, metadata.total_blocks);
	}
	if (rc) {
		close_disk();
		return rc;
	}

	memset(open_files, 0, sizeof(open_files));

	printf("\033[37mDisk mounted. Free blocks: %u, Free inodes: %u\033[0m\n", metadata.free_blocks, metadata.free_inodes);
	return SIMFS_OK;
}

simfs_error_t simfs_touch(const char *name) { return create_object(name, TYPE_FILE); }

simfs_error_t simfs_mkdir(const char *name) { return create_object(name, TYPE_DIR); }

simfs_fd_t simfs_open(const char *name) {

	dir_lookup_t lookup;
	inode_t			 inode;

	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;

	auto rc = resolve_path(name, nullptr, &lookup);
	if (rc)
		return rc;
	rc = read_inode(lookup.entry.inode_num, &inode);
	if (rc)
		return rc;
	if (inode.type != TYPE_FILE)
		return SIMFS_ERR_IS_A_DIRECTORY;

	for (auto i = 0u; i < max_open_files; ++i) {
		if (open_files[i].stat == STAT_VACANT) {
			open_files[i].stat			= STAT_IN_USE;
			open_files[i].inode_num = lookup.entry.inode_num;
			open_files[i].offset		= 0;
			return (int)i;
		}
	}

	return SIMFS_ERR_TOO_MANY_OPEN_FILES;
}

simfs_error_t simfs_close(int fd) {
	if (fd < 0 || fd >= (int)max_open_files || open_files[fd].stat == STAT_VACANT)
		return SIMFS_ERR_BAD_FILE_DESCRIPTOR;

	open_files[fd].stat			 = STAT_VACANT;
	open_files[fd].inode_num = 0;
	open_files[fd].offset		 = 0;
	return SIMFS_OK;
}

simfs_fd_t simfs_read(int fd, void *buffer, uint32_t size) {

	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;
	if (!buffer)
		return SIMFS_ERR_INVALID_ARGUMENT;
	if (fd < 0 || fd >= (int)max_open_files || open_files[fd].stat == STAT_VACANT)
		return SIMFS_ERR_BAD_FILE_DESCRIPTOR;

	inode_t inode;
	auto		rc = read_inode(open_files[fd].inode_num, &inode);
	if (rc)
		return rc;
	if (inode.type != TYPE_FILE)
		return SIMFS_ERR_IS_A_DIRECTORY;

	auto offset = open_files[fd].offset;
	if (offset >= inode.size)
		return 0;

	auto to_read		= min_u32(size, inode.size - offset);
	auto bytes_read = 0u;

	while (bytes_read < to_read) {
		auto block_slot = offset / block_size;
		auto block_off	= offset % block_size;
		auto chunk			= min_u32(to_read - bytes_read, block_size - block_off);
		auto disk_block = 0u;

		if (block_slot >= num_direct_pointers)
			break;
		if (inode.direct_pointers[block_slot] == invalid_block)
			break;

		disk_block = inode.direct_pointers[block_slot];
		rc				 = read_at(data_block_offset(disk_block) + block_off, (uint8_t *)buffer + bytes_read, chunk);
		if (rc)
			return rc;

		bytes_read += chunk;
		offset += chunk;
	}

	open_files[fd].offset = offset;
	return (int)bytes_read;
}

simfs_error_t simfs_write(int fd, const void *buffer, uint32_t size) {

	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;
	if (!buffer && size > 0)
		return SIMFS_ERR_INVALID_ARGUMENT;
	if (fd < 0 || fd >= (int)max_open_files || open_files[fd].stat == STAT_VACANT)
		return SIMFS_ERR_BAD_FILE_DESCRIPTOR;

	inode_t inode;
	auto		rc = read_inode(open_files[fd].inode_num, &inode);
	if (rc)
		return rc;
	if (inode.type != TYPE_FILE)
		return SIMFS_ERR_IS_A_DIRECTORY;

	auto offset				 = open_files[fd].offset;
	auto bytes_written = 0u;

	while (bytes_written < size) {
		auto block_slot = offset / block_size;
		auto block_off	= offset % block_size;
		auto chunk			= min_u32(size - bytes_written, block_size - block_off);

		if (block_slot >= num_direct_pointers)
			break;

		if (inode.direct_pointers[block_slot] == invalid_block) {
			auto		new_block							 = invalid_block;
			uint8_t zero_block[block_size] = {0};

			rc = allocate_block(&new_block);
			if (rc)
				break;

			inode.direct_pointers[block_slot] = new_block;
			rc																= write_at(data_block_offset(new_block), zero_block, sizeof(zero_block));
			if (rc)
				return rc;
		}

		rc = write_at(data_block_offset(inode.direct_pointers[block_slot]) + block_off,
									(const uint8_t *)buffer + bytes_written, chunk);
		if (rc)
			return rc;

		bytes_written += chunk;
		offset += chunk;
	}

	if (offset > inode.size)
		inode.size = offset;

	open_files[fd].offset = offset;

	rc = write_inode(open_files[fd].inode_num, &inode);
	if (rc)
		return rc;
	rc = flush_metadata();
	if (rc)
		return rc;

	if (bytes_written == 0 && size > 0)
		return SIMFS_ERR_NO_SPACE;
	return (int)bytes_written;
}

simfs_error_t simfs_remove(const char *name) {

	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;

	auto				 parent_inode = fs_rootdir;
	inode_t			 inode;
	dir_lookup_t lookup;
	auto				 rc = resolve_path(name, &parent_inode, &lookup);
	if (rc)
		return rc;
	if (lookup.entry.inode_num == fs_rootdir)
		return SIMFS_ERR_INVALID_ARGUMENT;

	rc = read_inode(lookup.entry.inode_num, &inode);
	if (rc)
		return rc;

	if (inode.type == TYPE_DIR && !is_directory_empty(&inode))
		return SIMFS_ERR_DIRECTORY_NOT_EMPTY;

	for (auto i = 0u; i < max_open_files; ++i) {
		if (open_files[i].stat != STAT_IN_USE && open_files[i].inode_num == lookup.entry.inode_num) {
			open_files[i] = (simfs_file_descriptor_t){
				.stat			 = STAT_VACANT,
				.inode_num = 0u,
				.offset		 = 0u,
			};
		}
	}

	for (auto i = 0u; i < num_direct_pointers; ++i) {
		if (inode.direct_pointers[i] != invalid_block)
			free_block(inode.direct_pointers[i]);
	}

	inode = init_inode(TYPE_FILE, STAT_VACANT);

	rc = write_inode(lookup.entry.inode_num, &inode);
	if (rc)
		return rc;
	free_inode_slot(lookup.entry.inode_num);

	rc = remove_entry_from_directory(parent_inode, &lookup);
	if (rc)
		return rc;
	rc = flush_metadata();
	if (rc)
		return rc;

	return SIMFS_OK;
}

simfs_error_t simfs_ls(void) { return simfs_ls_path("/"); }

simfs_error_t simfs_ls_path(const char *path) {

	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;

	auto dir_inode = fs_rootdir;
	auto rc				 = resolve_directory_inode(path, &dir_inode);
	if (rc)
		return rc;

	return list_directory_inode(dir_inode);
}

simfs_error_t simfs_cd(const char *path) {

	if (!vdisk)
		return SIMFS_ERR_NOT_MOUNTED;

	auto dir_inode = fs_rootdir;
	return resolve_directory_inode(path, &dir_inode);
}
