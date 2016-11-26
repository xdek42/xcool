#include "layout.h"
#include "semant.h"
#include <vector>
#include <string>
#include "AstTree.h"
#include "SymbolTab.h"
#include "auxiliary.h"
#include <fstream>
#include "codegen.h"

using namespace xcool::ast;
using xcool::Layout;
using std::shared_ptr;
using std::vector;
using std::string;
using xcool::TreeNode;
using xcool::InherTree;
using xcool::SymbolTable;

vector<string> data_segment;
vector<shared_ptr<Layout>> class_layouts;
vector<string> text_segment;

namespace {
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
    SymbolTable symbol_table;
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
        for (auto &e : expr.expr_list) {
            e->accept_visitor(code_generator);
        }
    }
    void CodeGenVisitor::visit(NewExpr &) {}
    void CodeGenVisitor::visit(LetExpr &) {}
    void CodeGenVisitor::visit(AssExpr &expr) 
    {
        expr.value->accept_visitor(code_generator);
        std::string offset = symbol_table.get_value(expr.name);
        text_segment.push_back("    movl 8(%ebp), %eax");
        text_segment.push_back("    addl $" + offset + ", %eax");
        text_segment.push_back("    addl $4, %eax");
        text_segment.push_back("    popl %ebx");
        text_segment.push_back("    movl %ebx, (%eax)");
    }
    void CodeGenVisitor::visit(UnaryExpr &) {}
    void CodeGenVisitor::visit(DisExpr &expr)
    {   
        for (int i = expr.argument_list.size() - 1; i >= 0; i--) {
            expr.argument_list[i]->accept_visitor(code_generator);
        }
        //object is self
        expr.object->accept_visitor(code_generator);

        int offset = -1;
        for (const auto &layout : class_layouts) {
            if (layout->name == expr.type) {
                for (const auto &fun : layout->dis_table->fun_table) {
                    offset++;
                    if (get_method_name(fun) == expr.method_name) {
                        break;
                    }
                }
                break;
            }
        }
        text_segment.push_back("    movl (%esp), %eax");
        text_segment.push_back("    movl (%eax), %ebx");
        text_segment.push_back("    addl $" + int2str(offset*4) + ", %ebx");
        text_segment.push_back("    movl (%ebx), %eax");
        text_segment.push_back("    call *%eax");
        text_segment.push_back("    addl $" + int2str((expr.argument_list.size() + 1)*4) + ", %esp");
    }
    void CodeGenVisitor::visit(IntExpr &expr) 
    {
        text_segment.push_back("    pushl $" + expr.value);
    }
    void CodeGenVisitor::visit(BoolExpr &)  {}
    void CodeGenVisitor::visit(StrExpr &expr) 
    {
        data_segment.push_back("CONST_STR1:");
        data_segment.push_back("    .asciz \"" + expr.value + "\"");
        text_segment.push_back("    pushl $CONST_STR1");
    }
    void CodeGenVisitor::visit(IdExpr &expr) 
    {
        std::string offset = symbol_table.get_value(expr.name);
        text_segment.push_back("    movl 8(%ebp), %eax");
        text_segment.push_back("    addl $" + offset + ", %eax");
        text_segment.push_back("    pushl %eax");
    }
    void CodeGenVisitor::visit(WhileExpr &) {}
    void CodeGenVisitor::visit(IfExpr &) {}
    void CodeGenVisitor::visit(BinExpr &expr) 
    {
        switch (expr.op) {
            case '+' :
                expr.left->accept_visitor(code_generator);
                expr.right->accept_visitor(code_generator);
                text_segment.push_back("    popl %eax");
                text_segment.push_back("    popl %ebx");
                text_segment.push_back("    addl $4, %eax");
                text_segment.push_back("    addl $4, %ebx");
                text_segment.push_back("    movl (%eax), %eax");
                text_segment.push_back("    movl (%ebx), %ebx");
                text_segment.push_back("    addl %eax, %ebx");
                text_segment.push_back("    pushl %ebx");
                break;
            default:
                break;
        }
    }
    void CodeGenVisitor::visit(CaseExpr &) {}

    bool is_basic_class(std::string name)
    {
        if (name == "Object" || name == "IO" || name == "Int" || name == "String" || name == "Bool")
            return true;
        else 
            return false;
    }
    void emit_method_code(shared_ptr<Layout> layout, std::unique_ptr<Method> &method) 
    {
        symbol_table.enter_scope();
        for (int i  = 0; i < layout->attr.size(); i++) {
            symbol_table.insert(layout->attr[i], int2str(layout->offset[i]));
        }
        string method_name = layout->name + "_" + method->name;
        //函数声明
        text_segment.push_back(".global " + method_name);
        text_segment.push_back(".type " + method_name + ", @function");
        text_segment.push_back(method_name + ":");
        //准备工作
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        //函数参数符号表
        for (int i = method->formal_list.size() - 1; i >= 0; i--) {
            symbol_table.insert(method->formal_list[i]->name, int2str(4*i + 4) + "(%ebp)");
        }
        symbol_table.insert("self", "0");
        method->body->accept_visitor(code_generator);
        //返回
        symbol_table.exit_scope();
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
    }
    void emit_basic_class_code()
    {
        //emit out_string code:
        text_segment.push_back(".global IO_out_string");
        text_segment.push_back(".type IO_out_string, @function");
        text_segment.push_back("IO_out_string:");
        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    pushl 12(%ebp)");
        text_segment.push_back("    call puts");
        text_segment.push_back("    addl $8, %esp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
        //emit Object_abort
        text_segment.push_back(".global Object_abort");
        text_segment.push_back(".type Object_abort, @function");
        text_segment.push_back("Object_abort:");
        text_segment.push_back("    ret");
        //emit Object_type_name
        text_segment.push_back(".global Object_type_name");
        text_segment.push_back(".type Object_type_name, @function");
        text_segment.push_back("Object_type_name:");
        text_segment.push_back("    ret");
        //emit Object_copy
        text_segment.push_back(".global Object_copy");
        text_segment.push_back(".type Object_copy, @function");
        text_segment.push_back("Object_copy:");
        text_segment.push_back("    ret");
        //emit IO_out_int
        
        data_segment.push_back("OUT_INT:");
        data_segment.push_back("    .asciz \"%d\\n\"");

        text_segment.push_back(".global IO_out_int");
        text_segment.push_back(".type IO_out_int, @function");
        text_segment.push_back("IO_out_int:");

        text_segment.push_back("    pushl %ebp");
        text_segment.push_back("    movl %esp, %ebp");
        text_segment.push_back("    movl 12(%ebp), %eax");
        text_segment.push_back("    addl $4, %eax");
        text_segment.push_back("    pushl (%eax)");
        text_segment.push_back("    pushl $OUT_INT");
        text_segment.push_back("    call printf");
        text_segment.push_back("    addl $8, %esp");
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
        //emit IO_in_string
        text_segment.push_back(".global IO_in_string");
        text_segment.push_back(".type IO_in_string, @function");
        text_segment.push_back("IO_in_string:");
        text_segment.push_back("    ret");
        //emit IO_in_int
        text_segment.push_back(".global IO_in_int");
        text_segment.push_back(".type IO_in_int, @function");
        text_segment.push_back("IO_in_int:");
        text_segment.push_back("    ret");
    }
    void emit_class_code(shared_ptr<Layout> layout, shared_ptr<TreeNode> node)
    {
        if (is_basic_class(node->node->name)) {
            return;
        };
        for(auto &method : node->node->method_list) {
            emit_method_code(layout, method);
        }
    }
    void _emit_code(vector<shared_ptr<Layout>> &layouts, shared_ptr<TreeNode> root)
    {
        std::string class_name = root->node->name;
        for (auto layout : layouts) {
            if (layout->name == class_name)
                emit_class_code(layout, root);
        }
        for (auto son_node : root->son_list) {
            _emit_code(layouts, son_node);
        }
    }
}

