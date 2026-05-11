#include "header.h"

// External variable to store last command exit status
extern int last_exit_status;

// Head pointer for stopped jobs linked list
extern stopped_jobs *head;

void execute_internal_commands(char *input_string)
{
    // Buffer to store current working directory
    char buff[100];

    // Exit command to terminate the shell
    if (strcmp(input_string, "exit") == 0)
    {
        // Free dynamically allocated external commands
        free_external_commands(external_commands);
        exit(0);
    }

    // Print present working directory
    else if (strcmp(input_string, "pwd") == 0)
    {
        if (getcwd(buff, sizeof(buff)) != NULL)
            printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET, buff);
        else
            perror(ANSI_COLOR_RED "pwd" ANSI_COLOR_RESET);
    }

    // Change directory command
    else if (strncmp(input_string, "cd", 2) == 0)
    {
        // If directory path is provided
        if (input_string[2] == ' ')
        {
            if (chdir(input_string + 3) != 0)
                perror(ANSI_COLOR_RED "cd" ANSI_COLOR_RESET);
        }
        // If no path is provided, change to HOME directory
        else
        {
            char *home = getenv("HOME");
            if (home)
            {
                if (chdir(home) != 0)
                    perror(ANSI_COLOR_RED "cd" ANSI_COLOR_RESET);
                printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET, home);
            }
            else
            {
                // HOME environment variable not set
                printf(ANSI_COLOR_RED "cd: HOME not set\n" ANSI_COLOR_RESET);
            }
        }
    }

    // Print current shell process ID
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf(ANSI_COLOR_CYAN "%d\n" ANSI_COLOR_RESET, getpid());
    }

    // Print exit status of last executed command
    else if (strcmp(input_string, "echo $?") == 0)
    {
        printf(ANSI_COLOR_YELLOW "%d\n" ANSI_COLOR_RESET, last_exit_status);
    }

    // Print SHELL environment variable
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        char *ptr = getenv("SHELL");
        if (ptr)
            printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET, ptr);
        else
            printf(ANSI_COLOR_RED "SHELL not set\n" ANSI_COLOR_RESET);
    }

    // Display list of stopped background jobs
    else if (strcmp(input_string, "jobs") == 0)
    {
        print_jobs(head);
    }

    // Bring the most recent stopped job to foreground
    else if (strcmp(input_string, "fg") == 0)
    {
        if (head != NULL)
        {
            // Resume the stopped process
            kill(head->pid, SIGCONT);
            pid = head->pid;

            // Print command being brought to foreground
            printf(ANSI_COLOR_CYAN "%s\n" ANSI_COLOR_RESET, head->sleep_command);

            // Wait for the process to finish or stop again
            waitpid(pid, &status, WUNTRACED);
            pid = 0;

            // Remove job from jobs list
            delete_jobs(&head);
        }
    }

    // Resume stopped job in background
    else if (strcmp(input_string, "bg") == 0)
    {
        if (head != NULL)
        {
            // Enable SIGCHLD handling for background job
            signal(SIGCHLD, signal_handler);

            // Resume the stopped process
            kill(head->pid, SIGCONT);

            // Print background job information
            printf(ANSI_COLOR_BLUE "[%d] %s &\n" ANSI_COLOR_RESET,
                   head->pid, head->sleep_command);

            // Remove job from jobs list
            delete_jobs(&head);
        }
        else
        {
            // No stopped job available
            printf(ANSI_COLOR_YELLOW "bg: no current job\n" ANSI_COLOR_RESET);
        }
    }
}
