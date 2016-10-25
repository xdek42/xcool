#include "semant.h"
#include "error.h"
#include "auxiliary.h"
#include "AstTree.h"
#include <queue>
#include "SymbolTab.h"

using xcool::ast::Program;
using xcool::InherGraph;
using xcool::SemantChecker;
using xcool::semant_error;
using namespace xcool::ast;
namespace {
    xcool::SymbolTable symbol_table;
    class SemantVisitor : public ExprVisitor {
        public:
            ~SemantVisitor() {}
            virtual void visit(AssExpr &) override;
            virtual void visit(UnaryExpr &) override;
            virtual void visit(DisExpr &) override;
            virtual void visit(BlockExpr &) override;
            virtual void visit(IntExpr &) override;
            virtual void visit(BoolExpr &) override;
            virtual void visit(StrExpr &) override;
            virtual void visit(LetExpr &) override;
            virtual void visit(NewExpr &) override;
            virtual void visit(IdExpr &) override;
            virtual void visit(WhileExpr &) override;
            virtual void visit(IfExpr &) override;
            virtual void visit(BinExpr &) override;
            virtual void visit(CaseExpr &) override;
    };
    SemantVisitor semant_checker;
    void SemantVisitor::visit(AssExpr &expr)
    {
        std::string left_type = symbol_table.get_type(expr.name);
        if (left_type == "") 
            throw semant_error(expr.position, "undefine name: " + expr.name);
        expr.value->accept_visitor(semant_checker);
        std::string right_type = expr.value->static_type;
        if (isvalid_assign(left_type, right_type)) {
            std::cout << left_type << " <- " << right_type << std::endl;
            throw semant_error(expr.position, "Assign expression occur type conflict");
        }
    expr.static_type = left_type; 
    }

    void SemantVisitor::visit(UnaryExpr &expr)
    {
        expr.body->accept_visitor(semant_checker);
        expr.static_type = expr.body->static_type;
    }

    void SemantVisitor::visit(DisExpr &expr)
    {
        if (expr.object) {
            expr.object->accept_visitor(semant_checker);
        }
        for (auto &x : expr.argument_list) 
            x->accept_visitor(semant_checker);
        std::string ob_type;
        if (expr.type != "")
            ob_type = expr.type;
        else
            ob_type = expr.object->static_type;
        expr.static_type = get_return_type(expr.position, ob_type, expr.method_name);
        if (expr.static_type == "SELF_TYPE") 
            expr.static_type = symbol_table.get_type("self");
    }
    void SemantVisitor::visit(BlockExpr &expr)
    {
        for (auto &e : expr.expr_list)
            e->accept_visitor(semant_checker);
        expr.static_type = expr.expr_list[expr.expr_list.size() - 1]->static_type;
    }

    void SemantVisitor::visit(IntExpr &expr)
    {
        expr.static_type = "Int";
    }
    void SemantVisitor::visit(BoolExpr &expr)
    {
        expr.static_type = "Bool";
    }
    void SemantVisitor::visit(StrExpr &expr)
    {
        expr.static_type = "String";
    }
    void SemantVisitor::visit(LetExpr &expr)
    {
        symbol_table.enter_scope();
        for (auto &attr : expr.var_list) {
            symbol_table.insert(attr->name, attr->type);
            if (attr->initial)
                attr->initial->accept_visitor(semant_checker);
        }
        expr.expr->accept_visitor(semant_checker);
        expr.static_type = expr.expr->static_type;
        symbol_table.exit_scope();
    }

    void SemantVisitor::visit(NewExpr &expr)
    {
        expr.static_type = expr.type;
    } 

    void SemantVisitor::visit(IdExpr &expr)
    {
        std::string type = symbol_table.get_type(expr.name);
        if (type == "")
            throw semant_error(expr.position, "undefined name: " + expr.name);
        expr.static_type = type;
    }

