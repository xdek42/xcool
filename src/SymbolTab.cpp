#include "SymbolTab.h"
#include <memory>

using xcool::Table;
using xcool::SymbolTable;

void SymbolTable::enter_scope()
{
    std::shared_ptr<Table> temp = std::make_shared<Table>();
    temp->parent = currentTab;
    currentTab = temp;
}

void SymbolTable::exit_scope()
{
    currentTab = currentTab->parent;
}

bool SymbolTable::find(std::string name)
{
    return currentTab->find(name);
}

int SymbolTable::insert(std::string name, std::string type)
{
    return currentTab->insert(name, type);
}

std::string SymbolTable::get_type(std::string type)
{
    return currentTab->get_type(type);
}

bool Table::find(std::string name)
{
    if (table.find(name) != table.end())
        return true;
    if (parent == nullptr)
        return false;
    return parent->find(name);
}

int Table::insert(std::string name, std::string type)
{
    if (table.find(name) != table.end())
        return -1;
     else {
        table[name] = type;
        return 0;
     }
}
// 使用者需保证name在符号表中存在
std::string Table::get_type(std::string name)
{
    if (table.find(name) != table.end())
        return table[name];
    std::shared_ptr<Table> temp = parent;
    while (temp) {
        if (temp->table.find(name) != temp->table.end())
            return (temp->table)[name];
        temp = temp->parent;
    }
    return "";
}
