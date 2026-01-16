# Pipex

## 📌 Overview

**Pipex** is a project from the 42 curriculum whose goal is to deeply understand how **Unix pipes** work, along with the correct usage of **fork**, **dup2**, **execve**, and **file descriptors**.

The program reproduces the behavior of the following shell command:

```bash
< infile cmd1 | cmd2 > outfile
```

In other words, the output of `cmd1` is passed as input to `cmd2` through a pipe, with proper input/output redirection.

---

## 🎯 Objective

Implement a C program that:

- Creates child processes using `fork`
- Connects processes using `pipe`
- Redirects `stdin` and `stdout` with `dup2`
- Executes commands using `execve`
- Correctly manages file descriptors

All without relying on high-level shell utilities.

---

## ⚙️ Usage

### Syntax

```bash
./pipex infile "cmd1 arg1 arg2" "cmd2 arg1 arg2" outfile
```

### Example

```bash
./pipex infile "cat" "wc -l" outfile
```

Equivalent to:

```bash
< infile cat | wc -l > outfile
```

---

## 🧠 Program Flow

The program follows this logic:

1. The parent process creates a pipe
2. The first child:
   - Redirects `stdin` to the infile
   - Redirects `stdout` to the pipe write end
   - Executes `cmd1`
3. The second child:
   - Redirects `stdin` to the pipe read end
   - Redirects `stdout` to the outfile
   - Executes `cmd2`
4. The parent process:
   - Closes unused file descriptors
   - Waits for both children using `waitpid`

---

## 🧩 Concepts Covered

- `fork()` — process creation
- `pipe()` — inter-process communication
- `dup2()` — input/output redirection
- `execve()` — program execution
- File descriptor management
- Manual PATH resolution using environment variables

---

## 🧪 Error Handling

The program handles cases such as:

- Invalid number of arguments
- File opening errors
- Pipe or fork failures
- Invalid or non-existent commands
- Proper cleanup of resources

---

## 📂 Project Structure

```
.
├── include/
│   └── pipex.h
├── libft/
├── src/
│   ├── pipex.c
│   └── utils.c
├── Makefile
└── README.md
```

---

## 🛠️ Compilation

```bash
make
```

Clean object files:

```bash
make clean
```

Clean everything:

```bash
make fclean
```

---

## 🚀 Execution

```bash
./pipex infile "cmd1" "cmd2" outfile
```

---

## 📚 Key Learnings

This project was essential to consolidate knowledge about:

- Process creation and synchronization
- Unix pipes and IPC
- File descriptor lifecycle management
- Debugging concurrent programs
- Understanding how the shell works internally

---

## ✨ Author

👩‍💻 **BabiDrummond**  - Project developed as part of the **42** curriculum.
