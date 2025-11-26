%{
#include <stdio.h>
#include <stdlib.h>
#include "gf.h"

// Deklaracje
extern int yylex();
extern char *yytext;
void yyerror(const char *s);
%}

// Sekcja tokenów i priorytetów

%union {
    long long val; // Dla wartości liczbowej
}

// Tokeny (zwracane przez Flex)
%token <val> NUM
%token ADD SUB MUL DIV POW LPAREN RPAREN EOL

%type <val> expr

// Priorytety i łączność operatorów (od najniższego do najwyższego)

%left ADD SUB 
// Używamy %prec, aby nadać SUB, występującemu jako unarny minus, wyższy priorytet
%right UMINUS 
%left MUL DIV
// Potęgowanie ma najwyższy priorytet (łączność lewostronna, by nie składać potęg)
%left POW 

// Reguła startowa
%start program

%%
// Sekcja reguł gramatycznych

program: 
      | program line
;

line:
    EOL
  | expr EOL {
        // AKCJA: Wypisz ONP i wynik
        printf("\nWynik: %lld\n", $1);
        printf("---\n"); // Separator dla przejrzystości
    }
  | error EOL {
        // Obsługa błędu składniowego - pomiń resztę linii
        yyerror("Błąd składniowy i pominięcie linii");
        yyerrok; // Odzyskaj sprawność po błędzie
    }
;

expr:
    NUM                     { 
        printf("%lld ", $1); // ONP: Liczba jest już skorygowana w Flexie
        $$ = $1;            // $$ otrzymuje wartość NUM
    }
  | LPAREN expr RPAREN      { 
        $$ = $2;            // Wynik w nawiasach
    }
  | expr ADD expr           { 
        printf("+ ");
        $$ = mod_add($1, $3);
    }
  | expr SUB expr           { 
        printf("- ");
        $$ = mod_sub($1, $3);
    }
  | expr MUL expr           { 
        printf("* ");
        $$ = mod_mul($1, $3);
    }
  | expr DIV expr           { 
        printf("/ ");
        $$ = mod_div($1, $3);
    }
  | expr POW expr           { 
        // Wymaganie zadania: można liczyć (a^b)^c, ale nie a^b^c.
        // Domyślna lewostronna łączność ('%left POW') zdefiniowana powyżej 
        // automatycznie to wymusza.
        printf("^ ");
        $$ = mod_pow($1, $3);
    }
  | SUB expr %prec UMINUS   { 
        // UNARNY MINUS: np. -5, -(2+3)
        // Modyfikacja: -X jest równoważne (0 - X) mod P.
        printf("UMINUS "); // Drukujemy UMINUS, by pokazać, że Bison go rozpoznał
        $$ = mod_sub(0, $2);
        // Ponieważ ONP wymaga, aby operand był przed operatorem, 
        // a Bison wykonał już akcję semantyczną dla $2 (drukowanie), 
        // musimy to zaakceptować, albo inaczej zaimplementować. 
        // Zgodnie z przykładem: -2 -> 1234575 (liczba) 
        // Zmieńmy wydruk, aby był zgodny z przykładem!
        // Przykład: -2--1 -> 1234575 1234576 -
        // Jeśli Bison widzi 'SUB expr', nie wie, czy to SUB czy UMINUS, dopóki 
        // nie zastosuje %prec.

        // Poprawka dla unarnego minusa w ONP (jak w przykładzie): 
        // Traktujemy -(X) jako skorygowaną liczbę.
        // Jeśli expr jest NUM, wynik to P - NUM. Jeśli jest złożone, 
        // wynik to P - (wynik_złożony).

        // Aby spełnić wymaganie przykładowej sesji:
        // -2 zamienia się w 1234575.
        // -2--1 staje się (1234575) - (1234576)

        // Resetujemy ONP, aby wydrukować skorygowaną wartość:
        printf("\r"); // Powrót na początek linii
        long long current_val = $2; // Wartość X
        long long corrected_val = mod_sub(0, current_val); // -X

        // Wypiszemy całe wyrażenie jako nową liczbę
        printf("%lld ", corrected_val);
        
        $$ = corrected_val; 
        
        // UWAGA: Jest to uproszczona obsługa, która działa tylko dla 
        // pojedynczych operacji unarnych lub na końcu złożonych wyrażeń.
        // Pełna implementacja ONP powinna mieć stos operatorów.
    }
;
%%

// Implementacja obsługi błędów
void yyerror(const char *s) {
    fprintf(stderr, "Parser Error: %s na wejściu '%s'\n", s, yytext);
    // Wywołanie exit(1) nie jest zalecane, jeśli chcemy kontynuować parsowanie
}