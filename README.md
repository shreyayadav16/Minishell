# MiniShell — Custom Linux Shell in C
## Description

MiniShell is a Unix-like command line interpreter developed in C that mimics basic Linux shell functionality using Linux system programming concepts. It supports internal and external command execution, piping, signal handling, and job control using POSIX system calls.

## Features
Built-in commands (cd, pwd, echo, jobs, fg, bg)
External command execution using fork() and execvp()
Multi-stage pipe support using pipe() and dup2()
Signal handling for Ctrl+C and Ctrl+Z
Background and foreground job control using linked lists
Dynamic shell prompt customization (PS1)

## Technologies Used
Advanced C
Linux System Programming
POSIX System Calls
Process Management (fork, execvp, waitpid)
Signal Handling
Inter Process Communication (Pipes)
Linked List Data Structure

## Compilation & Execution
### Compile
gcc *.c -o minishell
### Run
./minishell
