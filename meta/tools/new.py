#!/usr/bin/python

import datetime
import os
import sys

date = datetime.datetime.now().date()

file = sys.argv[1]
module = sys.argv[2]

template_h = f"""/*
 * Copyright (c) {date.year}, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef {module.upper()}_{file.upper()}_H
#define {module.upper()}_{file.upper()}_H
#include <emerald/std.h>


#endif
"""

template_f = f"""/*
 * Copyright (c) {date.year}, Abb1x
 *
 * SPDX-License-Identifier: MIT
 */

#include "{file}.h"

"""

with open(f'{file}.h', 'w') as f:
    f.write(template_h)
    
with open(f'{file}.c', 'w') as f:
    f.write(template_f)

os.system("git add {file}.*")
