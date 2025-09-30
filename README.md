# simple_shell


**Repository:** `holbertonschool-simple_shell`


## Project description


`simple_shell` is a simple UNIX command interpreter implemented in C for educational purposes. It accepts and executes commands entered by the user, supports basic features such as command execution with arguments, path resolution, environment variable handling, and built-in commands like `exit` and `env`.


The shell is created to demonstrate understanding of processes, system calls, parsing, and project structure.


## Features


- Read command lines from standard input (interactive) or from scripts (non-interactive).
- Parse commands and arguments (tokenization using whitespace separators).
- Resolve commands using the `PATH` environment variable.
- Execute binaries by creating child processes with `fork()` and `execve()`.
- Builtin commands: `exit`, `env` (implement at least these two).
- Proper exit statuses and basic error messages to `stderr`.
- Support for comments: lines beginning with `#` are ignored.


> _Optional extensions (not required but useful):_ support for `;` command separators, `&&`/`||` logic, variable expansion, and simple redirections.


## Requirements


- Language: C (ISO C99 compatible)
- Allowed functions and syscalls: `fork`, `execve`, `wait`, `waitpid`, `access`, `stat`, `open`, `close`, `read`, `write`, `malloc`, `free`, `perror`, `strtok`, `getenv`, and other standard library functions as permitted by the project rubric.
- The program should compile with `gcc -Wall -Werror -Wextra -pedantic` and run on a Linux environment.


## Installation


1. Clone the repository:


```bash
git clone https://github.com/YOUR_USERNAME/holbertonschool-simple_shell.git
cd holbertonschool-simple_shell
