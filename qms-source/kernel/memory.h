/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef _MEMORY_H
#define _MEMORY_H

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096

class MemoryManager {
public:
    MemoryManager(uintptr_t start, size_t size);
    void* allocate(size_t size);
    void  free(void* p, size_t size);
    size_t get_total_size() const;
    size_t get_free_size() const;
    size_t get_used_size() const;

private:
    uint8_t* bitmap;      
    uintptr_t base_addr;  
    size_t total_pages;   
    size_t free_pages;    

    void mark_page(size_t page_idx, bool used);
    bool is_page_used(size_t page_idx) const;
};

extern MemoryManager* kernel_mem_manager;
extern MemoryManager* user_mem_manager;

void init_memory_manager(uintptr_t start, size_t size);

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* p) noexcept;
void operator delete[](void* p) noexcept;
void operator delete(void* p, size_t size) noexcept;

#endif
