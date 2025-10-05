#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

/* функция поиска команды в PATH */
char *find_command(char *cmd)
{
    char *path = getenv("PATH"); // получить PATH
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    static char full_path[256];

    while (dir)
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) // если исполнимая
        {
            free(path_copy);
            return full_path;
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL; // не найдено
}

int main(void)
{
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    char *token;
    pid_t pid;
    int status;

    while (1)
    {
        printf(":) "); // prompt
        if (fgets(input, MAX_INPUT, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0'; // убрать \n

        /* разделение строки на аргументы */
        int i = 0;
        token = strtok(input, " ");
        while (token && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] == NULL)
            continue;

        /* выход */
        if (strcmp(args[0], "exit") == 0)
            break;

        /* найти путь к команде */
        char *command = args[0];
        if (access(command, X_OK) != 0)
        {
            command = find_command(args[0]);
            if (command == NULL)
            {
                fprintf(stderr, "%s: command not found\n", args[0]);
                continue; // не найдено → не форкаем
            }
        }

        pid = fork();
        if (pid == 0)
        {
            execve(command, args, environ);
            perror("execve");
            exit(1);
        }
        else if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }
        else
        {
            perror("fork");
        }
    }

    return 0;
}

