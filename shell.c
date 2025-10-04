#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

/**
 * execute_command - forks and executes a command
 * @line: command to execute
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char *argv[] = {line, NULL};

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

	while (1)
	{
		printf("#cisfun$ ");
		fflush(stdout);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			printf("\n");
			break;
		}

		line[strcspn(line, "\n")] = '\0';
		if (line[0] == '\0')
			continue;

		execute_command(line);
	}

	free(line);
	return (0);
}

