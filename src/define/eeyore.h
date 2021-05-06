//
// Created by 史记 on 2021/4/30.
//

#ifndef SYSYCOMPILER_EEYORE_H
#define SYSYCOMPILER_EEYORE_H

#include <ostream>
#include <utility>

extern std::string op2str(int op);

class EeyoreValue {
private:
public:
    virtual ~EeyoreValue() = default;

    virtual std::string name() = 0;
};

class EeyoreNumber : public EeyoreValue {
private:
    int val_;
public:
    explicit EeyoreNumber(int val) : val_(val) {}

    [[nodiscard]] int val() const { return val_; }

    std::string name() override { return std::to_string(val_); }
};

class EeyoreSymbol : public EeyoreValue {
private:
    int id_;
public:
    explicit EeyoreSymbol(int id) : id_(id) {}

    [[nodiscard]] virtual char prefix() const = 0;

    [[nodiscard]] int id() const { return id_; }

    std::string name() override { return prefix() + std::to_string(id_); }
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
    EeyoreSymbol *symbol_;
    int size_;
public:
    explicit EeyoreDeclaration(EeyoreSymbol *symbol, int size = 0) : symbol_(symbol), size_(size) {}

    [[nodiscard]] const EeyoreSymbol *symbol() const { return symbol_; }

    [[nodiscard]] int size() const { return size_; }

    void dump(std::ostream &os, const char prefix[] = "") {
        os << prefix << "var ";
        if (size_)os << size_ << " ";
        os << symbol_->name() << std::endl;
    }
};

class EeyoreStatement {
private:
public:
    virtual ~EeyoreStatement() = default;

    virtual void dump(std::ostream &os, const char prefix[]) = 0;
};

class EeyoreBinaryStmt : public EeyoreStatement {
private:
    int op_;
    EeyoreSymbol *dst_;
    EeyoreValue *lhs_, *rhs_;
public:
    EeyoreBinaryStmt(int op, EeyoreSymbol *dst, EeyoreValue *lhs, EeyoreValue *rhs) :
            op_(op), dst_(dst), lhs_(lhs), rhs_(rhs) {}

    ~EeyoreBinaryStmt() override {
        delete dst_;
        delete lhs_;
        delete rhs_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << dst_->name() << " = " << lhs_->name() << " " << op2str(op_) << " " << rhs_->name() << std::endl;
    }
};

class EeyoreUnaryStmt : public EeyoreStatement {
private:
    int op_;
    EeyoreSymbol *dst_;
    EeyoreValue *rhs_;
public:
    EeyoreUnaryStmt(int op, EeyoreSymbol *dst, EeyoreValue *rhs) : op_(op), dst_(dst), rhs_(rhs) {}

    ~EeyoreUnaryStmt() override {
        delete dst_;
        delete rhs_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << dst_->name() << " = " << op2str(op_) << rhs_->name() << std::endl;
    }
};

class EeyoreAssignStmt : public EeyoreStatement {
private:
    EeyoreSymbol *dst_;
    EeyoreValue *val_;
    EeyoreValue *dst_offset_, *val_offset_;
public:
    EeyoreAssignStmt(EeyoreSymbol *dst, EeyoreValue *val,
                     EeyoreValue *dst_offset = nullptr, EeyoreValue *val_offset = nullptr) :
            dst_(dst), val_(val), dst_offset_(dst_offset), val_offset_(val_offset) {}

    ~EeyoreAssignStmt() override {
        delete dst_;
        delete val_;
        delete dst_offset_;
        delete val_offset_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << dst_->name();
        if (dst_offset_)os << "[" << dst_offset_->name() << "]";
        os << " = " << val_->name();
        if (val_offset_)os << "[" << val_offset_->name() << "]";
        os << std::endl;
    }
};

class EeyoreIfStmt : public EeyoreStatement {
private:
    int op_;
    EeyoreValue *lhs_, *rhs_;
    EeyoreLabel *label_;
public:
    EeyoreIfStmt(int op, EeyoreValue *lhs, EeyoreValue *rhs, EeyoreLabel *label) :
            op_(op), lhs_(lhs), rhs_(rhs), label_(label) {}

