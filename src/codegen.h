#ifndef CODEGEN_H
#define CODEGEN_H
#include <memory>
#include "semant.h"
#include <vector>
#include "layout.h"
#include <fstream>

namespace xcool {
    void emit_code(std::vector<std::shared_ptr<xcool::Layout>> &layouts, xcool::InherTree &cool_program, std::ofstream &);
}

#endif