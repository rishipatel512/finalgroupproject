#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Check if command is a built-in
int is_builtin(char *argv[], int argc) {
    if (argc == 0) {
        return 0;
    }
    
    if (strcmp(argv[0], "exit") == 0) {
        return 1;
    }
    if (strcmp(argv[0], "quit") == 0) {
        return 1;
    }
    if (strcmp(argv[0], "cd") == 0) {
        return 1;
    }
    
    return 0;
}

// Execute cd command
void builtin_cd(char *argv[], int argc) {
    char *path;
    
    if (argc < 2) {
        // No argument - go to HOME
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    } else {
        // Use provided path
        path = argv[1];
    }
    
    if (chdir(path) != 0) {
        perror("cd");
    }
}

// Execute a built-in command
void execute_builtin(char *argv[], int argc) {
    if (argc == 0) {
        return;
    }
    
    // exit or quit
    if (strcmp(argv[0], "exit") == 0) {
        exit(0);
    }
    
    if (strcmp(argv[0], "quit") == 0) {
        exit(0);
    }
    
    // cd
    if (strcmp(argv[0], "cd") == 0) {
        builtin_cd(argv, argc);
    }
}
