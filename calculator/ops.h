// typedef for main 
#ifndef OPS_H
#define OPS_H

typedef enum {
    OP_INVALID,     // 0
    ADD,            // 1
    SUB,            // 2
    DIV,            // 3
    MUL,            // 4
    FACT,           // 5  
    NPR,            // 6
    NCR,            // 7
} op_t;

op_t op_from_string(const char *s);

#endif // OPS_H