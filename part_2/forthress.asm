%include "lib.inc"
%include "macro.inc"
%include "user_words.inc"
%include "system_words.inc"

const input_buf, input_buf

const last_word, last_word

const mode, mode

const here, [here]

section .data
	err_msg: db "no such word", 0
	mode: dq 0
	here: dq forth_mem
	stack_start: dq 0
	last_word: dq _lw

section .bss
	resq 1023
	rstack_start: resq 1
	forth_mem: resq 65536
	input_buf: resb 1024
  
global _start

section .text
next:      
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]

_start:
	mov rstack, rstack_start
	mov qword[mode], 0
	mov pc, main_loop
	mov [stack_start], rsp
	jmp next
	
main_loop:
    dq xt_input_buf, xt_read
    branch0 .eof

    dq xt_input_buf, xt_interpret
    branch main_loop
.eof:
    dq xt_bye
    