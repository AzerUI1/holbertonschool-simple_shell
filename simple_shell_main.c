#include "main.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

/**
 * main - Entry point of the simple shell.
 * Return: 0 on success, >0 on failure.
 */
int main(void)
{
    char buffer[1024];
    char **argv;
    int ret = 0;
    ssize_t nread;
    int interactive;
    int i;

    /* Ignore Ctrl+C */
    signal(SIGINT, SIG_IGN);
    interactive = isatty(STDIN_FILENO);

    while (1)
    {
        if (interactive)
            write(1, "#simple_shell$ ", 14);

        nread = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
        if (nread <= 0)
        {
            if (interactive)
                write(1, "\n", 1);
            break; /* Ctrl+D or EOF */
        }

        buffer[nread] = '\0';
        if (buffer[nread - 1] == '\n')
            buffer[nread - 1] = '\0';

        if (buffer[0] == '\0')
            continue;

        /* Handle 'env' command */
        if (buffer[0] == 'e' && buffer[1] == 'n' &&
            buffer[2] == 'v' && buffer[3] == '\0')
        {
            print_env();
            continue;
        }

        /* Handle 'exit' command */
        if (buffer[0] == 'e' && buffer[1] == 'x' &&
            buffer[2] == 'i' && buffer[3] == 't' && buffer[4] == '\0')
            break;

        /* Split input into arguments using custom function */
        argv = fill_args(buffer);
        if (!argv)
            continue;

        /* Execute command */
        if (argv[0])
            ret = execute_command(argv);

        /* Free memory */
        i = 0;
        while (argv[i])
        {
            free(argv[i]);
            i++;
        }
        free(argv);
    }

    return ret;
}

