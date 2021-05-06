//
// Created by 史记 on 2021/4/18.
//

#ifndef SYSYCOMPILER_SYSY_H
#define SYSYCOMPILER_SYSY_H

#include <utility>
#include <vector>
#include <string>
#include "eeyore.h"
#include "environment.h"

class EeyoreGenerator;

class SysYExpression {
private:
public:
    virtual ~SysYExpression() = default;

    virtual EeyoreValue *generateEeyore(EeyoreGenerator *gen) const = 0;
};

class SysYBinary : public SysYExpression {
private:
    int op_;
    SysYExpression *lhs_, *rhs_;
public:
    ~SysYBinary() override {
        delete lhs_;
        delete rhs_;
    }

    SysYBinary(SysYExpression *lhs, int op, SysYExpression *rhs) : lhs_(lhs), op_(op), rhs_(rhs) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const SysYExpression *lhs() const { return lhs_; }

    [[nodiscard]] const SysYExpression *rhs() const { return rhs_; }
};

class SysYUnary : public SysYExpression {
private:
    int op_;
    SysYExpression *rhs_;
public:
    ~SysYUnary() override { delete rhs_; }

    SysYUnary(int op, SysYExpression *rhs) : op_(op), rhs_(rhs) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const SysYExpression *rhs() const { return rhs_; }
};

class SysYNumber : public SysYExpression {
private:
    int val_;
public:
    ~SysYNumber() override = default;

    explicit SysYNumber(int val) : val_(val) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int val() const { return val_; }
};

class SysYFunCall : public SysYExpression {
private:
    std::string ident_;
    std::vector<SysYExpression *> params_;
public:
    ~SysYFunCall() override {
        for (auto *exp:params_)delete exp;
    }

    SysYFunCall(std::string *ident, std::vector<SysYExpression *> *params) : ident_(*ident), params_(*params) {
        delete ident;
        delete params;
    }

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<SysYExpression *> &params() const { return params_; }
};

class SysYLVal : public SysYExpression {
private:
    std::string ident_;
    std::vector<SysYExpression *> indices_;
public:
    ~SysYLVal() override {
        for (auto *exp:indices_)delete exp;
    }

    SysYLVal(std::string *ident, std::vector<SysYExpression *> *indices) : ident_(*ident), indices_(*indices) {
        delete ident;
        delete indices;
    }

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<SysYExpression *> &indices() const { return indices_; }
};

class SysYStatement {
private:
public:
    virtual ~SysYStatement() = default;

    virtual EeyoreValue *generateEeyore(EeyoreGenerator *gen) const = 0;
};

class SysYBlockStmt : public SysYStatement {
private:
    std::vector<SysYStatement *> stmts_;
public:
    ~SysYBlockStmt() override {
        for (auto *stmt:stmts_)delete stmt;
    }

    explicit SysYBlockStmt(std::vector<SysYStatement *> *stmts) : stmts_(*stmts) { delete stmts; }

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::vector<SysYStatement *> &stmts() const { return stmts_; }
};

class SysYExpStmt : public SysYStatement {
private:
    SysYExpression *exp_;
public:
    ~SysYExpStmt() override { delete exp_; }

    explicit SysYExpStmt(SysYExpression *exp = nullptr) : exp_(exp) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] SysYExpression *exp() const { return exp_; }
};

class SysYAssignStmt : public SysYStatement {
private:
    SysYLVal *lhs_;
    SysYExpression *rhs_;
public:
    ~SysYAssignStmt() override {
        delete lhs_;
        delete rhs_;
    }

    SysYAssignStmt(SysYLVal *lhs, SysYExpression *rhs) : lhs_(lhs), rhs_(rhs) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const SysYLVal *lhs() const { return lhs_; }

    [[nodiscard]] const SysYExpression *rhs() const { return rhs_; }
};

class SysYIfStmt : public SysYStatement {
private:
    SysYExpression *cond_;
    SysYStatement *then_stmt_, *else_stmt_;
public:
    ~SysYIfStmt() override {
        delete cond_;
        delete then_stmt_;
        delete else_stmt_;
    }

