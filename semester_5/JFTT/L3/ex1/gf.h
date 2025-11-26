#ifndef GF_H
#define GF_H

#define P 1234577LL

// Korekcja wartości do zakresu [0, P-1]
long long mod_correct(long long val);

// Operacje arytmetyczne modularne
long long mod_add(long long a, long long b);
long long mod_sub(long long a, long long b);
long long mod_mul(long long a, long long b);
long long mod_div(long long a, long long b);
long long mod_pow(long long base, long long exp);

#endif // GF_H