
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
