/*
 * File: simple_shell.c
 * Auth: ChatGPT for Azer
 * Project: holbertonschool-simple_shell (single-file reference)
 * Description: A compact, readable POSIX shell that demonstrates
 *              parsing, forking, and executing commands. Written
 *              to follow Betty-style conventions (comments, spacing,
 *              simple functions). This single-file example is meant
 *              to be used as a reference for style and structure.
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

/*
 * Function: print_prompt
 * ----------------------
 * Print the interactive prompt to stdout.
 */
void print_prompt(void)
{
    if (isatty(STDIN_FILENO))
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
}

/*
 * Function: read_line
 * -------------------
 * Read a line of input from stdin using getline(). Returns the
 * allocated buffer which must be freed by the caller. On EOF
 * returns NULL.
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

/*
 * Function: split_line
 * --------------------
 * Split a line into tokens (arguments). Returns a NULL-terminated
 * array of pointers. Caller must free the array and tokens buffer.
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

/*
 * Function: launch
 * ----------------
 * Fork and execute the program specified by args. Parent waits for
 * child to finish. Returns 1 to continue the shell, 0 to exit.
 */
int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        /* Child process */
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        /* Error forking */
        perror("fork");
    }
    else
    {
        /* Parent process waits for child */
        do
        {
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

/*
 * Function: execute
 * -----------------
 * Execute built-in commands or launch external programs. Returns 1
 * to keep the shell running, or 0 to request exit.
 */
int execute(char **args)
{
    int i;

    if (args[0] == NULL)
        return (1);

    /* Built-in: exit */
    if (strcmp(args[0], "exit") == 0)
        return (0);

    /* Built-in: env -- print environment variables using exec of env */
    if (strcmp(args[0], "env") == 0)
    {
        /* Replace current args to run /usr/bin/env if available */
        return (launch(args));
    }

    /* Not a built-in command: launch external */
    i = launch(args);
    return (i);
}

/*
 * Function: free_args
 * -------------------
 * Free the memory allocated for the tokens array. Note: tokens
 * point inside the original line buffer, so only the tokens array
 * needs to be freed here.
 */
void free_args(char **args)
{
    free(args);
}

/*
 * Function: main
 * --------------
 * Entry point: loop that reads input, parses it, and executes it.
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
            break; /* EOF or read error -> exit shell */

        args = split_line(line);
        status = execute(args);

        free_args(args);
        free(line);

        if (status == 0)
            break; /* user typed exit */
    }

    return (EXIT_SUCCESS);
}

