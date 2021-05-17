#include "tigger.h"
#include "parser.tab.hpp"
#include "operator.h"

void TiggerDeclaration::dumpRISC(std::ostream &os) const {
    if (size_) {
        os.width(12);
        os << "  .comm" << var_->name() << ", " << size_ << ", " << 4 << std::endl << std::endl;
    } else {
        os.width(12);
        os << "  .global" << var_->name() << std::endl;
        os.width(12);
        os << "  .section" << ".sdata" << std::endl;
        os.width(12);
        os << "  .align" << 2 << std::endl;
        os.width(12);
        os << "  .type" << var_->name() << ", @object" << std::endl;
        os.width(12);
        os << "  .size" << var_->name() << ", " << 4 << std::endl;
        os << var_->name() << ":" << std::endl;
        os.width(12);
        os << "  .word" << init_ << std::endl << std::endl;
    }
}

void TiggerFunc::dumpRISC(std::ostream &os) const {
    os.width(12);
    os << "  .text" << std::endl;
    os.width(12);
    os << "  .align" << 2 << std::endl;
    os.width(12);
    os << "  .global" << ident_ << std::endl;
    os.width(12);
    os << "  .type" << ident_ << ", @function" << std::endl;
    os << ident_ << ":" << std::endl;
    int STK = (slot_num / 4 + 1) * 16;
    if (-STK < -2048 || -STK > 2047) {
        os.width(12);
        os << "  li" << "t0, " << -STK << std::endl;
        os.width(12);
        os << "  add" << "sp, sp, t0" << std::endl;
    } else {
        os.width(12);
        os << "  addi" << "sp, sp, " << -STK << std::endl;
    }
    if (-STK < -2048 || -STK > 2047) {
        os.width(12);
        os << "  li" << "t0, " << STK - 4 << std::endl;
        os.width(12);
        os << "  add" << "t0, t0, sp" << std::endl;
        os.width(12);
        os << "  sw" << "ra, " << "0(t0)" << std::endl;
    } else {
        os.width(12);
        os << "  sw" << "ra, " << STK - 4 << "(sp)" << std::endl;
    }
    for (const auto &stmt:stmts_)stmt->dumpRISC(os, this);
    os.width(12);
    os << "  .size" << ident_ << ", .-" << ident_ << std::endl << std::endl;
}


void TiggerBinaryStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    auto rhs_num = std::dynamic_pointer_cast<TiggerNum>(rhs_);
    auto rhs = std::dynamic_pointer_cast<TiggerReg>(rhs_);
    if (rhs_num) {
        if ((op_ == ADD || op_ == LESS) && rhs_num->num() >= -2048 && rhs_num->num() <= 2047) {
            os.width(12);
            if (op_ == ADD)os << "  addi";
            else os << "  slti";
            os << dst_->name() << ", " << lhs_->name() << ", " << rhs_num->num() << std::endl;
            return;
        } else {
            if (rhs_num->num() == 0)rhs = std::make_shared<TiggerReg>("x0");
            else {
                os.width(12);
                os << "  li" << "t0, " << rhs_num->num() << std::endl;
                rhs = std::make_shared<TiggerReg>("t0");
            }
        }
    }
    os.width(12);
    switch (op_) {
        case ADD:
            os << "  add";
            break;
        case SUB:
            os << "  sub";
            break;
        case MUL:
            os << "  mul";
            break;
        case DIV:
            os << "  div";
            break;
        case MOD:
            os << "  rem";
            break;
        case LESS:
            os << "  slt";
            break;
        case GREAT:
        case LESSEQ:
            os << "  sgt";
            break;
        case GREATEQ:
            os << "  slt";
            break;
        case LAND:
            os << "  snez";
            break;
        case LOR:
            os << "  or";
            break;
        case NEQ:
        case EQ:
            os << "  xor";
            break;
    }
    os << dst_->name() << ", " << lhs_->name() << ", " << rhs->name() << std::endl;
    switch (op_) {
        case LESSEQ:
        case GREATEQ:
            os.width(12);
            os << "  seqz" << dst_->name() << ", " << dst_->name() << std::endl;
            break;
        case LAND:
            os << "  snez" << "t0, " << rhs->name() << std::endl;
            os.width(12);
            os << "  and" << dst_->name() << ", " << dst_->name() << ", t0" << std::endl;
            break;
        case LOR:
        case NEQ:
            os.width(12);
            os << "  snez" << dst_->name() << ", " << dst_->name() << std::endl;
            break;
        case EQ:
            os.width(12);
            os << "  seqz" << dst_->name() << ", " << dst_->name() << std::endl;
            break;
    }
}

void TiggerUnaryStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    os.width(12);
    if (op_ == SUB)os << "  neg";
    if (op_ == LNOT)os << "  seqz";
    os << dst_->name() << ", " << rhs_->name() << std::endl;
}

void TiggerAssignStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    if (dst_offset_) {
        if (dst_offset_->num() < -2048 || dst_offset_->num() > 2047) {
            os.width(12);
            os << "  li" << "t0, " << dst_offset_->num() << std::endl;
            os.width(12);
            os << "  add" << "t0, t0, " << dst_->name() << std::endl;
            os.width(12);
            os << "  sw" << val_->name() << ", 0(t0)" << std::endl;
        } else {
            os.width(12);
            os << "  sw" << val_->name() << ", " << dst_offset_->num() << "(" << dst_->name() << ")" << std::endl;
        }
    } else if (val_offset_) {
        if (val_offset_->num() < -2048 || val_offset_->num() > 2047) {
            os.width(12);
            os << "  li" << "t0, " << val_offset_->num() << std::endl;
            os.width(12);
            os << "  add" << "t0, t0, " << val_->name() << std::endl;
            os.width(12);
            os << "  lw" << dst_->name() << ", 0(t0)" << std::endl;
        } else {
            os.width(12);
            os << "  lw" << dst_->name() << ", " << val_offset_->num() << "(" << val_->name() << ")" << std::endl;
        }
    } else {
        auto val_num = std::dynamic_pointer_cast<TiggerNum>(val_);
        os.width(12);
        if (val_num)os << "  li" << dst_->name() << ", " << val_num->num() << std::endl;
        else
            os << "  mv" << dst_->name() << ", " << std::dynamic_pointer_cast<TiggerReg>(val_)->name() << std::endl;
    }
}

void TiggerIfStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    os.width(12);
    switch (op_) {
        case LESS:
            os << "  blt";
            break;
        case GREAT:
            os << "  bgt";
            break;
        case LESSEQ:
            os << "  ble";
            break;
        case GREATEQ:
            os << "  bge";
            break;
        case NEQ:
            os << "  bne";
            break;
        case EQ:
            os << "  beq";
            break;
    }
    os << lhs_->name() << ", " << rhs_->name() << ", ." << label_->name() << std::endl;
}

void TiggerGotoStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    os.width(12);
    os << "  j" << "." << label_->name() << std::endl;
}

void TiggerLabelStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    os << "." + label_->name() << ":" << std::endl;
}

void TiggerFunCall::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    os.width(12);
    os << "  call" << ident_ << std::endl;
}

void TiggerReturnStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    int STK = (func->slot_num / 4 + 1) * 16;
    if (STK - 4 < -2048 || STK - 4 > 2047) {
        os.width(12);
        os << "  li" << "t0, " << STK - 4 << std::endl;
        os.width(12);
        os << "  add" << "t0, t0, sp" << std::endl;
        os.width(12);
        os << "  lw" << "ra, " << "0(t0)" << std::endl;
    } else {
        os.width(12);
        os << "  lw" << "ra, " << STK - 4 << "(sp)" << std::endl;
    }
    if (STK < -2048 || STK > 2047) {
        os.width(12);
        os << "  li" << "t0, " << STK << std::endl;
        os.width(12);
        os << "  add" << "sp, sp, t0" << std::endl;
    } else {
        os.width(12);
        os << "  addi" << "sp, sp, " << STK << std::endl;
    }
    os << "  ret" << std::endl;
}

void TiggerStoreStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    if (offset_->num() < -512 || offset_->num() > 511) {
        os.width(12);
        os << "  li" << "t0, " << offset_->num() * 4 << std::endl;
        os.width(12);
        os << "  add" << "t0, t0, sp" << std::endl;
        os.width(12);
        os << "  sw" << reg_->name() << ", 0(t0)" << std::endl;
    } else {
        os.width(12);
        os << "  sw" << reg_->name() << ", " << offset_->num() * 4 << "(sp)" << std::endl;
    }
}

void TiggerLoadStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    auto var = std::dynamic_pointer_cast<TiggerVariable>(from_);
    if (var) {
        os.width(12);
        os << "  lui" << reg_->name() << ", %hi(" << var->name() << ")" << std::endl;
        os.width(12);
        os << "  lw" << reg_->name() << ", %lo(" << var->name() << ")(" << reg_->name() << ")" << std::endl;
        return;
    }
    auto offset = std::dynamic_pointer_cast<TiggerNum>(from_)->num();
    if (offset < -512 || offset > 511) {
        os.width(12);
        os << "  li" << "t0, " << offset * 4 << std::endl;
        os.width(12);
        os << "  add" << "t0, t0, sp" << std::endl;
        os.width(12);
        os << "  lw" << reg_->name() << ", 0(t0)" << std::endl;
    } else {
        os.width(12);
        os << "  lw" << reg_->name() << ", " << offset * 4 << "(sp)" << std::endl;
    }
}

void TiggerLoadaddrStmt::dumpRISC(std::ostream &os, const TiggerFunc *func) const {
    auto var = std::dynamic_pointer_cast<TiggerVariable>(from_);
    if (var) {
        os.width(12);
        os << "  la " << reg_->name() << ", " << var->name() << std::endl;
        return;
    }
    auto offset = std::dynamic_pointer_cast<TiggerNum>(from_)->num();
    if (offset < -512 || offset > 511) {
        os.width(12);
        os << "  li" << "t0, " << offset * 4 << std::endl;
        os.width(12);
        os << "  add" << reg_->name() << ", sp, t0" << std::endl;
    } else {
        os.width(12);
        os << "  addi" << reg_->name() << ", sp, " << offset * 4 << std::endl;
    }
}

void TiggerProgram::dumpRISC(std::ostream &os) const {
    os.setf(std::ios::left);
    for (const auto &decl:decls_)decl->dumpRISC(os);
    for (const auto &func:funcs_)func->dumpRISC(os);
}