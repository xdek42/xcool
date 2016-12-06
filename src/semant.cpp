#include "semant.h"
#include "AstTree.h"
#include "auxiliary.h"
#include "error.h"
#include "SymbolTab.h"
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <iostream>

using xcool::Layout;
using namespace xcool::ast;
using std::unique_ptr;
using std::string;
using std::vector;
using std::shared_ptr;
using xcool::semant_error;
using std::cout;

namespace {
    class TreeNode;
    shared_ptr<TreeNode> tree_root;
    //define symbol table used for semant check
    xcool::SymbolTable symbol_table;
    //define the semant checker for each expression
    class SemantVisitor : public ExprVisitor {
        public:
            ~SemantVisitor() {}
            virtual void visit(AssExpr &) override;
            virtual void visit(UnaryExpr &) override;
            virtual void visit(DisExpr &) override;
            virtual void visit(BlockExpr &) override;
            virtual void visit(IntExpr &) override;
            virtual void visit(BoolExpr &) override;
            virtual void visit(StrExpr &) override;
            virtual void visit(LetExpr &) override;
            virtual void visit(NewExpr &) override;
            virtual void visit(IdExpr &) override;
            virtual void visit(WhileExpr &) override;
            virtual void visit(IfExpr &) override;
            virtual void visit(BinExpr &) override;
            virtual void visit(CaseExpr &) override;
    } semant_checker;

    void SemantVisitor::visit(AssExpr &expr) {}
    void SemantVisitor::visit(UnaryExpr &expr) {}
    void SemantVisitor::visit(DisExpr &expr) {}
    void SemantVisitor::visit(BlockExpr &expr) {}
    void SemantVisitor::visit(IntExpr &expr) {}
    void SemantVisitor::visit(BoolExpr &expr) {}
    void SemantVisitor::visit(StrExpr &expr) {}
    void SemantVisitor::visit(LetExpr &expr) {}
    void SemantVisitor::visit(NewExpr &expr) {}
    void SemantVisitor::visit(IdExpr &expr) {}
    void SemantVisitor::visit(WhileExpr &expr) {}
    void SemantVisitor::visit(IfExpr &expr) {}
    void SemantVisitor::visit(BinExpr &expr) {}
    void SemantVisitor::visit(CaseExpr &expr) {}

    class TreeNode {
        public:
            unique_ptr<Class> class_node;
            vector<shared_ptr<TreeNode>> son_list;
    };
    //install some basic class in cool: eg Object, IO, Int
    void install_basic_class(shared_ptr<TreeNode> root)
    {
        //install Object class
        root->class_node = make_unique<Class>();
        root->class_node->name = "Object";
        //install abort method of Object class
        auto abort = make_unique<Method>();
        abort->type = "Object";
        abort->name = "abort";
        root->class_node->method_list.push_back(std::move(abort));
        //install type_name method of Object class
        auto type_name = make_unique<Method>();
        type_name->type = "String";
        type_name->name = "type_name";
        root->class_node->method_list.push_back(std::move(type_name));
        //install copy method of Object class
        auto copy = make_unique<Method>();
        copy->type = "SELF_TYPE";
        copy->name = "copy";
        root->class_node->method_list.push_back(std::move(copy));
        //install IO class
        std::shared_ptr<TreeNode> son = std::make_shared<TreeNode>();
        son->class_node = make_unique<Class>();
        son->class_node->name = "IO";
        son->class_node->parent = "Object";
        //install out_string method of IO class
        auto out_string = make_unique<Method>();
        out_string->name = "out_string";
        out_string->type = "SELF_TYPE";
        out_string->formal_list.push_back(make_unique<Formal>("x", "string"));
        son->class_node->method_list.push_back(std::move(out_string));
        //install out_int method of IO class
        auto out_int = make_unique<Method>();
        out_int->name = "out_int";
        out_int->type = "SELF_TYPE";
        out_int->formal_list.push_back(make_unique<Formal>("x", "Int"));
        son->class_node->method_list.push_back(std::move(out_int));
        //install in_string method of IO class
        auto in_string = make_unique<Method>();
        in_string->name = "in_string";
        in_string->type = "String";
        in_string->formal_list.push_back(make_unique<Formal>("x", "string"));
        son->class_node->method_list.push_back(std::move(in_string));
        //install in_int method of IO class
        auto in_int = make_unique<Method>();
        in_int->name = "in_int";
        in_int->type = "Int";
        in_int->formal_list.push_back(make_unique<Formal>("x", "Int"));
        son->class_node->method_list.push_back(std::move(in_int));
        
        root->son_list.push_back(son);

        //install String class
        son = std::make_shared<TreeNode>();
        son->class_node = make_unique<Class>();
        son->class_node->name = "String";
        son->class_node->parent = "Object";
        //install length method of String class
        auto length = make_unique<Method>();
        length->type = "Int";
        length->name = "length";
        son->class_node->method_list.push_back(std::move(length));
        //install concat method of String class
        auto concat = make_unique<Method>();
        concat->type = "String";
        concat->name = "concat";
        concat->formal_list.push_back(make_unique<Formal>("s", "String"));
        son->class_node->method_list.push_back(std::move(concat));
        //install substr method of String class
        auto substr = make_unique<Method>();
        substr->type = "String";
        substr->name = "substr";
        substr->formal_list.push_back(make_unique<Formal>("i", "Int"));
        substr->formal_list.push_back(make_unique<Formal>("l", "Int"));
        son->class_node->method_list.push_back(std::move(substr));
        
        root->son_list.push_back(son);

        //install int class
        son = std::make_shared<TreeNode>();
        son->class_node = make_unique<Class>();
        son->class_node->name = "Int";
        son->class_node->parent = "Object";
        
        root->son_list.push_back(son);
        
        //安装Bool
        son = std::make_shared<TreeNode>();
        son->class_node = make_unique<Class>();
        son->class_node->name = "Bool";
        son->class_node->name = "Object";
        
        root->son_list.push_back(son);
    }

