#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Parse command line into tokens
void parse_command(char line[], char *argv[], int *argc, char *input_file, 
                   char *output_file, int *append, int *background,
                   int *has_pipe, char *pipe_argv[], int *pipe_argc) {
    
    int i;
    int j;
    char token[256];
    int in_pipe;
    
    i = 0;
    j = 0;
    in_pipe = 0;
    
    // Initialize
    *argc = 0;
    *pipe_argc = 0;
    *background = 0;
    *append = 0;
    *has_pipe = 0;
    input_file[0] = '\0';
    output_file[0] = '\0';
    
    // Parse character by character
    while (line[i] != '\0') {
        // Skip spaces
        while (line[i] == ' ' || line[i] == '\t') {
            i = i + 1;
        }
        
        if (line[i] == '\0') {
            break;
        }
        
        // Read a token
        j = 0;
        while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') {
            token[j] = line[i];
            j = j + 1;
            i = i + 1;
        }
        token[j] = '\0';
        
        // Check what this token is
        if (strcmp(token, "&") == 0) {
            *background = 1;
        }
        else if (strcmp(token, "|") == 0) {
            *has_pipe = 1;
            in_pipe = 1;
        }
        else if (strcmp(token, "<") == 0) {
            // Next token is input file
            while (line[i] == ' ' || line[i] == '\t') {
                i = i + 1;
            }
            j = 0;
            while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') {
                input_file[j] = line[i];
                j = j + 1;
                i = i + 1;
            }
            input_file[j] = '\0';
        }
        else if (strcmp(token, ">") == 0) {
            // Next token is output file
            *append = 0;
            while (line[i] == ' ' || line[i] == '\t') {
                i = i + 1;
            }
            j = 0;
            while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') {
                output_file[j] = line[i];
                j = j + 1;
                i = i + 1;
            }
            output_file[j] = '\0';
        }
        else if (strcmp(token, ">>") == 0) {
            // Next token is output file (append)
            *append = 1;
            while (line[i] == ' ' || line[i] == '\t') {
                i = i + 1;
            }
            j = 0;
            while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t') {
                output_file[j] = line[i];
                j = j + 1;
                i = i + 1;
            }
            output_file[j] = '\0';
        }
        else {
            // Regular argument - allocate and copy
            char *arg;
            arg = malloc(strlen(token) + 1);
            strcpy(arg, token);
            
            if (in_pipe == 1) {
                pipe_argv[*pipe_argc] = arg;
                *pipe_argc = *pipe_argc + 1;
            } else {
                argv[*argc] = arg;
                *argc = *argc + 1;
            }
        }
    }
    
    // NULL-terminate arrays
    argv[*argc] = NULL;
    pipe_argv[*pipe_argc] = NULL;
}
