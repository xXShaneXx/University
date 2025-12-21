%{
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_MAX 1000

long stack[STACK_MAX];
int sp = 0;

int error = 0;
char errmsg[256];

void push(long v) {
    if (sp < STACK_MAX) stack[sp++] = v;
}

int pop(long *v) {
    if (sp == 0) return 0;
    *v = stack[--sp];
    return 1;
}

void reset() {
    sp = 0;
    error = 0;
    errmsg[0] = 0;
}

void oper(char op) {
    long b, a;
    if (!pop(&b) || !pop(&a)) {
        error = 1;
        sprintf(errmsg, "Błąd: za mała liczba argumentów");
        return;
    }

    long r;
    switch(op) {
        case '+': r = a + b; break;
        case '-': r = a - b; break;
        case '*': r = a * b; break;
        case '/':
            if (b == 0) {
                error = 1;
                sprintf(errmsg, "Błąd: dzielenie przez zero");
                return;
            }
            r = a / b; 
            break;
        case '%':
            if (b == 0) {
                error = 1;
                sprintf(errmsg, "Błąd: modulo przez zero");
                return;
            }
            r = a % b; 
            break;
        case '^': {
            long res = 1;
            if (b < 0) {
                error = 1;
                sprintf(errmsg, "Błąd: ujemny wykładnik");
                return;
            }
            for (int i=0; i<b; i++) res *= a;
            r = res;
            break;
        }
        default:
            error = 1;
            sprintf(errmsg, "Błąd: nieznany operator '%c'", op);
            return;
    }

    push(r);
}

%}

%%

[ \t]+          ;   // ignoruj spacje
"-"?[0-9]+      { if (!error) push(atol(yytext)); }
"+"|"-"|"*"|"/"|"%"|"^"   { if (!error) oper(yytext[0]); }
\n              {
                    if (!error) {
                        if (sp == 1) {
                            printf("= %ld\n", stack[0]);
                        } else if (sp > 1) {
                            printf("Błąd: za mała liczba operatorów\n");
                        }
                    } else {
                        printf("%s\n", errmsg);
                    }
                    reset();
                }

. { 
      if (!error) {
         sprintf(errmsg, "Błąd: zły symbol \"%s\"", yytext);
         error = 1;
      }
  }

%%

int main() {
    printf("Postfix kalkulator RPN\n");
    yylex();
    return 0;
}