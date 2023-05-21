#include "log.h"

FILE* l;

void createLog(char* name){
    l = fopen(name, "w");
}

void writeLog(struct timespec* t, char* op){
    //char* line;
    //fputs(line, log);
}

void closeLog(){
    fclose(l);
}