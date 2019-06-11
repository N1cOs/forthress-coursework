native "drop", drop
    add rsp, 8
    jmp next

native "swap", swap
    pop rsi
    pop rdi
    push rsi
    push rdi
    jmp next

native "dup", dup
    push qword[rsp]
    jmp next

native "+", plus
    pop rsi
    add [rsp], rsi
    jmp next

native "-", minus
    pop rsi
    sub [rsp], rsi
    jmp next

native "*", mult
    pop rsi
    pop rdi
    imul rdi, rsi
    push rdi
    jmp next

native "/", div
    pop rsi
    pop rax
    idiv rsi
    push rax
    jmp next

native "%", mod
    pop rsi
    pop rax
    idiv rsi
    push rdx
    jmp next

native "=", equals
    pop rsi
    pop rdi
    cmp rsi, rdi
    jne .ne
    push 1
    jmp next
.ne:
    push 0
    jmp next

native "<", grt
    pop rsi
    pop rdi
    cmp rsi, rdi
    jle .false
    push 1
    jmp next
.false:
    push 0
    jmp next

native "not", not
    pop rsi
    cmp rdi, 0
    je .zero
    push 0
    jmp next
.zero:
    push 1
    jmp next

native "and", and
    pop rsi
    and [rsp], rsi
    jmp next

native "or", or
    pop rsi
    or [rsp], rsi
    jmp next

native "land", land
    pop rsi
    pop rdi
    cmp rsi, rdi
    je .first
    push rdi
    jmp next
.first:
    push rsi
    jmp next

native "lor", lor
    pop rsi
    pop rdi
    cmp rax, rax
    jne .first
    push rdi
    jmp next
.first:
    push rsi
    jmp next

native ">r", to_rstack
    pop rsi
    rpush rsi
    jmp next

native "r>", from_rstack
    rpop rsi
    push rsi
    jmp next

native "r@", copy_rstack
    push qword[rstack]
    jmp next

native "@", fetch
    pop rsi
    push qword[rsi]
    jmp next

native "!", store
    pop rsi ; addr
    pop rdi ; val
    mov [rsi], rdi
    jmp next

native "c!", store_byte
    pop rsi ; addr
    pop rdi ; byte
    mov [rsi], dil
    jmp next

native "c@", read_byte
    pop rsi ; addr
    movzx rdi, byte[rsi]
    push rdi
    jmp next

native "execute", exec
    pop rsi
    mov w, rsi
    jmp [rsi]

native "docol", docol
    rpush pc
    add w, 8
    mov pc, w
    jmp next

native "branch", branch
    mov pc, [pc]
    jmp next

native "branch0", branch0
    pop rsi
    cmp rsi, rsi
    jne .skip
    mov pc, [pc]
    jmp next
.skip:
    add pc, 8
    jmp next

native "exit", exit
    rpop pc
    jmp next

native "lit", lit
    push qword [pc]
    add pc, 8
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

native ".S", show_stack
    mov rsi, rsp
.loop:
    cmp rsi, [stack_base]
    jae next
    mov rdi, [rsi]
    push rsi
    call print_int
    call print_newline
    pop rsi
    add rsi, 8
    jmp .loop
