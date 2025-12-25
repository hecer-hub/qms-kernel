/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "../func/writet.h"
#include "../func/input.h"
#include "../func/string.h"
#include "../func/fs.h"
#include "../memory.h"

#define KEY_ESC 27

extern "C" void run_editor() {
    clear_screen();
    print_string_ln("--- Text Editor v1.0 ---");
    print_string_ln("Type your text. Press ESC to save and exit.");
    print_string_ln("-------------------------------------------");

    char* text_buffer = new char[4096];
    int cursor = 0;
    char filename[64];
    
    print_string("Save as: ");
    int fn_ptr = 0;
    while(1) {
        char c = get_char();
        if (c == '\n') { filename[fn_ptr] = '\0'; break; }
        if (c == '\b' && fn_ptr > 0) { 
            fn_ptr--; 
            move_cursor_back(); print_char(' '); move_cursor_back(); 
        } else if (c >= ' ' && fn_ptr < 63) {
            filename[fn_ptr++] = c;
            print_char(c);
        }
    }
    print_char('\n');

    while (1) {
        char key = get_char();

        if (key == KEY_ESC) {
            break;
        }

        if (key == '\b') {
            if (cursor > 0) {
                cursor--;
                move_cursor_back();
                print_char(' ');
                move_cursor_back();
            }
        } else if (key == '\n') {
            if (cursor < 4095) {
                text_buffer[cursor++] = '\n';
                print_char('\n');
            }
        } else if (key != 0) {
            if (cursor < 4095) {
                text_buffer[cursor++] = key;
                print_char(key);
            }
        }
    }

    if (cursor > 0) {
        ram_fs_manager->create_file(filename, 0);
        ram_fs_manager->write_to_file(filename, text_buffer, cursor);
        print_string("\nFile saved: ");
        print_string_ln(filename);
    }

    delete[] text_buffer;
    print_string_ln("Exiting editor...");
}
