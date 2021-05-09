//
// Created by 史记 on 2021/4/18.
//

#ifndef SYSYCOMPILER_SYSY_H
#define SYSYCOMPILER_SYSY_H

#include <utility>
#include <vector>
#include <string>
#include "eeyore.h"
#include "nested_map.h"

class EeyoreGenerator;

class SysYExpression {
private:
public:
    virtual ~SysYExpression() = default;

    virtual std::shared_ptr<EeyoreValue> genEeyore(EeyoreGenerator *gen) const = 0;
};

class SysYBinary : public SysYExpression {
private:
    int op_;
    std::shared_ptr<SysYExpression> lhs_, rhs_;
public:
    SysYBinary(SysYExpression *lhs, int op, SysYExpression *rhs) : lhs_(lhs), op_(op), rhs_(rhs) {}

    SysYBinary(std::shared_ptr<SysYExpression> lhs, int op, std::shared_ptr<SysYExpression> rhs) :
            lhs_(std::move(lhs)), op_(op), rhs_(std::move(rhs)) {}

    std::shared_ptr<EeyoreValue> genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<SysYExpression> &lhs() const { return lhs_; }

    [[nodiscard]] const std::shared_ptr<SysYExpression> &rhs() const { return rhs_; }
};

class SysYUnary : public SysYExpression {
private:
    int op_;
    std::shared_ptr<SysYExpression> rhs_;
public:
    SysYUnary(int op, SysYExpression *rhs) : op_(op), rhs_(rhs) {}

    std::shared_ptr<EeyoreValue> genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<SysYExpression> &rhs() const { return rhs_; }
};

class SysYNumber : public SysYExpression {
private:
    int num_;
public:
    ~SysYNumber() override = default;

    explicit SysYNumber(int num) : num_(num) {}

    std::shared_ptr<EeyoreValue> genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int num() const { return num_; }
};

class SysYFunCall : public SysYExpression {
private:
    std::string ident_;
    std::vector<std::shared_ptr<SysYExpression>> params_;
public:
    SysYFunCall(std::string *ident, std::vector<std::shared_ptr<SysYExpression>> *params) :
            ident_(*ident), params_(*params) {
        delete ident;
        delete params;
    }

    std::shared_ptr<EeyoreValue> genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<std::shared_ptr<SysYExpression>> &params() const { return params_; }
};

class SysYLVal : public SysYExpression {
private:
    std::string ident_;
    std::vector<std::shared_ptr<SysYExpression>> indices_;
public:
    SysYLVal(std::string *ident, std::vector<std::shared_ptr<SysYExpression>> *indices) :
            ident_(*ident), indices_(*indices) {
        delete ident;
        delete indices;
    }

    std::shared_ptr<EeyoreValue> genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<std::shared_ptr<SysYExpression>> &indices() const { return indices_; }
};

class SysYBlockItem {
private:
public:
    virtual ~SysYBlockItem() = default;

    virtual void genEeyore(EeyoreGenerator *gen) const = 0;
};

class SysYStatement : public SysYBlockItem {
private:
public:
    ~SysYStatement() override = default;

    void genEeyore(EeyoreGenerator *gen) const override = 0;
};

class SysYBlockStmt : public SysYStatement {
private:
    std::vector<std::shared_ptr<SysYBlockItem>> items_;
public:
    explicit SysYBlockStmt(std::vector<std::shared_ptr<SysYBlockItem>> *items) : items_(*items) { delete items; }

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::vector<std::shared_ptr<SysYBlockItem>> &items() const { return items_; }
};

class SysYExpStmt : public SysYStatement {
private:
    std::shared_ptr<SysYExpression> exp_;
public:
    explicit SysYExpStmt(SysYExpression *exp = nullptr) : exp_(exp) {}

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::shared_ptr<SysYExpression> &exp() const { return exp_; }
};

class SysYAssignStmt : public SysYStatement {
private:
    std::shared_ptr<SysYLVal> lhs_;
    std::shared_ptr<SysYExpression> rhs_;
public:
    SysYAssignStmt(SysYLVal *lhs, SysYExpression *rhs) : lhs_(lhs), rhs_(rhs) {}

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::shared_ptr<SysYLVal> &lhs() const { return lhs_; }

    [[nodiscard]] const std::shared_ptr<SysYExpression> &rhs() const { return rhs_; }
};

