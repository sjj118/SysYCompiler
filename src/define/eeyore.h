//
// Created by 史记 on 2021/4/30.
//

#ifndef SYSYCOMPILER_EEYORE_H
#define SYSYCOMPILER_EEYORE_H

#include <vector>
#include <memory>
#include <ostream>
#include <utility>
#include "tigger.h"

class E2TTransformer;

class EeyoreValue {
private:
public:
    [[nodiscard]] virtual std::string name() const = 0;
};

class EeyoreNumber : public EeyoreValue {
private:
    int num_;
public:
    explicit EeyoreNumber(int num) : num_(num) {}

    [[nodiscard]] int num() const { return num_; }

    [[nodiscard]]std::string name() const override { return std::to_string(num_); }
};

class EeyoreSymbol : public EeyoreValue {
private:
    int id_;
public:
    explicit EeyoreSymbol(int id) : id_(id) {}

    [[nodiscard]] virtual char prefix() const = 0;

    [[nodiscard]] int id() const { return id_; }

    [[nodiscard]] std::string name() const override { return prefix() + std::to_string(id_); }
};

class EeyorePrimSymbol : public EeyoreSymbol {
private:
public:
    explicit EeyorePrimSymbol(int id) : EeyoreSymbol(id) {}

    [[nodiscard]] char prefix() const override { return 'T'; }
};

class EeyoreTempSymbol : public EeyoreSymbol {
private:
public:
    explicit EeyoreTempSymbol(int id) : EeyoreSymbol(id) {}

    [[nodiscard]] char prefix() const override { return 't'; }
};

class EeyoreArgSymbol : public EeyoreSymbol {
private:
public:
    explicit EeyoreArgSymbol(int id) : EeyoreSymbol(id) {}

    [[nodiscard]] char prefix() const override { return 'p'; }
};

class EeyoreLabel : public EeyoreSymbol {
private:
public:
    explicit EeyoreLabel(int id) : EeyoreSymbol(id) {}

    [[nodiscard]] char prefix() const override { return 'l'; }
};

class EeyoreDeclaration {
private:
    std::shared_ptr<EeyoreSymbol> symbol_;
    int size_;
public:
    explicit EeyoreDeclaration(std::shared_ptr<EeyoreSymbol> symbol, int size = 0) :
            symbol_(std::move(symbol)), size_(size) {}

    [[nodiscard]] const std::shared_ptr<EeyoreSymbol> &symbol() const { return symbol_; }

    [[nodiscard]] int size() const { return size_; }

    void dump(std::ostream &os, const char prefix[] = "") const {
        os << prefix << "var ";
        if (size_)os << size_ << " ";
        os << symbol_->name() << std::endl;
    }
};

class EeyoreStatement {
private:
public:
    virtual void dump(std::ostream &os, const char prefix[]) const = 0;

    virtual void genTigger(E2TTransformer *gen) const = 0;
};

