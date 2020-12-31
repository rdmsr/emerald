#ifndef TASK_H
#define TASK_H
#pragma once
#include <stdint.h>
typedef struct
{
    uint64_t virtual_adress;
    int id;
    char name[];
} process_t;


#endif
