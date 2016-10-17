#include "lexer.h"
#include "error.h"
#include <stdexcept>

using xcool::Tokenizer;
using xcool::Token;

namespace {
    int comNestDeep = 0;
    int curr_column = 0;
    int curr_row = 1;
    int buffer[10];
    int buffer_esp = -1;
    int get_char(std::istream &file)
    {
        curr_column++;
        if (buffer_esp < 0)
            return file.get();
        return buffer[buffer_esp--];
    }
    void unget_char(int c)
    {
        curr_column--;
        buffer[++buffer_esp] = c;
    }
    Token make_token(int r, int c, int t, std::string v)
    {
        Token ret(r, c, t, v);
        return ret;
    }
    Token make_token(int r, int c, int t)
    {
        Token ret(r, c, t);
        return ret;
    }
    bool is_valid(int c)
    {
        if (isalnum(c) || c == '_')
            return true;
        else 
            return false;
    }
}

Token::Token(int r, int c, int t, std::string v) : type(t), value(v) 
{
    position.first = r;
    position.second = c;
}

Token::Token(int r, int c, int t) : type(t) 
{
    position.first = r;
    position.second = c;
}

Tokenizer::Tokenizer(std::string fileName)
{
    file.open(fileName);
    if (!file)
        throw std::runtime_error("can't open the file : " + fileName);
}

Tokenizer::~Tokenizer()
{
    file.close();
}

Token Tokenizer::get_token()
{
    int c, lookAhead;
    while (c = get_char(file)) {
       switch(c) {
           case ';': 
           case ':': 
           case ',':
           case '@': 
           case '.': 
           case '{': 
           case '}': 
           case ')': 
           case '+': 
           case '*':
           case '/': 
           case '~':  
               return make_token(curr_row, curr_column, c);
           case ' ':
           case '\t':
           case '\f':
           case '\v':
               break;  
           case '\n':
               curr_row++;
               curr_column = 0;
               break;
           case '(':
               lookAhead = get_char(file);
               if (lookAhead == '*') {
                   comNestDeep++;
                   get_comment();
                   break;
               }
               else {
                   unget_char(lookAhead);
                   return make_token(curr_row, curr_column, c);
               }
           case EOF:
               return make_token(curr_row, curr_column, EOF);
           case '<':
               lookAhead = get_char(file);
               if (lookAhead == '-')
                  return make_token(curr_row, curr_column, ASSIGN);
               if (lookAhead == '=')
                  return make_token(curr_row, curr_column, LE);
               else {
                   unget_char(lookAhead);
                   return make_token(curr_row, curr_column, LESS);
               }
           case '"':
               return get_string();
           case '-':
               lookAhead = get_char(file);
               if (lookAhead == '-') {
                   get_single_comment();
                   break;
               }
               else {
                   unget_char(lookAhead);
                   return make_token(curr_row, curr_column, '-');
               }
           case '=':
               lookAhead = get_char(file);
               if (lookAhead == '>') 
                   return make_token(curr_row, curr_column, DARROW);
               else {
                   unget_char(lookAhead);
                   return make_token(curr_row, curr_column, EQUAL);
               }
           default:
               if ('a' <= c && c <= 'z') {
                   unget_char(c);
                   return get_objectId();
               }
               else if ('A' <= c && c <= 'Z') {
                   unget_char(c);
                   return get_typeId();
               }
               else if (isdigit(c)) {
                   unget_char(c);
                   return get_number();
               }
               else {
                   std::pair<int, int> position(curr_row, curr_column);
                   throw xcool::token_error(position, "unkonw character: " + c);
               }
       }
    }
}

void Tokenizer::get_comment()
{
   int c;
   while (c = get_char(file)) {
       if (c == '*') {
           int lookAhead = get_char(file);
           if (lookAhead == ')') {
               if ((--comNestDeep) == 0)
                   return;
           }
           unget_char(lookAhead);
       }
       else if (c == '(') {
           int lookAhead = get_char(file);
           if (lookAhead == '*') {
               comNestDeep++;
           }
           unget_char(lookAhead);
       }
       else if (c == '\n') {
           curr_row++;
           curr_column = 0;
       }
       else if (c == EOF) {
           unget_char(c);
           std::pair<int, int> position(curr_row, curr_column);
           throw xcool::token_error(position, "unterminated multi-line comment");
       }
       else 
           continue;
   } 
}

