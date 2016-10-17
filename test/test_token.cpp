#include <iostream>
#include <string>
#include "lexer.h"
#include <fstream>
using namespace std;
using xcool::TokenType;

int main(int argc, char **argv)
{
    ifstream file;
    if (argc > 1) {
        xcool::Tokenizer t(argv[1]);
        while (1) {
            xcool::Token token = t.next_token();
            int type = token.type;
            int row = token.position.first;
            int column = token.position.second;
            if (type == EOF) {
                cout << "end of file" << endl;
                break;
            }
            switch (type) {
                case TokenType::CLASS:
                    cout << row << ", " << column << " : " << "CLASS" << endl;
                    break;
                case TokenType::ELSE:
                    cout << row << ", " << column << " : " << "ELSE" << endl;
                    break;
                case TokenType::FI:
                    cout << row << ", " << column << " : " << "FI" << endl;
                    break;
                case TokenType::IN:
                    cout << row << ", " << column << " : " << "IN" << endl;
                    break;
                case TokenType::INHERITS:
                    cout << row << ", " << column << " : " << "INHERITS" << endl;
                    break;
                case TokenType::NEW:
                    cout << row << ", " << column << " : " << "NEW" << endl;
                    break;
                case TokenType::POOL:
                    cout << row << ", " << column << " : " << "POOL" << endl;
                    break;
                case TokenType::WHILE:
                    cout << row << ", " << column << " : " << "WHILE" << endl;
                    break;
                case TokenType::LESS:
                    cout << row << ", " << column << " : " << "LESS" << endl;
                    break;
                case TokenType::LOOP:
                    cout << row << ", " << column << " : " << "LOOP" << endl;
                    break;
                case TokenType::DARROW:
                    cout << row << ", " << column << " : " << "DARROW" << endl;
                    break;
                case TokenType::ASSIGN:
                    cout << row << ", " << column << " : " << "ASSIGN" << endl;
                    break;
                case TokenType::EQUAL:
                    cout << row << ", " << column << " : " << "EQUAL" << endl;
                    break;
                case TokenType::LET: 
                    cout << row << ", " << column << " : " << "LET" << endl;
                    break;
                case TokenType::THEN:
                    cout << row << ", " << column << " : " << "THEN" << endl;
                    break;
                case TokenType::OBJECTID:
                    cout << row << ", " << column << " : " << "OBJECTID" << " : " << token.value << endl;
                    break;
                case TokenType::TYPEID:
                    cout << row << ", " << column << " : " << "TYPEID" << " : " << token.value << endl;
                    break;
                case TokenType::STR_CONST:
                    cout << row << ", " << column << " : " << "STR_CONST" << " : " << token.value << endl;
                    break;
                case TokenType::BOOL_CONST:
                    cout << row << ", " << column << " : " << "BOOL_CONST" << " : " << token.value << endl;
                    break;
                case TokenType::INT_CONST:
                    cout << row << ", " << column << " : " << "INT_CONST" << " : " << token.value << endl;
                    break;
                default:
                    cout << row << ", " << column << " : " << "SINGLE_CHAR" << " : " << (char)(token.type) << endl;
                    break;
            }
        }
    }
}
