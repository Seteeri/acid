#ifndef WORKER_H
#define WORKER_H
#include "task.h"

void workers_setup();
void workers_teardown();
void worker_add_task(task_t *task);

#endif  // WORKER_H