Token Tokenizer::get_objectId()
{
    int c, column = curr_column + 1;
    std::string str;
    while (c = get_char(file)) {
        if (!is_valid(c)) {
            unget_char(c);
            int type = isKeyWord(str);
            //not is keyword
            if (type == -1) {
                return make_token(curr_row, column, OBJECTID, str);
            }
            else if (type == BOOL_CONST) {
                return make_token(curr_row, column, BOOL_CONST, str);
            }
            else
                return make_token(curr_row, column, type);
        }
        str += c;
    }
}

Token Tokenizer::get_typeId()
{
    int c, column = curr_column + 1;
    std::string str;
    while (c = get_char(file)) {
        if (!is_valid(c)) {
            unget_char(c);
            int type = isKeyWord(str);
            //not is keyword
            if (type == -1) {
                return make_token(curr_row, column, TYPEID, str);
            }
            else if (type == BOOL_CONST) {
                return make_token(curr_row, column, BOOL_CONST, str);
            }
            else
                return make_token(curr_row, column, type);
        }
        str += c;
    }
}

int Tokenizer::isKeyWord(std::string str)
{
    if (str == "class")
        return CLASS;
    if (str == "else")
        return ELSE;
    if (str == "if")
        return IF;
    if (str == "fi")
        return FI;
    if (str == "in")
        return IN;
    if (str == "inherits")
        return INHERITS;
    if (str == "let")
        return LET;
    if (str == "loop")
        return LOOP;
    if (str == "pool")
        return POOL;
    if (str == "then")
        return THEN;
    if (str == "while")
        return WHILE;
    if (str == "case")
        return CASE;
    if (str == "esac")
        return ESAC;
    if (str == "of")
        return OF;
    if (str == "new")
        return NEW;
    if (str == "isvoid")
        return ISVOID;
    if (str == "not")
        return NOT;
    if (str == "true" || str == "false") {
        return BOOL_CONST;
    }
    return -1;
}

Token Tokenizer::get_number()
{
    int c, column = curr_column + 1;
    std::string str;
    while (c = get_char(file)) {
        if (isdigit(c))
            str += c;
        else {
            unget_char(c);
            return make_token(curr_row, column, INT_CONST, str);
        }
    }
}

void Tokenizer::get_single_comment()
{
    int c;
    while (c = get_char(file)) {
        if (c == '\n') {
            curr_row++;
            curr_column = 0;
            return;
        }
    }
}

Token Tokenizer::get_string()
{
    int c, lookAhead, column = curr_column+1;
    std::string str;
    while (c = get_char(file)) {
        if (c == '"') {
            return make_token(curr_row, column, STR_CONST, str);
        }
        else if (c == '\\') {
            lookAhead = get_char(file);
            if (lookAhead == 'n')
                str += '\n';
            else if (lookAhead == 't')
                str += '\t';
            else {
                str += lookAhead;
            }
        }
        else if (c == EOF) {
            unget_char(c);
            std::pair<int, int> position(curr_row, curr_column);
            throw token_error(position,"unterminate string");
        }
        else 
            str += c;
    }
}

void Tokenizer::consume()
{
    next_token();
}

Token Tokenizer::lookahead(int n)
{
    int sum = n - buffer.size();
    for (int i = 0; i < sum; i++) {
        buffer.push_back(get_token());
    }
    return buffer[n-1];
}

Token Tokenizer::next_token()
{
    if (buffer.size()) {
        Token ret = buffer[0];
        buffer.erase(buffer.begin());
        return ret;
    }
    return get_token();
}
bool Tokenizer::assert(int type)
{
    Token t = next_token();
    if (t.type == type)
        return true;
    else 
        return false;
}