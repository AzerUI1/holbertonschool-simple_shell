#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ; /* For PATH access */

char *find_command(char *cmd)
{
    int i;
    char *path_env, *path_copy, *token, *full_path;
    size_t cmd_len, path_len;

    cmd_len = strlen(cmd);
    path_env = NULL;

    /* Search for PATH in environ manually */
    for (i = 0; environ[i] != NULL; i++)
    {
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            path_env = environ[i] + 5; /* Skip "PATH=" */
            break;
        }
    }

    if (path_env == NULL)
        return NULL;

    path_copy = strdup(path_env);
    if (path_copy == NULL)
        return NULL;

    token = strtok(path_copy, ":");
    while (token != NULL)
    {
        path_len = strlen(token);
        full_path = malloc(path_len + 1 + cmd_len + 1); /* path + '/' + cmd + '\0' */
        if (full_path == NULL)
        {
            free(path_copy);
            return NULL;
        }
        strcpy(full_path, token);
        full_path[path_len] = '/';
        strcpy(full_path + path_len + 1, cmd);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path; /* Found executable */
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL; /* Not found */
}

