#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "main.h"

/**
 * execute_command - forks and executes a command
 * @line: command to execute
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char *argv[2];

	argv[0] = line;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}

	if (pid == 0)
	{
		if (execve(line, argv, environ) == -1)
		{
			perror("./shell");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(&status);
}

/**
 * main - simple UNIX command line interpreter
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *command;

	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf("#cisfun$ ");
			fflush(stdout);
		}

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		command = strtok(line, "\n");
		while (command != NULL)
		{
			/* Trim leading spaces */
			while (*command == ' ' || *command == '\t')
				command++;

			/* Trim trailing spaces */
			char *end = command + strlen(command) - 1;
			while (end > command && (*end == ' ' || *end == '\t'))
			{
				*end = '\0';
				end--;
			}

			if (*command != '\0')
				execute_command(command);

			command = strtok(NULL, "\n");
		}
	}

	free(line);
	return (0);
}

