#include "codegen.h"
#include "SymbolTab.h"
#include "auxiliary.h"

using std::string;
using std::vector;
using std::shared_ptr;
using xcool::Layout;

namespace {
    //the text segment and data segment in assembly code
    vector<string> data_segment;
    vector<string> text_segment;
    xcool::SymbolTable symbol_table;
    int const_index = 1;
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
    void CodeGenVisitor::visit(DisExpr &expr) {}
    void CodeGenVisitor::visit(IntExpr &expr) {}
    void CodeGenVisitor::visit(BoolExpr &)  {}
    void CodeGenVisitor::visit(StrExpr &expr) {}
    void CodeGenVisitor::visit(IdExpr &expr) {}
    void CodeGenVisitor::visit(WhileExpr &) {}
    void CodeGenVisitor::visit(IfExpr &) {}
    void CodeGenVisitor::visit(BinExpr &expr) {}
    void CodeGenVisitor::visit(CaseExpr &) {}

    bool is_basic_class(std::string name)
    {
        if (name == "Object" || name == "IO" || name == "Int" || name == "String" || name == "Bool")
            return true;
        else 
            return false;
    }   
    bool is_para_offset(string offset) 
    {
        int end = offset.size() - 1;
        return offset[end] == ')';
    }

    //emit class code
    void emit_class_code(shared_ptr<Layout> layout) 
    {
        symbol_table.enter_scope();
        //add class attr into symbolk table
        for (int i = 0; i < layout->attrs_name.size(); i++) {
            symbol_table.insert(attrs_name[i]);
            symbol_table.insert(int2str(attrs_offset));
        }

        for (const auto &method : layout->method_list) {
            symbol_table.enter_scope();
            //add method parameter to symbol table;
            for (int i = method->formal_list.size() - 1; i >=0; i--) {
                symbol_table.insert(method->formal_list[i]->name, int2str(i*4 + 12) + "(%ebp)");
            }
            symbol_table.insert("self", "8(%ebp)");
            text_segment.push_back("# " + layout->name + "_" + method->name);
            text_segment.push_back(".global " + layout->name + "_" + method->name);
            text_segment.push_back(".type " + layout->name + "_" + method->name + "," + "@function");
            text_segment.push_back(layout->name + "_" + method->name);
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
void xcool::code_gen(vector<shared_ptr<Layout>> *layout_list, std::ofstream &out_file)
{
    for (const auto &class_layout : layout_list) {
        if (is_basic_class(class_layout->name)) {
            emit_basic_class_code(class_layout->name);
        }
        else {
            emit_class_code(class_layout);
        }

    }
}