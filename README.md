# Simple Shell Project

## Description
This is a simple UNIX command line interpreter written in C.  
It is part of the Holberton School curriculum and was developed as a team project by **Azer Aslanov** and **Suleyman Hajizada**.

The shell provides a basic interface to interact with the operating system by executing commands, handling input and output, and managing processes.

---

## Features
- Displays a prompt and waits for the user to type a command
- Executes commands using their full path (e.g. `/bin/ls`)
- Handles command lines with arguments
- Supports the `PATH` environment variable
- Built-in commands:
  - `exit` → exits the shell
  - `env` → prints the current environment
- Works in both interactive and non-interactive mode
- Error handling for invalid commands
- Compatible with **Ubuntu 20.04 LTS**

---

## Compilation
The shell should be compiled this way:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

