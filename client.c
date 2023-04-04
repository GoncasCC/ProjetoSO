int execute_client(int client_id, struct comm_buffers* buffers, struct main_data* data){
    struct operation op;
    //não sei se é suposto ser um apontador ou não
    int counter = 0;
    while (1) {
        client_get_operation(&op, client_id, buffers, data);
        if (*(&data->terminate) == 1) {
                return counter;
            }
        if (op.id != -1) {
            client_process_operation(&op, client_id, data, counter);
            client_send_operation(&op, buffers, data);
        }
    }
}

void client_get_operation(struct operation* op, int client_id, struct comm_buffers* buffers, struct main_data* data){
    if (*(&data->terminate) != 1) {
        read_main_client_buffer(&buffers->main_client, client_id, &data->buffers_size, op);
    }
}

void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter){
    &op->receiving_client = client_id;
    &op->status = 'C';
    *counter += 1;
    (&data->results)[&op->id] = op;
}

void client_send_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data){
    write_client_interm_buffer(&buffers->client_interm, &data->buffers_size, op);
}