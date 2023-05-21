#include "apsignal.h"
#include <stdio.h>

struct main_data* data;
struct comm_buffers* buffers;
struct semaphores* sems;
unsigned int al;
int* c;

void quitExec(){
    stop_execution(data, buffers, sems);
}


void first_alarm(struct comm_buffers* b, struct main_data* d, struct semaphores* s, int a, int* counter){
    data = d;
    buffers = b;
    sems = s;
    al = a;
    c = counter;
    exec_alarm();
}

void exec_alarm(){
    signal(SIGALRM, write_alarm);
    printf("alarm");
    alarm(al);
}

void write_alarm(){
    struct operation* op;
    for (int i = 0; i < *c; i++)
    {
        op = &(data->results[i]);
        printf("op: %d status: %c start_time: %jd ", op->id, op->status, op->start_time.tv_sec);
        switch (op->status)
        {
        case 'M':
            printf("\n");
            break;
        
        case 'C':
            printf("receiving_client: %d client_time: %jd ", op->receiving_client, op->client_time.tv_sec);
            break;

        case 'I':
            printf("receiving_client: %d client_time: %jd receiving_intermediary: %d intermediary_time: %jd", op->receiving_client, op->client_time.tv_sec , op->receiving_interm, op->intermed_time.tv_sec);
            break;

        case 'A':
            printf("receiving_client: %d client_time: %jd receiving_intermediary: %d intermediary_time: %jd receiving_enterprise: %d enterprise_time: %jd", 
                op->receiving_client, op->client_time.tv_sec, op->receiving_interm, op->intermed_time.tv_sec, op->receiving_enterp, op->enterp_time.tv_sec);
            break;

        case 'E':
            printf("receiving_client: %d client_time: %jd receiving_intermediary: %d intermediary_time: %jd receiving_enterprise: %d enterprise_time: %jd", 
                op->receiving_client, op->client_time.tv_sec, op->receiving_interm, op->intermed_time.tv_sec, op->receiving_enterp, op->enterp_time.tv_sec);
            break;

        default:
            break;
        }
    }
    exec_alarm();
}