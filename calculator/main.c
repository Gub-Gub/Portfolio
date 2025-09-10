// main.c
#include <stdio.h>
#include "ops.h"
#include "parse.h"
#include "combi.h"

int main (int argc, char **argv) {
    // confirm correct number of arguments
    if (argc < 2) {
        fprintf(stderr,
        "Usage:\nadd or '+'\nsub or '-'\ndiv or '/'\nmul or 'x or *'\nfact or '!'\nnpr or 'n'\ncpr or 'c'\n<num1> <num2>\n");
        return 2;
    }

    // parse operation & check validity
    op_t operation = op_from_string(argv[1]);

    if (operation == NPR) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s npr <n> <r>\n", argv[0]);
            return 2;
        }
        unsigned long long n, k, ans;
        if (!parse_ull_strict10(argv[2], &n) || !parse_ull_strict10(argv[3], &k)) {
            fprintf(stderr, "Invalid Intergers. %s and %s must be non-negative integers\n", argv[2], argv[3]);
            return 1;
        }
        if (!ull_npr(n, k, &ans)) {
            fprintf(stderr, "Invalid Integers: Values %llu and %llu are too large -- overflow or n < r\n", n, k);
            return 1;
        }
        printf("Result: %llu\n", ans);
        return 0;
    }

    if (operation == NCR) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s cpr <n> <r>\n", argv[0]);
            return 2;
        }
        unsigned long long n, k, ans;
        if (!parse_ull_strict10(argv[2], &n) || !parse_ull_strict10(argv[3], &k)) {
            fprintf(stderr, "Invalid Intergers. %s and %s must be non-negative integers\n", argv[2], argv[3]);
            return 1;
        }
        if (!ull_ncr(n, k, &ans)) {
            fprintf(stderr, "Invalid Integers: Values %llu and %llu are too large -- overflow or n < r\n", n, k);
            return 1;
        }
        printf("Result: %llu\n", ans);
        return 0;
    }

    // Factorial
    if (operation == FACT) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s fact <n>\n", argv[0]);
            return 2;
        }
        unsigned long long n, ans;
        if (!parse_ull_strict10(argv[2], &n)) {
            fprintf(stderr, "Invalid Integer. %s must be non-negative integer\n", argv[1]);
            return 1;
        }
        if (!ull_factorial(n, &ans)) {
            fprintf(stderr, "Invalid Integer: Value %llu is too large -- overflow\n", n);
            return 1;
        }
        printf("Result: %llu\n", ans);
        return 0;
    }

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <add (+) | sub (-) | div (/) | mul (x)> <num1> <num2>\n", argv[0]);
        return 2;
    }

    if (operation == OP_INVALID) {
        fprintf(stderr, "Error: invalid operation: %s\n", argv[1]);
        return 1;
    }

    // Binary ops
    double num1, num2;
    // normalize values
    if (!parse_double_strict(argv[2], &num1)) {
        fprintf(stderr, "Error: first number is not a valid double: %s\n", argv[2]);
        return 1;
    }
    if (!parse_double_strict(argv[3], &num2)) {
        fprintf(stderr, "Error: second number is not a valid double: %s\n", argv[3]);
        return 1;
    }

    double result;
    switch(operation) {
        case ADD: result = num1 + num2; break;
        case SUB: result = num1 - num2; break;
        case DIV: if (num2 == 0) {fprintf(stderr, "Error: division by zero\n"); return 1;} result = num1 / num2; break;
        case MUL: result = num1 * num2; break;
        default: fprintf(stderr, "Error: unknown operation\n"); return 1;
    }
    return printf("Result: %g\n", result), 0;
}