; nasm -fbin mandelbrot.asm
; qemu-system-i386 -drive format=raw,file=mandelbrot

bits 16
org 0x7c00

boot:
    mov ax, 0x2401
    int 0x15
    mov ax, 0x13        ; wlaczenie trybu graficznego 320x200
    int 0x10
    cli
    lgdt [gdt_pointer]  ; ustawienie tablicy GDT
    mov eax, cr0        ; wlaczenie trybu chronionego
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:boot2

gdt_start:              ; tablica GDT
    dq 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:
gdt_pointer:
    dw gdt_end - gdt_start
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

bits 32
boot2:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ebx, 0xa0000    ; adres ekranu graficznego
    xor esi, esi        ; indeks piksela w pamięci wideo

    ; Stałe dla zbioru Mandelbrota
    fldz                ; Ustaw 0 na stosie (pomocnicze)

    ; Wartości graniczne przestrzeni zespolonej
    mov dword [xmin], -200000
    mov dword [xmax], 200000
    mov dword [ymin], -100000
    mov dword [ymax], 100000

    mov ecx, 200        ; Iteracja po linii y
row_loop:
    push ecx
    mov ecx, 320        ; Iteracja po piksela x
    push ecx

    ; Oblicz y0 dla danego wiersza
    mov eax, [ymax]
    sub eax, [ymin]
    mov ebx, 200
    imul ebx, ecx       ; ebx = (row * (ymax - ymin))
    cwd
    idiv ebx            ; eax = (row / 200) * (ymax - ymin)
    add eax, [ymin]     ; y0 = ymin + ...
    push eax            ; Zapisz y0 na stosie

column_loop:
    ; Oblicz x0 dla danego kolumny
    mov eax, [xmax]
    sub eax, [xmin]
    mov ebx, 320
    imul ebx, ecx       ; ebx = (col * (xmax - xmin))
    cwd
    idiv ebx            ; eax = (col / 320) * (xmax - xmin)
    add eax, [xmin]     ; x0 = xmin + ...
    push eax            ; Zapisz x0 na stosie

    ; Iteracyjne obliczanie z_n = z_n^2 + c
    xor ebx, ebx        ; Iteracje = 0
    fldz                ; Z = 0
    fldz

    mov ecx, 256        ; Maksymalna liczba iteracji
mandelbrot_iter:
    ; Oblicz Z^2 = (zr^2 - zi^2) + 2*zr*zi*i
    fld st0             ; Zr
    fld st0             ; Zr Zr
    fmul st0, st1       ; Zr^2
    fld st2             ; Zi Zr^2
    fld st2             ; Zi Zi Zr^2
    fmul st0, st1       ; Zi^2 Zr^2
    fsubp st1, st0      ; Re(Z^2) Zr^2

    ; Sprawdzenie czy |Z| > 2
    fld st0             ; Zr
    fld st0             ; Zr Zr
    fmul st0, st1       ; Zr^2
    fld st2             ; Zi Zr^2
    fld st2             ; Zi Zi Zr^2
    fmul st0, st1       ; Zi^2 Zr^2
    faddp st1, st0      ; |Z|^2
    fld1                ; 4
    fadd st0, st0
    fcomi st0, st1
    ja escape           ; Jeśli |Z| > 2, przerwij iteracje

    ; Z = Z^2 + C
    ; Pomijam uproszczenia
    inc bl
    loop mandelbrot_iter

escape:
    ; Wstaw kolor na podstawie iteracji
    mov [ebx + esi], bl
    inc esi

    pop eax
    loop column_loop
    pop eax
    loop row_loop

halt:
    cli
    hlt

; Dane do obliczeń
xmin dd 0
xmax dd 0
ymin dd 0
ymax dd 0

; Wypełniacz sektora rozruchowego
times 510 - ($-$$) db 0
dw 0xaa55
