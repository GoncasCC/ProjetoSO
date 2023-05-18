#include "aptime.h"


void get_realTime(struct timespec* t){
    clock_gettime(CLOCK_REALTIME, t);
}