//
// Created by 史记 on 2021/5/5.
//

#ifndef SYSYCOMPILER_TIGGER_H
#define SYSYCOMPILER_TIGGER_H

#include <vector>
#include <memory>
#include <ostream>
#include <utility>
#include "operator.h"

class TiggerValue {
private:
public:
    [[nodiscard]] virtual std::string name() const = 0;
};

class TiggerGlobal {
private:
public:
    [[nodiscard]] virtual std::string name() const = 0;
};

class TiggerNum : public TiggerValue, public TiggerGlobal {
private:
    int num_;
public:
    explicit TiggerNum(int num) : num_(num) {}

    [[nodiscard]] int num() const { return num_; }

    [[nodiscard]]std::string name() const override { return std::to_string(num_); }
};


class TiggerReg : public TiggerValue {
private:
    char prefix_;
    int id_;
public:
    TiggerReg(char prefix, int id) : prefix_(prefix), id_(id) {};

    [[nodiscard]] char prefix() const { return prefix_; }

    [[nodiscard]] int id() const { return id_; }

    [[nodiscard]] std::string name() const override { return prefix() + std::to_string(id_); }
};

class TiggerVariable : public TiggerGlobal {
private:
    int id_;
public:
    explicit TiggerVariable(int id) : id_(id) {};

    [[nodiscard]] int id() const { return id_; }

    [[nodiscard]] std::string name() const override { return 'v' + std::to_string(id_); }
};

class TiggerLabel {
private:
    int id_;
public:
    explicit TiggerLabel(int id) : id_(id) {};

    [[nodiscard]] int id() const { return id_; }

    [[nodiscard]] std::string name() const { return 'l' + std::to_string(id_); }
};

class TiggerDeclaration {
private:
    std::shared_ptr<TiggerVariable> var_;
    int size_{}, init_{};
public:
    TiggerDeclaration(std::shared_ptr<TiggerVariable> var, int size, int init) :
            var_(std::move(var)), size_(size), init_(init) {}

    void dump(std::ostream &os) const {
        os << var_->name() << " = ";
        if (size_)os << "malloc " << size_;
        else os << init_;
        os << std::endl;
    }
};

class TiggerStatement {
private:
public:
    virtual void dump(std::ostream &os, const char prefix[]) const = 0;
};

class TiggerBinaryStmt : public TiggerStatement {
private:
    int op_;
    std::shared_ptr<TiggerReg> dst_, lhs_;
    std::shared_ptr<TiggerValue> rhs_;
public:
    TiggerBinaryStmt(int op, std::shared_ptr<TiggerReg> dst,
                     std::shared_ptr<TiggerReg> lhs, std::shared_ptr<TiggerValue> rhs) :
            op_(op), dst_(std::move(dst)), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << dst_->name() << " = " << lhs_->name() << " " << op2str(op_) << " " << rhs_->name() << std::endl;
    }
};

class TiggerUnaryStmt : public TiggerStatement {
private:
    int op_;
    std::shared_ptr<TiggerReg> dst_, rhs_;
public:
    TiggerUnaryStmt(int op, std::shared_ptr<TiggerReg> dst, std::shared_ptr<TiggerReg> rhs) :
            op_(op), dst_(std::move(dst)), rhs_(std::move(rhs)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << dst_->name() << " = " << op2str(op_) << rhs_->name() << std::endl;
    }
};

class TiggerAssignStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerReg> dst_;
    std::shared_ptr<TiggerValue> val_;
    std::shared_ptr<TiggerValue> dst_offset_, val_offset_;
public:
    TiggerAssignStmt(std::shared_ptr<TiggerReg> dst, std::shared_ptr<TiggerValue> val,
                     std::shared_ptr<TiggerValue> dst_offset = nullptr,
                     std::shared_ptr<TiggerValue> val_offset = nullptr) :
            dst_(std::move(dst)), val_(std::move(val)),
            dst_offset_(std::move(dst_offset)), val_offset_(std::move(val_offset)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << dst_->name();
        if (dst_offset_)os << "[" << dst_offset_->name() << "]";
        os << " = " << val_->name();
        if (val_offset_)os << "[" << val_offset_->name() << "]";
        os << std::endl;
    }
};

