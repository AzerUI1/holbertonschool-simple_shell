#include "main.h"

/**
 * parse_line - splits input line into tokens
 * @line: input string
 * Return: NULL-terminated array of args
 */
char **parse_line(char *line)
{
	char *token;
	char **tokens;
	int bufsize = 64, i = 0;

	tokens = malloc(sizeof(char *) * bufsize);
	if (!tokens)
		return (NULL);

	token = strtok(line, " \t\r\n");
	while (token)
	{
		tokens[i++] = token;
		if (i >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, sizeof(char *) * bufsize);
			if (!tokens)
				return (NULL);
		}
		token = strtok(NULL, " \t\r\n");
	}
	tokens[i] = NULL;
	return (tokens);
}

