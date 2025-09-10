
#ifndef PARSE_H
#define PARSE_H

int parse_double_strict(const char *s, double *out);
int parse_ull_strict10(const char *s, unsigned long long *out);

#endif // PARSE_H