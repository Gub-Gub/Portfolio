#include "combi.h"
#include <limits.h>
#include <stdint.h>

unsigned long long gcd_ull(unsigned long long a, unsigned long long b) {
    // find lowest common denominator
    while (b) {unsigned long long t = a % b; a = b; b = t;}
    return a;
}

int ull_ncr(unsigned long long n, unsigned long long k, unsigned long long *out) {
    if (!out) return 0;
    if (k > n) return 0;
    if (k == 0 || k == n) { *out = 1ULL; return 1;}

    // symmetry to keep k small
    if (k > n - k) k = n - k;
    unsigned long long res = 1;
    for (unsigned long long i = 1; i <=k; i++) {
        unsigned long long num = n - k + i;
        unsigned long long den = i;

        unsigned long long g = gcd_ull(num, den);
        num /= g;
        den /= g;

        g = gcd_ull(res, den);
        res /= g;
        den /= g;

        unsigned __int128 t = (unsigned __int128)res * num;

        if (den != 1) {
            if(t % den != 0) return 0;
            t /= den;
        }
        if (t > (unsigned __int128)ULLONG_MAX) return 0;
        res = (unsigned long long)t;
    }
    *out = res;
    return 1;
}

int ull_npr(unsigned long long n, unsigned long long k, unsigned long long *out) {
    // permutations
    if (k > n) return 0;
    unsigned long long res = 1;
    for (unsigned long long i = 0; i < k; i++) {
        if (res > ULLONG_MAX / (n - i)) return 0;
        res *= (n - i);
    }
    *out = res;
    return 1;
}

int ull_factorial(unsigned long long n, unsigned long long *out) {
    // max size allowed
    if (n > 20) return 0;
    
    unsigned long long res = 1;
    for (unsigned long long i = 2; i <= n; i++) {
        if (res == ULLONG_MAX / i) return 0;
        res *= i;
    }
    *out = res;
    return 1;
}

/*
// combinations
    if (k > n) return 0;
    if (k == 0) return *out = 1;

    unsigned long long res = 1;
    for (unsigned long long i = 0; i <= k; i++) {
        unsigned long long num = n + 1 - i;
        unsigned long long den = i;

        // cancel gcd between num and den
        unsigned long long g = gcd_ull(num, den);
        num /= g;
        den /= g;

        // cancel gcd between den and res
        g = gcd_ull(res, den);
        res /= g;
        den /= g;

        res *= num;
    }
    *out = res;
    return 1;*/