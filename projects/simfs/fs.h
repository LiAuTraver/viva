#include "fs_impl.h"

struct {
	__typeof__(simfs_format) *const	 format;
	__typeof__(simfs_mount) *const	 mount;
	__typeof__(simfs_touch) *const	 touch;
	__typeof__(simfs_open) *const		 open;
	__typeof__(simfs_close) *const	 close;
	__typeof__(simfs_read) *const		 read;
	__typeof__(simfs_write) *const	 write;
	__typeof__(simfs_remove) *const	 remove;
	__typeof__(simfs_mkdir) *const	 mkdir;
	__typeof__(simfs_ls) *const			 ls;
	__typeof__(simfs_ls_path) *const ls_path;
	__typeof__(simfs_cd) *const			 cd;

} static const simfs_ops = {
	.format	 = simfs_format,
	.mount	 = simfs_mount,
	.touch	 = simfs_touch,
	.open		 = simfs_open,
	.close	 = simfs_close,
	.read		 = simfs_read,
	.write	 = simfs_write,
	.remove	 = simfs_remove,
	.mkdir	 = simfs_mkdir,
	.ls			 = simfs_ls,
	.ls_path = simfs_ls_path,
	.cd			 = simfs_cd,
};

#pragma GCC poison simfs_format
#pragma GCC poison simfs_mount
#pragma GCC poison simfs_touch
#pragma GCC poison simfs_open
#pragma GCC poison simfs_close
#pragma GCC poison simfs_read
#pragma GCC poison simfs_write
#pragma GCC poison simfs_remove
#pragma GCC poison simfs_mkdir
#pragma GCC poison simfs_ls
#pragma GCC poison simfs_ls_path
#pragma GCC poison simfs_cd
