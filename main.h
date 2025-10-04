#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

extern char **environ;

char *find_path(char *command);
void execute_command(char **args);
char **parse_line(char *line);

#endif /* MAIN_H */

