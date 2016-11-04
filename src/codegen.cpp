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
vector<string> class_layout_segment;
vector<string> text_segment;

namespace {
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
    void CodeGenVisitor::visit(BlockExpr &) {}
    void CodeGenVisitor::visit(NewExpr &) {}
    void CodeGenVisitor::visit(LetExpr &) {}
    void CodeGenVisitor::visit(AssExpr &) {}
    void CodeGenVisitor::visit(UnaryExpr &) {}
    void CodeGenVisitor::visit(DisExpr &expr)
    {
        for (int i = expr.argument_list.size() - 1; i >= 0; i--) {
            expr.argument_list[i]->accept_visitor(code_generator);
        }
        if (expr.method_name == "out_string") {
            text_segment.push_back("    movl $Main_dispatch_table, %ebx");
            text_segment.push_back("    addl $12, %ebx");
            text_segment.push_back("    movl (%ebx), %eax");
            text_segment.push_back("    call *%eax");
        }
    }
    void CodeGenVisitor::visit(IntExpr &) {}
    void CodeGenVisitor::visit(BoolExpr &) {}
    void CodeGenVisitor::visit(StrExpr &expr) 
    {
        data_segment.push_back("CONST_STR1:");
        data_segment.push_back("    .asciz \"" + expr.value + "\"");
        text_segment.push_back("    pushl $CONST_STR1");
    }
    void CodeGenVisitor::visit(IdExpr &) {}
    void CodeGenVisitor::visit(WhileExpr &) {}
    void CodeGenVisitor::visit(IfExpr &) {}
    void CodeGenVisitor::visit(BinExpr &) {}
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
        symbol_table.insert("self", "4(%ebp)");
        method->body->accept_visitor(code_generator);
        //返回
        symbol_table.exit_scope();
        text_segment.push_back("    movl %ebp, %esp");
        text_segment.push_back("    popl %ebp");
        text_segment.push_back("    ret");
    }
    void emit_class_code(shared_ptr<Layout> layout, shared_ptr<TreeNode> node)
    {
        if (is_basic_class(node->node->name)) {
            for (auto &method : node->node->method_list) {
                string method_name = node->node->name + "_" + method->name;
                text_segment.push_back(".global " + method_name);
                text_segment.push_back(".type " + method_name + ", @function");
                text_segment.push_back(method_name + ":");
                text_segment.push_back("    ret\n");
            }
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
    shared_ptr<TreeNode> root = cool_program.get_root();
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
    out_file << "   pushl $Main_dispatch_table" << std::endl;
    out_file << "   call Main_main" << std::endl;
    out_file << "   movl $1, %eax" << std::endl;
    out_file << "   movl $0, %ebx" << std::endl;
    out_file << "   int $0x80" << std::endl;
    return;
}