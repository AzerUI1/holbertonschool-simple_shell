#include "main.h"

int main(void)
{
    char *line;
    size_t len;
    ssize_t nread;
    char **args;
    int i;

    line = NULL;
    len = 0;

    while (1)
    {
        printf(PROMPT);
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
            break;

        line[nread - 1] = '\0'; /* Remove newline */
        line = trim_whitespace(line); /* Trim spaces */

        if (line[0] == '\0')
            continue;

        args = split_line(line, " \t\n");
        if (!args)
            continue;

        execute_command(args); /* Execute command */

        /* Free memory allocated by split_line */
        for (i = 0; args[i] != NULL; i++)
            free(args[i]);
        free(args);
    }

    free(line);
    return 0;
}

