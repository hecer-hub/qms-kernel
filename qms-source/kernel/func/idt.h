/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// kernel/func/idt.h
#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

void init_idt();

extern "C" void default_exception_handler();

#endif
