#include "main.h"

/**
 * command_exists - checks if a command exists in PATH or is absolute
 * @cmd: command string
 * Return: malloc'd full path if exists, else NULL
 */
char *command_exists(char *cmd)
{
    char *path_env, *path_copy, *token, *full_path;
    struct stat st;

    if (!cmd)
        return NULL;

    if (strchr(cmd, '/')) /* Absolute or relative path */
    {
        if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
            return strdup(cmd);
        return NULL;
    }

    path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    token = strtok(path_copy, ":");
    while (token)
    {
        full_path = malloc(strlen(token) + strlen(cmd) + 2);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        sprintf(full_path, "%s/%s", token, cmd);

        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

/**
 * execute_command - executes a single command line
 * @line: command line string
 */
void execute_command(char *line)
{
    pid_t pid;
    int status;
    char *argv[64];
    int i = 0;
    char *token;
    char *full_cmd;

    /* Split command line into arguments */
    token = strtok(line, " \t\n");
    while (token && i < 63)
    {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;

    if (argv[0] == NULL) /* Empty command */
        return;

    full_cmd = command_exists(argv[0]);
    if (!full_cmd)
    {
        fprintf(stderr, "./shell: %s: not found\n", argv[0]);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(full_cmd);
        return;
    }

    if (pid == 0) /* Child process */
    {
        if (execve(full_cmd, argv, environ) == -1)
        {
            perror("./shell");
            free(full_cmd);
            exit(EXIT_FAILURE);
        }
    }
    else /* Parent process */
        wait(&status);

    free(full_cmd);
}

/**
 * main - simple shell main loop
 * Return: 0
 */
int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1)
    {
        printf("$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1) /* EOF or error */
        {
            putchar('\n');
            break;
        }

        /* Remove trailing newline */
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        execute_command(line);
    }

    free(line);
    return 0;
}

