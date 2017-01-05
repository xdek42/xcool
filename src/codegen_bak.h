#ifndef CODEGEN_H
#define CODEGEN_H
#include <memory>
#include "semant.h"
#include <vector>
#include "semant.h"
#include <fstream>

namespace xcool {
    void code_gen(std::vector<std::shared_ptr<xcool::Layout>> &layout_list, std::ofstream &out_file);
}

#endif