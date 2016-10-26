#include "semant.h"
#include "error.h"
#include "auxiliary.h"
#include "AstTree.h"
#include <queue>
#include "SymbolTab.h"

using xcool::ast::Program;
using xcool::semant_error;
using xcool::TreeNode;
using xcool::InherTree;
using namespace xcool::ast;

void xcool::build_tree(InherTree &tree, Program &program)
{}

void xcool::semant_check(InherTree &)
{}

std::shared_ptr<TreeNode> InherTree::find_node(std::string name)
{}