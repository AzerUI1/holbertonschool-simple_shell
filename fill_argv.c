#include "main.h"

/**
 * fill_args - Tokenizes a string into an array of arguments.
 * @input_buffer: The string to tokenize.
 *
 * Return: NULL-terminated array of argument strings.
 */
char **fill_args(char *input_buffer)
{
    char *token;
    char **argv;
    int count = 0, i = 0;
    char *buffer_copy;

    /* Ignore leading spaces and tabs */
    while (*input_buffer == ' ' || *input_buffer == '\t')
        input_buffer++;

    /* Make a copy of input_buffer for counting tokens */
    buffer_copy = strdup(input_buffer);
    if (!buffer_copy)
        return (NULL);

    /* Count number of tokens */
    token = strtok(buffer_copy, " \t");
    while (token)
    {
        count++;
        token = strtok(NULL, " \t");
    }
    free(buffer_copy);

    /* Allocate exact memory for argv */
    argv = malloc((count + 1) * sizeof(char *));
    if (!argv)
        return (NULL);

    /* Fill argv */
    i = 0;
    token = strtok(input_buffer, " \t");
    while (token)
    {
        argv[i++] = token;
        token = strtok(NULL, " \t");
    }
    argv[i] = NULL;

    return (argv);
}
