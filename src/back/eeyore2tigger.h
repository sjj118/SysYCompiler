//
// Created by 史记 on 2021/5/11.
//

#ifndef SYSYCOMPILER_EEYORE2TIGGER_H
#define SYSYCOMPILER_EEYORE2TIGGER_H

#include <utility>
#include <memory>
#include "nested_map.h"
#include "tigger.h"
#include "eeyore.h"

class E2TSymbolEntry {
private:
    std::shared_ptr<TiggerVariable> var_{};
    int offset_{};
    bool is_array_{};
public:
    explicit E2TSymbolEntry(std::shared_ptr<TiggerVariable> var, bool is_array) :
            var_(std::move(var)), is_array_(is_array) {}

    explicit E2TSymbolEntry(int offset, bool is_array) : offset_(offset), is_array_(is_array) {}

    [[nodiscard]] const std::shared_ptr<TiggerVariable> &var() const { return var_; }

    [[nodiscard]] int offset() const { return offset_; }

    [[nodiscard]] bool is_array() const { return is_array_; }
};

class E2TTransformer {
private:
    int v_cnt{}, p_cnt{};
    NestedMap<std::string, E2TSymbolEntry> vars;
    std::vector<std::shared_ptr<TiggerStatement>> inits;
    std::shared_ptr<TiggerProgram> root;
    std::shared_ptr<TiggerFunc> func;
#define REG_POINTER(i)  std::shared_ptr<TiggerReg> i = std::make_shared<TiggerReg>(#i);

    struct {
        TIGGER_REGS(REG_POINTER)

        [[nodiscard]] std::shared_ptr<TiggerReg> a(int i) const {
            switch (i) {
                case 0:
                    return a0;
                case 1:
                    return a1;
                case 2:
                    return a2;
                case 3:
                    return a3;
                case 4:
                    return a4;
                case 5:
                    return a5;
                case 6:
                    return a6;
                case 7:
                    return a7;
                default:
                    return nullptr;
            }
        }
    } REG;

    template<class Tp, class ...Args>
    void new_stmt(const Args &...args) { func->push_stmt(std::make_shared<Tp>(args...)); }

    void load(const std::shared_ptr<EeyoreValue> &val, const std::shared_ptr<TiggerReg> &reg);

    void store(const std::shared_ptr<TiggerReg> &reg, const std::shared_ptr<EeyoreSymbol> &symbol);

public:
    E2TTransformer() {
        root = std::make_shared<TiggerProgram>();
    }

    void generateOn(const EeyoreBinaryStmt *ast);

    void generateOn(const EeyoreUnaryStmt *ast);

    void generateOn(const EeyoreAssignStmt *ast);

    void generateOn(const EeyoreIfStmt *ast);

    void generateOn(const EeyoreGotoStmt *ast);

    void generateOn(const EeyoreLabelStmt *ast);

    void generateOn(const EeyoreParamStmt *ast);

    void generateOn(const EeyoreFunCall *ast);

    void generateOn(const EeyoreReturnStmt *ast);

    void generateOn(const EeyoreFunc *ast);

    std::shared_ptr<TiggerProgram> generateOn(const EeyoreProgram *ast);
};

#endif //SYSYCOMPILER_EEYORE2TIGGER_H