    ~EeyoreIfStmt() override {
        delete lhs_;
        delete rhs_;
        delete label_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << "if " << lhs_->name() << " " << op2str(op_) << " " << rhs_->name()
           << " goto " << label_->name() << std::endl;
    }
};

class EeyoreGotoStmt : public EeyoreStatement {
private:
    EeyoreLabel *label_;
public:
    explicit EeyoreGotoStmt(EeyoreLabel *label) : label_(label) {}

    ~EeyoreGotoStmt() override {
        delete label_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << "goto " << label_->name() << std::endl;
    }
};

class EeyoreLabelStmt : public EeyoreStatement {
private:
    EeyoreLabel *label_;
public:
    explicit EeyoreLabelStmt(EeyoreLabel *label) : label_(label) {}

    ~EeyoreLabelStmt() override {
        delete label_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << label_->name() << ":" << std::endl;
    }
};

class EeyoreParamStmt : public EeyoreStatement {
private:
    EeyoreValue *param_;
public:
    explicit EeyoreParamStmt(EeyoreValue *param) : param_(param) {}

    ~EeyoreParamStmt() override {
        delete param_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << "param " << param_->name() << std::endl;
    }
};

class EeyoreFunCall : public EeyoreStatement {
private:
    std::string ident_;
    EeyoreValue *dst_;
public:
    explicit EeyoreFunCall(std::string ident, EeyoreValue *dst = nullptr) : ident_(std::move(ident)), dst_(dst) {}

    ~EeyoreFunCall() override {
        delete dst_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix;
        if (dst_)os << dst_->name() << " = ";
        os << "call " << ident_ << std::endl;
    }
};

class EeyoreReturnStmt : public EeyoreStatement {
private:
    EeyoreValue *val_;
public:
    explicit EeyoreReturnStmt(EeyoreValue *val) : val_(val) {}

    ~EeyoreReturnStmt() override {
        delete val_;
    }

    void dump(std::ostream &os, const char prefix[]) override {
        os << prefix << "return";
        if (val_)os << " " << val_->name();
        os << std::endl;
    }
};

class EeyoreFunc {
private:
    std::string ident_;
    int arg_num_;
    std::vector<EeyoreDeclaration *> decls_;
    std::vector<EeyoreStatement *> stmts_;
public:
    EeyoreFunc(std::string ident, int arg_num) : ident_(std::move(ident)), arg_num_(arg_num) {}

    void push_var(EeyoreDeclaration *decl) {
        decls_.push_back(decl);
    }

    void push_stmt(EeyoreStatement *stmt) {
        stmts_.push_back(stmt);
    }

    void dump(std::ostream &os) {
        os << ident_ << " [" << arg_num_ << "]" << std::endl;
        for (auto *decl:decls_)decl->dump(os, "  ");
        os << std::endl;
        for (auto *stmt:stmts_)stmt->dump(os, "  ");
        os << "end " << ident_ << std::endl;
    }
};

class EeyoreProgram {
private:
    std::vector<EeyoreDeclaration *> decls_;
    std::vector<EeyoreAssignStmt *> inits_;
    std::vector<EeyoreFunc *> funcs_;
public:
    EeyoreProgram() = default;

    void push_var(EeyoreDeclaration *decl) {
        decls_.push_back(decl);
    }

    void push_init(EeyoreAssignStmt *init) {
        inits_.push_back(init);
    }

    void push_func(EeyoreFunc *func) {
        funcs_.push_back(func);
    }

    void dump(std::ostream &os) {
        for (auto *decl:decls_)decl->dump(os, "");
        os << std::endl;
        for (auto *init:inits_)init->dump(os, "");
        os << std::endl;
        for (auto *func:funcs_)func->dump(os), os << std::endl;
    }
};


#endif //SYSYCOMPILER_EEYORE_H