    void SemantVisitor::visit(WhileExpr &expr)
    {
        expr.condition->accept_visitor(semant_checker);
        expr.body->accept_visitor(semant_checker);
        expr.static_type = "Object";
    }

    void SemantVisitor::visit(IfExpr &expr)
    {
        expr.condition->accept_visitor(semant_checker);
        expr.then_body->accept_visitor(semant_checker);
        expr.else_body->accept_visitor(semant_checker);
        std::string then_type = expr.then_body->static_type;
        std::string else_type = expr.else_body->static_type;
        expr.static_type = "Undetermined";
    }

    void SemantVisitor::visit(BinExpr &expr)
    {
        expr.left->accept_visitor(semant_checker);
        expr.right->accept_visitor(semant_checker);
        std::string left_type = expr.left->static_type;
        std::string right_type = expr.right->static_type;
        if (left_type != right_type)
            throw semant_error(expr.position, "type confict");
        expr.static_type = left_type;
    }

    void SemantVisitor::visit(CaseExpr &expr)
    {
        expr.value->accept_visitor(semant_checker);
        for (auto &branch : expr.branch_list) {
            symbol_table.enter_scope();
            symbol_table.insert(branch->name, branch->type);
            branch->body->accept_visitor(semant_checker);
            symbol_table.exit_scope();
        }
        expr.static_type = "Undetermined";
}
}

//安装基本类
void SemantChecker::install_base_class(std::shared_ptr<InherGraph> root) 
{
    root->parent = nullptr;
    //安装Object类
    root->node = make_unique<Class>();
    root->node->name = "Object";
    auto abort = make_unique<Method>();
    abort->type = "Object";
    abort->name = "abort";
    root->node->method_list.push_back(std::move(abort));
    auto type_name = make_unique<Method>();
    type_name->type = "String";
    type_name->name = "type_name";
    root->node->method_list.push_back(std::move(type_name));
    auto copy = make_unique<Method>();
    copy->type = "SELF_TYPE";
    copy->name = "copy";
    root->node->method_list.push_back(std::move(copy));
    //安装IO
    std::shared_ptr<InherGraph> son = std::make_shared<InherGraph>();
    son->node = make_unique<Class>();
    son->node->name = "IO";
    // 安装IO类的out_string函数
    auto out_string = make_unique<Method>();
    out_string->name = "out_string";
    out_string->type = "SELF_TYPE";
    out_string->formal_list.push_back(make_unique<Formal>("x", "string"));
    son->node->method_list.push_back(std::move(out_string));
    // 安装IO类的out_int函数
    auto out_int = make_unique<Method>();
    out_int->name = "out_int";
    out_int->type = "SELF_TYPE";
    out_int->formal_list.push_back(make_unique<Formal>("x", "Int"));
    son->node->method_list.push_back(std::move(out_int));
    //安装IO类in_string函数
    auto in_string = make_unique<Method>();
    in_string->name = "in_string";
    in_string->type = "String";
    in_string->formal_list.push_back(make_unique<Formal>("x", "string"));
    son->node->method_list.push_back(std::move(in_string));
    //安装IO类in_int函数
    auto in_int = make_unique<Method>();
    in_int->name = "in_int";
    in_int->type = "Int";
    in_int->formal_list.push_back(make_unique<Formal>("x", "Int"));
    son->node->method_list.push_back(std::move(in_int));
    
    son->parent = root;
    root->son_list.push_back(son);

    //安装String 
    son = std::make_shared<InherGraph>();
    son->node = make_unique<Class>();
    son->node->name = "String";
    auto length = make_unique<Method>();
    length->type = "Int";
    length->name = "length";
    son->node->method_list.push_back(std::move(length));
    auto concat = make_unique<Method>();
    concat->type = "String";
    concat->name = "concat";
    concat->formal_list.push_back(make_unique<Formal>("s", "String"));
    son->node->method_list.push_back(std::move(concat));
    auto substr = make_unique<Method>();
    substr->type = "String";
    substr->name = "substr";
    substr->formal_list.push_back(make_unique<Formal>("i", "Int"));
    substr->formal_list.push_back(make_unique<Formal>("l", "Int"));
    son->node->method_list.push_back(std::move(substr));
    
    son->parent = root;
    root->son_list.push_back(son);

    //安装Int
    son = std::make_shared<InherGraph>();
    son->node = make_unique<Class>();
    son->node->name = "Int";
    
    son->parent = root;
    root->son_list.push_back(son);
    
    //安装Bool
    son = std::make_shared<InherGraph>();
    son->node = make_unique<Class>();
    son->node->name = "Bool";
    
    son->parent = root;
    root->son_list.push_back(son); 
}

