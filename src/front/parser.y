%code requires {
#include <string>
#include <vector>
#include "sysy.h"

std::string op2str();
}
%{
#include <cstdio>
#include <cstdlib>
#include "sysy.h"

SysYCompUnit *root;
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
    SysYExpression *exp;
    std::vector<std::shared_ptr<SysYExpression>> *exp_list;
    SysYLVal *lval;
    SysYStatement *stmt;
    std::vector<std::shared_ptr<SysYBlockItem>> *block_list;
    SysYDefine *def;
    std::vector<std::shared_ptr<SysYDefine>> *def_list;
    SysYVarDef *vardef;
    std::vector<std::shared_ptr<SysYVarDef>> *vardef_list;
    SysYInitVal *init;
    std::vector<std::shared_ptr<SysYInitVal>> *init_list;
    SysYCompUnit *comp_unit;
};
%token <num> INT_CONST
%token <ident> IDENT
%token <token> CONST INT VOID IF ELSE WHILE BREAK CONTINUE RETURN
%token <token> ADD SUB MUL DIV MOD EQ NEQ LESS GREAT LESSEQ GREATEQ LNOT LAND LOR
%type <exp> Exp Cond PrimaryExp Number UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp
%type <exp_list> FuncRParams ArrayBlock
%type <lval> LVal
%type <stmt> Stmt Block
%type <block_list> BlockItems
%type <def> FuncDef
%type <def_list> DeclDefs
%type <vardef> FuncFParam ConstDef VarDef
%type <vardef_list> FuncFParams Decl ConstDecl ConstDefs VarDecl VarDefs
%type <init> InitVal
%type <init_list> InitVals
%type <comp_unit> CompUnit
%type <token> BType

%nonassoc IFX
%nonassoc ELSE

%start CompUnit
%%
CompUnit: DeclDefs  { root = $$ = new SysYCompUnit($1); }

DeclDefs: Decl              { $$ = new std::vector<std::shared_ptr<SysYDefine>>(); $$->insert($$->end(), $1->begin(), $1->end()); delete $1; }
        | FuncDef           { $$ = new std::vector<std::shared_ptr<SysYDefine>>(); $$->push_back(std::shared_ptr<SysYDefine>($1)); }
        | DeclDefs Decl     { $$ = $1; $$->insert($$->end(), $2->begin(), $2->end()); delete $2; }
        | DeclDefs FuncDef  { $$ = $1; $$->push_back(std::shared_ptr<SysYDefine>($2)); }
        ;
Decl: ConstDecl
    | VarDecl
    ;
ConstDecl: CONST BType ConstDefs ';'    { $$ = $3; }
         ;
BType: INT;
ConstDefs: ConstDef                 { $$ = new std::vector<std::shared_ptr<SysYVarDef>>; $$->push_back(std::shared_ptr<SysYVarDef>($1)); }
         | ConstDefs ',' ConstDef   { $$ = $1; $$->push_back(std::shared_ptr<SysYVarDef>($3)); }
         ;
ConstDef: IDENT ArrayBlock '=' InitVal { $$ = new SysYVarDef(true, $1, $2, $4); }
        ;
VarDecl: BType VarDefs ';'  { $$ = $2; }
       ;
VarDefs: VarDef             { $$ = new std::vector<std::shared_ptr<SysYVarDef>>; $$->push_back(std::shared_ptr<SysYVarDef>($1)); }
       | VarDefs ',' VarDef { $$ = $1; $$->push_back(std::shared_ptr<SysYVarDef>($3)); }
       ;
VarDef: IDENT ArrayBlock               { $$ = new SysYVarDef(false, $1, $2); }
      | IDENT ArrayBlock '=' InitVal   { $$ = new SysYVarDef(false, $1, $2, $4); }
      ;
InitVal: Exp                { $$ = new SysYInitVal($1); }
       | '{' '}'            { $$ = new SysYInitVal(new std::vector<std::shared_ptr<SysYInitVal>>); }
       | '{' InitVals '}'   { $$ = new SysYInitVal($2); }
       ;
InitVals: InitVal               { $$ = new std::vector<std::shared_ptr<SysYInitVal>>; $$->push_back(std::shared_ptr<SysYInitVal>($1)); }
        | InitVals ',' InitVal  { $$ = $1; $$->push_back(std::shared_ptr<SysYInitVal>($3)); }
        ;
ArrayBlock:                         { $$ = new std::vector<std::shared_ptr<SysYExpression>>; }
          | '[' Exp ']'             { $$ = new std::vector<std::shared_ptr<SysYExpression>>; $$->push_back(std::shared_ptr<SysYExpression>($2)); }
          | ArrayBlock '[' Exp ']'  { $$ = $1; $$->push_back(std::shared_ptr<SysYExpression>($3)); }
          ;

FuncDef: BType IDENT '(' FuncFParams ')' Block   { $$ = new SysYFuncDef(false, $2, $4, (SysYBlockStmt *)$6); }
       | BType IDENT '(' ')' Block               { $$ = new SysYFuncDef(false, $2, new std::vector<std::shared_ptr<SysYVarDef>> , (SysYBlockStmt *)$5); }
       | VOID IDENT '(' FuncFParams ')' Block    { $$ = new SysYFuncDef(true, $2, $4, (SysYBlockStmt *)$6); }
       | VOID IDENT '(' ')' Block                { $$ = new SysYFuncDef(true, $2, new std::vector<std::shared_ptr<SysYVarDef>> , (SysYBlockStmt *)$5); }
