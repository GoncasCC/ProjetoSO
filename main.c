#include "include/main.h"
#include "include/process.h"


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
    if (argc == 5)
    {
        data->max_ops = argv[0];
        data->buffers_size = argv[1];
        data->n_clients = argv[2];
        data->n_intermediaries = argv[3];
        data->n_enterprises = argv[4];
    }
}


void create_dynamic_memory_buffers(struct main_data* data){
    create_dynamic_memory(0); //por continuar
}


void create_shared_memory_buffers(struct main_data* data, struct comm_buffers* buffers){
    create_shared_memory(buffers->client_interm, 0);
    create_shared_memory(buffers->interm_enterp, 0);
    create_shared_memory(buffers->main_client, 0);
    create_shared_memory(data->results, MAX_RESULTS);
    create_shared_memory(data->terminate, 1);
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

}


void destroy_memory_buffers(struct main_data* data, struct comm_buffers* buffers){
    
}