    //find node in inhertiance tree
    std::shared_ptr<TreeNode> find_node(std::shared_ptr<TreeNode> root, std::string name)
    {
        if (root->class_node->name == name)
            return root;
        for (const auto & node : root->son_list) {
            std::shared_ptr<TreeNode> result = find_node(node, name);
            if (result)
                return result;
        }
        return nullptr;
    }
    //check assign valid
    bool is_valid_assign(string left_type, string right_type)
    {
        auto left_node = find_node(tree_root, left_type);
        if (left_node == nullptr)
            return false;
        if (find_node(left_node, right_type))
            return true;
        return false;
    }
    //build cool class inheritance graph
    shared_ptr<TreeNode> build_tree(Program &program)
    {
        shared_ptr<TreeNode> root = std::make_shared<TreeNode>();
        install_basic_class(root);
        for (auto &cls : program.class_list) {
            shared_ptr<TreeNode> class_node = std::make_shared<TreeNode>();
            shared_ptr<TreeNode> parent_node = find_node(root, cls->parent);
            if (!parent_node) 
                throw semant_error(cls->position, "undefined parent class " + cls->parent);
            parent_node->son_list.push_back(class_node);
            class_node->class_node = std::move(cls);
        }
        return root;
    }
    //semant check each class in program
    void _semant_check(shared_ptr<TreeNode> root)
    {
        symbol_table.enter_scope();
        symbol_table.insert("self", root->class_node->name);
        //check attribute
        for (auto &attr : root->class_node->attribute_list) {
            if (!symbol_table.insert(attr->name, attr->type)) {
                throw semant_error(attr->position, "redefine attribute name: " + attr->name);
            }
            if (attr->initial != nullptr) {
                attr->initial->accept_visitor(semant_checker);
                //check assign valid
                auto right_type = attr->initial->static_type;
                auto left_type = attr->type;
                if (!is_valid_assign(left_type, right_type)) {
                    throw semant_error(attr->position, "invalid assign: " + attr->name);
                }
            }
        }
        //check method
        for (auto &method : root->class_node->method_list) {
            symbol_table.enter_scope();
            //put method parameter into symbol table
            for (auto &para : method->formal_list) {
                if (!symbol_table.insert(para->name, para->type)) {
                    throw semant_error(para->position, "redefine parameter name: " + para->name);
                }
            }
            //check method body
            if (method->body != nullptr) {
                method->body->accept_visitor(semant_checker);
                auto static_return_type = method->body->static_type;
                auto return_type = method->type;
                if (return_type == "SELF_TYPE")
                    return_type = symbol_table.get_value("self");
                if(!is_valid_assign(return_type, static_return_type)) {
                    throw semant_error(method->position, "method return type conflict");
                }
            }
            symbol_table.exit_scope();
        }
        //do same check for son class
        for (auto &cls_node : root->son_list)
            _semant_check(cls_node);
        symbol_table.exit_scope();
    }
    //print inheritance graph
    void print_tree(shared_ptr<TreeNode> root)
    {
        if (!root)
            return;
        std::queue<std::shared_ptr<TreeNode>> que;
        que.push(root);
        int count = 0, current_sum = 1, next_sum = 0;
        while (!que.empty()) {
            std::shared_ptr<TreeNode> node = que.front();
            std::cout << node->class_node->name << " ";
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
    //install some basic class layout
    void install_Object_layout(vector<shared_ptr<Layout>> &layout_list)
    {
        //install Object class
        shared_ptr<Layout> object_layout = std::make_shared<Layout>();
        object_layout->name = "Object";
        object_layout->length = 4;
        object_layout->dis_table.push_back("Object_abort");
        object_layout->dis_table.push_back("Object_type_name");
        object_layout->dis_table.push_back("Object_layout");
        layout_list.push_back(object_layout);
    }
    //to determine whether it is a basical class
    bool is_basic_class(const string &name) 
    {
        if (name == "Object" || name == "IO" || name == "String" || name == "Int" || name == "Bool")
            return true;
        return false;
    }
    //get mehtod name from dispath name
    std::string get_method_name(const std::string &name)
    {
        std::string ret;
        int flag = 0;
        for (int i = 0; i < name.size(); i++) {
            if (name[i] == '_' && flag == 0) {
                flag = 1;
                continue;
            }
            if (flag) {
                ret = ret + name[i];
            }
        }
        return ret;
    }
    //get layout from layout list
    shared_ptr<Layout> find_layout(const vector<shared_ptr<Layout>> &layout_list, const string &class_name)
    {
        for (auto & layout : layout_list) {
            if (layout->name == class_name)
                return layout;
        }
        return nullptr;
    }
    //build class layout
    void build_layout(vector<shared_ptr<Layout>> &layout_list, shared_ptr<TreeNode> root)
    {
        if (root->class_node->name != "Object") {
            shared_ptr<Layout> layout = std::make_shared<Layout>();
            layout->name = root->class_node->name;
            //get parent class name
            string parent_name = root->class_node->parent;
            shared_ptr<Layout> parent_layout = find_layout(layout_list, parent_name);
            //install parent dispatch table
            for (const string &dis : parent_layout->dis_table) {
                layout->dis_table.push_back(dis);
            }
            //install calss method
            for (auto &method : root->class_node->method_list) {
                string method_name = method->name;
                //to determine whether it is override
                int flag = 0;
                for (int i = 0; i < parent_layout->dis_table.size(); i++) {
                    string inher_method_name = get_method_name(parent_layout->dis_table[i]);
                    if (method_name == inher_method_name) {
                        flag = 1;
                        layout->dis_table[i] = layout->name + "_" + method_name;
                        break;
                    }
                }
                if (flag == 0)
                    layout->dis_table.push_back(layout->name + "_" + method_name);
                layout->method_list.push_back(std::move(method));
            }
            //install attribute
            for (int i = 0; i < parent_layout->attrs_name.size(); i++) {
                layout->attrs_name.push_back(parent_layout->attrs_name[i]);
                layout->attrs_type.push_back(parent_layout->attrs_type[i]);
                layout->attrs_offset.push_back(parent_layout->attrs_offset[i]);
            }
            layout->length = parent_layout->length;
            for (const auto &attr : root->class_node->attribute_list) {
                layout->attrs_name.push_back(attr->name);
                layout->attrs_type.push_back(attr->type);
                layout->attrs_offset.push_back(layout->length);
                layout->length += 4;
            }
            layout_list.push_back(layout);
        }
        for (auto &son_node : root->son_list) {
            build_layout(layout_list, son_node);
        }
    }
    //print class layout
    void print_layout(const vector<shared_ptr<Layout>> &layout_list) 
    {
        for (const auto &layout : layout_list) {
            cout << "class name: " << layout->name << std::endl;
            cout << "length: " << layout->length << std::endl;
            cout << "dis_table: " << std::endl;
            for (const string &dis : layout->dis_table) {
                cout << "   " << dis << std::endl;
            }
            cout << "attribute:  " << std::endl;
            cout << "\tname\t" << "type\t" << "offset\t" << std::endl;
            for (int i = 0; i < layout->attrs_name.size(); i++) {
                cout << "\t" << layout->attrs_name[i] << "\t" << layout->attrs_type[i] << "\t" << layout->attrs_offset[i] << std::endl;
            }
            cout << "self Define method:" << std::endl;
            for (const auto &method : layout->method_list) {
                std::cout <<"    " << method->name << std::endl;
            }
            std::cout << "------------------------------------" <<std::endl;
        }

    }
}

void xcool::semant_check(vector<shared_ptr<Layout>> &layout_list, Program &program)
{
    shared_ptr<TreeNode> root = build_tree(program);
    install_Object_layout(layout_list);
    tree_root = root;
    //_semant_check(root);
    build_layout(layout_list, root);
    //print_layout(layout_list);

    tree_root = nullptr;
}