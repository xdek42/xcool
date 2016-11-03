#include "layout.h"
#include "semant.h"
#include <vector>
#include <string>
#include "AstTree.h"

using xcool::Layout;
using std::shared_ptr;
using std::vector;
using std::string;
using xcool::TreeNode;
using xcool::InherTree;

vector<string> data_segment;
vector<string> class_layout_segment;
vector<string> text_segment;

namespace {
    bool is_basic_class(std::string name)
    {
        if (name == "Object" || name == "IO" || name == "Int" || name == "String" || name == "Bool")
            return true;
        else 
            return false;
    }
    void emit_method_code(shared_ptr<Layout> layout, std::unique_ptr<Method> &method) 
    {
    }
    void emit_class_code(shared_ptr<Layout> layout, shared_ptr<TreeNode> node)
    {
        if (is_basic_class(node->node->name))
            return;
        for(const auto &method : node->node->method_list) {
            emit_method_code(layout, method);
        }
    }
    void _emit_code(vector<shared_ptr<Layout>> &layouts, shared_ptr<TreeNode> root)
    {
        std::string class_name = root->node->name;
        for (auto layout : layouts) {
            if (layout->name == class_name)
                emit_class_code(layout, root);
        }
        for (auto son_node : root->son_list) {
            _emit_code(layouts, son_node);
        }
    }
}

void emit_code(vector<shared_ptr<Laoyout>> &layouts, xcool::InherTree &cool_program)
{
    shared_ptr<TreeNode> root = cool_program.get_root();
    _emit_code(layouts, root);
}