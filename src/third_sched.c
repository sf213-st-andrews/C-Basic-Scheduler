// sf213
// third step: configuration file reader, FIFO batch scheduler


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_PID                 5       // Scheduler can hold 5 processes
#define MAX_COMMANDS            20      // Scheduler can only accept 20 commands from chars.conf
#define MAX_LINE_LENGTH         256     // Accepted commands are assumed to be 256 characters at most
#define MAX_PARAMETERS          8       // A command will have 8 or less parameters
#define MAX_PARAMETER_LENGTH    32      // A parameter will have 32 characters or less
#define QUANTUM                 500000  // The time allotted for a process to work

// Load commands from a configuration file into a given 2D char array/
int load_commands(char* conf_file, char commands[MAX_COMMANDS][MAX_LINE_LENGTH]) {
    FILE* fp = fopen(conf_file, "r");
    if (fp == NULL) {
        return 0;// Failiure
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;

    while ((read = getline(&line, &len, fp)) != -1 && count < MAX_COMMANDS) {
        if(read > MAX_LINE_LENGTH) {
            fclose(fp);
            free(line);
            return 0;// Failiure
        }
        // printf("conf: %s", line);
        strcpy(commands[count], line);
        count++;
    }

    fclose(fp);
    free(line);
    return 1;// Success
}

int parse_command(char command[MAX_LINE_LENGTH], char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH]) {
    char* token;
    char* rest = command;
    char* delim = " \n";
    int count = 0;
    
    while ((token = strtok_r(rest, delim, &rest)) && count < MAX_PARAMETERS) {
        strcpy(parameters[count], token);
        count++;
    }
    return 1;// Success
}

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

void set_Process_variables(Process* p, pid_t pid, char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH]) {
    p->pid = pid;
    p->priority = atoi(parameters[0]);
    strcpy(p->path, parameters[1]);

    for (int i = 0; i < MAX_PARAMETERS; i++) {
        strcpy(p->parameters[i], parameters[i]);    
    }
}

void set_Process_parameters(Process* p, char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH]) {
    p->priority = atoi(parameters[0]);
    strcpy(p->path, parameters[1]);

    for (int i = 0; i < MAX_PARAMETERS; i++) {
        strcpy(p->parameters[i], parameters[i]);    
    }
}

void set_Process_pid(Process* p, pid_t pid) {
    p->pid = pid;
}

void work_quantum_Process(Process* p, int quantum) {
    kill(p->pid, SIGCONT);
    usleep(quantum);
    kill(p->pid, SIGSTOP);   
}




// Scheduler
int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Error: too few arguments\n");
        return 0;
    }
    Process* p = malloc(sizeof(Process) * MAX_PID);
    char commands[MAX_COMMANDS][MAX_LINE_LENGTH];
    load_commands(argv[1], commands);
    
    for (int i = 0; i < MAX_PID; i++) {
        p[i].pid = fork();

        if(p[i].pid < 0) {
            printf("Something went wrong!\n");
        } else if(p[i].pid > 0) {// Is parent. Immeditiatly kill/stop child
            kill(p[i].pid, SIGSTOP);
        } else {
            char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH] = {0};
            parse_command(commands[i], parameters);
            set_Process_parameters(&p[i], parameters);
            execl(p[i].parameters[1], p[i].parameters[1], p[i].parameters[2], p[i].parameters[3], p[i].parameters[4], p[i].parameters[5], p[i].parameters[6], p[i].parameters[7], NULL);
        }
    }

    // This part will only be run by the parent process -- the scheduler
    if(p[MAX_PID].pid) {
        printf("Parent here %d.\n", p[MAX_PID].pid);
        int status = 0;
        pid_t w = 0;// waitpid(&p[i], &wstatus, WNOHANG);
        
        for (int i = 0; i < MAX_PID; i++) {
            while (w == 0) {
                w = waitpid(p[i].pid, &status, WNOHANG);
                work_quantum_Process(&p[i], QUANTUM);
            }
            w = 0;
        }     
    }

    // Kill all processes before finishing.
    printf("\nFinishing...\n");
    for (int i = 0; i < MAX_PID; i++) {
        kill(p[i].pid, SIGTERM);
    }
    free(p);
}