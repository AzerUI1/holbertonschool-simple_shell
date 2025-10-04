#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "main.h"

/**
 * command_exists - checks if command exists in PATH or as absolute path
 * @cmd: command to check
 *
 * Return: full path if exists (malloc), NULL otherwise
 */
char *command_exists(char *cmd)
{
	char *path_env, *path, *full_path;
	char *token;
	struct stat st;

	if (cmd == NULL)
		return (NULL);

	if (strchr(cmd, '/') != NULL) /* absolute or relative path */
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return strdup(cmd);
		return NULL;
	}

	path_env = getenv("PATH");
	if (!path_env)
		return NULL;

	path = strdup(path_env);
	if (!path)
		return NULL;

	token = strtok(path, ":");
	while (token != NULL)
	{
		full_path = malloc(strlen(token) + strlen(cmd) + 2);
		if (!full_path)
		{
			free(path);
			return NULL;
		}
		sprintf(full_path, "%s/%s", token, cmd);
		if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			free(path);
			return full_path;
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path);
	return NULL;
}

/**
 * execute_command - forks and executes a command with arguments
 * @line: command to execute
 */
void execute_command(char *line)
{
	pid_t pid;
	int status;
	char *argv[20];
	int i;
	char *token;
	char *full_cmd;

	i = 0;
	token = strtok(line, " \t");
	while (token != NULL && i < 19)
	{
		argv[i++] = token;
		token = strtok(NULL, " \t");
	}
	argv[i] = NULL;

	full_cmd = command_exists(argv[0]);
	if (!full_cmd)
	{
		fprintf(stderr, "./shell: %s: not found\n", argv[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(full_cmd);
		return;
	}

	if (pid == 0)
	{
		if (execve(full_cmd, argv, environ) == -1)
		{
			perror("./shell");
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(&status);

	free(full_cmd);
}

/**
 * main - simple UNIX command line interpreter with PATH support
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *command;
	char *end;

	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			printf(":) ");
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
			while (*command == ' ' || *command == '\t')
				command++;

			end = command + strlen(command) - 1;
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