;
FuncFParams: FuncFParam                 { $$ = new std::vector<std::shared_ptr<SysYVarDef>>; $$->push_back(std::shared_ptr<SysYVarDef>($1)); }
           | FuncFParams ',' FuncFParam { $$ = $1; $$->push_back(std::shared_ptr<SysYVarDef>($3)); }
           ;
FuncFParam: BType IDENT                     { $$ = new SysYVarDef(false, $2, new std::vector<std::shared_ptr<SysYExpression>>); }
          | BType IDENT '[' ']' ArrayBlock  { $5->insert($5->begin(), std::make_shared<SysYNumber>(0)); $$ = new SysYVarDef(false, $2, $5); }
          ;

Block: '{' BlockItems '}'   { $$ = new SysYBlockStmt($2); }
     ;
BlockItems:                 { $$ = new std::vector<std::shared_ptr<SysYBlockItem>>; }
          | Decl            { $$ = new std::vector<std::shared_ptr<SysYBlockItem>>; $$->insert($$->end(), $1->begin(), $1->end()); delete $1; }
          | Stmt            { $$ = new std::vector<std::shared_ptr<SysYBlockItem>>; $$->push_back(std::shared_ptr<SysYBlockItem>($1)); }
          | BlockItems Decl { $$ = $1; $$->insert($$->end(), $2->begin(), $2->end()); delete $2; }
          | BlockItems Stmt { $$ = $1; $$->push_back(std::shared_ptr<SysYBlockItem>($2)); }
          ;
Stmt: LVal '=' Exp ';'                  { $$ = new SysYAssignStmt($1, $3); }
    | ';'                               { $$ = new SysYExpStmt(); }
    | Exp ';'                           { $$ = new SysYExpStmt($1); }
    | Block
    | IF '(' Cond ')' Stmt %prec IFX    { $$ = new SysYIfStmt($3, $5); }
    | IF '(' Cond ')' Stmt ELSE Stmt    { $$ = new SysYIfStmt($3, $5, $7); }
    | WHILE '(' Cond ')' Stmt           { $$ = new SysYWhileStmt($3, $5); }
    | BREAK ';'                         { $$ = new SysYControlStmt($1); }
    | CONTINUE ';'                      { $$ = new SysYControlStmt($1); }
    | RETURN ';'                        { $$ = new SysYControlStmt($1); }
    | RETURN Exp ';'                    { $$ = new SysYControlStmt($1, $2); }
    ;
Exp: AddExp
   ;
Cond: LOrExp
    ;
LVal: IDENT ArrayBlock  { $$ = new SysYLVal($1, $2); }
    ;
PrimaryExp: '(' Exp ')' { $$ = $2; }
          | LVal
          | Number
          ;
Number: INT_CONST   { $$ = new SysYNumber($1); };
UnaryExp: PrimaryExp
        | IDENT '(' ')'             {
                                        if(*$1 == "starttime"){
                                            auto param = new std::vector<std::shared_ptr<SysYExpression>>;
                                            param->push_back(std::make_shared<SysYNumber>(yyget_lineno()));
                                            $$ = new SysYFunCall(new std::string("_sysy_starttime"), param);
                                            delete $1;
                                        } else if(*$1 == "stoptime"){
                                            auto param = new std::vector<std::shared_ptr<SysYExpression>>;
                                            param->push_back(std::make_shared<SysYNumber>(yyget_lineno()));
                                            $$ = new SysYFunCall(new std::string("_sysy_stoptime"), param);
                                            delete $1;
                                        } else { $$ = new SysYFunCall($1, new std::vector<std::shared_ptr<SysYExpression>>); }
                                    }
        | IDENT '(' FuncRParams ')' { $$ = new SysYFunCall($1, $3); }
        | ADD UnaryExp              { $$ = new SysYUnary($1, $2); }
        | SUB UnaryExp              { $$ = new SysYUnary($1, $2); }
        | LNOT UnaryExp             { $$ = new SysYUnary($1, $2); }
        ;
FuncRParams: Exp                    { $$ = new std::vector<std::shared_ptr<SysYExpression>>; $$->push_back(std::shared_ptr<SysYExpression>($1)); }
           | FuncRParams ',' Exp    { $$ = $1; $$->push_back(std::shared_ptr<SysYExpression>($3)); }
           ;
MulExp: UnaryExp
      | MulExp MUL UnaryExp { $$ = new SysYBinary($1, $2, $3); }
      | MulExp DIV UnaryExp { $$ = new SysYBinary($1, $2, $3); }
      | MulExp MOD UnaryExp { $$ = new SysYBinary($1, $2, $3); }
      ;
AddExp: MulExp
      | AddExp ADD MulExp   { $$ = new SysYBinary($1, $2, $3); }
      | AddExp SUB MulExp   { $$ = new SysYBinary($1, $2, $3); }
      ;
RelExp: AddExp
      | RelExp LESS AddExp      { $$ = new SysYBinary($1, $2, $3); }
      | RelExp GREAT AddExp     { $$ = new SysYBinary($1, $2, $3); }
      | RelExp LESSEQ AddExp    { $$ = new SysYBinary($1, $2, $3); }
      | RelExp GREATEQ AddExp   { $$ = new SysYBinary($1, $2, $3); }
      ;
EqExp: RelExp
     | EqExp EQ RelExp  { $$ = new SysYBinary($1, $2, $3); }
     | EqExp NEQ RelExp { $$ = new SysYBinary($1, $2, $3); }
     ;
LAndExp: EqExp
       | LAndExp LAND EqExp { $$ = new SysYBinary($1, $2, $3); }
       ;
LOrExp: LAndExp
      | LOrExp LOR LAndExp  { $$ = new SysYBinary($1, $2, $3); }
      ;