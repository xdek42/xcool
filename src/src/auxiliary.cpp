#include "auxiliary.h"
#include <stdexcept>

std::string int2str(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}