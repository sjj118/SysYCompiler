%code requires {
#include <string>
#include <vector>
#include "ast.h"
}
%{
#include <cstdio>
#include <cstdlib>
extern int yydebug;

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
void yyerror(const char *s) {
    printf("Error(line: %d): %s\n", yyget_lineno(), s);
    yylex_destroy();
    if (!yydebug) std::exit(1);
}
#define YYERROR_VERBOSE true
#define YYDEBUG 1
%}

%union{
    int num;
    int token;
    std::string *ident;
    ExpressionAST *exp;
    std::vector<ExpressionAST *> *exp_list;
    LValAST *lval;
    StmtAST *stmt;
    std::vector<StmtAST *> *stmt_list;
    DefAST *def;
    std::vector<DefAST *> *def_list;
    VarDefAST *vardef;
    std::vector<VarDefAST *> *vardef_list;
    InitValAST *init;
    std::vector<InitValAST *> *init_list;
    CompUnitAST *comp_unit;
};
%token <num> INT_CONST
%token <ident> IDENT
%token <token> CONST INT VOID IF ELSE WHILE BREAK CONTINUE RETURN
%token <token> ADD SUB MUL DIV MOD EQ NEQ LESS GREAT LESSEQ GREATEQ LNOT LAND LOR
%type <exp> Exp Cond PrimaryExp Number UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp
%type <exp_list> FuncRParams ArrayBlock
%type <lval> LVal
%type <stmt> Stmt BlockItem Block
%type <stmt_list> BlockItems
%type <def> FuncDef ConstDef VarDef
%type <def_list> DeclDefs Decl ConstDecl ConstDefs VarDecl VarDefs
%type <vardef> FuncFParam
%type <vardef_list> FuncFParams
%type <init> InitVal
%type <init_list> InitVals
%type <comp_unit> CompUnit
%type <token> FuncType BType

%nonassoc IFX
%nonassoc ELSE

%start Start
%%
Start: CompUnit '}'   { $1->eval(nullptr); }

CompUnit: DeclDefs  { $$ = new CompUnitAST($1); }

DeclDefs: Decl              { $$ = $1; }
        | FuncDef           { $$ = new std::vector<DefAST *>(); $$->push_back($1); }
        | DeclDefs Decl     { $$ = $1; $$->insert($$->end(), $2->begin(), $2->end()); delete $2; }
        | DeclDefs FuncDef  { $$ = $1; $$->push_back($2); }
        ;
Decl: ConstDecl
    | VarDecl
    ;
ConstDecl: CONST BType ConstDefs ';'    { $$ = $3; }
         ;
BType: INT;
ConstDefs: ConstDef                 { $$ = new std::vector<DefAST *>; $$->push_back($1); }
         | ConstDefs ',' ConstDef   { $$ = $1; $$->push_back($3); }
         ;
ConstDef: IDENT ArrayBlock '=' InitVal { $$ = new VarDefAST(true, $1, $2, $4); }
        ;
VarDecl: BType VarDefs ';'  { $$ = $2; }
       ;
VarDefs: VarDef             { $$ = new std::vector<DefAST *>; $$->push_back($1); }
       | VarDefs ',' VarDef { $$ = $1; $$->push_back($3); }
       ;
VarDef: IDENT ArrayBlock               { $$ = new VarDefAST(false, $1, $2, nullptr); }
      | IDENT ArrayBlock '=' InitVal   { $$ = new VarDefAST(false, $1, $2, $4); }
      ;
InitVal: Exp                { $$ = new InitValAST($1); }
       | '{' '}'            { $$ = new InitValAST(new std::vector<InitValAST *>); }
       | '{' InitVals '}'   { $$ = new InitValAST($2); }
       ;
InitVals: InitVal               { $$ = new std::vector<InitValAST *>; $$->push_back($1); }
        | InitVals ',' InitVal  { $$ = $1; $$->push_back($3); }
        ;
ArrayBlock:                         { $$ = new std::vector<ExpressionAST *>; }
          | '[' Exp ']'             { $$ = new std::vector<ExpressionAST *>; $$->push_back($2); }
          | ArrayBlock '[' Exp ']'  { $$ = $1; $$->push_back($3); }
          ;

FuncDef: FuncType IDENT '(' FuncFParams ')' Block   { $$ = new FuncDefAST($1 == VOID, $2, $4, (BlockStmtAST *)$6); }
       | FuncType IDENT '(' ')' Block               { $$ = new FuncDefAST($1 == VOID, $2, new std::vector<VarDefAST *> , (BlockStmtAST *)$5); }
       ;
