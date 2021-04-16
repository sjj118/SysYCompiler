//
// Created by 史记 on 2021/4/16.
//

#ifndef SYSYCOMPILER_TOKEN_H
#define SYSYCOMPILER_TOKEN_H

#define SYSY_KEYWORDS(e) \
  e(Const, "const") e(Int, "int") e(Void, "void")\
  e(If, "if") e(Else, "else") e(While, "while")\
  e(Break, "break") e(Continue, "continue") e(Return, "return")

#define SYSY_OPERATORS(e) \
  e(Add, "+") e(Sub, "-") e(Mul, "*") e(Div, "/") e(Mod, "%") \
  e(Eq, "==") e(NotEq, "!=") e(Less, "<") e(Great, ">") e(LessEq, "<=") e(GreatEq, ">=") \
  e(LNot, "!") e(LAnd, "&&") e(LOr, "||") e(Assign, "=")

#define EXPAND_FIRST(i, ...)      i,
#define EXPAND_SECOND(i, j, ...)  j,

enum class Token {
    Error, End,
    Id, Integer, Keyword, Operator, Other,
};

enum class Keyword {
    SYSY_KEYWORDS(EXPAND_FIRST)
};
enum class Operator {
    SYSY_OPERATORS(EXPAND_FIRST)
};

#endif //SYSYCOMPILER_TOKEN_H
