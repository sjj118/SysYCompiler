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

std::string char2string(char c) {
    std::string s = " ";
    s[0] = c;
    return s;
}

bool isOperatorChar(char c) {
    static const char op_chars[] = "+-*/%=!<>&|";
    for (auto i:op_chars)if (i == c)return true;
    return false;
}

bool isIdChar(char c) {
    return std::isalnum(c) || c == '_';
}

Token Lexer::nextToken() {
    while (!in.eof() && std::isspace(in.peek()))in.get();       // ignore space
    if (in.eof())return Token::End;                                 // eof
    if (std::isalpha(in.peek()) || in.peek() == '_')return handleId();  // id or keyword
    if (std::isdigit(in.peek())) return handleInteger();
    if (isOperatorChar(in.peek()))return handleOperator();
    other_val_ = in.get();
    return Token::Other;
}

Token Lexer::handleLineComment() {
    while (!in.eol()) in.get();
    return nextToken();
}

Token Lexer::handleBlockComment() {
    in.get();
    while (!in.eof() && !(in.get() == '*' && in.peek() == '/'));
    if (in.eof())return logError("unterminated /* comment");
    in.get();
    return nextToken();
}

Token Lexer::handleId() {
    std::string id;
    while (!in.eof() && isIdChar(in.peek())) id += in.get();
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
    while (!in.eof() && isIdChar(in.peek())) {
        int d = char2digit(in.peek());
        if (d < 0 || d >= base)return logError("invalid character " + char2string(in.get()) + " in integer");
        num = num * base + d;
        in.get();
    }
    int_val_ = num;
    return Token::Integer;
}

Token Lexer::handleOperator() {
    std::string op;
    op += in.get();
    if (op[0] == '/' && !in.eof()) {         // possible comment
        if (in.peek() == '/')return handleLineComment();
        else if (in.peek() == '*')return handleBlockComment();
    }
    while (!in.eof() && isOperatorChar(in.peek()))op += in.get();
    int index = GetIndex(op, kOperators);
    if (index < 0) return logError("unknown operator " + op);
    op_val_ = Operator(index);
    return Token::Operator;
}

std::string Lexer::toString(Token token) const {
    if (token == Token::Id)return "<Id: " + id_val() + ">";
    else if (token == Token::Integer)return "<Int: " + std::to_string(int_val()) + ">";
    else if (token == Token::Keyword)return "<Key: " + std::string(kKeywords[(int) key_val()]) + ">";
    else if (token == Token::Operator)return "<Op: " + std::string(kOperators[(int) op_val()]) + ">";
    else if (token == Token::Other) return "<Other: " + char2string(other_val()) + ">";
    else if (token == Token::End)return "<End>";
    else return "<Error>";
}

Token Lexer::logError(const std::string &msg) const {
    std::cerr << in.name() << ":" << in.line() << ":" << in.col() << ": error: " << msg << std::endl;
    return Token::Error;
}