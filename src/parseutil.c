#include "parseutil.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

void skip_white(char** parseChar)
{
    if (!parseChar || !*parseChar)
    {
        return;
    }

    while (**parseChar == ' ' || **parseChar == '\t' || **parseChar == '\r' || **parseChar == '\n')
    {
        (*parseChar)++;
    }
}

int parse_int10(const char* string, int* out)
{
    if (!string || !out)
    {
        return -1;
    }

    const int DECIMAL = 10;
    errno = 0;
    char* end = NULL;
    long val = strtol(string, &end, DECIMAL);

    if (end == string)
    {
        return -2;
    }

    if (errno == ERANGE || val < INT_MIN || val > INT_MAX)
    {
        return -3;
    }

    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r' || *end == '\f' ||
           *end == '\v')
    {
        end++;
    }
    if (*end != '\0')
    {
        return -4;
    }

    *out = (int)val;
    return 0;
}

int parse_double(const char* string, double* out)
{
    if (!string || !out)
    {
        return -1;
    }

    errno = 0;
    char* end = NULL;
    double val = strtod(string, &end);

    if (end == string)
    {
        return -2;
    }

    if (errno == ERANGE)
    {
        return -3;
    }

    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r' || *end == '\f' ||
           *end == '\v')
    {
        end++;
    }
    if (*end != '\0')
    {
        return -4;
    }

    *out = val;
    return 0;
}
