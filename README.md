# minishell — As beautiful as a shell

*This project has been created as part of the 42 curriculum by hchouai, zchtaibi.*

---

## Description

**minishell** is a minimal Unix shell built in C, implemented as part of the 42 Network curriculum. The goal is to deeply understand how shells work under the hood — from lexing and parsing raw input, to forking processes, managing file descriptors, and handling signals correctly.

The result is a functional Bash-like shell that handles real commands, pipes, redirections, environment variables, and built-in commands — written entirely from scratch with no shortcuts.

### Features

- **Prompt** — displays a prompt and waits for user input
- **Command history** — navigate previous commands with arrow keys
- **Command execution** — resolves executables via `PATH`, relative, or absolute paths
- **Quoting**
  - Single quotes `'` — no interpretation of metacharacters
  - Double quotes `"` — interprets `$` only
- **Redirections**
  - `<` input redirect
  - `>` output redirect
  - `>>` output append
  - `<<` heredoc — reads until delimiter
- **Pipes** `|` — chains commands, connecting stdout to stdin
- **Environment variables** — `$VAR` expands to its value
- **Exit status** — `$?` expands to last command's exit code
- **Signals**
  - `ctrl-C` — new prompt on new line
  - `ctrl-D` — exit shell
  - `ctrl-\` — no action
- **Built-ins** — `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

---

## Technical Choices

- **Language:** C (compiled with `-Wall -Wextra -Werror`)
- **Norm:** compliant with 42 Norm
- **Memory:** all heap allocations are freed — zero leaks from project code (`readline` leaks are exempt per subject)
- **Signal handling:** single global variable stores only the signal number — no data structure access from signal handlers
- **Architecture:** lexer → parser → AST → executor pipeline

---

## Instructions

### Requirements

- Linux or macOS with readline installed
- `cc` compiler
- `readline` library (`libreadline-dev` on Ubuntu)

```bash
# Ubuntu/Debian
sudo apt-get install libreadline-dev
```

### Compilation

```bash
make
```

### Running

```bash
./minishell
```

You'll get an interactive prompt. Type any command just like in bash:

```bash
minishell$ ls -la | grep .c | wc -l
minishell$ export USER=houda
minishell$ echo $USER
houda
minishell$ cat Makefile | grep NAME
NAME = minishell
minishell$ exit
```

### Makefile rules

```bash
make        # build
make clean  # remove object files
make fclean # remove objects + binary
make re     # fclean + build
```

---

## Live Demo

👉 [Open interactive demo →](https://zakariachtaibi.github.io/Minishell/)

---

## Resources

### Processes, Pipes & Shell Internals

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/)
- [Linux man pages — fork, execve, pipe, dup2](https://man7.org/linux/man-pages/)
- [Writing a Unix Shell — Part I (Akash Pai)](https://indradhanush.github.io/blog/writing-a-unix-shell-part-1/)
- [The TTY demystified (Linus Åkesson)](https://www.linusakesson.net/programming/tty/)
- [readline library documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)

### AI Usage

AI tools were used during this project for:

- Clarifying edge cases in bash quoting behavior and `$?` expansion
- Reviewing signal handling patterns and the single global variable constraint
- Understanding heredoc (`<<`) implementation approach
- Helping draft and review this README

All suggestions were reviewed, tested, and validated by both team members before use. No code was integrated without full understanding.
