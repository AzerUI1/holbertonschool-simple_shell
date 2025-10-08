#include "main.h"
#include <string.h>

char *_getenv(const char *name)
{
    int i, j;
    size_t len;

    if (!name || !environ)
        return (NULL);

    len = strlen(name);
    for (i = 0; environ[i]; i++)
    {
        j = 0;
        while (j < (int)len && environ[i][j] == name[j])
            j++;
        if (j == (int)len && environ[i][j] == '=')
            return (&environ[i][len + 1]);
    }
    return (NULL);
}
