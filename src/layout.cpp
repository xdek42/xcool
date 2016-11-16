#include "layout.h"

using xcool::Layout;
using xcool::DisTable;
using std::shared_ptr;
using std::vector;
using std::string;
using std::make_shared;
using xcool::TreeNode;
namespace {
    void install_basic_class_layout(vector<shared_ptr<Layout>> &layouts)
    {
        //make object layout
        shared_ptr<Layout> object_layout = make_shared<Layout>();
        object_layout->name = "Object" ;
        object_layout->length = 4;
        shared_ptr<DisTable> object_distable = make_shared<DisTable>();
        object_distable->name = "Object";
        object_distable->fun_table.push_back("Object_abort");
        object_distable->fun_table.push_back("Object_type_name");
        object_distable->fun_table.push_back("Object_copy");
        object_layout->dis_table = object_distable;
        layouts.push_back(object_layout);
        //make Io layout
        shared_ptr<Layout> io_layout = make_shared<Layout>();
        io_layout->name = "IO";
        io_layout->length = 4;
        shared_ptr<DisTable> io_distable = make_shared<DisTable>();
        io_distable->name = "IO";
        for (std::string method : object_distable->fun_table) 
            io_distable->fun_table.push_back(method);
        io_distable->fun_table.push_back("IO_out_string");
        io_distable->fun_table.push_back("IO_out_int");
        io_distable->fun_table.push_back("IO_in_string");
        io_distable->fun_table.push_back("IO_in_int");
        io_layout->dis_table = io_distable;
        layouts.push_back(io_layout);
        //make int layout
        shared_ptr<Layout> int_layout = make_shared<Layout>();
        int_layout->name = "Int";
        int_layout->length = 8;
        shared_ptr<DisTable> int_distable = make_shared<DisTable>();
        int_distable->name = "Int";
        for (std::string method : object_distable->fun_table)
            int_distable->fun_table.push_back(method);
        int_layout->dis_table = int_distable;
        layouts.push_back(int_layout);
    }
    bool is_basic_class(std::string name)
    {
        if (name == "Object" || name == "IO" || name == "Int" || name == "String" || name == "Bool")
            return true;
        else 
            return false;
    }
    shared_ptr<Layout> find_layout(std::string name, vector<shared_ptr<Layout>> &layouts)
    {
        for (const auto &layout : layouts) {
            if (layout->name == name)
                return layout;
        }
        return nullptr;

    }
    bool is_override(const std::string &method, shared_ptr<DisTable> dis_table)
    {
        const std::vector<string> &table = dis_table->fun_table;
        for (const auto & m : table) {
            if (method == m)
                return true;
        }
        return false;
    }
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
    void _make_layout(shared_ptr<TreeNode> node, vector<shared_ptr<Layout>> &layouts)
    {
       std::string class_name = node->node->name;
       if (!is_basic_class(class_name)) {
           shared_ptr<Layout> class_layout = make_shared<Layout>();
           class_layout->name = class_name;
           std::string parent_name = node->parent->node->name;
           shared_ptr<Layout> parent_layout = find_layout(parent_name, layouts);
           //计算length
           class_layout->length = parent_layout->length;
           for (int i = 0; i < parent_layout->attr.size(); i++) {
               class_layout->attr.push_back(parent_layout->attr[i]);
               class_layout->offset.push_back(parent_layout->offset[i]);
           }
           int current_offset = parent_layout->length;
           for (auto &attr : node->node->attribute_list) {
               shared_ptr<Layout> attr_layout = find_layout(attr->type, layouts);
               class_layout->attr.push_back(attr->name);
               class_layout->offset.push_back(current_offset);
               current_offset += attr_layout->length;
               class_layout->length += attr_layout->length;
           }
           //构建dis_table;
           shared_ptr<DisTable> class_distable = make_shared<DisTable>();
           class_distable->name = class_name;
           //装载父类函数
           for (const string &method : parent_layout->dis_table->fun_table) {
               class_distable->fun_table.push_back(method);
           }
           //装载本类函数
           for (const auto &method : node->node->method_list) {
               string method_name = method->name;
               int flag = 0;
               //判断是否重载
               for (int i = 0; i < parent_layout->dis_table->fun_table.size(); i++) {
                   string parent_method_name = get_method_name(parent_layout->dis_table->fun_table[i]);
                   if (method_name == parent_method_name) {
                       flag = 1;
                       class_distable->fun_table[i] = class_name + "_" + method_name;
                       break;
                   }
               }
               if (flag == 0) {
                   class_distable->fun_table.push_back(class_name + "_" + method_name);
               }
           }
           class_layout->dis_table = class_distable;
           layouts.push_back(class_layout);
       }
       for (auto son_node : node->son_list) {
           _make_layout(son_node, layouts);
       }
    }
}

void xcool::make_layout(InherTree &inher_tree, std::vector<shared_ptr<Layout>> &layouts)
{
    shared_ptr<xcool::TreeNode> root = inher_tree.get_root();
    install_basic_class_layout(layouts);
    _make_layout(root, layouts);
}
