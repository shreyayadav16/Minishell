#include "header.h"

// Array to store all external commands read from file
char *external_commands[MAX_EXTERNAL_COMMANDS];

// Variables used for process control and exit status
int status;
int last_exit_status;
pid_t pid;

// Head pointer for stopped jobs linked list
stopped_jobs *head = NULL;

void scan_input(char *prompt, char *input_string)
{
    // Free previously allocated external commands
    free_external_commands(external_commands);

    // Extract external commands from file
    extract_external_commands(external_commands);

    // Register signal handlers for Ctrl+C and Ctrl+Z
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);

    // Infinite loop to keep shell running
    while (1)
    {
        // Reset pid before taking new input
        pid = 0;

        // Display shell prompt
        printf(ANSI_COLOR_GREEN "%s " ANSI_COLOR_RESET, prompt);

        // Read complete input line from user
        scanf("%[^\n]", input_string);
        getchar();

        // Check if user is trying to change the shell prompt
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            int i = 4;
            int flag = 1;

            // Remove trailing spaces from input
            remove_trailing_space(input_string);

            // Check for spaces in new prompt value
            while (input_string[i] != '\0')
            {
                if (input_string[i] == ' ')
                {
                    flag = 0;
                    break;
                }
                i++;
            }

            // Update prompt if no spaces are present
            if (flag)
                strcpy(prompt, input_string + 4);
            else
                printf(ANSI_COLOR_RED "PS1 error: spaces not allowed\n" ANSI_COLOR_RESET);
        }
        else
        {
            // Extract command name from input string
            char *cmd = get_command(input_string);

            // Identify command type (builtin, external, or invalid)
            int type = check_command_type(cmd);

            // Execute builtin command
            if (type == BUILTIN)
            {
                execute_internal_commands(input_string);
                last_exit_status = 0;
            }
            // Execute external command
            else if (type == EXTERNAL)
            {
                // Creating child process
                pid = fork();

                if (pid > 0)
                {
                    // In parent we are waiting for the child termination or stopping
                    waitpid(pid, &status, WUNTRACED);

                    // Store exit status if child exited normally
                    if (WIFEXITED(status))
                    {
                        last_exit_status = WEXITSTATUS(status);
                    }
                    // Store signal-based status if child was stopped
                    else if (WIFSTOPPED(status))
                    {
                        last_exit_status = 128 + WSTOPSIG(status);
                    }
                }
                else if (pid == 0)
                {
                    // In child we are setting signals to default
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);

                    // Execute external command
                    execute_external_commands(input_string);
                    exit(0);
                }
                else
                {
                    // Error message if child process not created
                    perror(ANSI_COLOR_RED "fork" ANSI_COLOR_RESET);
                }
            }
            // Command not found case
            else if (type == NO_COMMAND)
            {
                printf(ANSI_COLOR_RED "Command not found: %s\n" ANSI_COLOR_RESET, cmd);
            }
        }
    }
}

void signal_handler(int sig_num)
{
    // Handling Ctrl+C signal
    if (sig_num == SIGINT)
    {
        if (pid == 0)
        {
            // Print fresh prompt when no child is running
            printf(ANSI_COLOR_GREEN "\n%s " ANSI_COLOR_RESET, prompt);
            fflush(stdout);
        }
    }

    // Handling Ctrl+Z signal
    if (sig_num == SIGTSTP)
    {
        if (pid == 0)
        {
            // Print prompt when no child is running
            printf(ANSI_COLOR_GREEN "\n%s " ANSI_COLOR_RESET, prompt);
            fflush(stdout);
        }
        else if (pid > 0)
        {
            // Insert stopped process into jobs list
            printf(ANSI_COLOR_YELLOW "\n" ANSI_COLOR_RESET);
            insert_jobs_at_start(&head);
        }
    }
    // Handling terminated background processes
    else if (sig_num == SIGCHLD)
    {
        waitpid(-1, &status, WNOHANG);
    }
}
