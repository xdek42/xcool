#ifndef ASTTREE_H
#define ASTTREE_H

#include <memory>
#include <vector>

namespace xcool {
    namespace ast {
        // visitor for expression node in ast
        class ExprVisitor;
        // Expression node
        class Expr {
            public:
                std::pair<int, int> position;
                virtual ~Expr() {}
                std::string static_type;
                virtual void accept_visitor(ExprVisitor &visitor) = 0;
        };

        class AssExpr : public Expr {
            public:
                std::string name;
                std::unique_ptr<Expr> value;
                virtual void accept_visitor(ExprVisitor &visitor) override;
        };

        class UnaryExpr : public Expr {
            public:
                int op;
                std::unique_ptr<Expr> body;
                virtual void accept_visitor(ExprVisitor &visitor)  override ;
        };

        class DisExpr : public Expr {
            public:
                std::string type;
                std::unique_ptr<Expr> object;
                std::string method_name;
                std::vector<std::unique_ptr<Expr>> argument_list;
                virtual void accept_visitor(ExprVisitor &visitor)  override ;
        };

        class BlockExpr : public Expr {
            public:
                std::vector<std::unique_ptr<Expr>> expr_list;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class IntExpr : public Expr {
            public:
                std::string value;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class BoolExpr : public Expr {
            public:
                std::string value;
                virtual void accept_visitor(ExprVisitor &visitor)  override ;
        };

        class StrExpr : public Expr {
            public:
                std::string value;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };
        class Attribute;
        class LetExpr : public Expr {
            public:
                std::vector<std::unique_ptr<Attribute>> var_list;
                std::unique_ptr<Expr> expr;
                virtual void accept_visitor(ExprVisitor &visitor) override ;

        };

        class NewExpr : public Expr {
            public:
                std::string type;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };
        
        class IdExpr : public Expr {
            public:
                std::string name;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class WhileExpr : public Expr {
            public:
                std::unique_ptr<Expr> condition;
                std::unique_ptr<Expr> body;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class IfExpr : public Expr {
            public:
                std::unique_ptr<Expr> condition;
                std::unique_ptr<Expr> then_body;
                std::unique_ptr<Expr> else_body;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class BinExpr : public Expr {
            public:
                int op;
                std::unique_ptr<Expr> left;
                std::unique_ptr<Expr> right;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class CaseBranch {
            public:
                std::string name;
                std::string type;
                std::unique_ptr<Expr> body;
        };

        class CaseExpr : public Expr {
            public:
                std::unique_ptr<Expr> value;
                std::vector<std::unique_ptr<CaseBranch>> branch_list;
                virtual void accept_visitor(ExprVisitor &visitor) override ;
        };

        class Attribute {
            public:
                std::pair<int, int> position;
                std::string name;
                std::string type;
                std::unique_ptr<Expr> initial;
        };

        class Formal {
            public:
                Formal() = default;
                Formal(std::string n, std::string t) : name(n), type(t) {}
                std::pair<int, int> position;
                std::string name;
                std::string type;
        };

        class Method {
            public:
                std::pair<int, int> position;
                std::string name;
                std::vector<std::unique_ptr<Formal>> formal_list;
                std::string type;
                std::unique_ptr<Expr> body;
        };
        //the Class represents the class defination
        class Class {
            public:
                std::pair<int, int> position;
                std::string name;
                std::string parent;
                std::vector<std::unique_ptr<Attribute>> attribute_list;
                std::vector<std::unique_ptr<Method>> method_list;
        };
        //class program represents the top level structure of cool code
        class Program {
            public:
                std::vector<std::unique_ptr<Class>> class_list;
        };
        
        //the defination of ast visitor
        class ExprVisitor {
            public:
                virtual ~ExprVisitor() {}
                virtual void visit(AssExpr &) = 0;
                virtual void visit(StrExpr &) = 0;
                virtual void visit(UnaryExpr &) = 0;
                virtual void visit(DisExpr &) = 0;
                virtual void visit(BlockExpr &) = 0;
                virtual void visit(IntExpr &) = 0;
                virtual void visit(BoolExpr &) = 0;
                virtual void visit(LetExpr &) = 0;
                virtual void visit(NewExpr &) = 0;
                virtual void visit(IdExpr &) = 0;
                virtual void visit(WhileExpr &) = 0;
                virtual void visit(IfExpr &) = 0;
                virtual void visit(BinExpr &) = 0;
                virtual void visit(CaseExpr &) = 0;
        };
    }
}

#endif