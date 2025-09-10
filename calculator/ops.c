
#include "ops.h"
#include <ctype.h>
#include <string.h>

op_t op_from_string(const char *s) {
    if (!s) return OP_INVALID;
    // ensure lowercase input
    char buffer[32];
    size_t i = 0;
    for (; s[i] && i < sizeof(buffer)-1; i++) {
        buffer[i] = tolower((unsigned char)s[i]);
    } buffer[i] = '\0';

    // allow use of different symbols for operations
    if (buffer[0] && buffer[1] == '\0') {
        switch (buffer[0]) {
            case '+': return ADD;
            case '-': return SUB;
            case '/': return DIV;
            case 'x': return MUL;
            case '*': return MUL;
            case '!': return FACT;
            case 'p': return NPR;
            case 'c': return NCR;
        }
    }
    
    const char *names[] = {"add", "sub", "div", "mul", "fact", "npr", "ncr"};
    size_t n = sizeof(names)/sizeof(names[0]);
    for (size_t i = 0; i < n; i++) {
        if (strcmp(buffer, names[i]) == 0) {
            return (op_t)(i + 1); // return corresponding enum value 
        }
    }
    return OP_INVALID;
}


