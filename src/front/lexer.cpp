//
// Created by 史记 on 2021/4/16.
//

#include "lexer.h"

const char *kKeywords[] = {SYSY_KEYWORDS(EXPAND_SECOND)};
const char *kOperators[] = {SYSY_OPERATORS(EXPAND_SECOND)};

template<typename T, std::size_t N>
int GetIndex(std::string_view str, T (&str_array)[N]) {
    for (int i = 0; i < N; ++i) {
        if (str == str_array[i]) return i;
    }
    return -1;
}

bool isOperatorChar(char c) {
    static const char op_chars[] = "+-*/%=!<>&|";
    for (auto i:op_chars)if (i == c)return true;
    return false;
}

Token Lexer::nextToken() {
    while (!in.eof() && std::isspace(in.peek()))in.get();       // ignore space
    if (in.eof())return Token::End;                                 // eof
    if (in.peek() == '/') {                                         // possible comment
        in.get();
        if (!in.eof()) {
            if (in.peek() == '/')return handleLineComment();
            else if (in.peek() == '*')return handleBlockComment();
        }
        in.putback('/');
    }
    if (std::isalpha(in.peek()) || in.peek() == '_')return handleId();  // id or keyword
    if (std::isdigit(in.peek())) return handleInteger();
    if (isOperatorChar(in.peek()))return handleOperator();
    other_val_ = in.get();
    return Token::Other;
}

Token Lexer::handleLineComment() {
    while (!in.eof() && in.peek() != '\n' && in.peek() != '\r') in.get();
    return nextToken();
}

Token Lexer::handleBlockComment() {
    in.get();
    while (!in.eof() && !(in.get() == '*' && in.peek() == '/'));
    if (in.eof())return Token::Error;
    in.get();
    return nextToken();
}

Token Lexer::handleId() {
    std::string id;
    while (!in.eof() && (std::isalnum(in.peek()) || in.peek() == '_')) id += in.get();
    int index = GetIndex(id, kKeywords);
    if (index < 0) {
        id_val_ = std::move(id);
        return Token::Id;
    } else {
        key_val_ = Keyword(index);
        return Token::Keyword;
    }
}

int char2digit(char c) {
    if (c >= '0' && c <= '9')return c - '0';
    else if (c >= 'A' && c <= 'F')return c - 'A' + 10;
    else if (c >= 'a' && c <= 'f')return c - 'a' + 10;
    return -1;
}

Token Lexer::handleInteger() {
    int num = in.get() - '0';
    int base = 10;
    if (num == 0 && !in.eof()) {
        if (in.peek() == 'x' || in.peek() == 'X') {
            base = 16;
            in.get();
        } else base = 8;
    }
    int d;
    while (!in.eof() && (d = char2digit(in.peek())) != -1) {
        in.get();
        if (d < 0 || d >= base)return Token::Error;
        num = num * base + d;
    }
    int_val_ = num;
    return Token::Integer;
}

Token Lexer::handleOperator() {
    std::string op;
    while (!in.eof() && isOperatorChar(in.peek()))op += in.get();
    int index = GetIndex(op, kOperators);
    if (index < 0) return Token::Error;
    op_val_ = Operator(index);
    return Token::Operator;
}

std::string Lexer::toString(Token token) const {
    if (token == Token::Id)return "<Id: " + id_val() + ">";
    else if (token == Token::Integer)return "<Int: " + std::to_string(int_val()) + ">";
    else if (token == Token::Keyword)return "<Key: " + std::string(kKeywords[(int) key_val()]) + ">";
    else if (token == Token::Operator)return "<Op: " + std::string(kOperators[(int) op_val()]) + ">";
    else if (token == Token::Other) {
        std::string val = " ";
        val[0] = other_val();
        return "<Other: " + val + ">";
    } else if (token == Token::End)return "<End>";
    else return "<Error>";
}