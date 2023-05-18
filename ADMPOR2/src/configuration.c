#include <stdlib.h>
#include "configuration.h"

struct outputs config(char* config_File, struct main_data* data){
    struct outputs* out;
    char r = 'r';
    int max_num = 5;
    int max_str = 15;
    FILE *config = fopen (config_File, &r);
    char line[15];
    
        fgets (line, max_num, config);
        data->max_ops = atoi(line);

        fgets (line, max_num, config);
        data->buffers_size = atoi(line);

        fgets (line, max_num, config);
        data->n_clients = atoi(line);

        fgets (line, max_num, config);
        data->n_intermediaries = atoi(line);

        fgets (line, max_num, config);
        data->n_enterprises = atoi(line);
        data->terminate = 0;

        fgets (line, max_str, config);
        out->log = line;

        fgets (line, max_str, config);
        out->stats = line;

        fgets (line, max_num, config);
        out->alarm = atoi(line);

    fclose(config);
    return *out;
}