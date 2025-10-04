#include "main.h"

/**
 * find_path - search for a command in PATH
 * @command: command to find
 * Return: full path if found, else NULL
 */
char *find_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	size_t len;

	if (command == NULL)
		return (NULL);
	if (strchr(command, '/'))
		return (strdup(command));

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	dir = strtok(path_copy, ":");
	while (dir)
	{
		len = strlen(dir) + strlen(command) + 2;
		full_path = malloc(len);
		if (!full_path)
		{
			free(path_copy);
			return (NULL);
		}
		snprintf(full_path, len, "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

