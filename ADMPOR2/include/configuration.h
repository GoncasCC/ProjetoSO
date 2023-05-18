#include <stdio.h>
#include "main.h"

struct outputs{
    char* log;
    char* stats;
    int alarm;
};

/**
 */
struct outputs* config(char* config_File, struct main_data* data);