class TiggerIfStmt : public TiggerStatement {
private:
    int op_;
    std::shared_ptr<TiggerReg> lhs_, rhs_;
    std::shared_ptr<TiggerLabel> label_;
public:
    TiggerIfStmt(int op, std::shared_ptr<TiggerReg> lhs, std::shared_ptr<TiggerReg> rhs,
                 std::shared_ptr<TiggerLabel> label) :
            op_(op), lhs_(std::move(lhs)), rhs_(std::move(rhs)), label_(std::move(label)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "if " << lhs_->name() << " " << op2str(op_) << " " << rhs_->name()
           << " goto " << label_->name() << std::endl;
    }
};

class TiggerGotoStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerLabel> label_;
public:
    explicit TiggerGotoStmt(std::shared_ptr<TiggerLabel> label) : label_(std::move(label)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "goto " << label_->name() << std::endl;
    }
};

class TiggerLabelStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerLabel> label_;
public:
    explicit TiggerLabelStmt(std::shared_ptr<TiggerLabel> label) : label_(std::move(label)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << label_->name() << ":" << std::endl;
    }
};

class TiggerFunCall : public TiggerStatement {
private:
    std::string ident_;
public:
    explicit TiggerFunCall(std::string ident) : ident_(std::move(ident)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "call " << ident_ << std::endl;
    }
};

class TiggerReturnStmt : public TiggerStatement {
private:
public:
    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "return" << std::endl;
    }
};

class TiggerStoreStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerReg> reg_;
    std::shared_ptr<TiggerNum> num_;
public:
    TiggerStoreStmt(std::shared_ptr<TiggerReg> reg, std::shared_ptr<TiggerNum> num) :
            reg_(std::move(reg)), num_(std::move(num)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "store " << reg_->name() << " " << num_->name();
    }
};

class TiggerLoadStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerGlobal> num_;
    std::shared_ptr<TiggerReg> reg_;
public:
    TiggerLoadStmt(std::shared_ptr<TiggerGlobal> num, std::shared_ptr<TiggerReg> reg) :
            num_(std::move(num)), reg_(std::move(reg)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "load " << num_->name() << " " << reg_->name();
    }
};

class TiggerLoadaddrStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerGlobal> num_;
    std::shared_ptr<TiggerReg> reg_;
public:
    TiggerLoadaddrStmt(std::shared_ptr<TiggerGlobal> num, std::shared_ptr<TiggerReg> reg) :
            num_(std::move(num)), reg_(std::move(reg)) {}

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "loadaddr " << num_->name() << " " << reg_->name();
    }
};

class TiggerFunc {
private:
    std::string ident_;
    int arg_num_, slot_num_;
    std::vector<std::shared_ptr<TiggerStatement>> stmts_;
public:
    TiggerFunc(std::string ident, int arg_num, int slot_num) :
            ident_(std::move(ident)), arg_num_(arg_num), slot_num_(slot_num) {}

    void push_stmt(const std::shared_ptr<TiggerStatement> &stmt) {
        stmts_.push_back(stmt);
    }

    void dump(std::ostream &os) const {
        os << ident_ << " [" << arg_num_ << "] [" << slot_num_ << "]" << std::endl;
        for (const auto &stmt:stmts_)stmt->dump(os, "  ");
        os << "end " << ident_ << std::endl;
    }
};

class TiggerProgram {
private:
    std::vector<std::shared_ptr<TiggerDeclaration>> decls_;
    std::vector<std::shared_ptr<TiggerFunc>> funcs_;
public:
    void push_decl(const std::shared_ptr<TiggerDeclaration> &decl) {
        decls_.push_back(decl);
    }

    void push_func(const std::shared_ptr<TiggerFunc> &func) {
        funcs_.push_back(func);
    }

    void dump(std::ostream &os) const {
        for (const auto &decl:decls_)decl->dump(os);
        os << std::endl;
        for (const auto &func:funcs_)func->dump(os), os << std::endl;
    }
};

#endif //SYSYCOMPILER_TIGGER_H