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
        xcool::InherTree cool_program;
        xcool::build_tree(cool_program, program);
        cool_program.print_tree();
        xcool::semant_check(cool_program);
        //codegen
        std::vector<std::shared_ptr<xcool::Layout>> layouts;
        make_layout(cool_program, layouts);
        for (auto lay : layouts) {
            out_file << "class_name: " << lay->name << std::endl;
            out_file << "class_length " << lay->length << std::endl;
            out_file << lay->name + "_dispatch_table:" << std::endl;
            for (auto method : lay->dis_table->fun_table) {
                out_file << "    .long " << method << std::endl;
            }
            out_file << std::endl;
        }
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