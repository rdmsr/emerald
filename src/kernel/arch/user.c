/*
 * Copyright (c) 2021, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */
#include "user.h"
#include "emerald/str/str.h"
#include <emerald/str.h>
#include <syscall.h>

void user_jump()
{

    char *msg[4] = {
        "This is Major Tom to Ground Control",
        "I'm stepping through the door",
        "And I'm floating in a most peculiar way",
        "And the stars look very different today"};
    
    for (int i = 0; i < 4; i++)
    {
        be_log(msg[i]);
    }
}
