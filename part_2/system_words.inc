native "isimmediate", isimmediate
    pop rax
    push rax
    xor rdx, rdx
    mov dl, byte[rax - 1]
    push rdx
    jmp next

native "initcmd", initcmd
    pop rax
    push pc
    mov [program_stub], rax
    mov pc, program_stub
    mov w, [pc]
    pop pc
    jmp [w]

native "isbranch", isbranch
    pop rax
    push rax
    cmp byte[rax - 1], 2
    jne .continue
    mov byte[was_branch], 1
    .continue:
    jmp next

native "lit", lit
    push qword[pc]
    add pc, 8
    jmp next

native "unsetbranch", unsetbranch
    mov byte[was_branch], 0
    jmp next

native "savenum", savenum
    pop rax
    mov [here], rax
    mov rax, here
    add rax, 8
    mov qword[here], rax
    jmp next

native "wasbranch", wasbranch
    xor rdx, rdx
    mov dl, byte[was_branch]
    push rdx
    jmp next

native "pushmode", pushmode
    mov rax, [mode]
    push rax
    jmp next

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
    pop qword [rax]
    add qword [here], 8
    jmp next

; (flag name)
native "create", create
    mov rcx, [last_word]
    mov rsi, [here]
    mov [rsi], rcx
    mov [last_word], rsi
    add rsi, 8

    pop rdi ; name
    push rsi
    call string_copy
    pop rsi
    push rsi
    mov rdi, rsi
    call string_length
    pop rsi
    add rsi, rax
    inc rsi
    
    pop rax ; flag
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

find_word:
  xor eax, eax
  mov rsi, [last_word]

  .loop:
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi

    test rax, rax
    jnz .found
    mov rsi, [rsi]
    test rsi, rsi
    jnz .loop

    xor eax, eax
    ret

  .found:
    mov rax, rsi
    ret

cfa:
  xor eax, eax
  add rdi, 8
  push rdi
  call string_length
  pop rdi
  add rax, 1
  add rax, 1
  add rdi, rax
  mov rax, rdi
  ret

native "bye", bye
    xor rdi, rdi
    mov rax, 60
    syscall

native "exit", exit
    rpop pc
    jmp next

colon ":", colon
.restart:
    dq xt_input_buf
    dq xt_read

    branch0 .restart
    dq xt_lit, 0
    dq xt_input_buf
    dq xt_create
    dq xt_lit, 1, xt_mode
    dq xt_write

    dq xt_lit, i_docol
    dq xt_comma
    dq xt_exit

colon ";", semicolon, 1
    dq xt_lit, 0, xt_mode
    dq xt_write
    dq xt_lit, xt_exit
    dq xt_comma
    dq xt_exit

native "read", read
    pop rdi
    call read_word
    push rdx
    jmp next

native "parse_int", parse_int
    call parse_int
    push rax
    push rdx
    jmp next

native "cfa", cfa
    pop rdi
    call cfa
    push rax
    jmp next

native "find", find
    pop rdi
    call find_word
    push rax
    jmp next
