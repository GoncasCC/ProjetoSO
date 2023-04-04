int execute_intermediary(int interm_id, struct comm_buffers* buffers, struct main_data* data){
    struct operation op;
    //não sei se é suposto ser um apontador ou não
    int counter = 0;
    while (1) {
        intermediary_receive_operation(&op, buffers, data);
        if (*(&data->terminate) == 1) {
                return counter;
            }
        if (op.id != -1) {
            intermediary_process_operation(&op, interm_id, data, counter);
            intermediary_send_answer(&op, buffers, data);
        }
    }
}

void intermediary_receive_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data){
    if (*(&data->terminate) != 1) {
        read_client_interm_buffer(&buffers->client_interm, &data->buffers_size, op);
    }
}


void intermediary_process_operation(struct operation* op, int interm_id, struct main_data* data, int* counter){
    &op->receiving_interm = interm_id;
    &op->status = 'I';
    *counter += 1;
    (&data->results)[&op->id] = op;
}


void intermediary_send_answer(struct operation* op, struct comm_buffers* buffers, struct main_data* data){
    write_interm_enterp_buffer(&buffers->client_interm, &data->buffers_size, op);
}