//在inherGraph中查找某个类节点
std::shared_ptr<InherGraph> InherGraph::find_node(std::shared_ptr<InherGraph> root, std::string name)
{
    if (root->node->name == name)
        return root;
        else {
            for (const auto & node : root->son_list) {
                std::shared_ptr<InherGraph> result = find_node(node, name);
                if (result)
                    return result;
            }
        }
    return nullptr;
}
//构建类关系继承图
std::shared_ptr<InherGraph> SemantChecker::build_graph(Program &program)
{
    std::shared_ptr<InherGraph> root = std::make_shared<InherGraph>();
    install_base_class(root);
    for (auto &cls : program.class_list) {
        std::shared_ptr<InherGraph> son = std::make_shared<InherGraph>();
        if ((cls->parent).empty()) {
            son->parent = root;
            son->node = std::move(cls);
            root->son_list.push_back(son);
        }
        else {
            std::shared_ptr<InherGraph> parent_node = root->find_node(root, cls->parent);
            if(!parent_node)
                throw semant_error(cls->position, "undefined parent class" + cls->parent);
            son->node = std::move(cls);
            son->parent = parent_node;
            parent_node->son_list.push_back(son);
        }
    }
    return root;
}

//Check that the graph is well-formed.
void SemantChecker::checkCyclic(std::shared_ptr<InherGraph> root)
{
}

void InherGraph::print_graph(std::shared_ptr<InherGraph> root)
{
    std::queue<std::shared_ptr<InherGraph>> que;
    que.push(root);
    int count = 0, current_sum = 1, next_sum = 0;
    while (!que.empty()) {
        std::shared_ptr<InherGraph> node = que.front();
        std::cout << node->node->name << " ";
        count++;
        next_sum += node->son_list.size();
        for (auto x : node->son_list)
            que.push(x);
        if (count == current_sum) {
            std::cout << std::endl;
            current_sum = next_sum;
            next_sum = 0;
            count = 0;
        }
        que.pop();
    }
}

void Semant::semant_check(std::shared_ptr<InherGraph> root)
{
    symbol_table.enter_scope();
    symbol_table.insert("self", root->node->name);
    for (auto &attr : root->node->attribute_list) {
        if (!symbol_table.insert(attr->name, attr->type))
            throw semant_error(attr->position, "redefined name: " + attr->name);
        if (attr->initial)
            attr->initial->accept_visitor(semant_checker);
    }
    for (auto &method : root->node->method_list) {
        symbol_table.enter_scope();
        for (auto &formal : method->formal_list) {
            if (!symbol_table.insert(formal->name, formal->type)) 
                throw semant_error(formal->position, "redefined name: " formal->name);
        }
        if (method->body) {
            method->body->accept_visitor(semant_checker);
            std::string method_type = method->type;
            if (method_type == "SELF_TYPE")
                method_type = symbol_table.get_type("self");
            if (!isvalid_assign(method_type, method->body->static_type))
                throw semant_error(method->position, "method return type conflict");
        }
        symbol_table.exit_scope();
    }
    for (auto &cls : root->son_list) {
        semant_check(cls);
    }
    symbol_table.exit_scope();
}
