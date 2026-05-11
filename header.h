#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdbool.h>

// Macros to identify command types
#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3

// Macros defining buffer sizes
#define MAX_EXTERNAL_COMMANDS 200
#define MAX_PROMPT_LEN 25
#define MAX_INPUT_LEN  50

// ANSI color escape sequences for terminal output
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

extern char *builtins[];
extern char *external_commands[MAX_EXTERNAL_COMMANDS];
extern char input_string[MAX_INPUT_LEN];
extern pid_t pid;
extern int status;
extern int last_exit_status;
extern char prompt[MAX_PROMPT_LEN];

// Structure to maintain stopped jobs information
typedef struct list
{
    pid_t pid;
    char sleep_command[25];
    struct list *next;
} stopped_jobs;

// Function prototypes related to shell execution
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

// Function prototypes for command identification and execution
int check_command_type(char *command);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);

// Signal handler function prototype
void signal_handler(int sig_num);

// Function prototypes related to external command handling
void extract_external_commands(char **external_commands);
void check_rows_and_pipe(char *input_string, int *row, int *pipe_count);
void extract_external_commands_from_input_string(char *input_string, char *external_cmd_array[]);
void allocate_dynamic_cols(char **external_commands, char *buff, int *col, int *row);
void execute_piped_commands(char **external_cmd_array, int pipe_count);
void remove_trailing_space(char *input_string);

// Function prototypes to check command type
bool check_internal_command(char *command);
bool check_external_command(char *command);

// Function to free allocated external command memory
void free_external_commands(char **external_commands);

// Job control related function prototypes
void insert_jobs_at_start(stopped_jobs **head);
void print_jobs(stopped_jobs *head);
void delete_jobs(stopped_jobs **head);

// void copy_change(char *prompt, char *input_string);
// void echo(char *input_string, int status);

#endif