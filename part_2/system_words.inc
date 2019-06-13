native "lit", lit
    push qword[pc]
    add pc, 8
    jmp next

native "execute", execute
    pop rax
    mov w, rax
    jmp [rax]

native "branch", branch
    mov pc, [pc]
    jmp next

; jump if tos is 0
native "branch0", branch0
    pop rsi
    cmp rsi, 0
    jne .skip
    mov pc, [pc]
    jmp next
.skip:
    add pc, 8
    jmp next

native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

native ",", comma
    mov rax, [here]
    pop qword[rax]
    add qword[here], 8
    jmp next

; (flag name)
native "create", create
    ; link
    mov rcx, [last_word]
    mov rsi, [here]
    mov [rsi], rcx
    mov [last_word], rsi
    add rsi, 8
    mov byte[rsi], 0
    inc rsi

    ; name
    pop rdi
    push rsi
    call string_copy
    mov rdi, qword[rsp]
    call string_length
    pop rsi
    add rsi, rax
    inc rsi
    
    ; flag
    pop rax
    mov [rsi], al
    inc rsi
    mov [here], rsi
    jmp next


native "syscall", syscall
    pop r9
    pop r8
    pop r10
    pop rdx
    pop rsi
    pop rdi
    pop rax
    syscall
    push rax
    push rdx
    jmp next

native "bye", bye
    call exit

native "exit", exit
    rpop pc
    jmp next

; (word - header address)
native "find", find
    mov rsi, [last_word]
    mov rax, [mode]
    test rax, rax
    jz .loop
    mov rsi, [rsi]
.loop:
    mov rdi, [rsp]
    push rsi
    add rsi, 9
    call string_equals
    pop rsi
    test rax, rax
    jnz .found

    mov rsi, [rsi]
    test rsi, rsi
    jnz .loop
.not_found:
    mov qword [rsp], 0
    jmp next
.found:
    mov [rsp], rsi
    jmp next

; (word address - execution token)
native "cfa", cfa
    pop rsi
    add rsi, 9
.loop:
    mov al, [rsi]
    test al, al
    jz .end
    inc rsi
    jmp .loop

.end:
    add rsi, 2 ; skip immediate flag
    push rsi
    jmp next

colon "is_immediate", is_immediate
    dq xt_lit, 1, xt_minus, xt_fetch_byte, xt_exit

colon "follows_branch", follows_branch
    dq xt_here, xt_fetch, xt_lit, 8, xt_minus, xt_fetch
    dq xt_dup, xt_lit, xt_branch0, xt_equals, xt_swap
    dq xt_lit, xt_branch, xt_equals, xt_lor, xt_exit

colon ":", colon
.restart:
    dq xt_input_buf, xt_read
    branch0 .restart
    
    dq xt_lit, 0, xt_input_buf, xt_create
    dq xt_lit, 1, xt_mode, xt_write
    dq xt_lit, i_docol, xt_comma, xt_exit

colon ";", semicolon, 1
    dq xt_lit, 0, xt_mode, xt_write
    dq xt_lit, xt_exit, xt_comma, xt_exit

native "read", read
    pop rdi
    call read_word
    push rdx
    jmp next

native "parse_int", parse_int
    pop rdi
    call parse_int
    push rax
    push rdx
    jmp next

colon "interpret", interpret
; (tos - pointer to the word)
    dq xt_dup, xt_find, xt_dup
    branch0 .parse_num
    
    dq xt_swap, xt_drop, xt_cfa
    dq xt_mode, xt_fetch
    branch0 .interpreting

    dq xt_dup, xt_is_immediate, xt_not
    branch0 .interpreting

.compiling:
    dq xt_comma, xt_exit
.interpreting:
    dq xt_execute, xt_exit

.parse_num:
    dq xt_drop, xt_dup, xt_parse_int      
    branch0 .not_word

    dq xt_swap, xt_drop
    dq xt_mode, xt_fetch                  
    branch0 .exit

    dq xt_follows_branch, xt_not
    branch0 .branch
.not_branch:
    dq xt_lit, xt_lit, xt_comma
.branch:
    dq xt_comma, xt_exit
.not_word:
  dq xt_lit, err_msg, xt_prints, xt_drop, xt_drop
.exit:
    dq xt_exit