#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "error.h"
#include "auxiliary.h"
#include "AstTree.h"

namespace xcool {
    class Parser {
        public:
            Parser(xcool::Tokenizer &l) : lexer(l) {}
            xcool::ast::Program get_result();
        private:
            xcool::Tokenizer &lexer;
            std::unique_ptr<xcool::ast::Class> parser_class();
            std::unique_ptr<xcool::ast::Method> parser_method();
            std::unique_ptr<xcool::ast::Attribute> parser_attribute();
            std::unique_ptr<xcool::ast::Formal> parser_formal();
            std::unique_ptr<xcool::ast::Expr> parser_expr();
            std::unique_ptr<xcool::ast::Expr> parser_not();
            std::unique_ptr<xcool::ast::Expr> parser_logical();
            std::unique_ptr<xcool::ast::Expr> parser_add();
            std::unique_ptr<xcool::ast::Expr> parser_mul();
            std::unique_ptr<xcool::ast::Expr> parser_isvoid();
            std::unique_ptr<xcool::ast::Expr> parser_dispatch();
            void parser_dispatch_tail(std::unique_ptr<xcool::ast::DisExpr> &);
            std::unique_ptr<xcool::ast::Expr> parser_base();
            std::unique_ptr<xcool::ast::Expr> parser_string();
            std::unique_ptr<xcool::ast::Expr> parser_bool();
            std::unique_ptr<xcool::ast::Expr> parser_int();
            std::unique_ptr<xcool::ast::Expr> parser_lparen();
            std::unique_ptr<xcool::ast::Expr> parser_block();
            std::unique_ptr<xcool::ast::Expr> parser_let();
            std::unique_ptr<xcool::ast::Expr> parser_new();
            std::unique_ptr<xcool::ast::Expr> parser_while();
            std::unique_ptr<xcool::ast::Expr> parser_if();
            std::unique_ptr<xcool::ast::Expr> parser_case();
            std::unique_ptr<xcool::ast::CaseBranch> parser_case_branch();
            std::unique_ptr<xcool::ast::Expr> parser_identifier();

            Token assert(int type, std::string msg);
    };
}

#endif