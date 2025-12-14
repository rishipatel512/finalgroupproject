#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "../include/parse.h"
#include "../include/execute.h"
#include "../include/builtin.h"
#include "../include/signals.h"
#include "../include/logger.h"

// Global variables
int log_file;

// Function declarations
void parse_command(char line[], char *argv[], int *argc, char *input_file, 
                   char *output_file, int *append, int *background,
                   int *has_pipe, char *pipe_argv[], int *pipe_argc);
void execute_command(char *argv[], char *input_file, 
                     char *output_file, int append, int background,
                     int has_pipe, char *pipe_argv[],
                     char original_line[]);
int is_builtin(char *argv[], int argc);
void execute_builtin(char *argv[], int argc);
void setup_signals(void);
void init_logger(void);
void log_command(int pid, char cmdline[], int status);
void close_logger(void);

int main() {
    char line[1024];
    char *argv[64];
    int argc;
    char input_file[256];
    char output_file[256];
    int append;
    int background;
    int has_pipe;
    char *pipe_argv[64];
    int pipe_argc;
    int i;
    
    log_file = -1;
    
    // Initialize logger
    init_logger();
    
    // Setup signals
    setup_signals();
    
    // Main shell loop
    while (1) {
        // Print prompt
        printf("myshell> ");
        fflush(stdout);
        
        // Read input
        if (fgets(line, 1024, stdin) == NULL) {
            break;
        }
        
        // Remove newline at end
        for (i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\n') {
                line[i] = '\0';
                break;
            }
        }
        
        // Skip empty lines
        if (line[0] == '\0') {
            continue;
        }
        
        // Parse the command
        parse_command(line, argv, &argc, input_file, output_file, 
                      &append, &background, &has_pipe, pipe_argv, &pipe_argc);
        
        if (argc == 0) {
            continue;
        }
        
        // Check for built-in commands
        if (is_builtin(argv, argc)) {
            execute_builtin(argv, argc);
        } else {
            // Execute external command
            execute_command(argv, input_file, output_file, append, 
                          background, has_pipe, pipe_argv, line);
        }
        
        // Free allocated memory
        for (i = 0; i < argc; i++) {
            free(argv[i]);
        }
        for (i = 0; i < pipe_argc; i++) {
            free(pipe_argv[i]);
        }
    }
    
    // Cleanup
    close_logger();
    printf("\n");
    return 0;
}
