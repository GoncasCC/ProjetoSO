#include "main.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
//init data structures
struct main_data* data = create_dynamic_memory(sizeof(struct main_data));
struct comm_buffers* buffers = create_dynamic_memory(sizeof(struct comm_buffers));
buffers->main_client = create_dynamic_memory(sizeof(struct rnd_access_buffer));
buffers->client_interm = create_dynamic_memory(sizeof(struct circular_buffer));
buffers->interm_enterp = create_dynamic_memory(sizeof(struct rnd_access_buffer));
//execute main code
main_args(argc, argv, data);
create_dynamic_memory_buffers(data);
create_shared_memory_buffers(data, buffers);
launch_processes(buffers, data);
user_interaction(buffers, data);
//release memory before terminating
destroy_dynamic_memory(data);
destroy_dynamic_memory(buffers->main_client);
destroy_dynamic_memory(buffers->client_interm);
destroy_dynamic_memory(buffers->interm_enterp);
destroy_dynamic_memory(buffers);
}


void main_args(int argc, char* argv[], struct main_data* data){
    if (argc == 6)
    {
        char *prt;
        data->max_ops = strtol(argv[1], &prt, 10);
        data->buffers_size = strtol(argv[2], &prt, 10);
        data->n_clients = strtol(argv[3], &prt, 10);
        data->n_intermediaries = strtol(argv[4], &prt, 10);
        data->n_enterprises = strtol(argv[5], &prt, 10);
    }
}


void create_dynamic_memory_buffers(struct main_data* data){
    data->client_pids = create_dynamic_memory(data->n_clients);
    data->intermediary_pids = create_dynamic_memory(data->n_intermediaries);
    data->enterprise_pids = create_dynamic_memory(data->n_enterprises);
    data->client_stats = create_dynamic_memory(data->n_clients);
    data->intermediary_stats = create_dynamic_memory(data->n_intermediaries);
    data->enterprise_stats = create_dynamic_memory(data->n_enterprises);
}


void create_shared_memory_buffers(struct main_data* data, struct comm_buffers* buffers){
    buffers->client_interm = create_shared_memory("client-interm", data->buffers_size);
    buffers->interm_enterp = create_shared_memory("interm-enterp", data->buffers_size);
    buffers->main_client = create_shared_memory("main-client", data->buffers_size);
    data->results = create_shared_memory("result", MAX_RESULTS);
    data->terminate = create_shared_memory("terminate", 1);
}


void launch_processes(struct comm_buffers* buffers, struct main_data* data){
    for (int i = 0; i < data->n_clients; i++)
    {
        data->client_pids[i] = launch_client(i, buffers, data);
    }

    for (int i = 0; i < data->n_intermediaries; i++)
    {
        data->intermediary_pids[i] = launch_interm(i, buffers, data);
    }

    for (int i = 0; i < data->n_enterprises; i++)
    {
        data->enterprise_pids[i] = launch_enterp(i, buffers, data);
    }
}


void user_interaction(struct comm_buffers* buffers, struct main_data* data){

}


void create_request(int* op_counter, struct comm_buffers* buffers, struct main_data* data){
    
}


void read_status(struct main_data* data){

}


void stop_execution(struct main_data* data, struct comm_buffers* buffers){
    data -> terminate = 1;
    wait_process(data);
    write_statistics(data);
    destroy_memory_buffers(data);
}


void wait_processes(struct main_data* data){

    //colocar todos os processos client em espera
    for (int i = 0; i < data->n_clients; i++)
    {
        wait_process(data->client_pids[i]);
    }

    //colocar todos os processos intermediarie em espera
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        wait_process(data->intermediary_pids[i]);
    }

    //colocar todos os processos enterprise em espera
    for (int i = 0; i < data->n_enterprises; i++)
    {
        wait_process(data->enterprise_pids[i]);
    }
    
}


void write_statistics(struct main_data* data){
    for (int i = 0; i < data->n_clients; i++)
    {
        printf("Client" + i + data->client_stats[i])
    }

    //colocar todos os processos intermediarie em espera
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        printf("Intermediate" + i + data->intermediaries_stats[i])
    }

    //colocar todos os processos enterprise em espera
    for (int i = 0; i < data->n_enterprises; i++)
    {
        printf("Enterprise" + i + data->Enterprise_stats[i])
    }
    
}


void destroy_memory_buffers(struct main_data* data, struct comm_buffers* buffers){

    destroy_dynamic_memory(data->client_pids);
    destroy_dynamic_memory(data->intermediary_pids);
    destroy_dynamic_memory(data->enterprise_pids);
    destroy_dynamic_memory(data->client_stats);
    destroy_dynamic_memory(data->enterprise_stats);
    destroy_dynamic_memory(data->intermediary_stats);

    destroy_shared_memory("client-interm", buffers ->client_interm, data->buffers_size);
    destroy_shared_memory("interm-enterp", buffers ->interm_enterp, data->buffers_size);
    destroy_shared_memory("main-client", buffers ->main_client, data->buffers_size);
    destroy_shared_memory("result", data->results, MAX_RESULTS);
    destroy_shared_memory("terminate", data->terminate, 1);
}
