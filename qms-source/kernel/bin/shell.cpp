/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "shell.h"
#include "calc.h"
#include "editor.h"
#include "../func/writet.h"
#include "../func/input.h"
#include "../func/string.h"
#include "../func/fs.h"
#include "../memory.h" 

#define COMMAND_BUFFER_SIZE 256

void run_shell() {
    char command_buffer[COMMAND_BUFFER_SIZE];
    int cursor = 0;
    
    print_string_ln("QMSH v1.7");
    
    while(1) {
        print_string("\nQMSH> ");
        cursor = 0;
        memset(command_buffer, 0, COMMAND_BUFFER_SIZE);

        while(1) {
            char key = get_char();
            if (key == '\n') {
                command_buffer[cursor] = '\0';
                break;
            } else if (key == '\b') {
                if (cursor > 0) {
                    cursor--;
                    move_cursor_back(); print_char(' '); move_cursor_back();
                }
            } else if (key != 0 && cursor < COMMAND_BUFFER_SIZE - 1) {
                command_buffer[cursor++] = key; print_char(key);
            }
        }
        print_char('\n');

        if (strlen(command_buffer) == 0) continue;

        if (strcmp(command_buffer, "help") == 0) {
            print_string_ln("Available commands:");
            print_string_ln("ls          - list files");
            print_string_ln("touch <n>   - create empty file");
            print_string_ln("wrd <n> <d> - write text to file");
            print_string_ln("cat <n>     - read file content");
            print_string_ln("rm <n>      - delete file");
            print_string_ln("clear       - clear screen");
            print_string_ln("mem         - show memory info");

        } else if (strcmp(command_buffer, "ls") == 0) {
            ram_fs_manager->list_files();

        } else if (strncmp(command_buffer, "touch ", 6) == 0) {
            char* filename = command_buffer + 6;
            ram_fs_manager->create_file(filename, 0);

        } else if (strncmp(command_buffer, "wrd ", 4) == 0) {
            char* first_space = strchr(command_buffer + 4, ' ');
            if (first_space) {
                *first_space = '\0';
                char* filename = command_buffer + 4;
                char* data = first_space + 1;
                ram_fs_manager->write_to_file(filename, data, strlen(data));
            } else {
                print_string_ln("Usage: wrd <filename> <data>");
            }

        } else if (strncmp(command_buffer, "cat ", 4) == 0) {
            char* filename = command_buffer + 4;
            size_t sz = ram_fs_manager->get_file_size(filename);
            if (sz > 0) {
                char* data = ram_fs_manager->read_from_file(filename);
                if (data) {
                    print_string_ln(data);
                    kernel_mem_manager->free(data, sz + 1);
                }
            } else {
                print_string_ln("File is empty or not found.");
            }

        } else if (strncmp(command_buffer, "rm ", 3) == 0) {
            char* filename = command_buffer + 3;
            ram_fs_manager->delete_file(filename);

        } else if (strcmp(command_buffer, "clear") == 0) {
            clear_screen();

        } else if (strcmp(command_buffer, "mem") == 0) {
            print_string("Total: "); print_int(kernel_mem_manager->get_total_size() / 1024); print_string_ln(" KB");
            print_string("Used:  "); print_int(kernel_mem_manager->get_used_size() / 1024); print_string_ln(" KB");
            print_string("Free:  "); print_int(kernel_mem_manager->get_free_size() / 1024); print_string_ln(" KB");

        } else if (strcmp(command_buffer, "calc") == 0) {
            run_calc();
        } else if (strcmp(command_buffer, "edit") == 0) {
            run_editor();
        } else {
            print_string("Command not found: ");
            print_string_ln(command_buffer);
        }
    }
}
