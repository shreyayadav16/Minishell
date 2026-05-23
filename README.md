# 🐚 MiniShell — Linux Command Line Interpreter in C

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)
![Compiler](https://img.shields.io/badge/Compiler-GCC-orange.svg)

MiniShell is a custom command-line interpreter developed in C to explore core Linux system programming concepts such as process handling, signal management, pipes, and job control.

The project focuses on understanding how Unix-based shells work internally using POSIX system calls and process-oriented programming.

---

## 🚀 Features

### Process Execution

* Created child processes using `fork()`
* Executed commands using `execvp()`
* Managed parent-child synchronization with `waitpid()`
* Supported foreground and background process execution

### Signal Handling

* Handled `Ctrl+C` using `SIGINT`
* Managed stopped processes with `SIGTSTP`
* Restored default signal behavior for child processes
* Used `SIGCHLD` for process state management

### Pipe & IPC Support

* Implemented multi-command pipelines
* Used `pipe()` and `dup2()` for inter-process communication
* Enabled data transfer between multiple commands

### Job Control

* Added support for:

  * `jobs`
  * `fg`
  * `bg`
* Maintained job information using linked-list structures

### Shell Customization

* Dynamic shell prompt support using:

```bash
PS1=MiniShell
```

---

## 🛠 Technologies Used

* C Programming
* Linux System Calls
* POSIX APIs
* GCC Compiler

---

## 📂 Project Structure

| File             | Purpose                        |
| ---------------- | ------------------------------ |
| `main.c`         | Program initialization         |
| `scan_input.c`   | Input handling and shell loop  |
| `internal_cmd.c` | Built-in command handling      |
| `external_cmd.c` | External command execution     |
| `commands.c`     | Command parsing and validation |
| `header.h`       | Global declarations and macros |

---

## 🎯 Learning Outcomes

* Linux process management
* Signal handling in interactive programs
* Inter-process communication using pipes
* Job control implementation
* Modular programming in C
* Understanding Unix shell internals

---

## 👤 Author
* Shreya Yadav
*  Embedded Systems Trainee
* Linux & Embedded Systems Enthusiast

---

## 📌 Note

This project was developed for learning and understanding Linux shell internals and low-level systems programming concepts.
