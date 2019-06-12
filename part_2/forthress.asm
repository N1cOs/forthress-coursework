%include "lib.inc"
%include "macro.inc"
%include "user_words.inc"
%include "system_words.inc"

section .data
	xt_run: dq run
	xt_loop: dq main_loop
	program_stub: dq 0
	err_msg: db ": no such word", 0
	mode: dq 0
	was_branch: db 0
	here: dq forth_mem
	stack_start: dq 0
	last_word: dq _lw

section .bss
	resq 1023
	rstack_start: resq 1
	forth_mem: resq 65536
	input_buf: resb 1024
	user_buf: resb 1024

const input_buf, input_buf

const user_buf, user_buf

const last_word, last_word

const mode, mode

const here, [here]
  
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
	mov pc, xt_run
	mov [stack_start], rsp
	jmp next

run:
  dq i_docol

main_loop:
    dq xt_input_buf, xt_read
    branch0 exit
    dq xt_input_buf
    dq xt_find

    dq xt_pushmode
    branch0 .interpreter_mode

.compiler_mode:
    dq xt_dup
    branch0 .compiler_number

    dq xt_cfa

    dq xt_isimmediate
    branch0 .notImmediate

.immediate:
      dq xt_initcmd
      branch main_loop

.notImmediate:
      dq xt_isbranch
      dq xt_comma
      branch main_loop

.compiler_number:
      dq xt_drop
      dq xt_parse_int
      branch0 .no_word
      dq xt_wasbranch
      branch0 .lit

      dq xt_unsetbranch
      dq xt_savenum
      branch main_loop

.lit:
    dq xt_lit, xt_lit
    dq xt_comma
	dq xt_comma
    branch main_loop


.interpreter_mode:
	dq xt_dup
    branch0 .interpreter_number

    dq xt_cfa
    dq xt_initcmd
    branch main_loop

.interpreter_number:
    dq xt_drop
    dq xt_parse_int
    branch0 .no_word
    branch main_loop

.no_word:
	dq xt_drop
	push err_msg
  	dq xt_prints
    branch main_loop

exit:
  dq xt_bye
