/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "writet.h"
#include "string.h"
#include "vga_module.h"

static uint16_t SCREEN_WIDTH;
static uint16_t SCREEN_HEIGHT;

static char* video_memory = (char*)0xB8000;

static int cursor_pos_x = 0;
static int cursor_pos_y = 0;

void init_writet() {
    vga_get_dimensions(&SCREEN_WIDTH, &SCREEN_HEIGHT);
}

void scroll_screen() {
    for (int y = 1; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int from_offset = (y * SCREEN_WIDTH + x) * 2;
            int to_offset = ((y - 1) * SCREEN_WIDTH + x) * 2;
            video_memory[to_offset] = video_memory[from_offset];
            video_memory[to_offset + 1] = video_memory[from_offset + 1];
        }
    }
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        int offset = ((SCREEN_HEIGHT - 1) * SCREEN_WIDTH + x) * 2;
        video_memory[offset] = ' ';
        video_memory[offset + 1] = 0x07;
    }
    cursor_pos_y = SCREEN_HEIGHT - 1;
}

void print_char(char character) {
    if (character == '\n') {
        cursor_pos_x = 0;
        cursor_pos_y++;
    } else {
        int offset = (cursor_pos_y * SCREEN_WIDTH + cursor_pos_x) * 2;
        video_memory[offset] = character;
        video_memory[offset + 1] = 0x07;
        cursor_pos_x++;
        if (cursor_pos_x >= SCREEN_WIDTH) {
            cursor_pos_x = 0;
            cursor_pos_y++;
        }
    }

    if (cursor_pos_y >= SCREEN_HEIGHT) {
        scroll_screen();
    }
}

void print_string(const char* s) {
    while (*s) {
        print_char(*s++);
    }
}

void print_string_ln(const char* s) {
    print_string(s);
    print_char('\n');
}

void move_cursor_back() {
    if (cursor_pos_x > 0) {
        cursor_pos_x--;
    }
}

void clear_screen() {
    char* video_memory = (char*)0xB8000;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = 0x07;
    }
    cursor_pos_x = 0;
    cursor_pos_y = 0;
}

void print_int(int n) {
    if (n == 0) {
        print_char('0');
        return;
    }

    bool is_negative = false;
    if (n < 0) {
        is_negative = true;
        n = -n; 
        print_char('-');
    }

    char buffer[12];
    int i = 0;

    while (n != 0) {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        print_char(buffer[j]);
    }
}

void print_double(double d) {
    print_string("<double>");
}

void show_banner() {
	print_string_ln(" _____ _____ _____ ");
	print_string_ln("|     |     |   __|");
	print_string_ln("|  |  | | | |__   |");
	print_string_ln("|__  _|_|_|_|_____|");
	print_string_ln("   |__|            ");
}
