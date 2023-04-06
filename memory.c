#include "memory.h"
#include <string.h>
#include <stdilb.h>


void* create_shared_memory(char* name, int size) {
    int fd;
    int ret;
    void* addr;
    
    // Criar zona de memória partilhada
    fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    
    // Definir tamanho da zona de memória partilhada
    ret = ftruncate(fd, size);
    if (ret == -1) {
        perror("ftruncate");
        exit(2);
    }
    
    // Mapear zona de memória partilhada na memória do processo
    addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(3);
    }
    
    // Preencher zona de memória partilhada com o valor 0
    // (Acho que isto não era preciso pois memória já está com valor 0)
    memset(addr, 0, size);
    
    return addr;
}

void* create_dynamic_memory(char* name, int size) {
    //Malloc reserva zona de memória dinâmica
    //Memset preenche a zona de memória com 0
    void* ptr = malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void destroy_shared_memory(char* name, void* ptr, int size) {
    int ret;
    ret = munmap(ptr,size);
    if (ret == -1) {
        perror("munmap");
        exit(1);
    }
    
    ret = shm_unlink(name);
    if (ret == -1) {
        perror("shm_unlink");
        exit(2);
    }
}


void destroy_dynamic_memory(void* ptr){
    free(ptr);
}



void write_main_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op) {
    int pos = -1;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == 0) {
            pos = i;
            break;
        }
    }
    //Verifica que há posições vagas
    if (pos != -1) {
        buffer->buffer[pos] = *op;
        buffer->ptrs[pos] = 1;
    }
}


void write_main_client_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op) {
	// Obtem o valor atual de in e out do buffer
	int in = buffer->ptrs->in;
	int out = buffer->ptrs->out;

	// Calcula a próxima posição para escrever a operação
	int next_in = (in + 1) % buffer_size;

	// Se a próxima posição não for a mesma que a posição out, o buffer não está cheio
	if (next_in != out) {
		// Escreve a operação no buffer na posição in
		buffer->buffer[in] = *op;

		// Atualiza o valor de in
		buffer->ptrs->in = next_in;
	}
}


void write_interm_enterp_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    int pos = -1;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == 0) {
            pos = i;
            break;
        }
    }
    //Verifica que há posições vagas
    if (pos != -1) {
        buffer->buffer[pos] = *op;
        buffer->ptrs[pos] = 1;
    }
}

void read_main_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op) {
    int pos = -1;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == client_id) {
            pos = i;
            break;
        }
    }
    if (pos != -1) {
        *op = buffer->buffer[pos];
        buffer->ptrs[pos] = 0;
    }
    else {
        op->id = -1;
    }
}


void read_client_interm_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op) {
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;

    // Verifica se há operações no buffer
    if (in != out) {
        *op = buffer->buffer[out]; // Lê operação do buffer

        // Atualiza o valor de out_pos no buffer circular
        out = (out + 1) % buffer_size;
        buffer->ptrs->out = out;
    } else {
        op->id = -1; // Não há operações disponíveis
    }
}

void read_interm_enterp_buffer(struct rnd_access_buffer* buffer, int enterp_id, int buffer_size, struct operation* op){
    int pos = -1;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == client_id) {
            pos = i;
            break;
        }
    }
    if (pos != -1) {
        *op = buffer->buffer[pos];
        buffer->ptrs[pos] = 0;
    }
    else {
        op->id = -1;
    }
}








