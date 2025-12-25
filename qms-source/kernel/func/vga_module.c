/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "vga_module.h"
#include "string.h"
#include "font_8x8.h"

#define VGA_SEQ_INDEX    0x3C4
#define VGA_SEQ_DATA     0x3C5
#define VGA_GC_INDEX     0x3CE
#define VGA_GC_DATA      0x3CF
#define VGA_CRTC_INDEX   0x3D4
#define VGA_CRTC_DATA    0x3D5

static uint16_t current_width = 80;
static uint16_t current_height = 25;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void vga_write_reg(uint16_t index_port, uint8_t reg, uint8_t val) {
    outb(index_port, reg);
    outb(index_port + 1, val);
}

void vga_disable_hardware_cursor() {
    vga_write_reg(VGA_CRTC_INDEX, 0x0A, 0x20);
}

void vga_load_font_8x8_full() {
    vga_write_reg(VGA_SEQ_INDEX, 0x00, 0x01);
    vga_write_reg(VGA_SEQ_INDEX, 0x02, 0x04);
    vga_write_reg(VGA_SEQ_INDEX, 0x04, 0x07);
    vga_write_reg(VGA_SEQ_INDEX, 0x00, 0x03);

    vga_write_reg(VGA_GC_INDEX, 0x04, 0x02);
    vga_write_reg(VGA_GC_INDEX, 0x05, 0x00);
    vga_write_reg(VGA_GC_INDEX, 0x06, 0x00);

    uint8_t* font_ptr = (uint8_t*)0xA0000;
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 8; j++) {
            font_ptr[i * 32 + j] = font_8x8[i * 8 + j];
        }

        for (int j = 8; j < 32; j++) font_ptr[i * 32 + j] = 0;
    }

    vga_write_reg(VGA_SEQ_INDEX, 0x02, 0x03); 
    vga_write_reg(VGA_SEQ_INDEX, 0x04, 0x03); 
    vga_write_reg(VGA_GC_INDEX, 0x04, 0x00);
    vga_write_reg(VGA_GC_INDEX, 0x05, 0x10);
    vga_write_reg(VGA_GC_INDEX, 0x06, 0x0E);
}


int vgacfg(const char* mode_str) {
    if (strcmp(mode_str, "80x25") == 0) {
        current_width = 80;
        current_height = 25;
        vga_disable_hardware_cursor();
        return 0;
    }

    if (strcmp(mode_str, "80x50") == 0) {
        vga_load_font_8x8_full();
        
        vga_write_reg(VGA_CRTC_INDEX, 0x09, 0x07); 
        
        vga_disable_hardware_cursor();
        current_width = 80;
        current_height = 50;
        vga_clear_screen();
        return 0;
    }

    return -1;
}

void vga_get_dimensions(uint16_t* width, uint16_t* height) {
    *width = current_width;
    *height = current_height;
}

void vga_clear_screen() {
    uint16_t* video_mem = (uint16_t*)0xB8000;
    uint16_t blank = (0x07 << 8) | ' ';
    for (int i = 0; i < current_width * current_height; i++) {
        video_mem[i] = blank;
    }
}

char* vga_get_current_resolution() {
    static char buf[16];
    if (current_height == 50) strcpy(buf, "80x50");
    else strcpy(buf, "80x25");
    return buf;
}
