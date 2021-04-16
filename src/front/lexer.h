//
// Created by 史记 on 2021/4/16.
//

#ifndef SYSYCOMPILER_LEXER_H
#define SYSYCOMPILER_LEXER_H

#include <istream>
#include "reader.h"
#include "token.h"

class Lexer {
private:
    Reader &in;
    std::string id_val_;
    int int_val_;
    Keyword key_val_;
    Operator op_val_;
    char other_val_;

    Token handleLineComment();

    Token handleBlockComment();

    Token handleId();

    Token handleInteger();

    Token handleOperator();

    Token logError(const std::string &msg) const;

public:
    explicit Lexer(Reader &in) : in(in) {}

    Token nextToken();

    std::string id_val() const { return id_val_; }

    int int_val() const { return int_val_; }

    Keyword key_val() const { return key_val_; }

    Operator op_val() const { return op_val_; }

    char other_val() const { return other_val_; }

    std::string toString(Token token) const;
};

#endif //SYSYCOMPILER_LEXER_H
