#ifndef SCHEDULING_H
#define SCHEDULING_H
#include <stdint.h>
typedef struct
{
  uint64_t virtual_adress;
  char* task_name;
} process_t;

#endif
