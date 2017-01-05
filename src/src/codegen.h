#ifndef CODEGEN_H
#define GODEGEN_H

#include <memory>
#include <vector>
#include <fstream>
#include "semant.h"

namespace xcool {
    void code_gen(const std::vector<std::shared_ptr<xcool::Layout>> &layout_list, std::ofstream &outfile);
}








#endif