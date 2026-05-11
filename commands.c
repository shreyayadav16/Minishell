#include "header.h"

// Array containing all supported builtin commands
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help",
                    "jobs", "fg", "bg", NULL};

// Function to extract the command name from input string
char *get_command(char *input_string)
{
    // Static buffer to store command name
    static char cmd[50];
    int i = 0;

    // Copy characters until space or end of string
    while (input_string[i] != ' ' && input_string[i] != '\0')
    {
        cmd[i] = input_string[i];
        i++;
    }

    // Null terminate the command string
    cmd[i] = '\0';
    return cmd;
}

// Function to identify the type of command
int check_command_type(char *command)
{
    if (check_internal_command(command))
        return BUILTIN;
    else if (check_external_command(command))
        return EXTERNAL;
    else
        return NO_COMMAND;
}

// Function to check whether command is a builtin command
bool check_internal_command(char *command)
{
    int i = 0;

    // Compare command with builtin command list
    while (builtins[i] != NULL)
    {
        if (strcmp(builtins[i], command) == 0)
            return true;
        i++;
    }
    return false;
}

// Function to check whether command is an external command
bool check_external_command(char *command)
{
    int i = 0;

    // Compare command with external command list
    while (external_commands[i] != NULL)
    {
        if (strcmp(external_commands[i], command) == 0)
            return true;
        i++;
    }
    return false;
}

// Function to allocate memory for each extracted command token
void allocate_dynamic_cols(char **external_commands, char *buff, int *col, int *row)
{
    // Null terminate the buffer
    buff[*col] = '\0';

    // Allocate memory for the command
    external_commands[*row] = malloc(strlen(buff) + 1);
    if (external_commands[*row] == NULL)
    {
        // Error message if memory allocation fails
        perror(ANSI_COLOR_RED "malloc" ANSI_COLOR_RESET);
        return;
    }

    // Copy command into allocated memory
    strcpy(external_commands[*row], buff);
    (*row)++;
}

// Function to convert input string into 2D command array
void extract_external_commands_from_input_string(char *input_string, char *external_cmd_array[])
{
    int row = 0;
    int col = 0;
    char buff[100];

    // Parsing input string character by character
    for (int i = 0; input_string[i] != '\0'; i++)
    {
        if (input_string[i] == ' ')
        {
            // Allocate memory when a token ends
            if (col > 0)
            {
                allocate_dynamic_cols(external_cmd_array, buff, &col, &row);
                col = 0;
            }
        }
        else
        {
            buff[col++] = input_string[i];
        }
    }

    // Handling the last token
    if (col > 0)
    {
        allocate_dynamic_cols(external_cmd_array, buff, &col, &row);
    }

    // Mark end of command array
    external_cmd_array[row] = NULL;
}

// Function to read external commands from file
void extract_external_commands(char **external_commands)
{
    // Open file containing external command list
    int fd = open("ext_cmd.txt", O_RDONLY);
    if (fd == -1)
    {
        // Error message if file open fails
        perror(ANSI_COLOR_RED "open" ANSI_COLOR_RESET);
        return;
    }

    char ch;
    char buff[256];
    int row = 0, col = 0;

    // Read file character by character
    while (read(fd, &ch, 1) == 1)
    {
        if (ch == '\n')
        {
            allocate_dynamic_cols(external_commands, buff, &col, &row);
            col = 0;
        }
        else
        {
            buff[col++] = ch;
        }
    }

    // Allocate memory for the last command
    allocate_dynamic_cols(external_commands, buff, &col, &row);

    // Mark end of external commands array
    external_commands[row] = NULL;

    // Close the file
    close(fd);
}

// Function to free dynamically allocated external command memory
void free_external_commands(char **external_commands)
{
    int i = 0;

    // Free each allocated command string
    while (external_commands[i] != NULL)
    {
        free(external_commands[i]);
        i++;
    }
}

// Function to remove trailing spaces from input string
void remove_trailing_space(char *input_string)
{
    int len = strlen(input_string) - 1;

    // Remove spaces from the end of the string
    while (input_string[len] == ' ' && (len >= 0))
    {
        input_string[len--] = '\0';
    }
}

// Function to count number of commands and pipes in input string
void check_rows_and_pipe(char *input_string, int *row, int *pipe_count)
{
    int i;
    *row = 0;
    *pipe_count = 0;

    // Count spaces and pipe symbols
    for (i = 0; input_string[i] != '\0'; i++)
    {
        if (input_string[i] == '|')
            (*pipe_count)++;

        if (input_string[i] == ' ')
            (*row)++;
    }

    // Increment row count for last command
    (*row)++;
}

// Function to print list of stopped jobs
void print_jobs(stopped_jobs *head)
{
    int i = 1;

    // Traverse and display each stopped job
    while (head)
    {
        printf(ANSI_COLOR_YELLOW "[%d]   Stopped                 %s\n"
               ANSI_COLOR_RESET, i++, head->sleep_command);
        head = head->next;
    }
}

// Function to insert a stopped job at the beginning of jobs list
void insert_jobs_at_start(stopped_jobs **head)
{
    // Allocate memory for new job node
    stopped_jobs *newNode = (stopped_jobs *)malloc(sizeof(stopped_jobs));

    if (newNode == NULL)
        return;

    // Store process details
    newNode->pid = pid;
    strcpy(newNode->sleep_command, input_string);
    newNode->next = NULL;

    // Insert node at the start of the list
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        newNode->next = *head;
        *head = newNode;
    }
}

// Function to delete the most recent job from jobs list
void delete_jobs(stopped_jobs **head)
{
    stopped_jobs *temp = *head;
    *head = temp->next;
    free(temp);
}
