%{
#include "y.tab.h"
#include "gf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Deklaracja funkcji z Bisona do obsługi błędu
extern int yyerror(const char *s);

// Funkcja pomocnicza do parsowania liczby (z korekcją)
static long long parse_number(char *text);

// Licznik nawiasów dla obsługi unarnego minusa w Bisonie
// To jest uproszczenie; Bison lepiej sobie z tym radzi za pomocą %prec.
// W tym kodzie to nie jest potrzebne, ale często pojawia się w starych przykładach.
// Zostawiamy czysty leksykalny podział.
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
            // Komentarz: Czytaj do końca linii
            int c;
            while ((c = input()) != '\n' && c != 0)
                ;
            if (c == '\n') {
                unput(c); // Wróć '\n' do strumienia, by został przetworzony jako EOL
            }
        }

"\\"    { 
            // Kontynuacja linii: Czytaj do '\n' i ignoruj go
            int c = input();
            if (c != '\n') {
                fprintf(stderr, "Błąd leksykalny: Znak kontynuacji '\\' musi być bezpośrednio przed nową linią.\n");
            }
        }

.       { 
            // Obsługa błędu - nierozpoznany znak
            fprintf(stderr, "Błąd leksykalny: Nieznany znak '%s'\n", yytext); 
            // Zwrócenie 0 lub -1 powoduje zatrzymanie parsowania
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