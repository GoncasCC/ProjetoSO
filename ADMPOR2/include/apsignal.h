#include <signal.h>
#include "main.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>

void quitExec();

void first_alarm(struct comm_buffers* b, struct main_data* d, struct semaphores* s, int a, int* counter);

void exec_alarm();

void write_alarm();