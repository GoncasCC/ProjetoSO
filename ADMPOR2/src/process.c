/**
 * Gonçalo Correia -> 56316
 * Daniel Dias -> 56350
 * Rafael Francisco -> 58204
 * 
 * */
#include "process.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "client.h"
#include "intermediary.h"
#include "enterprise.h"
#include <sys/wait.h>

int launch_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid = fork();
    if (pid == -1) {    //em caso de erro
        perror("process");
        exit(1);
    }
    if (pid == 0)   {
        execute_client(client_id, buffers, data, sems);
        exit(0);
    }
    else    {
        return pid;
    }
    
}


int launch_interm(int interm_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid = fork();
    if (pid == -1) {    //em caso de erro
        perror("process");
        exit(1);
    }
    if (pid == 0)   {
        execute_intermediary(interm_id, buffers, data, sems);
        exit(0);
    }
    else    {
        return pid;
    }
}



int launch_enterp(int enterp_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    int pid = fork();
    if (pid == -1) {    //em caso de erro
        perror("process");
        exit(1);
    }
    if (pid == 0)   {
        execute_enterprise(enterp_id, buffers, data, sems);
        exit(0);
    }
    else    {
        return pid;
    }
}


int wait_process(int process_id){
    int* status;
    return waitpid(process_id, status, WNOHANG);
}