FuncType: VOID | INT;
FuncFParams: FuncFParam                 { $$ = new std::vector<VarDefAST *>; $$->push_back($1); }
           | FuncFParams ',' FuncFParam { $$ = $1; $$->push_back($3); }
           ;
FuncFParam: BType IDENT                     { $$ = new VarDefAST(false, $2, new std::vector<ExpressionAST *>, nullptr); }
          | BType IDENT '[' ']' ArrayBlock  { $5->insert($5->begin(), nullptr); $$ = new VarDefAST(false, $2, $5, nullptr); }
          ;

Block: '{' BlockItems '}'   { $$ = new BlockStmtAST($2); }
     ;
BlockItems:                         { $$ = new std::vector<StmtAST *>; }
          | BlockItem               { $$ = new std::vector<StmtAST *>; $$->push_back($1); }
          | BlockItems BlockItem    { $$ = $1; $$->push_back($2); }
          ;
BlockItem: Decl
         | Stmt
         ;
Stmt: LVal '=' Exp ';'               { $$ = new AssignStmtAST($1, $3); }
    | ';'                               { $$ = new ExpStmtAST(nullptr); }
    | Exp ';'                           { $$ = new ExpStmtAST($1); }
    | Block
    | IF '(' Cond ')' Stmt %prec IFX    { $$ = new IfStmtAST($3, $5, nullptr); }
    | IF '(' Cond ')' Stmt ELSE Stmt    { $$ = new IfStmtAST($3, $5, $7); }
    | WHILE '(' Cond ')' Stmt           { $$ = new WhileStmtAST($3, $5); }
    | BREAK ';'                         { $$ = new ControlStmtAST($1, nullptr); }
    | CONTINUE ';'                      { $$ = new ControlStmtAST($1, nullptr); }
    | RETURN ';'                        { $$ = new ControlStmtAST($1, nullptr); }
    | RETURN Exp ';'                    { $$ = new ControlStmtAST($1, $2); }
    ;
Exp: AddExp
   ;
Cond: LOrExp
    ;
LVal: IDENT ArrayBlock  { $$ = new LValAST($1, $2); }
    ;
PrimaryExp: '(' Exp ')' { $$ = $2; }
          | LVal
          | Number
          ;
Number: INT_CONST   { $$ = new NumberAST($1); };
UnaryExp: PrimaryExp
        | IDENT '(' ')'             { $$ = new FunCallAST($1, new std::vector<ExpressionAST *>); }
        | IDENT '(' FuncRParams ')' { $$ = new FunCallAST($1, $3); }
        | ADD UnaryExp              { $$ = new UnaryAST($1, $2); }
        | SUB UnaryExp              { $$ = new UnaryAST($1, $2); }
        | LNOT UnaryExp             { $$ = new UnaryAST($1, $2); }
        ;
FuncRParams: Exp                    { $$ = new std::vector<ExpressionAST *>; $$->push_back($1); }
           | FuncRParams ',' Exp    { $$ = $1; $$->push_back($3); }
           ;
MulExp: UnaryExp
      | MulExp MUL UnaryExp { $$ = new BinaryAST($1, $2, $3); }
      | MulExp DIV UnaryExp { $$ = new BinaryAST($1, $2, $3); }
      | MulExp MOD UnaryExp { $$ = new BinaryAST($1, $2, $3); }
      ;
AddExp: MulExp
      | AddExp ADD MulExp   { $$ = new BinaryAST($1, $2, $3); }
      | AddExp SUB MulExp   { $$ = new BinaryAST($1, $2, $3); }
      ;
RelExp: AddExp
      | RelExp LESS AddExp      { $$ = new BinaryAST($1, $2, $3); }
      | RelExp GREAT AddExp     { $$ = new BinaryAST($1, $2, $3); }
      | RelExp LESSEQ AddExp    { $$ = new BinaryAST($1, $2, $3); }
      | RelExp GREATEQ AddExp   { $$ = new BinaryAST($1, $2, $3); }
      ;
EqExp: RelExp
     | EqExp EQ RelExp  { $$ = new BinaryAST($1, $2, $3); }
     | EqExp NEQ RelExp { $$ = new BinaryAST($1, $2, $3); }
     ;
LAndExp: EqExp
       | LAndExp LAND EqExp { $$ = new BinaryAST($1, $2, $3); }
       ;
LOrExp: LAndExp
      | LOrExp LOR LAndExp  { $$ = new BinaryAST($1, $2, $3); }
      ;