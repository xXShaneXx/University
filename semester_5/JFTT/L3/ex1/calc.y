%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gf.h"

extern int yylex();
extern char *yytext;
void yyerror(const char *s);
#define P 1234577LL
char onp[10000] = "";
int valid = 1;
%}


%union {
    long long val;
    // Dla wartości liczbowej
}

%token <val> NUM
%token ADD SUB MUL DIV POW LPAREN RPAREN EOL

%type <val> expr

// Priorytety i łączność operatorów (od najniższego do najwyższego)

%left ADD SUB 
%right UMINUS 
%left MUL DIV
%left POW 

// Reguła startowa
%start program

%%

program: 
      |
program line
;

line:
    EOL { valid = 1; onp[0] = 0; }
  | expr EOL {
        if(valid) {
            printf("%s\nWynik: %lld\n", onp, $1);
            printf("---\n"); // Separator dla przejrzystości
        }
        valid = 1;
        onp[0] = 0;
    }
  |
error EOL {
        yyerror("Błąd składniowy i pominięcie linii");
        yyerrok; // Odzyskaj sprawność po błędzie
        valid = 0;
        onp[0] = 0;
    }
;
expr:
    NUM                     { 
        // POPRAWKA ONP: Drukowanie wartości liczbowej natychmiast
        sprintf(onp + strlen(onp), "%lld ", $1);
        $$ = $1;
    }
  |
LPAREN expr RPAREN      { 
        $$ = $2;
    }
  |
  | expr ADD expr           { 
        sprintf(onp + strlen(onp), "+ "); 
        $$ = mod_add($1, $3);
    }
  | expr SUB expr           { 
        sprintf(onp + strlen(onp), "- "); 
        $$ = mod_sub($1, $3);
    }
  | expr MUL expr           { 
        sprintf(onp + strlen(onp), "* "); 
        $$ = mod_mul($1, $3);
    }
  | expr DIV expr           { 
        sprintf(onp + strlen(onp), "/ "); 
        $$ = mod_div($1, $3);
    }
  | expr POW expr           { 
        sprintf(onp + strlen(onp), "^ "); 
        long long base = $1;
        long long exp_val = $3; 
        long long result;

        if (exp_val > P / 2) { 
            long long abs_exp = P - exp_val;
            long long temp_pow = mod_pow(base, abs_exp);
            result = mod_inverse(temp_pow);
        } else {
            result = mod_pow(base, exp_val);
        }

        $$ = result;
    }
  |
SUB expr %prec UMINUS   { 
        sprintf(onp + strlen(onp), "~ "); 
        $$ = mod_sub(0, $2);
    }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser Error: %s na wejściu '%s'\n", s, yytext);
}