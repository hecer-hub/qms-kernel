/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* destination, const char* source);
int strncmp(const char* str1, const char* str2, size_t num);
char* strncpy(char* destination, const char* source, size_t num);
size_t strlen(const char* str);
char* strchr(const char* str, int character);

#ifdef __cplusplus
}
#endif

#endif
