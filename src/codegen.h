#ifndef CODEGEN_H
#define CODEGEN_H
#include "semant.h"
#include <string>
#include <map>

namespace xcool {
    class ClassLayout {
        public:
            int class_tag;
            int object_size;
            int dispatch_ptr;
            std::vector<std::string> attr_name;
            std::vector<int> attr_value;
    };

    class DisTable {
        public:
            std::vector<std::string> fun_name;
            std::vector<int> fun_addr;
    };
}

#endif