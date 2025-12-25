/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef _FS_H
#define _FS_H

#include <stddef.h>

#define RAMFS_BLOCK_SIZE 4096
#define RAMFS_MAX_BLOCKS 256
#define RAMFS_INODE_COUNT 64

typedef struct {
    char name[16];
    size_t size;
    int type;
    int blocks[8];
} Inode;

typedef struct {
    size_t total_blocks;
    size_t free_blocks;
    size_t total_inodes;
    size_t free_inodes;
    char block_map[RAMFS_MAX_BLOCKS];
    char inode_map[RAMFS_INODE_COUNT];
} Superblock;

class RamFileSystemManager {
public:
    RamFileSystemManager(char* start_addr, size_t size);
    void create_file(const char* name, int type);
    void write_to_file(const char* name, const char* data, size_t size);
    char* read_from_file(const char* name);
    void delete_file(const char* name);
    size_t get_file_size(const char* name);
    void list_files();

private:
    char* fs_start_address;
    Superblock* superblock;
    Inode* inode_table;
    char* data_blocks;
    size_t total_fs_size;
};

extern RamFileSystemManager* ram_fs_manager;

#endif
