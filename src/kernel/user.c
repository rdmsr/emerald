/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include "user.h"
#include "emerald/str/str.h"
#include <emerald/str.h>

void user_jump()
{
    asm volatile("mov $1, %rax\n"
                 "int $0x80\n");

    const char *msg[4] = {
        "This is Major Tom to Ground Control",
        "I'm stepping through the door",
        "And I'm floating in a most peculiar way",
        "And the stars look very different today"};

    asm volatile("xor %rax, %rax");

    for (int i = 0; i < 4; i++)
    {
        asm volatile(
            "mov %0, %%rbx\n"
            "int $0x80\n"
            :
            : "a"(msg[i]));
    }
}
