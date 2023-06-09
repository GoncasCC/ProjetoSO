/**
 * Gonçalo Correia -> 56316
 * Daniel Dias -> 56350
 * Rafael Francisco -> 58204
 * 
 * */
#include "intermediary.h"

int execute_intermediary(int interm_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    struct operation op;
    //não sei se é suposto ser um apontador ou não
    
    while (1) {
        intermediary_receive_operation(&op, buffers, data, sems);
        if (*(data->terminate) == 1) {
                return data->intermediary_stats[interm_id];
            }
        if (op.id != -1) {
            intermediary_process_operation(&op, interm_id, data, &(data->intermediary_stats[interm_id]), sems);
            intermediary_send_answer(&op, buffers, data, sems);
        }
    }
}

void intermediary_receive_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    if (*(data->terminate) != 1) {
        consume_begin(sems->client_interm);

        read_client_interm_buffer(buffers->client_interm, data->buffers_size, op);

        consume_end(sems->client_interm);
    }
}


void intermediary_process_operation(struct operation* op, int interm_id, struct main_data* data, int* counter, struct semaphores* sems){
    get_realTime(&(op->intermed_time));
    op->receiving_interm = interm_id;
    op->status = 'I';
    (*counter) = *counter + 1;
    (data->results)[op->id] = *op;
}


void intermediary_send_answer(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems){
    produce_begin(sems->interm_enterp);

    write_interm_enterp_buffer(buffers->interm_enterp, data->buffers_size, op);

    produce_end(sems->interm_enterp);
}