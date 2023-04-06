int launch_client(int client_id, struct comm_buffers* buffers, struct main_data* data){
    int pid = fork();
    if (pid == -1) {    //em caso de erro
        perror();
        exit(1);
    }
    if (pid == 0)   {
        execute_client(client_id, buffers, data);
    }
    else    {
        return pid;
    }
    
}


int launch_interm(int interm_id, struct comm_buffers* buffers, struct main_data* data){
    int pid = fork();
    if (pid == -1) {    //em caso de erro
        perror();
        exit(1);
    }
    if (pid == 0)   {
        execute_intermediary(interm_id, buffers, data);
    }
    else    {
        return pid;
    }
}



int launch_enterp(int enterp_id, struct comm_buffers* buffers, struct main_data* data){
    int pid = fork();
    if (pid == -1) {    //em caso de erro
        perror();
        exit(1);
    }
    if (pid == 0)   {
        execute_enterprise(enterp_id, buffers, data);
    }
    else    {
        return pid;
    }
}



int wait_process(int process_id){
    return waitpid(process_id);
}
