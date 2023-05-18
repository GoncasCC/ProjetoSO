#include "synchronization.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


sem_t * semaphore_create(char* name, int value){
    
    char uidName[strlen(name)+15];
    int x = getuid();
    sprintf(uidName,"%s%d", name, x);

    return sem_open(uidName, O_CREAT, 0xFFFFFFFF, value);
}


void semaphore_destroy(char* name, sem_t* semaphore){
    char uidName[strlen(name)+15];
    sprintf(uidName,"%s%d", name, getuid());

    if (sem_close(semaphore) == -1){
        perror("sem_Close");
    }
    if (sem_unlink(uidName) == -1){
        perror("sem_Unlink");
    }
}


void produce_begin(struct prodcons* pc){
    sem_wait(pc->empty);
    sem_wait(pc->mutex);
}


void produce_end(struct prodcons* pc){
    sem_post(pc->mutex); 
    sem_post(pc->full);
}


void consume_begin(struct prodcons* pc){
    sem_wait(pc->full);
    sem_wait(pc->mutex);
}


void consume_end(struct prodcons* pc){
    sem_post(pc->mutex);
    sem_post(pc->empty);
}


void semaphore_mutex_lock(sem_t* sem){
    sem_wait(sem);
}


void semaphore_mutex_unlock(sem_t* sem){
    sem_post(sem);
}