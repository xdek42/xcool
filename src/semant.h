#ifndef SEMANT_H
#define SEMANT_H
#include "AstTree.h"
#include <memory>

namespace xcool {
    //the inheritance graph
    class InherTree;
    void build_tree(InherTree &, xcool::ast::Program &);
    void semant_check(InherTree &);
    class TreeNode {
        public:
            std::shared_ptr<TreeNode> parent;
            std::vector<std::shared_ptr<TreeNode>> son_list;
            std::unique_ptr<xcool::ast::Class> node;
    };
    class InherTree {
        friend void build_tree(InherTree &, xcool::ast::Program &);
        friend void semant_check(InherTree &);
        public:
            void print_tree();
            std::shared_ptr<TreeNode> find_node(std::string name);
            void set_root(std::shared_ptr<TreeNode> r) {root = r;}
            bool is_ancestor(std::string, std::string);
            std::shared_ptr<TreeNode> get_root() {return root;}
        private:
            std::shared_ptr<TreeNode> root;
    };
};






#endif