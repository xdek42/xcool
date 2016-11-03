class_name: Object
class_length 4
Object_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy

class_name: IO
class_length 4
IO_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy
    .long IO_out_string
    .long IO_out_int
    .long IO_in_string
    .long IO_in_int

class_name: Int
class_length 8
Int_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy

class_name: Main
class_length 4
Main_dispatch_table:
    .long Object_abort
    .long Object_type_name
    .long Object_copy
    .long IO_out_string
    .long IO_out_int
    .long IO_in_string
    .long IO_in_int
    .long Main_main

