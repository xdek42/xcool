.section .data
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
   .long Main_menu
   .long Main_prompt
   .long Main_get_int
   .long Main_is_even
   .long Main_class_type
   .long Main_print
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
A_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long A_value
   .long A_set_var
   .long A_method1
   .long A_method2
   .long A_method3
   .long A_method4
   .long A_method5
B_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long A_value
   .long A_set_var
   .long A_method1
   .long A_method2
   .long A_method3
   .long A_method4
   .long B_method5
C_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long A_value
   .long A_set_var
   .long A_method1
   .long A_method2
   .long A_method3
   .long A_method4
   .long C_method5
   .long C_method6
D_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long A_value
   .long A_set_var
   .long A_method1
   .long A_method2
   .long A_method3
   .long A_method4
   .long B_method5
   .long D_method7
E_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long A_value
   .long A_set_var
   .long A_method1
   .long A_method2
   .long A_method3
   .long A_method4
   .long B_method5
   .long D_method7
   .long E_method6
A2I_dispatch_table:
   .long Object_abort
   .long Object_type_name
   .long Object_layout
   .long A2I_c2i
   .long A2I_i2c
   .long A2I_a2i
   .long A2I_a2i_aux
   .long A2I_i2a
   .long A2I_i2a_aux
# Main_menu
.global Main_menu
.type Main_menu,@function
Main_menu
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# Main_prompt
.global Main_prompt
.type Main_prompt,@function
Main_prompt
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# Main_get_int
.global Main_get_int
.type Main_get_int,@function
Main_get_int
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# Main_is_even
.global Main_is_even
.type Main_is_even,@function
Main_is_even
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# Main_class_type
.global Main_class_type
.type Main_class_type,@function
Main_class_type
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# Main_print
.global Main_print
.type Main_print,@function
Main_print
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# Main_main
.global Main_main
.type Main_main,@function
Main_main
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_value
.global A_value
.type A_value,@function
A_value
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_set_var
.global A_set_var
.type A_set_var,@function
A_set_var
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_method1
.global A_method1
.type A_method1,@function
A_method1
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_method2
.global A_method2
.type A_method2,@function
A_method2
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_method3
.global A_method3
.type A_method3,@function
A_method3
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_method4
.global A_method4
.type A_method4,@function
A_method4
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A_method5
.global A_method5
.type A_method5,@function
A_method5
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# B_method5
.global B_method5
.type B_method5,@function
B_method5
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# C_method6
.global C_method6
.type C_method6,@function
C_method6
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# C_method5
.global C_method5
.type C_method5,@function
C_method5
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# D_method7
.global D_method7
.type D_method7,@function
D_method7
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# E_method6
.global E_method6
.type E_method6,@function
E_method6
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A2I_c2i
.global A2I_c2i
.type A2I_c2i,@function
A2I_c2i
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A2I_i2c
.global A2I_i2c
.type A2I_i2c,@function
A2I_i2c
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A2I_a2i
.global A2I_a2i
.type A2I_a2i,@function
A2I_a2i
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A2I_a2i_aux
.global A2I_a2i_aux
.type A2I_a2i_aux,@function
A2I_a2i_aux
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A2I_i2a
.global A2I_i2a
.type A2I_i2a,@function
A2I_i2a
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
# A2I_i2a_aux
.global A2I_i2a_aux
.type A2I_i2a_aux,@function
A2I_i2a_aux
    pushl %ebp
    movl %esp, %ebp
    movl %ebp, %esp
    popl %ebp
    ret
