#ifndef SIMPLE_SHELL
#define SIMPLE_SHELL

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Global environment variable */
extern char **environ;

/* Command execution */
int file_exist(char *file);
int find_cmd_path(char *cmd, char *work_buffer);
int execute_command(char *argv[]);

/* Arguments */
char **fill_args(char *input_buffer);

/* Environment */
char *_getenv(const char *name);
void print_env(void);

/* Errors */
int shell_error(void);

#endif /* SIMPLE_SHELL */
