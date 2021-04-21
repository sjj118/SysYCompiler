//
// Created by 史记 on 2021/4/18.
//

#ifndef SYSYCOMPILER_AST_H
#define SYSYCOMPILER_AST_H

#include <utility>
#include <vector>
#include <string>
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

class StmtAST : public AST {
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

class FunCallAST : public ExpressionAST {
private:
    std::string ident;
    std::vector<ExpressionAST *> params;
public:
    FunCallAST(std::string ident, std::vector<ExpressionAST *> params) : ident(std::move(ident)),
                                                                         params(std::move(params)) {}

    int eval(Context *ctx) const override;
};

class LValAST : public ExpressionAST {
private:
    std::string ident;
    std::vector<ExpressionAST *> indices;
public:
    LValAST(std::string ident, std::vector<ExpressionAST *> indices) : ident(std::move(ident)),
                                                                       indices(std::move(indices)) {}

    int eval(Context *ctx) const override;

    [[nodiscard]] const std::string &getIdent() const { return ident; }
};

class BlockStmtAST : public StmtAST {
private:
    std::vector<StmtAST *> stmts;
public:
    explicit BlockStmtAST(std::vector<StmtAST *> stmts) : stmts(std::move(stmts)) {}

    int eval(Context *ctx) const override;
};

class ExpStmtAST : public StmtAST {
private:
    ExpressionAST *exp;
public:
    explicit ExpStmtAST(ExpressionAST *exp) : exp(exp) {}

    int eval(Context *ctx) const override;
};

class AssignStmtAST : public StmtAST {
private:
    LValAST *lhs;
    ExpressionAST *rhs;
public:
    AssignStmtAST(LValAST *lhs, ExpressionAST *rhs) : lhs(lhs), rhs(rhs) {}

    int eval(Context *ctx) const override;
};

class IfStmtAST : public StmtAST {
private:
    ExpressionAST *cond;
    StmtAST *then_stmt, *else_stmt;
public:
    IfStmtAST(ExpressionAST *cond, StmtAST *then_stmt, StmtAST *else_stmt) : cond(cond), then_stmt(then_stmt),
                                                                             else_stmt(else_stmt) {}

    int eval(Context *ctx) const override;
};

class WhileStmtAST : public StmtAST {
private:
    ExpressionAST *cond;
    StmtAST *stmt;
public:
    WhileStmtAST(ExpressionAST *cond, StmtAST *stmt) : cond(cond), stmt(stmt) {}

    int eval(Context *ctx) const override;
};

class ControlStmtAST : public StmtAST {
private:
    int type;
    ExpressionAST *exp;
public:
    ControlStmtAST(int type, ExpressionAST *exp) : type(type), exp(exp) {}

    int eval(Context *ctx) const override;
};

#endif //SYSYCOMPILER_AST_H
