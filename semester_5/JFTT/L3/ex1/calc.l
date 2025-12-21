%{
#include "y.tab.h"
#include "gf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

extern int yyerror(const char *s);

static long long parse_number(char *text);

%}

%option noyywrap

%%

[0-9]+  {
            // Token: LICZBA. Korekcja modulo P.
            yylval.val = parse_number(yytext);
            return NUM;
        }

"+"     { return ADD; }
"-"     { return SUB; }
"*"     { return MUL; }
"/"     { return DIV; }
"^"     { return POW; }

"("     { return LPAREN; }
")"     { return RPAREN; }

[ \t]+  { /* Ignoruj białe znaki */ }

\n      { return EOL; }

"#"     { 
            int c;
            while ((c = input()) != '\n' && c != 0)
                ;
            if (c == '\n') {
                unput(c); // Wróć '\n' do strumienia, by został przetworzony jako EOL
            }
        }

"\\"    { 
            int c = input();
            if (c != '\n') {
                fprintf(stderr, "Błąd leksykalny: Znak kontynuacji '\\' musi być bezpośrednio przed nową linią.\n");
            }
        }

.       { 
            fprintf(stderr, "Błąd leksykalny: Nieznany znak '%s'\n", yytext); 
            return 0;
        }

%%
// Sekcja kodu użytkownika

static long long parse_number(char *text) {
    long long val;
    // strtoll może obsłużyć duże liczby (aż do LLONG_MAX)
    errno = 0;
    val = strtoll(text, NULL, 10);

    if (errno == ERANGE) {
        fprintf(stderr, "Ostrzeżenie: Liczba '%s' jest poza zakresem long long, użyto wartości modularnej.\n", text);
        // W przypadku przepełnienia, zadowolimy się korekcją
    }

    return mod_correct(val);
}

// Używamy prostego main do uruchomienia yyparse()
int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    
    printf("Wprowadź wyrażenia (zakończ ctrl+d):\n");
    yyparse();
    return 0;
}