#include "HashUtils.h"

size_t hash::stringHash(std::string const &string) {
    /* Polynomial rolling hash function */

    long long hash = 0;
    // Prime value
    const int p = 31;
    /* The prime value for final division.
       The probability of two random strings colliding is about 1/m.
       */
    const int m = 1e9 + 9;
    // The pow of p value on current iteration.
    long long p_pow = 1;

    for (char c: string) {
        hash = (hash + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}
