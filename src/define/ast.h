//
// Created by 史记 on 2021/4/18.
//

#ifndef SYSYCOMPILER_AST_H
#define SYSYCOMPILER_AST_H


class AST {
public:
    virtual ~AST() = 0;
};

class Type {
private:
    bool is_const, is_void;
public:
    Type(bool is_const, bool is_void) : is_const(is_const), is_void(is_void) {}

    bool isConst() const { return is_const; }

    bool isVoid() const { return is_void; }
};

#endif //SYSYCOMPILER_AST_H
