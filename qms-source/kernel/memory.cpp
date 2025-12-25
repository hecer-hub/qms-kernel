/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "memory.h"
#include "func/writet.h"
#include "func/string.h"

void init_memory_manager(uintptr_t start, size_t size) {
    kernel_mem_manager = (MemoryManager*)start;
    
    uintptr_t real_start = start + sizeof(MemoryManager);
    
    *kernel_mem_manager = MemoryManager(real_start, size - sizeof(MemoryManager));
    
    user_mem_manager = kernel_mem_manager; 
}

MemoryManager::MemoryManager(uintptr_t start, size_t size) {
    base_addr = start;
    total_pages = size / PAGE_SIZE;
    free_pages = total_pages;

    bitmap = (uint8_t*)start;
    size_t bitmap_size = (total_pages / 8) + 1;
    
    memset(bitmap, 0, bitmap_size);

    size_t bitmap_pages = (bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    for (size_t i = 0; i < bitmap_pages; i++) {
        mark_page(i, true);
    }
}

void MemoryManager::mark_page(size_t page_idx, bool used) {
    if (used) {
        bitmap[page_idx / 8] |= (1 << (page_idx % 8));
        if (free_pages > 0) free_pages--;
    } else {
        bitmap[page_idx / 8] &= ~(1 << (page_idx % 8));
        free_pages++;
    }
}

bool MemoryManager::is_page_used(size_t page_idx) const {
    return bitmap[page_idx / 8] & (1 << (page_idx % 8));
}

void* MemoryManager::allocate(size_t size) {
    if (size == 0) return nullptr;
    size_t pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    size_t found_pages = 0;
    size_t start_page = 0;

    for (size_t i = 0; i < total_pages; i++) {
        if (!is_page_used(i)) {
            if (found_pages == 0) start_page = i;
            found_pages++;
            if (found_pages == pages_needed) {
                for (size_t j = start_page; j < start_page + pages_needed; j++) {
                    mark_page(j, true);
                }
                return (void*)(base_addr + (start_page * PAGE_SIZE));
            }
        } else {
            found_pages = 0;
        }
    }
    print_string_ln("OUT OF MEMORY!");
    return nullptr;
}

void MemoryManager::free(void* p, size_t size) {
    if (!p) return;
    uintptr_t addr = (uintptr_t)p;
    size_t start_page = (addr - base_addr) / PAGE_SIZE;
    size_t pages_to_free = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    for (size_t i = start_page; i < start_page + pages_to_free; i++) {
        mark_page(i, false);
    }
}

size_t MemoryManager::get_total_size() const { return total_pages * PAGE_SIZE; }
size_t MemoryManager::get_free_size() const { return free_pages * PAGE_SIZE; }
size_t MemoryManager::get_used_size() const { return get_total_size() - get_free_size(); }

void* operator new(size_t size) { return kernel_mem_manager->allocate(size); }
void* operator new[](size_t size) { return operator new(size); }
void operator delete(void* p) noexcept { } 
void operator delete[](void* p) noexcept { operator delete(p); }
void operator delete(void* p, size_t size) noexcept { kernel_mem_manager->free(p, size); }
