#include "parser.h"
#include "error.h"

using namespace xcool::ast;

//consume a token and verdict it whether hava a right type
xcool::Token xcool::Parser::assert(int type, std::string msg)
{
    Token token = lexer.next_token();
    if (type != token.type)
        throw parser_error(token.position, msg);
    return token;
}

std::unique_ptr<Expr> xcool::Parser::parser_identifier()
{
    Token token = lexer.lookahead(2);
    if (token.type == '(') {
        auto result = make_unique<DisExpr>();
        auto object = make_unique<IdExpr>();
        object->name = "self";
        result->object = std::move(object);
        result->position = token.position;
        parser_dispatch_tail(result);
        return result;
    }
    else {
        token = lexer.next_token();
        auto result = make_unique<IdExpr>();
        result->position = token.position;
        result->name = token.value;
        return result;
    }
}

std::unique_ptr<CaseBranch> xcool::Parser::parser_case_branch()
{
    auto result = make_unique<CaseBranch>();
    Token token = assert(OBJECTID, "syntax error: invalid case branch");
    result->name = token.value;
    assert(':', "syntax error: invalid case branch");
    token = assert(TYPEID, "syntax error: invalid case branch");
    result->type = token.value;
    assert(DARROW, "syntax error: invalid case branch");
    result->body = parser_expr();
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_case() 
{
    Token token = assert(CASE, "syntax error : invalid case expression");
    auto result = make_unique<CaseExpr>();
    result->position = token.position;
    result->value = parser_expr();
    lexer.consume();
    while (1) {
        result->branch_list.push_back(parser_case_branch());
        lexer.consume();
        Token token = lexer.lookahead(1);
        if (token.type == ESAC) {
            lexer.consume();
            return result;
        }
    }
}

std::unique_ptr<Expr> xcool::Parser::parser_if()
{
    Token token = assert(IF, "syntax error : invalid if expression");
    auto result = make_unique<IfExpr>();
    result->position = token.position;
    result->condition = parser_expr();
    assert(THEN, "syntax error: missing then");
    result->then_body = parser_expr();
    assert(ELSE, "syntax error: missing else");
    result->else_body = parser_expr();
    assert(FI, "syntax error: missing fi");
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_while() 
{
    Token token = assert(WHILE, "syntax error : invalid while expression");
    auto result = make_unique<WhileExpr>();
    result->position = token.position;
    result->condition = parser_expr();
    assert(LOOP, "syntax error: missing loop");
    result->body = parser_expr();
    assert(POOL, "syntax error: missing pool");
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_new() 
{
    lexer.consume();
    Token token = assert(TYPEID, "syntax error: invalid new expreesion");
    auto result = make_unique<NewExpr>();
    result->type = token.value;
    result->position = token.position;
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_let() 
{
    Token token = assert(LET, "syntax error: invalid let expression");
    auto result = make_unique<LetExpr>();
    result->position = token.position;
    result->var_list.push_back(parser_attribute());
    while (1) {
        Token token = lexer.lookahead(1);
        if (token.type == ',') {
            lexer.consume();
            result->var_list.push_back(parser_attribute());
        }
        else
            break;
    }
    assert(IN, "syntax error: invalid let expression");
    result->expr = parser_expr();
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_block() 
{
    Token token = assert('{', "syntax error: invalid block");
    auto result = make_unique<BlockExpr>();
    result->position = token.position;
    while (1) {
        Token token = lexer.lookahead(1);
        if (token.type == '}') {
            lexer.consume();
            return result;
        }
        result->expr_list.push_back(parser_expr());
        token = lexer.lookahead(1);
        if (token.type == ';') {
            lexer.consume();
        }
    }
}

std::unique_ptr<Expr> xcool::Parser::parser_lparen() 
{
    lexer.consume();
    auto result = parser_expr();
    Token token = assert(')', "syntax error: missing right paren");
    result->position = token.position;
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_string() 
{
    Token token = lexer.next_token();
    auto result = make_unique<StrExpr>();
    result->position = token.position;
    result->value = token.value;
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_int()
{
    Token token = lexer.next_token();
    auto result = make_unique<IntExpr>();
    result->position = token.position;
    result->value = token.value;
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_bool()
{
    Token token = lexer.next_token();
    auto result = make_unique<BoolExpr>();
    result->position = token.position;
    result->value = token.value;
    return result;
}

std::unique_ptr<Expr> xcool::Parser::parser_base() 
{
    Token token = lexer.lookahead(1);
    switch (token.type) {
        case CASE:
            return parser_case();
        case IF:
            return parser_if();
        case WHILE:
            return parser_while();
        case LET:
            return parser_let();
        case NEW:
            return parser_new();
        case '{':
            return parser_block();
        case '(':
            return parser_lparen();
        case STR_CONST:
            return parser_string();
        case INT_CONST:
            return parser_int();
        case BOOL_CONST:
            return parser_bool();
        case OBJECTID:
            return parser_identifier();
        default:
            throw parser_error(token.position, "syntax error");
    }
}

void xcool::Parser::parser_dispatch_tail(std::unique_ptr<DisExpr> &dis)
{
    Token token = assert(OBJECTID, "syntax error: invalid dispatch");
    dis->method_name = token.value;
    assert('(', "syntax error: missing (");
    token = lexer.lookahead(1);
    if (token.type == ')') {
        lexer.consume();
        return;
    }
    else {
        dis->argument_list.push_back(parser_expr());
        while (1) {
            token = lexer.lookahead(1);
            if (token.type == ',') {
                lexer.consume();
                dis->argument_list.push_back(parser_expr());
                continue;
            }
            if (token.type == ')') {
                lexer.consume();
                return;
            }
        }
    }
}

std::unique_ptr<Expr> xcool::Parser::parser_dispatch()
{
    auto left = parser_base();
    while (lexer.lookahead(1).type == '.' || lexer.lookahead(1).type == '@') {
        auto new_left = make_unique<DisExpr>();
        new_left->position = left->position;
        new_left->object = std::move(left);
        if (lexer.lookahead(1).type == '@') {
            lexer.consume();
            Token token = lexer.next_token();
            new_left->type = token.value;
        }
        assert('.', "syntax error: missing .");
        parser_dispatch_tail(new_left);
        left = std::move(new_left);
    }
    return left;
}

std::unique_ptr<Expr> xcool::Parser::parser_isvoid()
{
    Token token = lexer.lookahead(1);
    switch (token.type) {
        case ISVOID:
        case '~':
            lexer.consume();
            break;
        default:
            return parser_dispatch();
    }
    auto result = make_unique<UnaryExpr>();
    result->position = token.position;
    result->op = token.type;
    result->body = parser_isvoid();
    return result;
}   

std::unique_ptr<Expr> xcool::Parser::parser_mul()
{
    auto left = parser_isvoid();
    while (1) {
        Token token = lexer.lookahead(1);
        switch (token.type) {
            case '*':
            case '/':
                lexer.consume();
                break;
            default:
                return left;
        }
        auto result = make_unique<BinExpr>();
        result->position = token.position;
        result->op = token.type;
        result->left = std::move(left);
        result->right = parser_isvoid();
        left = std::move(result);
    }
}

std::unique_ptr<Expr> xcool::Parser::parser_add()
{
    auto left = parser_mul();
    while (1) {
        Token token = lexer.lookahead(1);
        switch (token.type) {
            case '+' :
            case '-' :
                lexer.consume();
                break;
            default :
                return left;
        } 
        auto result = make_unique<BinExpr>();
        result->position = token.position;
        result->op = token.type;
        result->left = std::move(left);
        result->right = parser_mul();
        left = std::move(result);
    }
}

std::unique_ptr<Expr> xcool::Parser::parser_logical()
{
    auto left = parser_add();
    while (1) {
        Token token = lexer.lookahead(1);
        switch (token.type) {
            case LE:
            case LESS:
            case EQUAL:
                lexer.consume();
                break;
            default:
                return left;
                }
        auto result = make_unique<BinExpr>();
        result->position = token.position;
        result->left = std::move(left);
        result->right = parser_add();
        result->op = token.type;
        left = std::move(result);
        }
}

std::unique_ptr<Expr> xcool::Parser::parser_not()
{
    Token token = lexer.lookahead(1);
    if (token.type == NOT) {
        lexer.consume();
        std::unique_ptr<UnaryExpr> result = make_unique<UnaryExpr>();
        result->position = token.position;
        result->op = token.type;
        result->body = parser_not();
        return result;
    }
    else
        return parser_logical();
}

std::unique_ptr<Expr> xcool::Parser::parser_expr()
{
   if (lexer.lookahead(1).type == OBJECTID && lexer.lookahead(2).type == ASSIGN) {
        Token token = lexer.next_token();
        std::unique_ptr<AssExpr> result = make_unique<AssExpr>();
        result->position = token.position;
        result->name = token.value;
        lexer.consume();
        result->value = parser_not();
        return result;
    }
    else 
        return parser_not();
}

std::unique_ptr<Attribute> xcool::Parser::parser_attribute()
{
    auto result = make_unique<Attribute>();
    Token token = assert(OBJECTID, "syntax error: invalid attribute definition");
    result->name = token.value;
    result->position = token.position;
    assert(':', "syntax error: misssing :");
    token = assert(TYPEID, "syntax error: invalid attribute definition");
    result->type = token.value;
    //initial;
    token = lexer.lookahead(1);
    if (token.type == ASSIGN) {
        lexer.consume();
        result->initial = parser_expr();
    }
    return result;
}

std::unique_ptr<Formal> xcool::Parser::parser_formal()
{
    auto result = make_unique<Formal>();
    Token token = assert(OBJECTID, "syntax error: invalid formal");
    result->position = token.position;
    result->name = token.value;
    assert(':', "syntax error: missing :");
    token = assert(TYPEID, "syntax error: invalid formal");
    result->type = token.value;
    return result;
}

std::unique_ptr<Method> xcool::Parser::parser_method()
{
    auto result = make_unique<Method>();
    Token token = lexer.next_token();
    result->position = token.position;
    result->name = token.value;
    assert('(', "syntax error: missing (");
    token = lexer.lookahead(1);
    if (token.type != ')') {
        result->formal_list.push_back(parser_formal());
        while (1) {
            token = lexer.lookahead(1);
            if (token.type == ',') {
                lexer.consume();
                result->formal_list.push_back(parser_formal());
            }
            else if (token.type == ')') {
                lexer.consume();
                break;
            }
            else 
                throw parser_error(token.position, "syntax error: invalid definitaion of method");
        }
    }
    else 
        lexer.consume();
    assert(':', "syntax error: missing :");
    token = assert(TYPEID, "syntax error: mising typeid");
    result->type = token.value;
    assert('{', "syntax error: missing {");
    result->body = parser_expr();
    assert('}', "syntax error: missing }");
    return result;
}

std::unique_ptr<Class> xcool::Parser::parser_class()
{
    auto result = make_unique<Class>();
    Token token = lexer.lookahead(1);
    if (token.type == EOF) {
        lexer.consume();
        return nullptr;
    }
    else 
        assert(CLASS, "syntax error: invalid definition of class");        
    result->position = token.position;
    token = assert(TYPEID, "syntax error: class definition missing typeid");
    result->name = token.value;
    token = lexer.lookahead(1);
    if (token.type == INHERITS) {
        lexer.consume();
        token = assert(TYPEID, "syntax error: class definition missing parent calss");
        result->parent = token.value;
    }
    assert('{', "syntax error: missing {");
    while (1) {
        token = lexer.lookahead(1);
        if (token.type == '}') {
            lexer.consume();
            break;
        }
        if (token.type != OBJECTID) {
            throw parser_error(token.position, "syntax error: feature definition missing objectid") ;
        }
        token = lexer.lookahead(2);
        //parser method 
        if (token.type == '(')
            result->method_list.push_back(parser_method());
        else if (token.type == ':')
            result->attribute_list.push_back(parser_attribute());
        else 
            throw parser_error(token.position, "syntax error: invalid character");
        assert(';', "syntax error: missing ;");
    }
    return result;
}

Program xcool::Parser::get_result()
{
    Program result;
    while (1) {
        auto class_result = parser_class();
        if (class_result)
            result.class_list.push_back(std::move(class_result));
        else 
            break;
        assert(';', "syntax error: missing ';'");
    }
    return result;
}