void xcool::emit_code(vector<shared_ptr<Layout>> &layouts, xcool::InherTree &cool_program, std::ofstream &out_file)
{
    class_layouts = layouts;
    shared_ptr<TreeNode> root = cool_program.get_root();
    emit_basic_class_code();
    _emit_code(layouts, root);
    //输出数据段
    out_file << ".section .data" << std::endl;
    for (auto &str : data_segment)
        out_file << str << std::endl;
    //输出函数表
    out_file << std::endl;
    for (auto lay : layouts) {
        out_file << lay->name + "_dispatch_table:" << std::endl;
        for (auto method : lay->dis_table->fun_table) {
            out_file << "    .long " << method << std::endl;
        }
        out_file << std::endl;
    }
    //输出代码段
    out_file << ".section .text" << std::endl;
    for (auto &str : text_segment)
        out_file << str << std::endl;
    out_file << std::endl;
    out_file << ".global _start" << std::endl;
    out_file << "_start:" << std::endl;
    //temp
    out_file << "   pushl $0" << std::endl;
    out_file << "   pushl $Int_dispatch_table" << std::endl;
    out_file << "   pushl $2" << std::endl;
    out_file << "   pushl $Int_dispatch_table" << std::endl;
    out_file << "   pushl $1" << std::endl;
    out_file << "   pushl $Int_dispatch_table" << std::endl;

    out_file << "   pushl $Main_dispatch_table" << std::endl;
    out_file << "   lea (%esp), %eax" << std::endl;
    out_file << "   pushl %eax" << std::endl;
    out_file << "   call Main_main" << std::endl;
    out_file << "   addl $4, %eax" << std::endl;
    out_file << "   movl $1, %eax" << std::endl;
    out_file << "   movl $0, %ebx" << std::endl;
    out_file << "   int $0x80" << std::endl;
    return;
}