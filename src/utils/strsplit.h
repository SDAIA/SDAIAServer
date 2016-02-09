#ifndef STR_SPLIT_H_

#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STR_SPLIT_H_

static char* str_split(char *a_str, const char a_delim)
{
    char **result = 0;
    size_t count = 0;
    char *tmp = a_str;
    char *last = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while(*tmp)
    {
        if(a_delim == *tmp)
        {
            count++;
            last = tmp;
        }
        tmp++;
    }

    count += last < (a_str + strlen(a_str) -1);
    count++;

    result = kore_malloc(sizeof(char*) * count);

    if(result)
    {
        size_t idx = 0;
        char *token = strsep(a_str, delim);

        while(token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strsep(0, delim);
        }
        assert(idx == count-1);
        *(result + idx) = 0;
    }

    return result;
}

#endif // STR_SPLIT_H_
