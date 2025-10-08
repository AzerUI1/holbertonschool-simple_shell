#include "main.h"
#include <string.h> 
/**
 * fill_args - split input line into arguments
 * @line: input line
 *
 * Return: NULL-terminated array of strings
 */
char **fill_args(char *line)
{
    char **args;
    char *arg;
    int i = 0, j, start, len = 0;

    if (!line)
        return (NULL);

    args = malloc(sizeof(char *) * 64); /* supports up to 63 args */
    if (!args)
        return (NULL);

    while (line[len])
    {
        while (line[len] == ' ' || line[len] == '\t' || line[len] == '\n')
            len++;
        if (!line[len])
            break;

        start = len;
        while (line[len] && line[len] != ' ' && line[len] != '\t' && line[len] != '\n')
            len++;

        arg = malloc(len - start + 1);
        if (!arg)
        {
            for (j = 0; j < i; j++)
                free(args[j]);
            free(args);
            return (NULL);
        }

        memcpy(arg, &line[start], len - start);
        arg[len - start] = '\0';
        args[i++] = arg;
    }

    args[i] = NULL;
    return (args);
}
