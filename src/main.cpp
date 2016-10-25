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
        xcool::Tokenizer lexer(argv[1]);
        xcool::Parser parser(lexer);
        xcool::ast::Program program = parser.get_result();
        xcool::SemantChecker semantChecker;
        std::shared_ptr<xcool::InherGraph> root = semantChecker.build_graph(program);
        root->print_graph(root);
    }
    catch (xcool::token_error err) {
        std::cout << "lexer error: " << err.what() << std::endl;
    }
    catch (xcool::parser_error err) {
        std::cout << "syntax error " << err.what() << std::endl;
    }
    return 0;
}