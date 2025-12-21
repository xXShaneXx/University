section .data
    hex_string db '0x00000000', 0xA  ; Bufor na wynik w formacie szesnastkowym z nową linią

section .bss

section .text
    global _start

_start:
    mov eax, 123456      ; Liczba do konwersji
    mov edi, hex_string + 9  ; Wskaźnik na koniec bufora (przed znakiem nowej linii)

convert_loop:
    mov ecx, eax         ; Kopiowanie liczby do ECX
    and ecx, 0xF         ; Maskowanie 4 najmłodszych bitów
    add ecx, '0'         ; Konwersja na znak ASCII
    cmp ecx, '9'
    jbe store_digit
    add ecx, 7           ; Konwersja na znak A-F

store_digit:
    mov [edi], cl        ; Zapisanie cyfry w buforze
    dec edi              ; Przesunięcie wskaźnika w lewo
    shr eax, 4           ; Przesunięcie liczby o 4 bity w prawo
    cmp edi, hex_string + 1  ; Sprawdzenie, czy doszliśmy do początku liczby
    jae convert_loop

    ; Wyświetlanie wyniku na standardowym wyjściu (stdout)
    mov eax, 4           ; Wywołanie systemowe 'write' (numer 4)
    mov ebx, 1           ; Deskryptor pliku (1 = standardowe wyjście)
    mov edx, 11          ; Długość ciągu (2 znaki "0x" + 8 cyfr + '\n')
    lea ecx, [hex_string] ; Wskaźnik na bufor
    int 0x80             ; Wywołanie systemowe

    ; Zakończenie programu
    mov eax, 1           ; Wywołanie systemowe 'exit' (numer 1)
    xor ebx, ebx         ; Kod zakończenia 0
    int 0x80             ; Wywołanie systemowe
    ;gdb ,.ex3
    ;gdb break _start
    ;gdb run
    ;gdb info registers
    ;gdb continue