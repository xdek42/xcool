.section .data
CONST_STR1:
    .asciz "Hello, World.\n"

Object_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy

IO_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy
    .long IO_out_string
    .long IO_out_int
    .long IO_in_string
    .long IO_in_int

Int_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy

Main_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy
    .long IO_out_string
    .long IO_out_int
    .long IO_in_string
    .long IO_in_int
    .long Main_main

.section .text
.global Object_abort
.type Object_abort, @function
Object_abort:
    ret

.global Object_type_name
.type Object_type_name, @function
Object_type_name:
    ret

.global Object_copy
.type Object_copy, @function
Object_copy:
    ret

.global IO_out_string
.type IO_out_string, @function
IO_out_string:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%ebp), %ecx
    movl $15, %edx
    movl $4, %eax
    movl $1, %ebx
    int $0x80
    movl %ebp, %esp
    popl %ebp
    ret

.global IO_out_int
.type IO_out_int, @function
IO_out_int:
    ret

.global IO_in_string
.type IO_in_string, @function
IO_in_string:
    ret

.global IO_in_int
.type IO_in_int, @function
IO_in_int:
    ret

.global Main_main
.type Main_main, @function
Main_main:
    pushl %ebp
    movl %esp, %ebp
    pushl $CONST_STR1
    movl $Main_dispatch_table, %ebx
    addl $12, %ebx
    movl (%ebx), %eax
    call *%eax
    movl %ebp, %esp
    popl %ebp
    ret

.global _start
_start:
   pushl $Main_dispatch_table
   call Main_main
   movl $1, %eax
   movl $0, %ebx
   int $0x80
