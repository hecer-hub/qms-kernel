/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "init.h"
#include "func/writet.h"
#include "func/fs.h"
#include "memory.h"
#include "func/idt.h"
#include "func/vga_module.h"
#include "bin/shell.h"

uint32_t multiboot_magic;
uint32_t multiboot_addr;

__asm__ (
    ".section .multiboot\n"
    ".align 4\n"
    ".long 0x1BADB002\n"            
    ".long 0x00000003\n"            
    ".long -(0x1BADB002 + 0x00000003)\n"

    ".section .text\n"
    ".global _start\n"
    "_start:\n"
    "mov %eax, multiboot_magic\n"   
    "mov %ebx, multiboot_addr\n"    
    "push %ebp\n"                   
    "mov %esp, %ebp\n"
    "call kernel_main\n"            
    "hlt\n"
);

struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
};

extern "C" {
    void show_banner(); 
}

extern "C" char end; 

MemoryManager* kernel_mem_manager = nullptr;
MemoryManager* user_mem_manager = nullptr;
RamFileSystemManager* ram_fs_manager = nullptr;

char km_obj[sizeof(MemoryManager)];
char um_obj[sizeof(MemoryManager)];
char fs_obj[sizeof(RamFileSystemManager)];

extern "C" void kernel_main() {
    vgacfg("80x25"); 
    init_writet();
    clear_screen();
    show_banner();
    print_string_ln("[QMS-KERNEL][V0.8.8 build-92]");

    uint32_t magic = multiboot_magic;
    multiboot_info* mbi = (multiboot_info*)multiboot_addr;

    print_string("MBI Flags: ");
    print_int(mbi->flags);
    print_string_ln("");

    size_t total_ram_kb = 0;

    if (magic == 0x2BADB002 && (mbi->flags & 1)) {
        total_ram_kb = mbi->mem_lower + mbi->mem_upper + 1024;
    } else {
        print_string_ln("Warning: Multiboot data not received. Fallback 8MB.");
        total_ram_kb = 8192; 
    }

    uintptr_t kernel_start = 0x100000;
    uintptr_t kernel_end = (uintptr_t)&end; 
    uintptr_t free_mem_start = (kernel_end + 4095) & ~4095;
    size_t usable_bytes = (total_ram_kb * 1024) - (free_mem_start - kernel_start);
    
    kernel_mem_manager = (MemoryManager*)km_obj;
    *kernel_mem_manager = MemoryManager(free_mem_start, usable_bytes / 2);

    user_mem_manager = (MemoryManager*)um_obj;
    *user_mem_manager = MemoryManager(free_mem_start + usable_bytes / 2, usable_bytes / 2);

    print_string("Detected RAM: "); 
    print_int(total_ram_kb / 1024); 
    print_string_ln(" MB");

    init_idt();

    size_t fs_size = 1024 * 512;
    void* fs_ptr = kernel_mem_manager->allocate(fs_size);
    if (fs_ptr) {
        ram_fs_manager = (RamFileSystemManager*)fs_obj;
        *ram_fs_manager = RamFileSystemManager((char*)fs_ptr, fs_size);
    }
    
    print_string_ln("Run QMSH...");
    run_shell();

    while(1) { __asm__ __volatile__("hlt"); }
}
