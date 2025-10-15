#include "main.h"
#include <stdlib.h>

/**
 * count_words - counts words in a string separated by spaces/tabs/newlines
 * @s: input string
 * Return: number of words
 */
int count_words(char *s)
{
    int count = 0, i = 0;
    while (s[i])
    {
        while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
            i++;
        if (s[i])
        {
            count++;
            while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '\n')
                i++;
        }
    }
    return count;
}

/**
 * fill_args - splits a line into tokens (no strtok, no memcpy)
 * @line: user input
 * Return: NULL-terminated array of strings
 */
char **fill_args(char *line)
{
    char **args;
    int i = 0, j, k, start, end, word_count;

    if (!line)
        return NULL;

    word_count = count_words(line);
    args = (char **)malloc((word_count + 1) * sizeof(char *));
    if (!args)
        return NULL;

    k = 0;
    while (line[i])
    {
        while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
            i++;
        if (line[i] == '\0')
            break;

        start = i;
        while (line[i] && line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
            i++;
        end = i;

        args[k] = (char *)malloc(end - start + 1);
        if (!args[k])
            return NULL;

        /* Manual copy instead of memcpy */
        for (j = 0; j < end - start; j++)
            args[k][j] = line[start + j];
        args[k][j] = '\0';

        k++;
    }
    args[k] = NULL;
    return args;
}

