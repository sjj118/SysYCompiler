//
// Created by 史记 on 2021/5/11.
//

#include "eeyore2tigger.h"
#include "eeyore.h"
#include "operator.h"

void E2TTransformer::generateOn(const EeyoreDeclaration *ast) {

}

void E2TTransformer::generateOn(const EeyoreBinaryStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreUnaryStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreAssignStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreIfStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreGotoStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreLabelStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreParamStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreFunCall *ast) {

}

void E2TTransformer::generateOn(const EeyoreReturnStmt *ast) {

}

void E2TTransformer::generateOn(const EeyoreFunc *ast) {

}

std::shared_ptr<TiggerProgram> E2TTransformer::generateOn(const EeyoreProgram *ast) {
    ast->decls();
    ast->inits();
    ast->funcs();
    return root;
}


void EeyoreDeclaration::genTigger(E2TTransformer *gen) const {}

void EeyoreBinaryStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreUnaryStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreAssignStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreIfStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreGotoStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreLabelStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreParamStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreFunCall::genTigger(E2TTransformer *gen) const {}

void EeyoreReturnStmt::genTigger(E2TTransformer *gen) const {}

void EeyoreFunc::genTigger(E2TTransformer *gen) const {}

std::shared_ptr<TiggerProgram> EeyoreProgram::genTigger(E2TTransformer *gen) const {}