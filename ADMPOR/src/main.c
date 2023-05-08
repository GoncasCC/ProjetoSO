/**
 * Gonçalo Correia -> 56316
 * Daniel Dias -> 56350
 * Rafael Francisco -> 58204
 * 
 * */

#include "main.h"
#include "process.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int i1;
int i2;

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
    buffers->client_interm->buffer = create_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER , data->buffers_size * sizeof(struct operation));
    buffers->interm_enterp->buffer = create_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, data->buffers_size * sizeof(struct operation));
    buffers->main_client->buffer = create_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, data->buffers_size * sizeof(struct operation));
    buffers->client_interm->ptrs = create_shared_memory(STR_SHM_CLIENT_INTERM_PTR , 2 *sizeof(int));
    buffers->interm_enterp->ptrs= create_shared_memory(STR_SHM_INTERM_ENTERP_PTR, data->buffers_size * sizeof(int));
    buffers->main_client->ptrs = create_shared_memory(STR_SHM_MAIN_CLIENT_PTR, data->buffers_size * sizeof(int));
    data->results = create_shared_memory(STR_SHM_RESULTS, MAX_RESULTS);
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
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
    char action[10];
    int op_counter = 0;
    printf("Ações disponíveis: \n ");
    printf("    op cliente empresa - criar uma nova operação \n");
    printf("    status id - consultar o estado de uma operação \n ");
    printf("    stop - termina a execução do AdmPor. \n ");
    printf("    help - imprime informação sobre as ações disponíveis. \n ");
    while (*data->terminate != 1)
    {
        printf("Introduzir ação: \n");
        scanf ("%s", action);
        
		if((strcmp(action , "op") == 0)){
            scanf("%d %d", &i1, &i2);
			create_request(&op_counter, buffers, data);
		}
        else if(strcmp(action , "status") == 0) {
            scanf("%d", &i1);
            i2 = op_counter;
			read_status(data);
		}
        else if(strcmp(action , "help") == 0) {
            printf("Ações disponíveis: \n ");
            printf("    op cliente empresa - criar uma nova operação \n");
            printf("    status id - consultar o estado de uma operação \n ");
            printf("    stop - termina a execução do AdmPor. \n ");
            printf("    help - imprime informação sobre as ações disponíveis. \n ");
		}
        else if(strcmp(action , "stop") == 0) {
			stop_execution(data, buffers);
		}else{
			printf("Ação não reconhecida, insira 'help' para assistência.");
		}
    }
    
}


void create_request(int* op_counter, struct comm_buffers* buffers, struct main_data* data){
    if(*op_counter < sizeof(data->results)){

		struct operation operation;
		operation.id = *op_counter;
		operation.requesting_client = i1;
        operation.requested_enterp = i2;

		if(*data->terminate == 1){return;}

		write_main_client_buffer(buffers->main_client, data->buffers_size, &operation);

		printf("O pedido #%d foi criado!\n", operation.id);
		*op_counter = *op_counter + 1;
	}else
	{
		printf("O número máximo de pedidos foi alcançado! \n");
	}
}


void read_status(struct main_data* data){
    if(i1 < 0 || i1 >= i2){
		printf("Erro: O id de operação fornecido nao é válido. \n");
	}else{
		struct operation op = data->results[i1];
        printf("Pedido %d com estado %c requisitado pelo cliente %d à empresa %d, ", op.id, op.status, op.requesting_client, op.requested_enterp);
        switch (op.status)
        {
        case 'M':
            printf("ainda não foi recebido. \n");
            break;
        
        case 'C':
            printf("foi recebida pelo cliente %d. \n", op.receiving_client);
            break;

        case 'I':
            printf("foi recebida pelo cliente %d e procesado pelo intermediario %d. \n", op.receiving_client, op.receiving_interm);
            break;

        case 'A':
            printf("foi recebida pelo cliente %d, procesado pelo intermediario %d e agendado pela empresa %d. \n", op.receiving_client, op.receiving_interm, op.receiving_enterp);
            break;

        case 'E':
        printf("foi recebida pelo cliente %d, procesado pelo intermediario %d e tratado pela empresa %d. \n", op.receiving_client, op.receiving_interm, op.receiving_enterp);
            break;

        default:
            break;
        }
		
	}
}


void stop_execution(struct main_data* data, struct comm_buffers* buffers){
    *data->terminate = 1;
    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data, buffers);
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
        printf("Cliente %d encaminhou %d pedidos." , i , data->client_stats[i]);
    }

    //colocar todos os processos intermediarie em espera
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        printf("Intermediario %d respondeu a %d pedidos." , i , data->intermediary_stats[i]);
    }

    //colocar todos os processos enterprise em espera
    for (int i = 0; i < data->n_enterprises; i++)
    {
        printf("Empresa %d recebeu %d pedidos." , i , data->enterprise_stats[i]);
    }

}


void destroy_memory_buffers(struct main_data* data, struct comm_buffers* buffers){
    destroy_dynamic_memory(data->client_pids);
    destroy_dynamic_memory(data->intermediary_pids);
    destroy_dynamic_memory(data->enterprise_pids);
    destroy_dynamic_memory(data->client_stats);
    destroy_dynamic_memory(data->enterprise_stats);
    destroy_dynamic_memory(data->intermediary_stats);

    destroy_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER , buffers->client_interm->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, buffers->interm_enterp->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, buffers->main_client->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_PTR , buffers->client_interm->ptrs, 2 *sizeof(int));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_PTR, buffers->interm_enterp->ptrs, data->buffers_size * sizeof(int));
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_PTR, buffers->main_client->ptrs, data->buffers_size * sizeof(int));
    destroy_shared_memory(STR_SHM_RESULTS, data->results, MAX_RESULTS);
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));

}
