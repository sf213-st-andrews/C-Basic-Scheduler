// sf213
// first step, hard coded fork() and exec() calls

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_COMMANDS            20      // Scheduler can only accept 20 commands from chars.conf
#define MAX_LINE_LENGTH         256     // Accepted commands are assumed to be 256 characters at most
#define MAX_PARAMETERS          8       // A command will have 8 or less parameters
#define MAX_PARAMETER_LENGTH    32      // A parameter will have 32 characters or less

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


int main(int argc, char *argv[]) {
    pid_t firstpid = 0, secondpid = 0, lastpid = 0;

    char commands[MAX_COMMANDS][MAX_LINE_LENGTH] = {0};

    load_commands("chars.conf", commands);

    // Start first process
    firstpid = fork();

    if(firstpid < 0) {
        printf("Something went wrong!\n");
    } else if(firstpid > 0) {    // We are parent. Immediately stop the new process
        kill(firstpid,SIGSTOP);
    } else {  // We are a child process -- overwrite our process space with the new program
        char parameters_0[MAX_PARAMETERS][MAX_PARAMETER_LENGTH] = {0};
        parse_command(commands[0], parameters_0);
        execl(parameters_0[1], parameters_0[1], parameters_0[2], parameters_0[3], parameters_0[4], parameters_0[5], parameters_0[6], parameters_0[7], NULL);
        
        // execl("./printchars", "./printchars", "a", NULL);       // Print some "a"s
    }

    // Start second process
    secondpid = fork();

    if(secondpid < 0) {
        printf("Something went wrong!\n");
    } else if(secondpid > 0) {    // We are parent. Immediately stop the new process
        kill(secondpid,SIGSTOP);
    } else {  // We are a child process -- overwrite our process space with the new program
        char parameters_1[MAX_PARAMETERS][MAX_PARAMETER_LENGTH] = {0};
        parse_command(commands[1], parameters_1);
        execl(parameters_1[1], parameters_1[1], parameters_1[2], parameters_1[3], parameters_1[4], parameters_1[5], parameters_1[6], parameters_1[7], NULL);
        // execl("./printchars", "./printchars", "b", NULL);       // Print some "b"s
    }

    // Start last? process
    lastpid = fork();

    if(lastpid < 0) {
        printf("Something went wrong!\n");
    } else if(lastpid > 0) {    // We are parent. Immediately stop the new process
        kill(lastpid,SIGSTOP);
    } else { // We are a child process -- overwrite our process space with the new program
        char parameters_2[MAX_PARAMETERS][MAX_PARAMETER_LENGTH] = {0};
        parse_command(commands[2], parameters_2);
        execl(parameters_2[1], parameters_2[1], parameters_2[2], parameters_2[3], parameters_2[4], parameters_2[5], parameters_2[6], parameters_2[7], NULL);
    }

    // This part will only be run by the parent process -- the scheduler
    if(lastpid) {
        printf("Parent here %d.\n", lastpid);

        kill(firstpid, SIGCONT);        // Start P1
        usleep(1500000);                // 500000
        kill(firstpid, SIGSTOP);

        kill(secondpid, SIGCONT);         // Start P2
        usleep(3000000);
        kill(secondpid, SIGSTOP);



        kill(firstpid, SIGCONT);        // Start P1
        usleep(1500000);                 // 500000
        kill(firstpid, SIGSTOP);
        
        kill(lastpid, SIGCONT);         // Start PL....
        usleep(500000);
        kill(lastpid, SIGSTOP);

        
    }

    // Kill all processes before finishing.
    printf("\nFinishing...\n");
    kill(firstpid, SIGTERM);
    kill(secondpid, SIGTERM);
    kill(lastpid, SIGTERM);
}

