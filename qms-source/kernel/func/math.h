/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef _MATH_H
#define _MATH_H

#ifdef __cplusplus
extern "C" {
#endif

char* add(const char* a, const char* b);
char* sub(const char* a, const char* b);
char* mul(const char* a, const char* b);
char* div(const char* a, const char* b);


int compare(const char* a, const char* b);

#ifdef __cplusplus
}
#endif

#endif
