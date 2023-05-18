/**
 * Gonçalo Correia -> 56316
 * Daniel Dias -> 56350
 * Rafael Francisco -> 58204
 * 
 * */
#include "client.h"

int execute_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    struct operation op;
    int counter = 0;
    while (1) {
        client_get_operation(&op, client_id, buffers, data, sems);
        if (*(data->terminate) == 1) {
                return counter;
            }
        if (op.id != -1) {
            client_process_operation(&op, client_id, data, &counter, sems);
            client_send_operation(&op, buffers, data, sems);
        }
    }
}

void client_get_operation(struct operation* op, int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if (*(data->terminate) != 1) {
        read_main_client_buffer(buffers->main_client, client_id, data->buffers_size, op);
    }
}

void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter, struct semaphores* sems){
    op->receiving_client = client_id;
    op->status = 'C';
    *counter += 1;
    data->client_stats[client_id] += 1;
    (data->results)[op->id] = *op;
}

void client_send_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    write_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
}