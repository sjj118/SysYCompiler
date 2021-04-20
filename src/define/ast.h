//
// Created by 史记 on 2021/4/18.
//

#ifndef SYSYCOMPILER_AST_H
#define SYSYCOMPILER_AST_H

#include "context.h"

class AST {
private:
public:
    virtual int eval(Context *ctx) const = 0;
};

class ExpressionAST : public AST {
private:
public:
};

class BinaryAST : public ExpressionAST {
private:
    int op;
    AST *lhs, *rhs;
public:
    BinaryAST(AST *lhs, int op, AST *rhs) : lhs(lhs), op(op), rhs(rhs) {}

    int eval(Context *ctx) const override;
};

class UnaryAST : public ExpressionAST {
private:
    int op;
    AST *rhs;
public:
    UnaryAST(int op, AST *rhs) : op(op), rhs(rhs) {}

    int eval(Context *ctx) const override;
};

class NumberAST : public ExpressionAST {
private:
    int val;
public:
    explicit NumberAST(int val) : val(val) {}

    int eval(Context *ctx) const override;
};

#endif //SYSYCOMPILER_AST_H
