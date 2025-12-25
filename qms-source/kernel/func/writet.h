/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef _FUNC_WRITET_H
#define _FUNC_WRITET_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_writet();
void print_char(char character);
void print_string_ln(const char* s);
void print_string(const char* s);
void move_cursor_back();

void print_int(int n);
void print_double(double d);
void clear_screen();
void show_banner();

#ifdef __cplusplus
}
#endif

#endif
