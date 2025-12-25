/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "fs.h"
#include "writet.h"
#include "../memory.h"
#include "string.h"

RamFileSystemManager::RamFileSystemManager(char* start_addr, size_t size) {
    fs_start_address = start_addr;
    total_fs_size = size;

    superblock = (Superblock*)fs_start_address;
    inode_table = (Inode*)(fs_start_address + sizeof(Superblock));
    data_blocks = fs_start_address + sizeof(Superblock) + (sizeof(Inode) * RAMFS_INODE_COUNT);

    memset(fs_start_address, 0, sizeof(Superblock) + (sizeof(Inode) * RAMFS_INODE_COUNT));
    
    superblock->total_inodes = RAMFS_INODE_COUNT;
    superblock->free_inodes = RAMFS_INODE_COUNT;
    superblock->total_blocks = RAMFS_MAX_BLOCKS;
    superblock->free_blocks = RAMFS_MAX_BLOCKS;

    print_string_ln("[RAMFS] Memory mapped and cleared.");
}

void RamFileSystemManager::create_file(const char* name, int type) {
    int inode_index = -1;
    for (int i = 0; i < RAMFS_INODE_COUNT; ++i) {
        if (superblock->inode_map[i] == 0) {
            inode_index = i;
            break;
        }
    }

    if (inode_index == -1) {
        print_string_ln("Error: RAMFS Full (No Inodes)");
        return;
    }

    Inode* new_inode = &inode_table[inode_index];
    memset(new_inode, 0, sizeof(Inode));
    strncpy(new_inode->name, name, 15);
    new_inode->type = type;
    new_inode->size = 0;

    superblock->inode_map[inode_index] = 1;
    superblock->free_inodes--;

    print_string("File created: ");
    print_string_ln(name);
}

void RamFileSystemManager::list_files() {
    print_string_ln("--- RAMFS File List ---");
    int count = 0;
    for (int i = 0; i < RAMFS_INODE_COUNT; i++) {
        if (superblock->inode_map[i] == 1) {
            print_string("- ");
            print_string(inode_table[i].name);
            print_string(" [Size: ");
            print_int(inode_table[i].size);
            print_string_ln(" bytes]");
            count++;
        }
    }
    if (count == 0) {
        print_string_ln("(Empty)");
    }
}

size_t RamFileSystemManager::get_file_size(const char* name) {
    for (int i = 0; i < RAMFS_INODE_COUNT; i++) {
        if (superblock->inode_map[i] == 1 && strcmp(inode_table[i].name, name) == 0) {
            return inode_table[i].size;
        }
    }
    return 0;
}

char* RamFileSystemManager::read_from_file(const char* name) {
    int idx = -1;
    for (int i = 0; i < RAMFS_INODE_COUNT; i++) {
        if (superblock->inode_map[i] == 1 && strcmp(inode_table[i].name, name) == 0) {
            idx = i; break;
        }
    }
    if (idx == -1) return nullptr;
    Inode* inode = &inode_table[idx];
    char* data = (char*)kernel_mem_manager->allocate(inode->size + 1);
    if (!data) return nullptr;
    
    size_t read_bytes = 0;
    for (int b = 0; read_bytes < inode->size && b < 8; b++) {
        size_t to_copy = inode->size - read_bytes;
        if (to_copy > RAMFS_BLOCK_SIZE) to_copy = RAMFS_BLOCK_SIZE;
        memcpy(data + read_bytes, data_blocks + (inode->blocks[b] * RAMFS_BLOCK_SIZE), to_copy);
        read_bytes += to_copy;
    }
    data[inode->size] = '\0';
    return data;
}

void RamFileSystemManager::write_to_file(const char* name, const char* data, size_t size) {
    int idx = -1;
    for (int i = 0; i < RAMFS_INODE_COUNT; i++) {
        if (superblock->inode_map[i] == 1 && strcmp(inode_table[i].name, name) == 0) {
            idx = i; break;
        }
    }
    if (idx == -1) return;
    Inode* inode = &inode_table[idx];
    inode->size = size;
    size_t written = 0;
    for (int b = 0; written < size && b < 8; b++) {
        int block_idx = -1;
        for (int m = 0; m < RAMFS_MAX_BLOCKS; m++) {
            if (superblock->block_map[m] == 0) { block_idx = m; break; }
        }
        if (block_idx == -1) break;
        superblock->block_map[block_idx] = 1;
        inode->blocks[b] = block_idx;
        size_t to_copy = size - written;
        if (to_copy > RAMFS_BLOCK_SIZE) to_copy = RAMFS_BLOCK_SIZE;
        memcpy(data_blocks + (block_idx * RAMFS_BLOCK_SIZE), data + written, to_copy);
        written += to_copy;
        superblock->free_blocks--;
    }
}

void RamFileSystemManager::delete_file(const char* name) {
    int idx = -1;
    for (int i = 0; i < RAMFS_INODE_COUNT; i++) {
        if (superblock->inode_map[i] == 1 && strcmp(inode_table[i].name, name) == 0) {
            idx = i; break;
        }
    }
    if (idx == -1) return;
    Inode* inode = &inode_table[idx];
    int blocks = (inode->size + RAMFS_BLOCK_SIZE - 1) / RAMFS_BLOCK_SIZE;
    for (int b = 0; b < blocks && b < 8; b++) {
        superblock->block_map[inode->blocks[b]] = 0;
        superblock->free_blocks++;
    }
    superblock->inode_map[idx] = 0;
    superblock->free_inodes++;
}
