# 🐚 MiniShell — Custom Command Line Interpreter in C

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)
![Build](https://img.shields.io/badge/Build-GCC-orange.svg)

MiniShell is a **Unix-like command line interpreter** implemented entirely in **C**, focusing on **Linux systems programming fundamentals** such as process management, signal handling, inter-process communication, and job control.

This project demonstrates a deep, low-level understanding of how modern shells operate internally using POSIX system calls.

---

## ❓ Why This Project?

Building a shell from scratch is one of the most **challenging and conceptually rich problems in Linux systems programming**.

This project was undertaken to:
- Gain hands-on experience with Linux process lifecycle management
- Understand signal-driven control flow in interactive applications
- Implement pipelines and job control without relying on existing shells
- Design modular, maintainable system-level C code

MiniShell is a **true shell implementation**, not a wrapper around `/bin/sh`.

---

## 🚀 Key Technical Achievements

### 🔹 Process Management
- Creation of child processes using `fork()`
- Execution of external binaries via `execvp()`
- Parent–child synchronization using `waitpid()` with `WUNTRACED`
- Accurate tracking of command exit status (`$?`)

### 🔹 Signal Handling
- Graceful handling of `SIGINT` (Ctrl+C) without terminating the shell
- Support for `SIGTSTP` (Ctrl+Z) to stop foreground jobs
- Proper restoration of default signal behavior in child processes
- Handling of `SIGCHLD` for background and stopped process management

### 🔹 Inter-Process Communication (IPC)
- Support for **multi-stage pipelines** (e.g., `ls | grep txt | wc -l`)
- Pipe creation using `pipe()`
- Standard input/output redirection using `dup2()`
- Each pipeline stage executes in a dedicated process

### 🔹 Job Control
- Custom **linked-list–based job table**
- Implementation of:
  - `jobs` — list stopped jobs
  - `fg` — resume job in foreground
  - `bg` — resume job in background
- Behavior closely aligned with standard Unix shells

### 🔹 Shell Customization
- Dynamic prompt modification using:
  ```bash
  PS1=MyShell
  ```
- Prompt updates immediately without restarting the shell

---

## 🔬 Technical Deep Dive: Pipe Execution Logic

Pipeline execution in MiniShell follows a structured, process-oriented approach:

1. The input command is tokenized and analyzed to determine pipe count
2. Pipes are created dynamically based on the number of pipeline stages
3. For each command in the pipeline:
   - A new process is created using `fork()`
   - `dup2()` redirects:
     - Standard output to the write end of the pipe
     - Standard input to the read end of the previous pipe
4. Parent processes wait for child execution where required
5. Data flows directly between processes via pipes without temporary files

This design mirrors the internal behavior of production-grade shells such as `bash`.

---

## 🛠 Installation & Usage

### Compile
```bash
gcc *.c -o minishell
```

### Run
```bash
./minishell
```

---

## 📂 Code Architecture

| File | Description |
|----|----|
| `main.c` | Program entry point and initialization |
| `scan_input.c` | Shell loop, input handling, and signal management |
| `internal_cmd.c` | Built-in command implementations |
| `external_cmd.c` | External command execution and pipe handling |
| `commands.c` | Command parsing, validation, and helpers |
| `header.h` | Global declarations, macros, and data structures |
| `ext_cmd.txt` | Whitelist of supported external commands |

---

## ⚠️ Limitations

- No advanced shell scripting support
- No command history or tab completion
- No file redirection (`>`, `<`)

These limitations are intentional to maintain focus on **core shell internals**.

---

## 🎯 Learning Outcomes

- Strong understanding of Linux process lifecycle
- Practical application of POSIX system calls
- Signal-safe interactive program design
- Pipeline-based inter-process communication
- Job control using data structures
- Modular and maintainable C programming

---

## 👤 Author

**Omkar Ashok Sawant**  
Pursuing *Emertxe Certified Embedded Professional (ECEP)*  
Linux Systems & Embedded Software Enthusiast  

---

## 📌 Note

This project is developed for **educational and skill-building purposes**, demonstrating internal shell mechanics at the operating system level.
