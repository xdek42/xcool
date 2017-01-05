#ifndef AUXI_H
#define AUXI_H

#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <map>

std::string int2str(int);
template<class T, class... Types>
std::unique_ptr<T> make_unique(Types&&... Args)
{
    return (std::unique_ptr<T>(new T(std::forward<Types>(Args)...))); 
}    

#endif