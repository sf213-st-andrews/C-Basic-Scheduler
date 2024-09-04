#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <limits.h>
// #include <signal.h>
// #include <sys/wait.h>
#include <string.h>

#define MAX_PID                 5       // Scheduler can hold 5 processes
#define MAX_PARAMETERS          8       // A command will have 8 or less parameters
#define MAX_PARAMETER_LENGTH    32      // A parameter will have 32 characters or less

typedef struct Process {
    pid_t pid;
    int priority;
    char path[MAX_PARAMETER_LENGTH];
    char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH];
} Process;

Process* new_Process(pid_t pid, char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH]) {
    Process* this = malloc(sizeof(Process));
    
    this->pid = pid;
    this->priority = atoi(parameters[0]);
    strcpy(this->path, parameters[1]);

    for (int i = 0; i < MAX_PARAMETERS; i++) {
        strcpy(this->parameters[i], parameters[i]);    
    }

    return this;
}

Process* get_highest_priority(Process p[MAX_PID]) {
    Process* ph = &(p[0]);
    for (int i = 1; i < MAX_PID; i++) {
        if (ph -> priority < p[i].priority) {
            ph = &p[i];
        }
    }
    return ph;
}