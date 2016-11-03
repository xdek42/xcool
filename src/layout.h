#ifndef LAYOUT_H
#define LAYOUT_H

#include "semant.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

namespace xcool {
    // the class layout in memory
    class DisTable;
    class Layout {
        public:
            std::string name;
            int length;
            std::shared_ptr<DisTable> dis_table;
            std::map<std::string, int> attr_table;
    };
    // the DisTable of class
    class DisTable {
        public:
            std::string name;
            std::vector<std::string> fun_table;
    };
    //build layout
    void make_layout(InherTree &inher_tree, std::vector<std::shared_ptr<Layout>> &layouts);
}


#endif