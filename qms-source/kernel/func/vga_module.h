/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef VGA_MODULE_H
#define VGA_MODULE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int vgacfg(const char* mode_str);

void vga_get_dimensions(uint16_t* width, uint16_t* height);
char* vga_get_current_resolution();
void vga_clear_screen();

void vga_disable_hardware_cursor();
void vga_load_font_8x8_full();

#ifdef __cplusplus
}
#endif

#endif
