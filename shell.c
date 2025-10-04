#include "main.h"

/**
 * main - simple shell main loop
 * Return: 0 on success
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;

	while (1)
	{
		printf("($) ");
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		args = parse_line(line);
		if (args && args[0])
			execute_command(args);

		free(args);
	}
	free(line);
	return (0);
}

