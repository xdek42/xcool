#include "codegen.h"
#include "SymbolTab.h"
#include "auxiliary.h"

using std::string;
using std::vector;
using std::shared_ptr;
using xcool::Layout;
using namespace xcool::ast;

namespace {
    //the text segment and data segment in assembly code
    vector<string> data_segment;
    vector<string> text_segment;
    xcool::SymbolTable symbol_table;
    std::vector<std::shared_ptr<xcool::Layout>> class_layouts;
    int const_index = 1;

    bool is_para_offset(const string &offset) 
    {
        int end = offset.size() - 1;
        return offset[end] == ')';
    } 
    std::string get_method_name(const std::string &name)
    {
        std::string ret;
        int flag = 0;
        for (int i = 0; i < name.size(); i++) {
            if (name[i] == '_' && flag == 0) {
                flag = 1;
                continue;
            }
            if (flag) {
                ret = ret + name[i];
            }
        }
        return ret;
    }
    class CodeGenVisitor : public ExprVisitor {
        public:
            ~CodeGenVisitor() {}
            virtual void visit(AssExpr &) override;
            virtual void visit(UnaryExpr &) override;
            virtual void visit(DisExpr &) override;
            virtual void visit(IntExpr &) override;
            virtual void visit(BoolExpr &) override;
            virtual void visit(StrExpr &) override;
            virtual void visit(IdExpr &) override;
            virtual void visit(WhileExpr &) override;
            virtual void visit(IfExpr &) override;
            virtual void visit(BinExpr &) override;
            virtual void visit(CaseExpr &) override;
            virtual void visit(LetExpr &) override;
            virtual void visit(NewExpr &) override;
            virtual void visit(BlockExpr &) override;
    } code_generator;
    void CodeGenVisitor::visit(BlockExpr &expr) 
    {
        for (const auto &e : expr.expr_list) {
            e->accept_visitor(code_generator);
        }
    }
    void CodeGenVisitor::visit(NewExpr &) {}
    void CodeGenVisitor::visit(LetExpr &) {}
    //delay
    void CodeGenVisitor::visit(AssExpr &expr) {}
    void CodeGenVisitor::visit(UnaryExpr &) {}
    void CodeGenVisitor::visit(DisExpr &expr) 
    {
        for (int i = expr.argument_list.size() - 1; i >=0; i--) {
            expr.argument_list[i]->accept_visitor(code_generator);
        }
        expr.object->accept_visitor(code_generator);

        int offset = -1;
        for (const auto &layout : class_layouts) {
             if (layout->name == expr.type) {
                 for (const auto &fun : layout->dis_table) {
                     offset++;
                     if (get_method_name(fun) == expr.method_name) {
                         break;
                     }
                 }
                 break;
             }
         }
         text_segment.push_back("    movl (%esp), %eax");
         text_segment.push_back("    movl 4(%eax), %ebx");
         text_segment.push_back("    addl $" + int2str(offset*4) + ", %ebx");
         text_segment.push_back("    movl (%ebx), %eax");
         text_segment.push_back("    call *%eax");
         text_segment.push_back("    addl $" + int2str((expr.argument_list.size() + 1)*4) + ", %esp");
    }
    void CodeGenVisitor::visit(IntExpr &expr) {}
    void CodeGenVisitor::visit(BoolExpr &)  {}
    void CodeGenVisitor::visit(StrExpr &expr) 
    {
        data_segment.push_back("CONST_STR" + int2str(const_index) + ":");
        data_segment.push_back("    .asciz \"" + expr.value + "\"");
        text_segment.push_back("    pushl $CONST_STR" + int2str(const_index++));
    }
    void CodeGenVisitor::visit(IdExpr &expr) 
    {
        std::string offset = symbol_table.get_value(expr.name);
        if (is_para_offset(offset)) {
            text_segment.push_back("    movl " + offset + ", %eax");
            text_segment.push_back("    pushl %eax");
        }
        else {
            text_segment.push_back("    movl 8(%ebp), %eax");
            text_segment.push_back("    addl $" + offset + ", %eax");

            if (expr.static_type == "Int" || expr.static_type == "Bool") {
                text_segment.push_back("    movl (%eax), %eax");
            }

            text_segment.push_back("    pushl %eax");
        }
    }
    void CodeGenVisitor::visit(WhileExpr &) {}
    void CodeGenVisitor::visit(IfExpr &) {}
    void CodeGenVisitor::visit(BinExpr &expr) {}
    void CodeGenVisitor::visit(CaseExpr &) {}
    
    
    void emit_asm_malloc(int length);
    bool is_basic_class(std::string name)
    {
        if (name == "Object" || name == "IO" || name == "Int" || name == "String" || name == "Bool")
            return true;
        else 
            return false;
    } 

