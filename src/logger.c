// logger.c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

// Global log file descriptor
extern int log_file;

// Initialize logger - open the log file
void init_logger(void) {
    // O_WRONLY | O_CREAT | O_APPEND = 1 | 64 | 1024 = 1089
    log_file = open("myshell.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_file < 0) {
        perror("open myshell.log");
    }
}

// Log a command to the file using write()
void log_command(int pid, char cmdline[], int status) {
    char buffer[1024];
    int len;
    int i;
    char pid_str[32];
    char status_str[32];
    
    if (log_file < 0) {
        return;
    }
    
    len = 0;
    
    // Add "[pid="
    buffer[len] = '[';
    len = len + 1;
    buffer[len] = 'p';
    len = len + 1;
    buffer[len] = 'i';
    len = len + 1;
    buffer[len] = 'd';
    len = len + 1;
    buffer[len] = '=';
    len = len + 1;
    
    // Convert pid to string
    sprintf(pid_str, "%d", pid);
    for (i = 0; pid_str[i] != '\0'; i = i + 1) {
        buffer[len] = pid_str[i];
        len = len + 1;
    }
    
    // Add "] cmd=\""
    buffer[len] = ']';
    len = len + 1;
    buffer[len] = ' ';
    len = len + 1;
    buffer[len] = 'c';
    len = len + 1;
    buffer[len] = 'm';
    len = len + 1;
    buffer[len] = 'd';
    len = len + 1;
    buffer[len] = '=';
    len = len + 1;
    buffer[len] = '"';
    len = len + 1;
    
    // Add command line
    for (i = 0; cmdline[i] != '\0'; i = i + 1) {
        buffer[len] = cmdline[i];
        len = len + 1;
    }
    
    // Add "\" status="
    buffer[len] = '"';
    len = len + 1;
    buffer[len] = ' ';
    len = len + 1;
    buffer[len] = 's';
    len = len + 1;
    buffer[len] = 't';
    len = len + 1;
    buffer[len] = 'a';
    len = len + 1;
    buffer[len] = 't';
    len = len + 1;
    buffer[len] = 'u';
    len = len + 1;
    buffer[len] = 's';
    len = len + 1;
    buffer[len] = '=';
    len = len + 1;
    
    // Convert status to string
    sprintf(status_str, "%d", status);
    for (i = 0; status_str[i] != '\0'; i = i + 1) {
        buffer[len] = status_str[i];
        len = len + 1;
    }
    
    // Add newline
    buffer[len] = '\n';
    len = len + 1;
    
    // Write to log file using write() system call
    write(log_file, buffer, len);
}

// Close the log file
void close_logger(void) {
    if (log_file >= 0) {
        close(log_file);
        log_file = -1;
    }
}
