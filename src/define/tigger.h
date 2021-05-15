//
// Created by 史记 on 2021/5/5.
//

#ifndef SYSYCOMPILER_TIGGER_H
#define SYSYCOMPILER_TIGGER_H

#include <vector>
#include <memory>
#include <ostream>
#include <utility>
#include <cstdlib>
#include "operator.h"

#define TIGGER_REGS(e) \
  e(x0)         \
  e(s0) e(s1) e(s2) e(s3) e(s4) e(s5) e(s6) e(s7) e(s8) e(s9) e(s10) e(s11) \
  e(t0) e(t1) e(t2) e(t3) e(t4) e(t5) e(t6)                                 \
  e(a0) e(a1) e(a2) e(a3) e(a4) e(a5) e(a6) e(a7)

class TiggerValue {
private:
public:
    [[nodiscard]] virtual std::string name() const = 0;
};

class TiggerAddr {
private:
public:
    [[nodiscard]] virtual std::string name() const = 0;
};

class TiggerNum : public TiggerValue, public TiggerAddr {
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

    explicit TiggerReg(std::string name) : prefix_(name[0]), id_(atoi(name.substr(1).c_str())) {}

    [[nodiscard]] char prefix() const { return prefix_; }

    [[nodiscard]] int id() const { return id_; }

    [[nodiscard]] std::string name() const override { return prefix() + std::to_string(id_); }
};

class TiggerVariable : public TiggerAddr {
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

    void set_init(int init) { init_ = init; }

    [[nodiscard]] const std::shared_ptr<TiggerVariable> &var() const { return var_; }

    [[nodiscard]] int size() const { return size_; }

    [[nodiscard]] int init() const { return init_; }

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

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &dst() const { return dst_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &lhs() const { return lhs_; }

    [[nodiscard]] const std::shared_ptr<TiggerValue> &rhs() const { return rhs_; }

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

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &dst() const { return dst_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &rhs() const { return rhs_; }

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

    [[nodiscard]] const std::shared_ptr<TiggerReg> &dst() const { return dst_; }

    [[nodiscard]] const std::shared_ptr<TiggerValue> &val() const { return val_; }

    [[nodiscard]] const std::shared_ptr<TiggerValue> &dst_offset() const { return dst_offset_; }

    [[nodiscard]] const std::shared_ptr<TiggerValue> &val_offset() const { return val_offset_; }

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

    [[nodiscard]] int op() const { return op_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &lhs() const { return lhs_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &rhs() const { return rhs_; }

    [[nodiscard]] const std::shared_ptr<TiggerLabel> &label() const { return label_; }

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

    [[nodiscard]] const std::shared_ptr<TiggerLabel> &label() const { return label_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "goto " << label_->name() << std::endl;
    }
};

class TiggerLabelStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerLabel> label_;
public:
    explicit TiggerLabelStmt(std::shared_ptr<TiggerLabel> label) : label_(std::move(label)) {}

    [[nodiscard]] const std::shared_ptr<TiggerLabel> &label() const { return label_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << label_->name() << ":" << std::endl;
    }
};

class TiggerFunCall : public TiggerStatement {
private:
    std::string ident_;
public:
    explicit TiggerFunCall(std::string ident) : ident_(std::move(ident)) {}

    [[nodiscard]] const std::string &ident() const { return ident_; }

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
    std::shared_ptr<TiggerNum> offset_;
public:
    TiggerStoreStmt(std::shared_ptr<TiggerReg> reg, std::shared_ptr<TiggerNum> offset) :
            reg_(std::move(reg)), offset_(std::move(offset)) {}

    TiggerStoreStmt(std::shared_ptr<TiggerReg> reg, int offset) :
            reg_(std::move(reg)), offset_(std::make_shared<TiggerNum>(offset)) {}

    [[nodiscard]] const std::shared_ptr<TiggerReg> &reg() const { return reg_; }

    [[nodiscard]] const std::shared_ptr<TiggerNum> &offset() const { return offset_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "store " << reg_->name() << " " << offset_->name() << std::endl;
    }
};

class TiggerLoadStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerAddr> from_;
    std::shared_ptr<TiggerReg> reg_;
public:
    TiggerLoadStmt(std::shared_ptr<TiggerAddr> from, std::shared_ptr<TiggerReg> reg) :
            from_(std::move(from)), reg_(std::move(reg)) {}

    [[nodiscard]] const std::shared_ptr<TiggerAddr> &from() const { return from_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &reg() const { return reg_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "load " << from_->name() << " " << reg_->name() << std::endl;
    }
};

class TiggerLoadaddrStmt : public TiggerStatement {
private:
    std::shared_ptr<TiggerAddr> from_;
    std::shared_ptr<TiggerReg> reg_;
public:
    TiggerLoadaddrStmt(std::shared_ptr<TiggerAddr> from, std::shared_ptr<TiggerReg> reg) :
            from_(std::move(from)), reg_(std::move(reg)) {}

    [[nodiscard]] const std::shared_ptr<TiggerAddr> &from() const { return from_; }

    [[nodiscard]] const std::shared_ptr<TiggerReg> &reg() const { return reg_; }

    void dump(std::ostream &os, const char prefix[]) const override {
        os << prefix << "loadaddr " << from_->name() << " " << reg_->name() << std::endl;
    }
};

class TiggerFunc {
private:
    std::string ident_;
    int arg_num_;
    std::vector<std::shared_ptr<TiggerStatement>> stmts_;
public:
    int slot_num{};

    TiggerFunc(std::string ident, int arg_num) :
            ident_(std::move(ident)), arg_num_(arg_num) {}

    [[nodiscard]] const std::string &ident() const { return ident_; }

    [[nodiscard]] int arg_num() const { return arg_num_; }

    [[nodiscard]] const std::vector<std::shared_ptr<TiggerStatement>> &stmts() const { return stmts_; }

    void push_stmt(const std::shared_ptr<TiggerStatement> &stmt) {
        stmts_.push_back(stmt);
    }

    void dump(std::ostream &os) const {
        os << ident_ << " [" << arg_num_ << "] [" << slot_num << "]" << std::endl;
        for (const auto &stmt:stmts_)stmt->dump(os, "  ");
        os << "end " << ident_ << std::endl;
    }
};

class TiggerProgram {
private:
    std::vector<std::shared_ptr<TiggerDeclaration>> decls_;
    std::vector<std::shared_ptr<TiggerFunc>> funcs_;
public:
    [[nodiscard]] const std::vector<std::shared_ptr<TiggerDeclaration>> &decls() const { return decls_; }

    [[nodiscard]] const std::vector<std::shared_ptr<TiggerFunc>> &funcs() const { return funcs_; }

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