    void emit_IO_code()
    {
        //emit out_string function code
        text_segment.push_back(".type IO_out_string, @function");
        text_segment.push_back("IO_out_string:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    pushl 12(%ebp)");
        text_segment.push_back("    call puts");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");

        text_segment.push_back(".type IO_out_int, @function");
        text_segment.push_back("IO_out_int:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
        
        text_segment.push_back(".type IO_in_string, @function");
        text_segment.push_back("IO_in_string:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
        
        text_segment.push_back(".type IO_in_int, @function");
        text_segment.push_back("IO_in_int:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");

    }

    void emit_Object_code()
    {
        text_segment.push_back(".type Object_abort, @function");
        text_segment.push_back("Object_abort:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
        
        text_segment.push_back(".type Object_type_name, @function");
        text_segment.push_back("Object_type_name:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");

        text_segment.push_back(".type Object_layout, @function");
        text_segment.push_back("Object_layout:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");



    }

    void emit_String_code()
    {
        text_segment.push_back(".type String_length, @function");
        text_segment.push_back("String_length:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");

        text_segment.push_back(".type String_concat, @function");
        text_segment.push_back("String_concat:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");

        text_segment.push_back(".type String_substr, @function");
        text_segment.push_back("String_substr:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");

    }

    void emit_basic_class_code(std::string class_name) 
    {
        if (class_name == "IO") {
            emit_IO_code();
        }

        if (class_name == "Object") {
            emit_Object_code();
        }

        if (class_name == "String") {
            emit_String_code();
        }
    }
    //emit class code
    void emit_class_code(shared_ptr<Layout> layout) 
    {
        symbol_table.enter_scope();
        //add class attr into symbolk table
        for (int i = 0; i < layout->attrs_name.size(); i++) {
            symbol_table.insert(layout->attrs_name[i], int2str(layout->attrs_offset[i]));
        }

        for (const auto &method : layout->method_list) {
            symbol_table.enter_scope();
            //add method parameter to symbol table;
            for (int i = method->formal_list.size() - 1; i >=0; i--) {
                symbol_table.insert(method->formal_list[i]->name, int2str(i*4 + 12) + "(%ebp)");
            }
            symbol_table.insert("self", "8(%ebp)");
            text_segment.push_back("#" + layout->name + "_" + method->name);
            text_segment.push_back(".global " + layout->name + "_" + method->name);
            text_segment.push_back(".type " + layout->name + "_" + method->name + ", " + "@function");
            text_segment.push_back(layout->name + "_" + method->name + ":");
            //emit init method code
            text_segment.push_back("    pushl %ebp");
            text_segment.push_back("    movl %esp, %ebp");
            method->body->accept_visitor(code_generator);
            text_segment.push_back("    movl %ebp, %esp");
            text_segment.push_back("    popl %ebp");
            text_segment.push_back("    ret");
            symbol_table.exit_scope();
        }
        symbol_table.exit_scope();
    }

}

void xcool::code_gen(const std::vector<std::shared_ptr<xcool::Layout>> &layout_list, std::ofstream &out_file)
{
    class_layouts = layout_list;
    for (const auto &class_layout : layout_list) {
        if(is_basic_class(class_layout->name)) {
            emit_basic_class_code(class_layout->name);
        }
        else {
            emit_class_code(class_layout);
        }
    }

    //output code into outfile

    out_file << ".section .data" << std::endl;
    for (const auto &layout : layout_list) {
        out_file << layout->name + "_dispatch_table:" << std::endl;
        for (const auto &method : layout->dis_table) {
            out_file << "   .long " << method << std::endl;
        }
    }

    for (const auto &data : data_segment)
        out_file << data << std::endl;

    out_file << ".section .text" << std::endl;
    for (const auto &inst : text_segment) {
        out_file << inst << std::endl;
    }

    //emit _start code
    
    out_file << ".global _start" << std::endl;
    out_file << "_start:" << std::endl;

    int class_index = 0;
    for (const auto &layout : layout_list) {
        if (layout->name == "Main") {

            out_file << "    pushl $" + int2str(layout->length) << std::endl;
            out_file << "    call malloc" << std::endl;
            
            out_file << "    movl $" + int2str(class_index) + ", %ebx" << std::endl;
            out_file << "    movl %ebx, (%eax)" << std::endl;

            out_file << "    movl $Main_dispatch_table, %ebx" << std::endl;
            out_file << "    movl %ebx, 4(%eax)" << std::endl;


            int attr_num = layout->attrs_name.size();

            for (int i = 0; i < attr_num; i++) {
                out_file << "    movl $0, %ebx" << std::endl;
                out_file << "    movl %ebx, " + int2str((i+2)*4) + "(%eax)" << std::endl;
            }
            break;
        }
        class_index++;
    }

    out_file << "   pushl %eax" << std::endl;
    out_file << "   call Main_main" << std::endl;
    out_file << "   addl $4, %eax" << std::endl;
    out_file << "   movl $1, %eax" << std::endl;
    out_file << "   movl $0, %ebx" << std::endl;
    out_file << "   int $0x80" << std::endl;

}