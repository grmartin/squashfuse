#ifndef SQFS_DIR_H
#define SQFS_DIR_H

#include <limits.h>

#include "common.h"
#include "squashfs_fs.h"

typedef struct {
	sqfs_inode_id inode;
	uint32_t inode_number;
	int type;
	char *name;
} sqfs_dir_entry;

typedef struct {
	sqfs *fs;
	
	sqfs_md_cursor cur;
	size_t remain;
	struct squashfs_dir_header header;
	
	char name[PATH_MAX+1];
	sqfs_dir_entry entry;
} sqfs_dir;

sqfs_err sqfs_opendir(sqfs *fs, sqfs_inode *inode, sqfs_dir *dir);
sqfs_dir_entry *sqfs_readdir(sqfs_dir *dir, sqfs_err *err);

#endif