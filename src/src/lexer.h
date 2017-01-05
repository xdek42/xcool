#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>

namespace xcool {
    //define token type
    enum TokenType {
        CLASS = 258,
        ELSE = 259,
        FI = 260,
        IF = 261,
        IN = 262,
        INHERITS = 263,
        LET = 264,
        LOOP = 265,
        POOL = 266,
        THEN = 267,
        WHILE = 268,
        CASE = 269,
        ESAC = 270,
        OF = 271,
        DARROW = 272,
        NEW = 273,
        ISVOID = 274,
        STR_CONST = 275,
        INT_CONST = 276,
        BOOL_CONST = 277,
        TYPEID = 278,
        OBJECTID = 279,
        ASSIGN = 280,
        NOT = 281,
        LE = 282,
        ERROR = 283,
        LESS = 284,
        LET_STMT = 285,
        EQUAL = 286,
    };
    //define token
    class Token {
        public:
            Token() = default;
            Token(int r, int c, int t, std::string v);
            Token(int r, int c, int t);
            int type;
            std::pair<int, int> position;
            std::string value;
    };
    //define tokenizer
    class Tokenizer {
        public:
            Tokenizer(std::string filename);
            void consume();
            //consume a token and assert it
            bool assert(int);
            Token lookahead(int num);
            Token next_token();
            ~Tokenizer();
        private:
            Token get_token();
            std::ifstream file;
            std::vector<Token> buffer;
            void get_comment();
            Token get_objectId();
            Token get_typeId();
            Token get_number();
            Token get_string();
            void get_single_comment();
            int isKeyWord(std::string);
    };
}


#endif