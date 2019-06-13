section .text

exit:
    xor rdi, rdi
    mov rax, 60
    syscall

string_length:
    xor rax, rax
.loop:
    cmp byte[rdi+rax], 0
    je .end
    inc rax
    jmp .loop
.end:
    ret

print_string:
    call string_length
    mov rsi, rdi
    mov rdi, 1
    mov rdx, rax
    mov rax, 1
    syscall
    ret

print_char:
    push rdi
    mov rsi, rsp
    mov rax, 1
    mov rdx, 1
    mov rdi, 1
    syscall
    pop rdi
    ret

print_newline:
    mov rdi, 0xa
    call print_char
    ret

print_uint:
    mov rax, rdi
    mov r8, 0xa
    push 0
    mov rcx, rsp
    sub rsp, 20
.loop:
    cmp rax, r8
    jb .one_digit
    xor rdx, rdx
    dec rcx
    div r8
    or dl, 0x30
    mov [rcx], dl
    cmp rax, r8
    jae .loop
    dec rcx
.one_digit:
    or al, 0x30
    mov [rcx], al
    mov rdi, rcx
    call print_string
    add rsp, 28
    ret

print_int:
    cmp rdi, 0
    jge .positive
    mov r8, rdi
    mov rdi, 0x2d
    call print_char
    mov rdi, r8
    neg rdi
.positive:
    call print_uint
    ret

read_char:
    mov rax, 0
    mov rdi, 0
    push 0
    mov rsi, rsp
    mov rdx, 1
    syscall
    pop rax
    ret

;rdi - buffer address, rsi - buffer size
; returns 0 or buffer adress 
read_word:
    push r12
    push r13
    push r14
    mov r12, rdi
    mov r13, rsi
    xor r14, r14
    dec r13
.first_char:
    call read_char
    cmp al, 0x20
    je .first_char
    cmp al, 0xd
    je .first_char
    cmp al, 0x9
    je .first_char
    cmp al, 0xa
    je .first_char
    cmp al, 0
    je .end
.next_char:
    mov byte[r12 + r14], al
    inc r14
    call read_char
    cmp al, 0x20
    je .end
    cmp al, 0xd
    je .end
    cmp al, 0x9
    je .end
    cmp al, 0xa
    je .end
    cmp al, 0
    je .end
	cmp r14, r13
    je .overflow
    jmp .next_char
.overflow:
    xor rax, rax
    pop r14
    pop r13
    pop r12
    ret
.end:
    mov byte[r12 + r14], 0
    mov rax, r12
    mov rdx, r14
    pop r14
    pop r13
    pop r12
    ret

parse_uint:
    xor r8, r8
    xor rax, rax
    mov r9, 10
.loop:
    mov r11b, byte[rdi + r8]
    cmp r11b, 0x30
    jb .end
    cmp r11b, 0x39
    ja .end
    and r11, 0xf
    mul r9
    add rax, r11
    inc r8
    jmp .loop
.end:
    mov rdx, r8
    ret

parse_int:
    mov r11b, byte[rdi]
    cmp r11b, 0x2d
    je .signed
    jmp parse_uint
.signed:
    inc rdi
    call parse_uint
    cmp rdx, 0
    je .nan
    inc rdx
    neg rax
    ret
.nan:
    xor rax, rax
    ret

string_equals:
    xor rdx, rdx
next_char:
    mov r9b, byte[rdi + rdx]
    cmp r9b, byte[rsi + rdx]
    jne .not_equals
    cmp byte[rdi + rdx], 0
    je .equals
    inc rdx
    jmp next_char
.not_equals:
    xor rax, rax
    ret
.equals:
    mov rax, 1
    ret

string_copy:
    xor r8, r8
.next_char:
    mov r9b, byte[rdi + r8]
    cmp r8, rdx
    jae .unfit
    mov byte[rsi + r8], r9b
    inc r8
    cmp r9, 0
    je .end
    jmp .next_char
.unfit:
    xor rax, rax
    ret
.end:
    mov rax, rsi
    ret
