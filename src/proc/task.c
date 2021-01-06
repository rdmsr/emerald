#include "task.h"
#include "PIT.h"
#include <debug-utilities/logger.h>
#include <libk.h>
struct process_struct *process_queue;
int size_of_queue = 0;
void queue_append(process_t process)
{
    process_queue[size_of_queue - 1] = process;
}
process_t EmeraldProc_Task_create_process(int id, uint8_t priority, uintptr_t virtual_adress, thread_t thread, char *name)
{
    process_t process;
    pagemap_t *m_pagemap = m_pagemap;

    EmeraldMem_VMM_create_pagemap(m_pagemap);

    process.name = name;
    process.id = id;
    process.thread = thread;
    process.thread.priority = priority;

    process.pagemap = m_pagemap;
    EmeraldMem_VMM_map_page(m_pagemap, lower_half(virtual_adress), virtual_adress, 0b11);
    int position = position;
    for (int i = 0; i < size_of_queue - 1; i++)
    {
        if (process_queue[size_of_queue].name == name)
        {
            position = i;
        }
    }
    size_of_queue++;
    queue_append(process);
    log(INFO, "Created process called: %s with id: %d, with virtual adress: %d and physical adress: %d, position in queue: %d", name, id, virtual_adress, lower_half(virtual_adress), position);

    return process;
}
/* This is probably not the best implementation but it works for now, bubble sort has a complexity of O(n2) which is not the best*/
void EmeraldProc_Scheduler_schedule_task()
{
    EmeraldProc_PIT_init(1000);
    /* Puts the process with the highest priority at the start of the queue */
    for (int i = 0; i < size_of_queue - 1; i++)
    {
        if (process_queue[i].thread.priority < process_queue[i + 1].thread.priority)
        {
            process_t temp = process_queue[i];
            process_queue[i] = process_queue[i + 1];
            process_queue[i + 1] = temp;

            log(INFO, "Swapped process named %s with process named %s", process_queue[i].name, process_queue[i + 1].name);
            kassert(process_queue[i].thread.priority > process_queue[i + 1].thread.priority);
        }
    }
}

void EmeraldProc_Scheduler_give_cpu(thread_t thread)
{
    log(INFO, "TODO: Implement EmeraldProc_Scheduler_give_cpu, thread's priority is %d", thread.priority);
}
