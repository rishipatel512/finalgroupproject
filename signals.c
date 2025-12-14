// signals.c
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// SIGCHLD handler to reap zombie processes
void sigchld_handler(int sig) {
    int status;
    int pid;
    
    // Avoid unused parameter warning
    (void)sig;
    
    // Reap all available children without blocking
    while (1) {
        pid = waitpid(-1, &status, 1);
        if (pid <= 0) {
            break;
        }
    }
}

// Setup signal handlers
void setup_signals(void) {
    // Ignore SIGINT (Ctrl-C) in the shell
    signal(SIGINT, SIG_IGN);
    
    // Install SIGCHLD handler to prevent zombies
    signal(SIGCHLD, sigchld_handler);
}
