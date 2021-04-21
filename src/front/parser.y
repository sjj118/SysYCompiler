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
    int op;
    std::string *ident;
    ExpressionAST *exp;
    std::vector<ExpressionAST *> *exp_list;
    LValAST *lval;
    StmtAST *stmt;
    std::vector<StmtAST *> *stmt_list;
};
%token <num> INT_CONST
%token <ident> IDENT
%token CONST INT VOID IF ELSE WHILE
%token <op> BREAK CONTINUE RETURN
%token <op> ADD SUB MUL DIV MOD EQ NEQ LESS GREAT LESSEQ GREATEQ LNOT LAND LOR ASSIGN
%type <exp> Exp Cond PrimaryExp Number UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp ConstExp
%type <exp_list> FuncRParams ArrayBlock ConstArrayBlock
%type <lval> LVal
%type <stmt> Stmt BlockItem Block
%type <stmt_list> BlockItems

%nonassoc IFX
%nonassoc ELSE

%start Start
%%
Start: Block    { $1->eval(nullptr); }

Program: CompUnit           {}
       | Program CompUnit   {}
       ;
CompUnit: Decl      {}
        | FuncDef   {}
        ;
Decl: ConstDecl {}
    | VarDecl   {}
    ;
ConstDecl: CONST BType ConstDefBlock ';'    {}
         ;
BType: INT  {};
ConstDefBlock: ConstDef                     {}
             | ConstDefBlock ',' ConstDef   {}
             ;
ConstDef: IDENT ConstArrayBlock '=' ConstInitVal {}
        ;
ConstArrayBlock:                                {}
          | '[' ConstExp ']'                    {}
          | ConstArrayBlock '[' ConstExp ']'    {}
          ;
ConstInitVal: ConstExp                  {}
            | '{' '}'                   {}
            | '{' ConstInitVals '}' {}
            ;
ConstInitVals: ConstInitVal                     {}
             | ConstInitVals ',' ConstInitVal   {}
             ;
VarDecl: BType VarDefs ";"  {}
       ;
VarDefs: VarDef             {}
       | VarDefs ',' VarDef {}
       ;
VarDef: IDENT ConstArrayBlock               {}
      | IDENT ConstArrayBlock '=' InitVal   {}
      ;
InitVal: Exp                    {}
       | '{' '}'                {}
       | '{' InitVals '}'   {}
       ;
InitVals: InitVal               {}
        | InitVals ',' InitVal  {}
        ;

FuncDef: FuncType IDENT '(' [FuncFParams] ')' Block {}
       | FuncType IDENT '(' ')' Block               {}
       ;
FuncType: VOID | INT    {};
FuncFParams: FuncFParam                 {}
           | FuncFParams ',' FuncFParam {}
           ;
FuncFParam: BType IDENT                         {}
          | BType IDENT '[' ']' ConstArrayBlock {}
          ;

Block: '{' BlockItems '}'   { $$ = new BlockStmtAST(*$2); delete $2; }
     ;
BlockItems:                         { $$ = new std::vector<StmtAST *>; }
          | BlockItem               { $$ = new std::vector<StmtAST *>; $$->push_back($1); }
          | BlockItems BlockItem    { $$ = $1; $$->push_back($2); }
          ;
BlockItem: Decl
         | Stmt
         ;
Stmt: LVal ASSIGN Exp ';'               { $$ = new AssignStmtAST($1, $3); }
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
LVal: IDENT ArrayBlock  { $$ = new LValAST(*$1, *$2); delete $1; delete $2; }
    ;
ArrayBlock:                         { $$ = new std::vector<ExpressionAST *>; }
          | '[' Exp ']'             { $$ = new std::vector<ExpressionAST *>; $$->push_back($2); }
          | ArrayBlock '[' Exp ']'  { $$ = $1; $$->push_back($3); }
          ;
PrimaryExp: '(' Exp ')' { $$ = $2; }
          | LVal
          | Number
          ;
Number: INT_CONST   { $$ = new NumberAST($1); };
UnaryExp: PrimaryExp
        | IDENT '(' ')'             { $$ = new FunCallAST(*$1, std::vector<ExpressionAST *>()); }
        | IDENT '(' FuncRParams ')' { $$ = new FunCallAST(*$1, *$3); delete $1; delete $3;}
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
ConstExp: AddExp
        ;