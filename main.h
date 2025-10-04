#ifndef MAIN_H
#define MAIN_H

extern char **environ;

void execute_command(char *line);
char *command_exists(char *cmd);

#endif /* MAIN_H */

