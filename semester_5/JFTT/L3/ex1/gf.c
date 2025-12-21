#include "gf.h"
#include <stdio.h>
#include <stdlib.h>

// Korekcja: Zapewnia, że wynik jest w [0, P-1]
long long mod_correct(long long val) {
    // Używamy LLONG_MAX, aby obsłużyć duże liczby przed modulo
    val %= P;
    if (val < 0) {
        val += P;
    }
    return val;
}

long long mod_add(long long a, long long b) {
    return mod_correct(a + b);
}

long long mod_sub(long long a, long long b) {
    return mod_correct(a - b);
}

long long mod_mul(long long a, long long b) {
    // Uwaga: a*b może przekroczyć 64 bity, jeśli P byłoby większe. 
    // Dla P = 1234577, wynik a*b mieści się w 64 bitach (max P^2 ~ 1.5 * 10^12).
    return mod_correct(a * b); 
}

// Funkcja potęgowania modularnego (a^b mod p)
long long mod_pow(long long base, long long exp) {
    long long res = 1;
    // Potęga musi być nieujemna w GF(p)
    if (exp < 0) {
        fprintf(stderr, "Błąd: Wykładnik potęgi musi być >= 0 w GF(%lld).\n", P);
        exit(1); 
    }
    
    base = mod_correct(base);
    while (exp > 0) {
        if (exp % 2 == 1) res = mod_mul(res, base);
        base = mod_mul(base, base);
        exp /= 2;
    }
    return res;
}

// Odwrotność modularna (b^-1 mod p) przy użyciu Małego Twierdzenia Fermata
// b^(p-2) mod p
long long mod_inverse(long long b) {
    if (b == 0) {
        fprintf(stderr, "Błąd: Dzielenie przez 0 w GF(%lld).\n", P);
        exit(1); 
    }
    return mod_pow(b, P - 2); 
}

// Dzielenie modularne (a / b mod p)
long long mod_div(long long a, long long b) {
    long long inv_b = mod_inverse(b);
    return mod_mul(a, inv_b);
}