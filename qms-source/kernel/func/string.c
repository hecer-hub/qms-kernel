/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "string.h"

char* strcpy(char* destination, const char* source) {
    char* original_dest = destination;
    while ((*destination++ = *source++));
    return original_dest;
}

char* strncpy(char* destination, const char* source, size_t num) {
    size_t i;
    for (i = 0; i < num && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    for (; i < num; i++) {
        destination[i] = '\0';
    }
    return destination;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int strncmp(const char* str1, const char* str2, size_t num) {
    if (num == 0) {
        return 0;
    }
    while (num-- != 0) {
        if (*str1 != *str2) {
            return (unsigned char)*str1 - (unsigned char)*str2;
        }
        if (*str1 == '\0') {
            break;
        }
        str1++;
        str2++;
    }
    return 0;
}

size_t strlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < num; ++i) {
        p[i] = (unsigned char)value;
    }
    return ptr;
}

void* memcpy(void* destination, const void* source, size_t num) {
    char* dest = (char*)destination;
    const char* src = (const char*)source;
    for (size_t i = 0; i < num; ++i) {
        dest[i] = src[i];
    }
    return destination;
}

char* strchr(const char* str, int character) {
    char c = (char)character;
    while (*str) {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    if (c == '\0') {
        return (char*)str;
    }
    return (char*)0;
}
