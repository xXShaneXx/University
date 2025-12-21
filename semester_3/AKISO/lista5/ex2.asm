%include        'functions.asm'

section .data
    matrix dd 1, 2, 3, 4, 5, 6, 7, 8, 9
    result_msg_total db "Sum of elements: ", 0
    result_msg_diag db "Sum of diagonal: ", 0
    newline db 10, 0

section .bss
    total_sum resd 1 ; Reserve space for total sum
    diagonal_sum resd 1 ; Reserve space for diagonal sum

section .text
    global _start

_start:
    ;clear each register
    xor eax, eax
    xor ebx, ebx    
    xor ecx, ecx

    ; Calculate Sums
    mov esi, matrix ; load addres of matrix into esi
    xor edi, edi    ; clear registers
matrix_loop:
    mov edx, [esi + edi*4] ; load current matrix element into edx
    add eax, edx ; add current element to eax sum
    cmp edi, 8             ; Check if `edi` is 0 (first diagonal element).
    je store_sums          ; Jump to store_sums if edi is 8
    test edi, 3
    jnz next_element       ; Jump to next_element if not zero
    add ebx, edx           ; Add the current element to ebx

next_element:
    inc edi         ; Increment edi
    jmp matrix_loop ; Jump back to the start of the loop

store_sums:
    add ebx, edx            ; Add the current element to ebx
    mov [total_sum], eax    ; Store the total sum in memory
    mov [diagonal_sum], ebx ; Store the diagonal sum in memory

    ; Wyświetl wyniki
    mov eax, 4              ; System call number (sys_write)
    mov ebx, 1              ; File descriptor (stdout)
    mov ecx, result_msg_total ; Pointer to the message to be printed
    mov edx, 18             ; Length of the message
    int 0x80                ; Call the kernel to print the message

    mov eax, [total_sum]   ; Load the value of total_sum into eax
    call iprintLF           ; Call the function to print the integer in eax followed by a newline

    mov eax, 4                  ; System call number (sys_write)
    mov ebx, 1                  ; File descriptor (stdout)
    mov ecx, result_msg_diag    ; Pointer to the message to be print
    mov edx, 18                 ; Length of the message
    int 0x80                    ; Call the kernel to print the message

    mov eax, [diagonal_sum]     ; Load the value of diagonal_sum into eax
    call iprintLF               ; Call the function to print the integer in eax followed by a newline    

    ; Wyjście
    call quit
