#include "main.h"

int shell_error(void)
{
    perror("./hsh - not found");
    return (EXIT_FAILURE);
}
