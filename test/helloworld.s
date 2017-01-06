.section .data
OUT_INT:
    .asciz "%d\n"
CONST_STR1:
    .asciz "Hello, World."
CONST_STR2:
    .asciz "cal test:"
CONST_STR3:
    .asciz "execute then"
CONST_STR4:
    .asciz "execute else"
Object_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
IO_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long IO_out_string
   .long IO_out_int
   .long IO_in_string
   .long IO_in_int
Main_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long IO_out_string
   .long IO_out_int
   .long IO_in_string
   .long IO_in_int
   .long Main_main
String_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long String_length
   .long String_concat
   .long String_substr
Int_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
Bool_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
.section .text
.type Object_abort, @function
Object_abort:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.type Object_type_name, @function
Object_type_name:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.type Object_layout, @function
Object_layout:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.type IO_out_string, @function
IO_out_string:
    pushl %ebp
    movl %esp, %ebp
    pushl 12(%ebp)
    call puts
    movl %ebp, %esp
    popl %ebp
    ret
.type IO_out_int, @function
IO_out_int:
    pushl %ebp
    movl %esp, %ebp
    movl 12(%ebp), %eax
    pushl %eax
    pushl $OUT_INT
    call printf
    movl %ebp, %esp
    popl %ebp
    ret
.type IO_in_string, @function
IO_in_string:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.type IO_in_int, @function
IO_in_int:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
#Main_main
.global Main_main
.type Main_main, @function
Main_main:
    pushl %ebp
    movl %esp, %ebp
    pushl $CONST_STR1
    movl 8(%ebp), %eax
    pushl %eax
    movl (%esp), %eax
    movl 4(%eax), %ebx
    addl $12, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
    pushl $CONST_STR2
    movl 8(%ebp), %eax
    addl $20, %eax
    popl %ebx
    movl %ebx, (%eax)
    pushl $1
    movl 8(%ebp), %eax
    addl $8, %eax
    popl %ebx
    movl %ebx, (%eax)
    pushl $2
    movl 8(%ebp), %eax
    addl $12, %eax
    popl %ebx
    movl %ebx, (%eax)
    movl 8(%ebp), %eax
    addl $8, %eax
    movl (%eax), %eax
    pushl %eax
    movl 8(%ebp), %eax
    addl $12, %eax
    movl (%eax), %eax
    pushl %eax
    popl %eax
    popl %ebx
    addl %eax, %ebx
    pushl %ebx
    movl 8(%ebp), %eax
    addl $16, %eax
    popl %ebx
    movl %ebx, (%eax)
    movl 8(%ebp), %eax
    addl $20, %eax
    movl (%eax), %eax
    pushl %eax
    movl 8(%ebp), %eax
    pushl %eax
    movl (%esp), %eax
    movl 4(%eax), %ebx
    addl $12, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
    movl 8(%ebp), %eax
    addl $16, %eax
    movl (%eax), %eax
    pushl %eax
    movl 8(%ebp), %eax
    pushl %eax
    movl (%esp), %eax
    movl 4(%eax), %ebx
    addl $16, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
    movl 8(%ebp), %eax
    addl $8, %eax
    movl (%eax), %eax
    pushl %eax
    movl 8(%ebp), %eax
    addl $12, %eax
    movl (%eax), %eax
    pushl %eax
    popl %eax
    popl %ebx
    cmpl %eax, %ebx
    setl %al
    movzbl %al, %eax
    pushl %eax
    popl %eax
    movl $1, %eax
    cmpl %eax, %ebx
    jne ElseLab1
ThenLab1:
    pushl $CONST_STR3
    movl 8(%ebp), %eax
    pushl %eax
    movl (%esp), %eax
    movl 4(%eax), %ebx
    addl $12, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
    jmp Endif1
ElseLab1:
    pushl $CONST_STR4
    movl 8(%ebp), %eax
    pushl %eax
    movl (%esp), %eax
    movl 4(%eax), %ebx
    addl $12, %ebx
    movl (%ebx), %eax
    call *%eax
    addl $8, %esp
Endif1:
    movl %ebp, %esp
    popl %ebp
    ret
.type String_length, @function
String_length:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.type String_concat, @function
String_concat:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.type String_substr, @function
String_substr:
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
.global _start
_start:
    pushl $24
    call malloc
    movl $2, %ebx
    movl %ebx, (%eax)
    movl $Main_dispatch_table, %ebx
    movl %ebx, 4(%eax)
    movl $0, %ebx
    movl %ebx, 8(%eax)
    movl $0, %ebx
    movl %ebx, 12(%eax)
    movl $0, %ebx
    movl %ebx, 16(%eax)
    movl $0, %ebx
    movl %ebx, 20(%eax)
   pushl %eax
   call Main_main
   addl $4, %eax
   movl $1, %eax
   movl $0, %ebx
   int $0x80