class SysYIfStmt : public SysYStatement {
private:
    std::shared_ptr<SysYExpression> cond_;
    std::shared_ptr<SysYStatement> then_stmt_, else_stmt_;
public:
    SysYIfStmt(SysYExpression *cond, SysYStatement *then_stmt, SysYStatement *else_stmt = nullptr) :
            cond_(cond), then_stmt_(then_stmt), else_stmt_(else_stmt) {}

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::shared_ptr<SysYExpression> &cond() const { return cond_; }

    [[nodiscard]] const std::shared_ptr<SysYStatement> &then_stmt() const { return then_stmt_; }

    [[nodiscard]] const std::shared_ptr<SysYStatement> &else_stmt() const { return else_stmt_; }
};

class SysYWhileStmt : public SysYStatement {
private:
    std::shared_ptr<SysYExpression> cond_;
    std::shared_ptr<SysYStatement> stmt_;
public:
    SysYWhileStmt(SysYExpression *cond, SysYStatement *stmt) : cond_(cond), stmt_(stmt) {}

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] const std::shared_ptr<SysYExpression> &cond() const { return cond_; }

    [[nodiscard]] const std::shared_ptr<SysYStatement> &stmt() const { return stmt_; }
};

class SysYControlStmt : public SysYStatement {
private:
    int type_;
    std::shared_ptr<SysYExpression> exp_;
public:
    explicit SysYControlStmt(int type, SysYExpression *exp = nullptr) : type_(type), exp_(exp) {}

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] int type() const { return type_; }

    [[nodiscard]] const std::shared_ptr<SysYExpression> &exp() const { return exp_; }
};

class SysYDefine {
private:
public:
    virtual ~SysYDefine() = default;

    virtual void genEeyore(EeyoreGenerator *gen) const = 0;
};

class SysYInitVal {
private:
    std::shared_ptr<SysYExpression> exp_;
    std::vector<std::shared_ptr<SysYInitVal>> *list_;
public:
    ~SysYInitVal() {
        delete list_;
    }

    explicit SysYInitVal(SysYExpression *exp) : exp_(exp), list_(nullptr) {}

    explicit SysYInitVal(std::vector<std::shared_ptr<SysYInitVal>> *list) : exp_(nullptr), list_(list) {}

    [[nodiscard]] const std::shared_ptr<SysYExpression> &exp() const { return exp_; }

    [[nodiscard]] const std::vector<std::shared_ptr<SysYInitVal>> *list() const { return list_; }
};

class SysYVarDef : public SysYDefine, public SysYBlockItem {
private:
    bool is_const_;
    std::string ident_;
    std::vector<std::shared_ptr<SysYExpression>> indices_;
    std::shared_ptr<SysYInitVal> init_;
public:
    SysYVarDef(bool is_const, std::string *ident, std::vector<std::shared_ptr<SysYExpression>> *indices,
               SysYInitVal *init = nullptr) :
            is_const_(is_const), ident_(*ident), indices_(*indices), init_(init) {
        delete ident;
        delete indices;
    }

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] bool is_const() const { return is_const_; }

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<std::shared_ptr<SysYExpression>> &indices() const { return indices_; }

    [[nodiscard]] const std::shared_ptr<SysYInitVal> &init() const { return init_; }
};

class SysYFuncDef : public SysYDefine {
private:
    bool is_void_;
    std::string ident_;
    std::vector<std::shared_ptr<SysYVarDef>> params_;
    std::shared_ptr<SysYBlockStmt> block_;
public:
    SysYFuncDef(bool is_void, std::string *ident, std::vector<std::shared_ptr<SysYVarDef>> *params,
                SysYBlockStmt *block) :
            is_void_(is_void), ident_(*ident), params_(*params), block_(block) {
        delete params;
        delete ident;
    }

    void genEeyore(EeyoreGenerator *gen) const override;

    [[nodiscard]] bool is_void() const { return is_void_; }

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::vector<std::shared_ptr<SysYVarDef>> &params() const { return params_; }

    [[nodiscard]] const std::vector<std::shared_ptr<SysYBlockItem>> &items() const { return block_->items(); }
};

class SysYCompUnit {
private:
    std::vector<std::shared_ptr<SysYDefine>> defs_;
public:
    explicit SysYCompUnit(std::vector<std::shared_ptr<SysYDefine>> *defs) : defs_(*defs) { delete defs; }

    std::shared_ptr<EeyoreProgram> genEeyore(EeyoreGenerator *gen) const;

    [[nodiscard]] const std::vector<std::shared_ptr<SysYDefine>> &defs() const { return defs_; }
};

#endif //SYSYCOMPILER_SYSY_H
