/**
 * Gonçalo Correia -> 56316
 * Daniel Dias -> 56350
 * Rafael Francisco -> 58204
 * 
 * */
#include "enterprise.h"

int execute_enterprise(int enterp_id, struct comm_buffers* buffers, struct main_data* data){
    struct operation op;
    //não sei se é suposto ser um apontador ou não
    int counter = 0;
    while (1) {
        enterprise_receive_operation(&op, enterp_id, buffers, data);
        if (*(data->terminate) == 1) {
                return counter;
            }
        if (op.id != -1) {
            enterprise_process_operation(&op, enterp_id, data, &counter);
        }
    }
}



void enterprise_receive_operation(struct operation* op, int enterp_id, struct comm_buffers* buffers, struct main_data* data){
    if (*(data->terminate) != 1) {
        read_interm_enterp_buffer(buffers->main_client, enterp_id, data->buffers_size, op);
    }
}


void enterprise_process_operation(struct operation* op, int enterp_id, struct main_data* data, int* counter){
    op->receiving_enterp = enterp_id;
    if (counter >= &data->max_ops) {
        op->status = 'A';
    } else {
        op->status = 'E';
    }
    *counter += 1;
    (&data->results)[op->id] = op;
}