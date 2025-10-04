#include "main.h"

/* Execute a command */
int execute_command(char **args)
{
    pid_t pid;
    int status;
    char *cmd_path;

    if (!args[0])
        return 1;

    /* Absolute or relative path */
    if (strchr(args[0], '/'))
        cmd_path = args[0];
    else
        cmd_path = find_command(args[0]);

    if (!cmd_path)
    {
        fprintf(stderr, "%s: command not found\n", args[0]);
        return 127;
    }

    pid = fork();
    if (pid == 0)
    {
        /* Child */
        execve(cmd_path, args, NULL);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        /* Parent */
        waitpid(pid, &status, 0);
    }

    /* If we malloced cmd_path in find_command, free it */
    if (!strchr(args[0], '/'))
        free(cmd_path);

    return status;
}

