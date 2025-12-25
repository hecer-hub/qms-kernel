/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "calc.h"
#include "../func/writet.h"
#include "../func/input.h"
#include "../func/string.h"
#include "../func/math.h"
#include "../memory.h"

void get_num(const char* buf, int& p, char* out) {
    int n = 0;
    while (buf[p] == ' ') p++;
    if (buf[p] == '-') out[n++] = buf[p++];
    while (buf[p] >= '0' && buf[p] <= '9') out[n++] = buf[p++];
    out[n] = '\0';
}

extern "C" void run_calc() {
    print_string_ln("--- calculator ---");
    while (1) {
        print_string("\ncalc> ");
        char buf[256]; int i = 0;
        while (1) {
            char c = get_char();
            if (c == '\n') break;
            if (c == '\b' && i > 0) { i--; move_cursor_back(); print_char(' '); move_cursor_back(); continue; }
            if (c >= ' ' && c <= '~' && i < 255) { buf[i++] = c; print_char(c); }
        }
        buf[i] = '\0'; print_char('\n');
        if (strcmp(buf, "q") == 0) break;
        if (i == 0) continue;

        int ptr = 0;
        char tmp[128];
        get_num(buf, ptr, tmp);
        
        char* total_sum = new char[2]{'0','\0'};
        char* current_term = new char[strlen(tmp) + 1];
        strcpy(current_term, tmp);
        char last_op = '+';

        while (buf[ptr] != '\0') {
            while (buf[ptr] == ' ') ptr++;
            if (buf[ptr] == '\0') break;
            char op = buf[ptr++];
            get_num(buf, ptr, tmp);

            if (op == '*' || op == '/') {
                char* next = (op == '*') ? mul(current_term, tmp) : div(current_term, tmp);
                delete[] current_term;
                current_term = next;
            } else {
                char* next_sum = (last_op == '+') ? add(total_sum, current_term) : sub(total_sum, current_term);
                delete[] total_sum;
                total_sum = next_sum;
                last_op = op;
                delete[] current_term;
                current_term = new char[strlen(tmp) + 1];
                strcpy(current_term, tmp);
            }
        }
        char* res = (last_op == '+') ? add(total_sum, current_term) : sub(total_sum, current_term);
        print_string("Result: "); print_string_ln(res);
        delete[] total_sum; delete[] current_term; delete[] res;
    }
}
