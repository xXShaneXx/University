section .bss
    buffer resb 128       ; Buffer for input
    result resb 12        ; Buffer for result (maximum 10 digits + newline + null terminator)

section .text
    global _start

_start:
    ; Read input from standard input
    mov eax, 3            ; syscall: sys_read
    mov ebx, 0            ; stdin
    mov ecx, buffer       ; Pointer to the buffer
    mov edx, 128          ; Buffer size
    int 0x80              ; System call

    ; Initialize sum to 0
    xor eax, eax          ; Clear eax
    mov dword [result], eax  ; Set sum to 0

    ; Process input
    mov esi, buffer       ; Pointer to input buffer

process_loop:
    mov al, byte [esi]    ; Load current character
    cmp al, 10            ; Check if newline (ASCII 10)
    je convert_to_string  ; If yes, go to result conversion
    sub al, '0'           ; Convert ASCII to digit
    add dword [result], eax  ; Add digit to sum
    inc esi               ; Move to next character
    jmp process_loop      ; Repeat for next character

convert_to_string:
    mov eax, [result]        ; Load the sum
    mov edi, result       ; Start writing result from `result` buffer
    add edi, 11           ; Move to the end of buffer (reserve space for 11 characters)
    mov byte [edi], 0     ; Null-terminate the string

convert_loop:
    dec edi               ; Move back to the previous position
    xor edx, edx          ; Clear edx
    mov ebx, 10           ; Divider (base 10)
    div ebx               ; eax = eax / 10, edx = remainder (current digit)
    add dl, '0'           ; Convert remainder to ASCII
    mov byte [edi], dl    ; Store ASCII character in the result buffer
    test eax, eax         ; Check if quotient is 0
    jnz convert_loop      ; If not, process next digit

write_result:
    ; Calculate the length of the result string
    lea ecx, [edi]        ; ecx points to the beginning of the result string
    mov edx, result + 12  ; edx points to the null terminator
    sub edx, ecx          ; Length = end - start

    ; Write result to standard output
    mov eax, 4            ; syscall: sys_write
    mov ebx, 1            ; stdout
    int 0x80              ; System call

    ; Exit program
    mov eax, 1            ; syscall: sys_exit
    xor ebx, ebx          ; Exit code 0
    int 0x80
