// tests.c
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ops.h"
#include "parse.h"
#include "combi.h"

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("%s:%d: ASSERT_TRUE failed: %s\n", __FILE__, __LINE__, #cond); \
        return 1; \
    } \
} while (0)

#define ASSERT_EQ_U64(exp, got) do { \
    if ((unsigned long long)(exp) != (unsigned long long)(got)) { \
        printf("%s:%d: ASSERT_EQ_U64 expected=%llu got=%llu\n", __FILE__, __LINE__, \
               (unsigned long long)(exp), (unsigned long long)(got)); \
        return 1; \
    } \
} while (0)

#define ASSERT_EQ_INT(exp, got) do { \
    if ((int)(exp) != (int)(got)) { \
        printf("%s:%d: ASSERT_EQ_INT expected=%d got=%d\n", __FILE__, __LINE__, (int)(exp), (int)(got)); \
        return 1; \
    } \
} while (0)

#define ASSERT_EQ_DBL(exp, got, eps) do { \
    double __e = (exp), __g = (got); \
    if (fabs(__e - __g) > (eps)) { \
        printf("%s:%d: ASSERT_EQ_DBL expected=%.12g got=%.12g (eps=%g)\n", __FILE__, __LINE__, __e, __g, (eps)); \
        return 1; \
    } \
} while (0)

static int test_ops(void) {
    ASSERT_EQ_INT(ADD, op_from_string("add"));
    ASSERT_EQ_INT(SUB, op_from_string("sub"));
    ASSERT_EQ_INT(DIV, op_from_string("div"));
    ASSERT_EQ_INT(MUL, op_from_string("mul"));
    ASSERT_EQ_INT(FACT, op_from_string("fact"));
    ASSERT_EQ_INT(NPR, op_from_string("npr"));
    ASSERT_EQ_INT(NCR, op_from_string("ncr"));

    // symbols / aliases (depends on your mapping)
    ASSERT_EQ_INT(ADD, op_from_string("+"));
    ASSERT_EQ_INT(SUB, op_from_string("-"));
    ASSERT_EQ_INT(DIV, op_from_string("/"));
    ASSERT_EQ_INT(MUL, op_from_string("*"));
    ASSERT_EQ_INT(MUL, op_from_string("x"));
    ASSERT_EQ_INT(FACT, op_from_string("!"));
    ASSERT_EQ_INT(NPR, op_from_string("p"));
    ASSERT_EQ_INT(NCR, op_from_string("c"));

    // case-insensitive
    ASSERT_EQ_INT(MUL, op_from_string("MuL"));
    ASSERT_EQ_INT(NCR, op_from_string("NcR"));

    // invalid
    ASSERT_EQ_INT(OP_INVALID, op_from_string(NULL));
    ASSERT_EQ_INT(OP_INVALID, op_from_string("unknown"));
    return 0;
}

static int test_parse_double(void) {
    double v;
    ASSERT_TRUE(parse_double_strict("0", &v));         ASSERT_EQ_DBL(0.0, v, 1e-12);
    ASSERT_TRUE(parse_double_strict("  -3.5 ", &v));   ASSERT_EQ_DBL(-3.5, v, 1e-12);
    ASSERT_TRUE(parse_double_strict("+2.5e1", &v));    ASSERT_EQ_DBL(25.0, v, 1e-12);
    ASSERT_TRUE(!parse_double_strict("", &v));
    ASSERT_TRUE(!parse_double_strict("  ", &v));
    ASSERT_TRUE(!parse_double_strict("nan", &v));
    ASSERT_TRUE(!parse_double_strict("inf", &v));
    ASSERT_TRUE(!parse_double_strict("12x", &v));
    return 0;
}

static int test_parse_ull(void) {
    unsigned long long u;
    ASSERT_TRUE(parse_ull_strict10("0", &u));          ASSERT_EQ_U64(0, u);
    ASSERT_TRUE(parse_ull_strict10("  12345 ", &u));   ASSERT_EQ_U64(12345, u);
    ASSERT_TRUE(!parse_ull_strict10("-1", &u));        // negative rejected
    ASSERT_TRUE(!parse_ull_strict10("", &u));          // empty
    ASSERT_TRUE(!parse_ull_strict10("  ", &u));        // spaces
    ASSERT_TRUE(!parse_ull_strict10("12.3", &u));      // decimal rejected
    ASSERT_TRUE(!parse_ull_strict10("1e3", &u));       // exponent rejected
    ASSERT_TRUE(!parse_ull_strict10("99x", &u));       // junk suffix
    return 0;
}

static int test_factorial(void) {
    unsigned long long out;
    ASSERT_TRUE(ull_factorial(0, &out));  ASSERT_EQ_U64(1, out);
    ASSERT_TRUE(ull_factorial(1, &out));  ASSERT_EQ_U64(1, out);
    ASSERT_TRUE(ull_factorial(5, &out));  ASSERT_EQ_U64(120, out);
    ASSERT_TRUE(ull_factorial(20, &out)); ASSERT_EQ_U64(2432902008176640000ULL, out);
    ASSERT_TRUE(!ull_factorial(21, &out)); // overflow
    return 0;
}

static int test_npr(void) {
    unsigned long long out;
    ASSERT_TRUE(ull_npr(5, 0, &out));   ASSERT_EQ_U64(1, out);
    ASSERT_TRUE(ull_npr(5, 3, &out));   ASSERT_EQ_U64(60, out);      // 5*4*3
    ASSERT_TRUE(ull_npr(10,10,&out));   ASSERT_EQ_U64(3628800, out); // 10!
    ASSERT_TRUE(!ull_npr(10,11,&out));  // k>n
    return 0;
}

static int test_ncr(void) {
    unsigned long long out;
    ASSERT_TRUE(ull_ncr(5, 0, &out));   ASSERT_EQ_U64(1, out);
    ASSERT_TRUE(ull_ncr(5, 2, &out));   ASSERT_EQ_U64(10, out);
    ASSERT_TRUE(ull_ncr(10,3, &out));   ASSERT_EQ_U64(120, out);
    ASSERT_TRUE(ull_ncr(52,5,&out));    ASSERT_EQ_U64(2598960ULL, out);
    // symmetry
    ASSERT_TRUE(ull_ncr(52,47,&out));   ASSERT_EQ_U64(2598960ULL, out);
    // domain
    ASSERT_TRUE(!ull_ncr(5, 7, &out));
    // large stress (should fit): C(66,33)
    ASSERT_TRUE(ull_ncr(66,33,&out));
    // likely overflow: C(70,35) — depending on your guards may return 0
    ASSERT_TRUE(!ull_ncr(70,35,&out));
    return 0;
}

int main(void) {
    struct { const char *name; int (*fn)(void); } tests[] = {
        {"ops", test_ops},
        {"parse_double", test_parse_double},
        {"parse_ull", test_parse_ull},
        {"factorial", test_factorial},
        {"npr", test_npr},
        {"ncr", test_ncr},
    };

    int failed = 0;
    for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); ++i) {
        int rc = tests[i].fn();
        if (rc == 0) {
            printf("%s\n", tests[i].name);
        } else {
            printf("%s (rc=%d)\n", tests[i].name, rc);
            failed++;
        }
    }
    if (failed == 0) {
        printf("\n🎉 All tests passed!\n");
        return 0;
    } else {
        printf("\n%d test group(s) failed.\n", failed);
        return 1;
    }
}
