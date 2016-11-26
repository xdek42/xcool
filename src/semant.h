#ifndef SEMANT_H
#define SEMANT_H

#include "AstTree.h"
#include <string>
#include <vector>

//merge smeant procedure and layout procedure
namespace xcool {
    //define the layout of class
    class Layout {
        public:
            std::string name;
            int length;
            std::vector<std::string> dis_table;
            std::vector<std::pair<std::string, int>> attr_list;
            std::vector<std::unique_ptr<xcool::ast::Method>> method_list;
    };
    void semant_check(std::vector<std::shared_ptr<Layout>> &, xcool::ast::Program &);
}


#endif