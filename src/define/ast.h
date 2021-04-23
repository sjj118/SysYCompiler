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
    std::string *ident;
    std::vector<ExpressionAST *> *params;
public:
    FunCallAST(std::string *ident, std::vector<ExpressionAST *> *params) : ident(ident),
                                                                           params(params) {}

    int eval(Context *ctx) const override;
};

class LValAST : public ExpressionAST {
private:
    std::string *ident;
    std::vector<ExpressionAST *> *indices;
public:
    LValAST(std::string *ident, std::vector<ExpressionAST *> *indices) : ident(ident),
                                                                         indices(indices) {}

    int eval(Context *ctx) const override;

    [[nodiscard]] const std::string &getIdent() const { return *ident; }
};

class StmtAST : public AST {
private:
public:
};

class BlockStmtAST : public StmtAST {
private:
    std::vector<StmtAST *> *stmts;
public:
    explicit BlockStmtAST(std::vector<StmtAST *> *stmts) : stmts(stmts) {}

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

class DefAST : public AST {
private:
public:
};

class InitValAST : public AST {
private:
    ExpressionAST *exp;
    std::vector<InitValAST *> *list;
public:
    explicit InitValAST(ExpressionAST *exp) : exp(exp), list(nullptr) {}

    explicit InitValAST(std::vector<InitValAST *> *list) : exp(nullptr), list(list) {}

    int eval(Context *ctx) const override;
};

class VarDefAST : public DefAST {
private:
    bool is_const;
    std::string *ident;
    std::vector<ExpressionAST *> *indices;
    InitValAST *init;
public:
    VarDefAST(bool is_const, std::string *ident, std::vector<ExpressionAST *> *indices, InitValAST *init) :
            is_const(is_const), ident(ident), indices(indices), init(init) {}

    int eval(Context *ctx) const override;
};

class FuncDefAST : public DefAST {
private:
    bool is_void;
    std::string *ident;
    std::vector<VarDefAST *> *params;
    BlockStmtAST *stmts;
public:
    FuncDefAST(bool is_void, std::string *ident, std::vector<VarDefAST *> *params, BlockStmtAST *stmts) :
            is_void(is_void), ident(ident), params(params), stmts(stmts) {}

    int eval(Context *ctx) const override;
};

class CompUnitAST : public AST {
private:
    std::vector<DefAST *> *defs;
public:
    explicit CompUnitAST(std::vector<DefAST *> *defs) : defs(defs) {}

    int eval(Context *ctx) const override;
};

#endif //SYSYCOMPILER_AST_H
