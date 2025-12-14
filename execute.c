// execute.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

// Function declarations
void log_command(int pid, char cmdline[], int status);

// Setup I/O redirection
void setup_redirects(char *input_file, char *output_file, int append) {
    int fd;
    
    // Input redirection
    if (input_file[0] != '\0') {
        fd = open(input_file, 0);
        if (fd < 0) {
            perror("open input file");
            exit(1);
        }
        dup2(fd, 0);
        close(fd);
    }
    
    // Output redirection
    if (output_file[0] != '\0') {
        if (append == 1) {
            fd = open(output_file, 1089, 0644);
        } else {
            fd = open(output_file, 577, 0644);
        }
        if (fd < 0) {
            perror("open output file");
            exit(1);
        }
        dup2(fd, 1);
        close(fd);
    }
}

// Execute simple command (no pipe)
void execute_simple(char *argv[], char *input_file, 
                    char *output_file, int append, int background,
                    char original_line[]) {
    int pid;
    int status;
    
    pid = fork();
    
    if (pid < 0) {
        perror("fork");
        return;
    }
    
    if (pid == 0) {
        // Child process
        signal(2, 0);
        
        // Setup redirections
        setup_redirects(input_file, output_file, append);
        
        // Execute the command
        execvp(argv[0], argv);
        
        // If we get here, execvp failed
        perror("execvp");
        exit(1);
    } else {
        // Parent process
        if (background == 1) {
            // Background job - don't wait
            printf("[bg] started pid %d\n", pid);
            log_command(pid, original_line, -1);
        } else {
            // Foreground job - wait for completion
            waitpid(pid, &status, 0);
            
            // Log the command
            log_command(pid, original_line, status);
        }
    }
}

// Execute piped command (cmd1 | cmd2)
void execute_pipe(char *argv[], char *input_file,
                  char *output_file, int append,
                  char *pipe_argv[],
                  char original_line[]) {
    int pipefd[2];
    int pid1;
    int pid2;
    int status;
    int fd;
    
    // Create pipe
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }
    
    // Fork first child (left side)
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return;
    }
    
    if (pid1 == 0) {
        // First child: cmd1
        signal(2, 0);
        
        // Redirect stdout to pipe
        dup2(pipefd[1], 1);
        close(pipefd[0]);
        close(pipefd[1]);
        
        // Input redirection
        if (input_file[0] != '\0') {
            fd = open(input_file, 0);
            if (fd < 0) {
                perror("open input file");
                exit(1);
            }
            dup2(fd, 0);
            close(fd);
        }
        
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    }
    
    // Fork second child (right side)
    pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return;
    }
    
    if (pid2 == 0) {
        // Second child: cmd2
        signal(2, 0);
        
        // Redirect stdin from pipe
        dup2(pipefd[0], 0);
        close(pipefd[0]);
        close(pipefd[1]);
        
        // Output redirection
        if (output_file[0] != '\0') {
            if (append == 1) {
                fd = open(output_file, 1089, 0644);
            } else {
                fd = open(output_file, 577, 0644);
            }
            if (fd < 0) {
                perror("open output file");
                exit(1);
            }
            dup2(fd, 1);
            close(fd);
        }
        
        execvp(pipe_argv[0], pipe_argv);
        perror("execvp");
        exit(1);
    }
    
    // Parent: close pipe and wait
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    log_command(pid2, original_line, status);
}

// Main execution function
void execute_command(char *argv[], char *input_file, 
                     char *output_file, int append, int background,
                     int has_pipe, char *pipe_argv[],
                     char original_line[]) {
    if (argv == 0) {
        return;
    }
    
    if (has_pipe == 1) {
        execute_pipe(argv, input_file, output_file, append,
                    pipe_argv, original_line);
    } else {
        execute_simple(argv, input_file, output_file, append,
                      background, original_line);
    }
}
