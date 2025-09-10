// parse.c
#include "parse.h"
#include <stdlib.h>   // strtod, strtoull
#include <ctype.h>    // isspace
#include <errno.h>    // errno, ERANGE
#include <math.h>     // isfinite

int parse_double_strict(const char *s, double *out) {
    if (s == NULL || *s == '\0') return 0; // Empty or no string

    errno = 0;
    char *end = NULL;
    double v = strtod(s, &end);

    if (end == s) return 0; // no digits
    if (errno == ERANGE) return 0; // overflow
    if (!isfinite(v)) return 0; // not finite
    while (*end && isspace((unsigned char)*end)) end++; // skip trailing whitespaces
    if (*end != '\0') return 0;

    *out = v;
    return 1;
}

int parse_ull_strict10(const char *s, unsigned long long *out) {
    if (s == NULL) return 0; // no string or empty string or negative number

    // skip leading whitespaces
    const char *p = s;
    while(*p && isspace((unsigned char)*p)) p++;
    if (*p == '\0') return 0; // if empty
    if (*p == '-') return 0; // if negative number

    errno = 0;
    char *end = NULL;
    unsigned long long v = strtoull(p, &end, 10);

    if (end == p) return 0; // no digits
    if (errno == ERANGE) return 0; // overflow
    while (*end && isspace((unsigned char)*end)) end++; // skip trailing whitespaces
    if (*end != '\0') return 0; // junk after numbers

    *out = v;
    return 1;
}
