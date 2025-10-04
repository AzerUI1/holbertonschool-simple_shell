#include "main.h"

/**
 * execute_command - fork and execute a command
 * @args: argument vector
 */
void execute_command(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	cmd_path = find_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "./shell: %s: not found\n", args[0]);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
			perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	else
		perror("fork");

	free(cmd_path);
}