class EeyoreBinaryStmt : public EeyoreStatement {
private:
    int op_;
    std::shared_ptr<EeyoreSymbol> dst_;
    std::shared_ptr<EeyoreValue> lhs_, rhs_;
public:
    EeyoreBinaryStmt(int op, std::shared_ptr<EeyoreSymbol> dst,
                     std::shared_ptr<EeyoreValue> lhs, std::shared_ptr<EeyoreValue> rhs) :
            op_(op), dst_(std::move(dst)), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<EeyoreSymbol> &dst() const { return dst_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &lhs() const { return lhs_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &rhs() const { return rhs_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << dst_->name() << " = " << lhs_->name() << " " << op2str(op_) << " " << rhs_->name() << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreUnaryStmt : public EeyoreStatement {
private:
    int op_;
    std::shared_ptr<EeyoreSymbol> dst_;
    std::shared_ptr<EeyoreValue> rhs_;
public:
    EeyoreUnaryStmt(int op, std::shared_ptr<EeyoreSymbol> dst, std::shared_ptr<EeyoreValue> rhs) :
            op_(op), dst_(std::move(dst)), rhs_(std::move(rhs)) {}

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<EeyoreSymbol> &dst() const { return dst_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &rhs() const { return rhs_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << dst_->name() << " = " << op2str(op_) << rhs_->name() << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreAssignStmt : public EeyoreStatement {
private:
    std::shared_ptr<EeyoreSymbol> dst_;
    std::shared_ptr<EeyoreValue> val_;
    std::shared_ptr<EeyoreValue> dst_offset_, val_offset_;
public:
    EeyoreAssignStmt(std::shared_ptr<EeyoreSymbol> dst, std::shared_ptr<EeyoreValue> val,
                     std::shared_ptr<EeyoreValue> dst_offset = nullptr,
                     std::shared_ptr<EeyoreValue> val_offset = nullptr) :
            dst_(std::move(dst)), val_(std::move(val)),
            dst_offset_(std::move(dst_offset)), val_offset_(std::move(val_offset)) {}

    [[nodiscard]] const std::shared_ptr<EeyoreSymbol> &dst() const { return dst_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &val() const { return val_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &dst_offset() const { return dst_offset_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &val_offset() const { return val_offset_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << dst_->name();
        if (dst_offset_)os << "[" << dst_offset_->name() << "]";
        os << " = " << val_->name();
        if (val_offset_)os << "[" << val_offset_->name() << "]";
        os << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreIfStmt : public EeyoreStatement {
private:
    int op_;
    std::shared_ptr<EeyoreValue> lhs_, rhs_;
    std::shared_ptr<EeyoreLabel> label_;
public:
    EeyoreIfStmt(int op, std::shared_ptr<EeyoreValue> lhs, std::shared_ptr<EeyoreValue> rhs,
                 std::shared_ptr<EeyoreLabel> label) :
            op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)), label_(std::move(label)) {}

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &lhs() const { return lhs_; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &rhs() const { return rhs_; }

    [[nodiscard]] const std::shared_ptr<EeyoreLabel> &label() const { return label_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "if " << lhs_->name() << " " << op2str(op_) << " " << rhs_->name()
           << " goto " << label_->name() << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreGotoStmt : public EeyoreStatement {
private:
    std::shared_ptr<EeyoreLabel> label_;
public:
    explicit EeyoreGotoStmt(std::shared_ptr<EeyoreLabel> label) : label_(std::move(label)) {}

    [[nodiscard]] const std::shared_ptr<EeyoreLabel> &label() const { return label_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "goto " << label_->name() << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreLabelStmt : public EeyoreStatement {
private:
    std::shared_ptr<EeyoreLabel> label_;
public:
    explicit EeyoreLabelStmt(std::shared_ptr<EeyoreLabel> label) : label_(std::move(label)) {}

    [[nodiscard]] const std::shared_ptr<EeyoreLabel> &label() const { return label_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << label_->name() << ":" << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreParamStmt : public EeyoreStatement {
private:
    std::shared_ptr<EeyoreValue> param_;
public:
    explicit EeyoreParamStmt(std::shared_ptr<EeyoreValue> param) : param_(std::move(param)) {}

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &param() const { return param_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "param " << param_->name() << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreFunCall : public EeyoreStatement {
private:
    std::string ident_;
    std::shared_ptr<EeyoreSymbol> dst_;
public:
    explicit EeyoreFunCall(std::string ident, std::shared_ptr<EeyoreSymbol> dst = nullptr) :
            ident_(std::move(ident)), dst_(std::move(dst)) {}

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] const std::shared_ptr<EeyoreSymbol> &dst() const { return dst_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix;
        if (dst_)os << dst_->name() << " = ";
        os << "call " << ident_ << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreReturnStmt : public EeyoreStatement {
private:
    std::shared_ptr<EeyoreValue> val_;
public:
    explicit EeyoreReturnStmt(std::shared_ptr<EeyoreValue> val) : val_(std::move(val)) {}

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &val() const { return val_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "return";
        if (val_)os << " " << val_->name();
        os << std::endl;
    }

    void genTigger(E2TTransformer *gen) const override;
};

class EeyoreFunc {
private:
    std::string ident_;
    int arg_num_;
    std::vector<std::shared_ptr<EeyoreDeclaration>> decls_;
    std::vector<std::shared_ptr<EeyoreStatement>> stmts_;
public:
    EeyoreFunc(std::string ident, int arg_num) : ident_(std::move(ident)), arg_num_(arg_num) {}

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] int arg_num() const { return arg_num_; }

    [[nodiscard]] const std::vector<std::shared_ptr<EeyoreDeclaration>> &decls() const { return decls_; }

    [[nodiscard]] const std::vector<std::shared_ptr<EeyoreStatement>> &stmts() const { return stmts_; }

    void push_decl(const std::shared_ptr<EeyoreDeclaration> &decl) {
        decls_.push_back(decl);
    }

    void push_stmt(const std::shared_ptr<EeyoreStatement> &stmt) {
        stmts_.push_back(stmt);
    }

    void dump(std::ostream &os) const {
        os << ident_ << " [" << arg_num_ << "]" << std::endl;
        for (const auto &decl:decls_)decl->dump(os, "  ");
        os << std::endl;
        for (const auto &stmt:stmts_)stmt->dump(os, "  ");
        os << "end " << ident_ << std::endl;
    }

    void genTigger(E2TTransformer *gen) const;
};

class EeyoreProgram {
private:
    std::vector<std::shared_ptr<EeyoreDeclaration>> decls_;
    std::vector<std::shared_ptr<EeyoreAssignStmt>> inits_;
    std::vector<std::shared_ptr<EeyoreFunc>> funcs_;
public:
    EeyoreProgram() = default;

    [[nodiscard]] const std::vector<std::shared_ptr<EeyoreDeclaration>> &decls() const { return decls_; }

    [[nodiscard]] const std::vector<std::shared_ptr<EeyoreAssignStmt>> &inits() const { return inits_; }

    [[nodiscard]] const std::vector<std::shared_ptr<EeyoreFunc>> &funcs() const { return funcs_; }

    void push_decl(const std::shared_ptr<EeyoreDeclaration> &decl) {
        decls_.push_back(decl);
    }

    void push_init(const std::shared_ptr<EeyoreAssignStmt> &init) {
        inits_.push_back(init);
    }

    void push_func(const std::shared_ptr<EeyoreFunc> &func) {
        funcs_.push_back(func);
    }

    void dump(std::ostream &os) const {
        for (const auto &decl:decls_)decl->dump(os, "");
        os << std::endl;
        for (const auto &init:inits_)init->dump(os, "");
        os << std::endl;
        for (const auto &func:funcs_)func->dump(os), os << std::endl;
    }

    std::shared_ptr<TiggerProgram> genTigger(E2TTransformer *gen) const;
};


#endif //SYSYCOMPILER_EEYORE_H