    SysYIfStmt(SysYExpression *cond, SysYStatement *then_stmt, SysYStatement *else_stmt = nullptr) : cond_(cond),
                                                                                                     then_stmt_(
                                                                                                             then_stmt),
                                                                                                     else_stmt_(
                                                                                                             else_stmt) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const SysYExpression *cond() const { return cond_; }

    [[nodiscard]] const SysYStatement *then_stmt() const { return then_stmt_; }

    [[nodiscard]] const SysYStatement *else_stmt() const { return else_stmt_; }
};

class SysYWhileStmt : public SysYStatement {
private:
    SysYExpression *cond_;
    SysYStatement *stmt_;
public:
    ~SysYWhileStmt() override {
        delete cond_;
        delete stmt_;
    }

    SysYWhileStmt(SysYExpression *cond, SysYStatement *stmt) : cond_(cond), stmt_(stmt) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const SysYExpression *cond() const { return cond_; }

    [[nodiscard]] const SysYStatement *stmt() const { return stmt_; }
};

class SysYControlStmt : public SysYStatement {
private:
    int type_;
    SysYExpression *exp_;
public:
    ~SysYControlStmt() override { delete exp_; }

    explicit SysYControlStmt(int type, SysYExpression *exp = nullptr) : type_(type), exp_(exp) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int type() const { return type_; }

    [[nodiscard]] const SysYExpression *exp() const { return exp_; }
};

class SysYDefine {
private:
public:
    virtual ~SysYDefine() = default;

    virtual EeyoreValue *generateEeyore(EeyoreGenerator *gen) const = 0;
};

class SysYInitVal {
private:
    SysYExpression *exp_;
    std::vector<SysYInitVal *> *list_;
public:
    ~SysYInitVal() {
        delete exp_;
        for (auto *init:*list_)delete init;
        delete list_;
    }

    explicit SysYInitVal(SysYExpression *exp) : exp_(exp), list_(nullptr) {}

    explicit SysYInitVal(std::vector<SysYInitVal *> *list) : exp_(nullptr), list_(list) {}

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const;

    [[nodiscard]] const SysYExpression *exp() const { return exp_; }

    [[nodiscard]] const std::vector<SysYInitVal *> *list() const { return list_; }
};

class SysYVarDef : public SysYDefine {
private:
    bool is_const_;
    std::string ident_;
    std::vector<SysYExpression *> indices_;
    SysYInitVal *init_;
public:
    ~SysYVarDef() override {
        for (auto *exp:indices_)delete exp;
        delete init_;
    }

    SysYVarDef(bool is_const, std::string *ident, std::vector<SysYExpression *> *indices, SysYInitVal *init = nullptr) :
            is_const_(is_const), ident_(*ident), indices_(*indices), init_(init) {
        delete ident;
        delete indices;
    }

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] bool is_const() const { return is_const_; }

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<SysYExpression *> &indices() const { return indices_; }

    [[nodiscard]] const SysYInitVal *init() const { return init_; }
};

class SysYFuncDef : public SysYDefine {
private:
    bool is_void_;
    std::string ident_;
    std::vector<SysYVarDef *> params_;
    SysYBlockStmt *stmts_;
public:
    ~SysYFuncDef() override {
        for (auto *param:params_)delete param;
        delete stmts_;
    }

    SysYFuncDef(bool is_void, std::string *ident, std::vector<SysYVarDef *> *params, SysYBlockStmt *stmts) :
            is_void_(is_void), ident_(*ident), params_(*params), stmts_(stmts) {
        delete params;
        delete ident;
    }

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] bool is_void() const { return is_void_; }

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<SysYVarDef *> &params() const { return params_; }

    [[nodiscard]] const SysYBlockStmt *stmts() const { return stmts_; }
};

class SysYCompUnit {
private:
    std::vector<SysYDefine *> defs_;
public:
    ~SysYCompUnit() {
        for (auto *def:defs_)delete def;
    }

    explicit SysYCompUnit(std::vector<SysYDefine *> *defs) : defs_(*defs) { delete defs; }

    EeyoreValue *generateEeyore(EeyoreGenerator *gen) const;

    [[nodiscard]] const std::vector<SysYDefine *> &defs() const { return defs_; }
};

#endif //SYSYCOMPILER_SYSY_H
