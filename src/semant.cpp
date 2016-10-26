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

namespace {
    xcool::SymbolTable symbol_table;
    void install_base_class(std::shared_ptr<TreeNode> root) 
    {
        root->parent = nullptr;
        //安装Object类
        root->node = make_unique<Class>();
        root->node->name = "Object";
        auto abort = make_unique<Method>();
        abort->type = "Object";
        abort->name = "abort";
        root->node->method_list.push_back(std::move(abort));
        auto type_name = make_unique<Method>();
        type_name->type = "String";
        type_name->name = "type_name";
        root->node->method_list.push_back(std::move(type_name));
        auto copy = make_unique<Method>();
        copy->type = "SELF_TYPE";
        copy->name = "copy";
        root->node->method_list.push_back(std::move(copy));
        //安装IO
        std::shared_ptr<TreeNode> son = std::make_shared<TreeNode>();
        son->node = make_unique<Class>();
        son->node->name = "IO";
        // 安装IO类的out_string函数
        auto out_string = make_unique<Method>();
        out_string->name = "out_string";
        out_string->type = "SELF_TYPE";
        out_string->formal_list.push_back(make_unique<Formal>("x", "string"));
        son->node->method_list.push_back(std::move(out_string));
        // 安装IO类的out_int函数
        auto out_int = make_unique<Method>();
        out_int->name = "out_int";
        out_int->type = "SELF_TYPE";
        out_int->formal_list.push_back(make_unique<Formal>("x", "Int"));
        son->node->method_list.push_back(std::move(out_int));
        //安装IO类in_string函数
        auto in_string = make_unique<Method>();
        in_string->name = "in_string";
        in_string->type = "String";
        in_string->formal_list.push_back(make_unique<Formal>("x", "string"));
        son->node->method_list.push_back(std::move(in_string));
        //安装IO类in_int函数
        auto in_int = make_unique<Method>();
        in_int->name = "in_int";
        in_int->type = "Int";
        in_int->formal_list.push_back(make_unique<Formal>("x", "Int"));
        son->node->method_list.push_back(std::move(in_int));
        
        son->parent = root;
        root->son_list.push_back(son);

        //安装String 
        son = std::make_shared<TreeNode>();
        son->node = make_unique<Class>();
        son->node->name = "String";
        auto length = make_unique<Method>();
        length->type = "Int";
        length->name = "length";
        son->node->method_list.push_back(std::move(length));
        auto concat = make_unique<Method>();
        concat->type = "String";
        concat->name = "concat";
        concat->formal_list.push_back(make_unique<Formal>("s", "String"));
        son->node->method_list.push_back(std::move(concat));
        auto substr = make_unique<Method>();
        substr->type = "String";
        substr->name = "substr";
        substr->formal_list.push_back(make_unique<Formal>("i", "Int"));
        substr->formal_list.push_back(make_unique<Formal>("l", "Int"));
        son->node->method_list.push_back(std::move(substr));
        
        son->parent = root;
        root->son_list.push_back(son);

        //安装Int
        son = std::make_shared<TreeNode>();
        son->node = make_unique<Class>();
        son->node->name = "Int";
        
        son->parent = root;
        root->son_list.push_back(son);
        
        //安装Bool
        son = std::make_shared<TreeNode>();
        son->node = make_unique<Class>();
        son->node->name = "Bool";
        
        son->parent = root;
        root->son_list.push_back(son); 
    }
    std::shared_ptr<TreeNode> _find_node(std::shared_ptr<TreeNode> root, std::string name)
    {
        if (root->node->name == name)
            return root;
        for (const auto & node : root->son_list) {
            std::shared_ptr<TreeNode> result = _find_node(node, name);
            if (result)
                return result;
        }
        return nullptr;
    }
}

void xcool::build_tree(InherTree &tree, Program &program)
{
    std::shared_ptr<TreeNode> root = std::make_shared<TreeNode>();
    install_base_class(root);
    for (auto &cls : program.class_list) {
        std::shared_ptr<TreeNode> son = std::make_shared<TreeNode>();
        if ((cls->parent).empty()) {
            son->parent = root;
            son->node = std::move(cls);
            root->son_list.push_back(son);
        }
        else {
            std::shared_ptr<TreeNode> parent_node = _find_node(root, cls->parent);
            if(!parent_node)
                throw semant_error(cls->position, "undefined parent class" + cls->parent);
            son->node = std::move(cls);
            son->parent = parent_node;
            parent_node->son_list.push_back(son);
        }
    }
    tree.set_root(root);
}

void xcool::semant_check(InherTree &)
{}

std::shared_ptr<TreeNode> InherTree::find_node(std::string name)
{
    return _find_node(root, name);
}

void InherTree::print_tree()
{
    if (!root)
        return;
    std::queue<std::shared_ptr<TreeNode>> que;
    que.push(root);
    int count = 0, current_sum = 1, next_sum = 0;
    while (!que.empty()) {
        std::shared_ptr<TreeNode> node = que.front();
        std::cout << node->node->name << " ";
        count++;
        next_sum += node->son_list.size();
        for (auto x : node->son_list)
            que.push(x);
        if (count == current_sum) {
            std::cout << std::endl;
            current_sum = next_sum;
            next_sum = 0;
            count = 0;
        }
        que.pop();
    }
}

