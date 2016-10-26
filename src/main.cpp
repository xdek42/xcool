#include "lexer.h"
#include "parser.h"
#include "AstTree.h"
#include <iostream>
#include <fstream>
#include "error.h"
#include "semant.h"

int main(int argc, char *argv[])
{
    try {
        //lexer
        xcool::Tokenizer lexer(argv[1]);
        //parser
        xcool::Parser parser(lexer);
        xcool::ast::Program program = parser.get_result();
        //semant check
        xcool::InherTree cool_program;
        xcool::build_tree(cool_program, program);
        xcool::semant_check(cool_program);
    }
    catch (xcool::token_error err) {
        std::cout << "lexer error: " << err.what() << std::endl;
    }
    catch (xcool::parser_error err) {
        std::cout << "syntax error: " << err.what() << std::endl;
    }
    catch (xcool::semant_error err) {
        std::cout << "semant error: " << err.what() << std::endl;
    }
    return 0;
}