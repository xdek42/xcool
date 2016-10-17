#include "lexer.h"
#include "parser.h"
#include "AstTree.h"
#include <iostream>
#include <fstream>
#include "error.h"

int main(int argc, char *argv[])
{
    try {
        xcool::Tokenizer lexer(argv[1]);
        xcool::Parser parser(lexer);
        xcool::ast::Program program = parser.get_result();
    }
    catch (xcool::token_error err) {
        std::cout << "lexer error: " << err.what() << std::endl;
    }
    catch (xcool::parser_error err) {
        std::cout << "syntax error " << err.what() << std::endl;
    }
    return 0;
}