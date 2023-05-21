/**
 * Gonçalo Correia -> 56316
 * Daniel Dias -> 56350
 * Rafael Francisco -> 58204
 * 
 * */
#include "enterprise.h"

int execute_enterprise(int enterp_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    struct operation op;
    //não sei se é suposto ser um apontador ou não
    
    while (1) {
        enterprise_receive_operation(&op, enterp_id, buffers, data, sems);
        if (*(data->terminate) == 1) {
                return data->enterprise_stats[enterp_id];
            }
        if (op.id != -1) {
            enterprise_process_operation(&op, enterp_id, data, &(data->enterprise_stats[enterp_id]), sems);
        }
    }
}



void enterprise_receive_operation(struct operation* op, int enterp_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if (*(data->terminate) != 1) {
        consume_begin(sems->interm_enterp);

        read_interm_enterp_buffer(buffers->interm_enterp, enterp_id, data->buffers_size, op);

        consume_end(sems->interm_enterp);
    }
}


void enterprise_process_operation(struct operation* op, int enterp_id, struct main_data* data, int* counter, struct semaphores* sems){
    get_realTime(&(op->enterp_time));
    op->receiving_enterp = enterp_id;
    if (*counter >= data->max_ops) {
        op->status = 'A';
    } else {
        op->status = 'E';
    }
    (*counter) = *counter + 1;
    (data->results)[op->id] = *op;
}