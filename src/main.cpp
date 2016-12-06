#include "lexer.h"
#include "parser.h"
#include "AstTree.h"
#include <iostream>
#include <fstream>
#include "error.h"
#include "semant.h"
#include "codegen.h"

void print_usage()
{
    ;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        print_usage();
        return 0;
    }
    std::ofstream out_file(argv[2]);
    try {
        //lexer
        xcool::Tokenizer lexer(argv[1]);
        //parser
        xcool::Parser parser(lexer);
        xcool::ast::Program program = parser.get_result();
        //semant check
        std::vector<std::shared_ptr<xcool::Layout>> layout_list;
        xcool::semant_check(layout_list, program);
        xcool::code_gen(layout_list, out_file);
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