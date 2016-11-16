.section .data
OUT_INT:
    .asciz "%d\n"
CONST_STR1:
    .asciz "Hello, World."

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
.global IO_out_string
.type IO_out_string, @function
IO_out_string:
    pushl %ebp
    movl %esp, %ebp
    pushl 12(%ebp)
    call puts
    addl $8, %esp
    movl %ebp, %esp
    popl %ebp
    ret
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
.global IO_out_int
.type IO_out_int, @function
IO_out_int:
    pushl %ebp
    movl %esp, %ebp
    movl 12(%ebp), %eax
    addl $4, %eax
    pushl (%eax)
    pushl $OUT_INT
    call printf
    addl $8, %esp
    movl %ebp, %esp
    popl %ebp
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
    movl 8(%ebp), %eax
    addl $0, %eax
    pushl %eax
    movl (%esp), %eax
    movl (%eax), %ebx
    addl $12, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
    movl 8(%ebp), %eax
    addl $4, %eax
    pushl %eax
    movl 8(%ebp), %eax
    addl $12, %eax
    pushl %eax
    popl %eax
    popl %ebx
    addl $4, %eax
    addl $4, %ebx
    movl (%eax), %eax
    movl (%ebx), %ebx
    addl %eax, %ebx
    pushl %ebx
    movl 8(%ebp), %eax
    addl $20, %eax
    addl $4, %eax
    popl %ebx
    movl %ebx, (%eax)
    movl 8(%ebp), %eax
    addl $20, %eax
    pushl %eax
    movl 8(%ebp), %eax
    addl $0, %eax
    pushl %eax
    movl (%esp), %eax
    movl (%eax), %ebx
    addl $16, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
    movl %ebp, %esp
    popl %ebp
    ret

.global _start
_start:
   pushl $0
   pushl $Int_dispatch_table
   pushl $2
   pushl $Int_dispatch_table
   pushl $1
   pushl $Int_dispatch_table
   pushl $Main_dispatch_table
   lea (%esp), %eax
   pushl %eax
   call Main_main
   addl $4, %eax
   movl $1, %eax
   movl $0, %ebx
   int $0x80
