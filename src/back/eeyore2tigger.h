//
// Created by 史记 on 2021/5/11.
//

#ifndef SYSYCOMPILER_EEYORE2TIGGER_H
#define SYSYCOMPILER_EEYORE2TIGGER_H

#include <utility>
#include "nested_map.h"
#include "tigger.h"
#include "eeyore.h"
#include "parser.tab.hpp"

class E2TTransformer {
private:
    std::shared_ptr<TiggerProgram> root;
public:
    void generateOn(const EeyoreDeclaration *ast);

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
