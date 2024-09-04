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

int main(void) {
    char commands[MAX_COMMANDS][MAX_LINE_LENGTH] = {0};
    char parameters[MAX_PARAMETERS][MAX_PARAMETER_LENGTH] = {0};

    load_commands("chars.conf", commands);
    
    for (int i = 0; i < 20; i++) {
        if(strlen(commands[i]) == 0) {
            continue;
        }
        printf("%s", commands[i]);
    }

    printf("===================\n");
    
    for (int i = 0; i < MAX_COMMANDS; i++) {
        if(strlen(commands[i]) == 0) {
            continue;
        }

        parse_command(commands[i], parameters);
        
        printf("%s %s %s %s %s %s %s %s\n", parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);

        // execl(parameters[1], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], NULL);
    }

    parse_command(commands[3], parameters);
    execl(parameters[1], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], NULL);
    
    execl("./printchars", "./printchars", "a", NULL);
    
    printf("===================\n");
    printf("%s %s %s %s %s %s %s %s\n", parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
    
    return 0;// Success in Main method
}
