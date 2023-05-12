#include "synchronization.h"

sem_t * semaphore_create(char* name, int value);


void semaphore_destroy(char* name, sem_t* semaphore);


void produce_begin(struct prodcons* pc);


void produce_end(struct prodcons* pc);


void consume_begin(struct prodcons* pc);


void consume_end(struct prodcons* pc);


void semaphore_mutex_lock(sem_t* sem);


void semaphore_mutex_unlock(sem_t* sem);