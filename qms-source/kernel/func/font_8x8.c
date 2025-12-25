/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "font_8x8.h"

#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

void vga_load_font_8x8(void) {
    uint8_t* font_memory = (uint8_t*)0xA0000;
    int i, j;
    
    uint8_t seq2, seq4, gc4, gc5, gc6;
    
    outb(VGA_SEQ_INDEX, 0x02);
    seq2 = inb(VGA_SEQ_DATA);
    
    outb(VGA_SEQ_INDEX, 0x04);
    seq4 = inb(VGA_SEQ_DATA);
    
    outb(VGA_GC_INDEX, 0x04);
    gc4 = inb(VGA_GC_DATA);
    
    outb(VGA_GC_INDEX, 0x05);
    gc5 = inb(VGA_GC_DATA);
    
    outb(VGA_GC_INDEX, 0x06);
    gc6 = inb(VGA_GC_DATA);
    
    outb(VGA_SEQ_INDEX, 0x02);
    outb(VGA_SEQ_DATA, 0x04);
    
    outb(VGA_SEQ_INDEX, 0x04);
    outb(VGA_SEQ_DATA, 0x07);
    
    outb(VGA_GC_INDEX, 0x04);
    outb(VGA_GC_DATA, 0x02);
    
    outb(VGA_GC_INDEX, 0x05);
    outb(VGA_GC_DATA, 0x00);
    
    outb(VGA_GC_INDEX, 0x06);
    outb(VGA_GC_DATA, 0x00);
    
    for (i = 0; i < 128; i++) {
        for (j = 0; j < 8; j++) {
            font_memory[i * 32 + j] = font_8x8[i * 8 + j];
        }
        for (j = 8; j < 32; j++) {
            font_memory[i * 32 + j] = 0x00;
        }
    }
    
    for (i = 128; i < 256; i++) {
        for (j = 0; j < 8; j++) {
            font_memory[i * 32 + j] = font_8x8[(i & 0x7F) * 8 + j];
        }
        for (j = 8; j < 32; j++) {
            font_memory[i * 32 + j] = 0x00;
        }
    }
    
    outb(VGA_SEQ_INDEX, 0x02);
    outb(VGA_SEQ_DATA, seq2);
    
    outb(VGA_SEQ_INDEX, 0x04);
    outb(VGA_SEQ_DATA, seq4);
    
    outb(VGA_GC_INDEX, 0x04);
    outb(VGA_GC_DATA, gc4);
    
    outb(VGA_GC_INDEX, 0x05);
    outb(VGA_GC_DATA, gc5);
    
    outb(VGA_GC_INDEX, 0x06);
    outb(VGA_GC_DATA, gc6);
}
