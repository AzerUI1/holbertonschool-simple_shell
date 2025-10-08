#include "main.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * _strlen - Returns the length of a string
 * @s: The string
 * Return: length
 */
int _strlen(const char *s)
{
    int len = 0;
    if (!s)
        return 0;

    while (s[len] != '\0')
        len++;

    return len;
}

/**
 * _strcpy - Copies a string from src to dest
 * @dest: Destination
 * @src: Source
 * Return: dest
 */
char *_strcpy(char *dest, const char *src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}

/**
 * file_exist - Checks if a file exists.
 * @file: The name of the file to check.
 * Return: 0 if exists, 1 if not
 */
int file_exist(char *file)
{
    struct stat st;
    if (stat(file, &st) == 0)
        return 0;
    return 1;
}

/**
 * find_cmd_path - Finds full path of a command in PATH.
 * @cmd: Command name
 * @work_buffer: Buffer to store full path
 * Return: 0 if found, 1 if not
 */
int find_cmd_path(char *cmd, char *work_buffer)
{
    char *var_value_path;
    char path_copy[1024];
    int i, j, k, m;

    /* Absolute or relative path */
    if ((cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/')) && file_exist(cmd) == 0)
    {
        _strcpy(work_buffer, cmd);
        return 0;
    }

    var_value_path = _getenv("PATH");
    if (!var_value_path || _strlen(var_value_path) == 0)
        return 1;

    /* Copy PATH so we can tokenize */
    i = 0;
    while (var_value_path[i] != '\0' && i < 1023)
    {
        path_copy[i] = var_value_path[i];
        i++;
    }
    path_copy[i] = '\0';

    i = 0;
    j = 0;
    while (1)
    {
        if (path_copy[i] == ':' || path_copy[i] == '\0')
        {
            path_copy[i] = '\0';

            /* build full path: token + "/" + cmd */
            for (k = 0; path_copy[j + k] != '\0'; k++)
                work_buffer[k] = path_copy[j + k];
            work_buffer[k++] = '/';

            m = 0;
            while (cmd[m] != '\0')
            {
                work_buffer[k++] = cmd[m];
                m++;
            }
            work_buffer[k] = '\0';

            if (file_exist(work_buffer) == 0)
                return 0;

            if (path_copy[i] == '\0')
                break;
            j = i + 1;
        }
        i++;
    }

    return 1;
}

/**
 * execute_command - Executes a command
 * @argv: Argument array
 * Return: Exit status
 */
int execute_command(char *argv[])
{
    pid_t pid;
    int status;
    char work_buffer[1024];

    if (find_cmd_path(argv[0], work_buffer))
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
        return 127;
    }

    pid = fork();
    if (pid < 0)
        return shell_error();

    if (pid == 0)
    {
        execve(work_buffer, argv, environ);
        /* If execve fails */
        fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
        exit(127);
    }

    wait(&status);

    if (WIFEXITED(status))
        status = WEXITSTATUS(status);

    return status;
}
