#include "header.h"

void execute_external_commands(char *input_string)
{
    // Variables to store number of pipes and number of commands
    int pipe_count = 0;
    int rows = 0;

    // Counting number of commands and pipe symbols in input string
    check_rows_and_pipe(input_string, &rows, &pipe_count);

    // Dynamically allocating memory for command array
    char **external_cmd_array = malloc((rows + 1) * sizeof(char *));

    // Converting input string into a 2D command array
    extract_external_commands_from_input_string(input_string, external_cmd_array);

    // If there is no pipe in the command
    if (pipe_count == 0)
    {
        // Executing single external command
        execvp(external_cmd_array[0], external_cmd_array);

        // Error message if execvp fails
        perror(ANSI_COLOR_RED "execvp" ANSI_COLOR_RESET);

        // Free allocated memory before exiting
        free_external_commands(external_cmd_array);
        free(external_cmd_array);
        exit(1);
    }
    else
    {
        // Executing commands with pipe support
        execute_piped_commands(external_cmd_array, pipe_count);

        // Free allocated memory after execution
        free_external_commands(external_cmd_array);
        free(external_cmd_array);
    }
}

void execute_piped_commands(char **external_cmd_array, int pipe_count)
{
    // Array to store pipe file descriptors
    int pipe_fd[2];

    // Array to store starting index of each command
    int cmd[pipe_count + 1];
    cmd[0] = 0;
    int count = 1;

    // Identifying pipe positions and splitting commands
    for (int i = 0; external_cmd_array[i] != NULL; i++)
    {
        if (strcmp(external_cmd_array[i], "|") == 0)
        {
            external_cmd_array[i] = NULL;
            cmd[count++] = i + 1;
        }
    }

    // Loop to create processes for each command in pipeline
    for (int i = 0; i <= pipe_count; i++)
    {
        // Creating pipe for inter-process communication
        if (i != pipe_count)
            pipe(pipe_fd);

        // Creating child process
        pid_t pid_pipe = fork();

        if (pid_pipe > 0)
        {
            // In parent process
            if (i != pipe_count)
            {
                // Redirecting standard input to read from pipe
                close(pipe_fd[1]);
                dup2(pipe_fd[0], 0);
                close(pipe_fd[0]);
            }

            // Waiting for child process to complete
            wait(NULL);
        }
        else if (pid_pipe == 0)
        {
            // In child process
            if (i != pipe_count)
            {
                // Redirecting standard output to write into pipe
                close(pipe_fd[0]);
                dup2(pipe_fd[1], 1);
                close(pipe_fd[1]);
            }

            // Executing the current command in pipeline
            execvp(external_cmd_array[cmd[i]], &external_cmd_array[cmd[i]]);

            // Error message if execvp fails
            perror(ANSI_COLOR_RED "execvp" ANSI_COLOR_RESET);
            exit(1);
        }
        else
        {
            // Error message if fork fails
            perror(ANSI_COLOR_RED "fork" ANSI_COLOR_RESET);
        }
    }
}
