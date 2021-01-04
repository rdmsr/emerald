#include "task.h"
#include <debug-utilities/logger.h>
process_t EmeraldProc_Task_create_process(int id,uint8_t priority, uintptr_t virtual_adress,thread_t thread,char *name)
{
  process_t process;
  pagemap_t* m_pagemap=m_pagemap;
  
  EmeraldMem_VMM_create_pagemap(m_pagemap);
  
  process.thread.priority = priority;
  process.name = name;
  process.id = id;
  process.thread = thread;
  process.pagemap = m_pagemap;
  
  EmeraldMem_VMM_map_page(m_pagemap, lower_half(virtual_adress), virtual_adress, 0b11);
  
  log(INFO,"Created process called: %s with id: %d, with virtual adress: %d and physical adress: %d",name,id,virtual_adress,lower_half(virtual_adress));
  return process;

}
