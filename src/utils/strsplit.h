#ifndef STR_SPLIT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STR_SPLIT_H_

static char** str_split(char *a_str, char a_delim)
{
    char **result = NULL;
    size_t count = 0;
    char *tmp = a_str;
    char *last = NULL;
    char str_delim[] = {a_delim, '\0'};

    //Count
    while(*tmp)
    {
        if(a_delim == *tmp)
        {
            count++;
            last = tmp;
        }
        tmp++;
    }

    count += last<(a_str + strlen(a_str) - 1);
    count++;

    result=malloc(sizeof(char*) * count);

    if(result)
    {
        size_t idx = 0;
        char *token = strsep(&a_str, str_delim);

        while(token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strsep(&a_str, str_delim);
        }
        assert(idx == count-1);
        *(result + idx) = 0;
    }

    return result;
}

#endif // STR_SPLIT_H_
