/*
 * File: simple_shell.c
 * Auth: ChatGPT for Azer
 * Project: holbertonschool-simple_shell
 * Description: A compact POSIX shell that demonstrates parsing,
 *              forking, and executing commands. Written to pass
 *              Betty-style documentation and style checks.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#define PROMPT "$ "

/**
 * print_prompt - Prints the interactive prompt to stdout
 *
 * Return: Nothing
 */
void print_prompt(void)
{
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

/**
 * read_line - Reads a line of input from stdin
 *
 * Return: Pointer to allocated buffer containing the line.
 *         NULL on EOF or error. Caller must free buffer.
 */
char *read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread;

    nread = getline(&line, &bufsize, stdin);
    if (nread == -1)
    {
        free(line);
        return (NULL);
    }
    return (line);
}

/**
 * split_line - Splits a line into tokens (arguments)
 * @line: Input string to split
 *
 * Return: Null-terminated array of strings (tokens).
 *         Caller must free the array but not the tokens themselves.
 */
char **split_line(char *line)
{
    int bufsize = TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL)
    {
        tokens[position++] = token;

        if (position >= bufsize)
        {
            bufsize += TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOK_DELIM);
    }
    tokens[position] = NULL;
    return (tokens);
}

/**
 * launch - Forks and executes a program
 * @args: Null-terminated array of arguments
 *
 * Return: 1 to continue running, 0 to exit
 */
int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("fork");
    }
    else
    {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1)
            {
                perror("waitpid");
                break;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return (1);
}

/**
 * execute - Executes built-in or external commands
 * @args: Null-terminated array of arguments
 *
 * Return: 1 to keep shell running, 0 to exit
 */
int execute(char **args)
{
    if (args[0] == NULL)
        return (1);

    if (strcmp(args[0], "exit") == 0)
        return (0);

    if (strcmp(args[0], "env") == 0)
        return (launch(args));

    return (launch(args));
}

/**
 * free_args - Frees the memory allocated for arguments array
 * @args: Null-terminated array of arguments
 *
 * Return: Nothing
 */
void free_args(char **args)
{
    free(args);
}

/**
 * main - Entry point of the shell
 *
 * Return: Always EXIT_SUCCESS
 */
int main(void)
{
    char *line;
    char **args;
    int status = 1;

    while (1)
    {
        print_prompt();
        line = read_line();
        if (line == NULL)
            break;

        args = split_line(line);
        status = execute(args);

        free_args(args);
        free(line);

        if (status == 0)
            break;
    }

    return (EXIT_SUCCESS);
}
