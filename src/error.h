#ifndef ERROR_H
#define ERROR_H
//error handle for tokenizer and parser

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>
#include "auxiliary.h"

namespace xcool {
    class token_error : public std::runtime_error {
        public:
            token_error(const std::pair<int, int> &position, std::string msg) : runtime_error("position (" + int2str(position.first) + ", " + int2str(position.second) + ") : " + msg){}
    };
    class parser_error : public std::runtime_error {
        public:
            parser_error(const std::pair<int, int> &position, std::string msg) : runtime_error("position (" + int2str(position.first) + ", " + int2str(position.second) + ") : " + msg){}
    };

}
#endif