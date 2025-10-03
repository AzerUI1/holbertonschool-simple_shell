#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

/**
 * main - Entry point for simple shell 0.1
 * Return: Always 0
 */
int main(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;
    pid_t pid;
    int status;

    while (1)
    {
        write(1, "#cisfun$ ", 9);
        nread = getline(&line, &bufsize, stdin);
        if (nread == -1)
        {
            write(1, "\n", 1);
            break;
        }

        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            continue;
        }

        if (pid == 0)
        {
            execve(line, NULL, environ);
            perror("./shell: No such file or directory");
            exit(1);
        }
        else
            wait(&status);
    }

    free(line);
    return (0);
}


