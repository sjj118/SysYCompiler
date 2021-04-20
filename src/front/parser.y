%code requires {
#include <string>
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
    std::string *ident;
    AST *ast;
    int num;
    int op;
};
%token <num> INT_CONST
%token <ident> IDENT
%token CONST INT VOID IF ELSE WHILE BREAK CONTINUE RETURN
%token <op> ADD SUB MUL DIV MOD EQ NEQ LESS GREAT LESSEQ GREATEQ LNOT LAND LOR ASSIGN
%type <ast> Number Exp PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp ConstExp

%nonassoc IFX
%nonassoc ELSE

%start Stmt
%%
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

Block: '{' BlockItems '}'   {}
     ;
BlockItems:                         {}
          | BlockItem               {}
          | BlockItems BlockItem    {}
          ;
BlockItem: Decl {}
         | Stmt {}
         ;
Stmt: LVal ASSIGN Exp ';'               {}
    | ';'                               {}
    | Exp ';'                           { printf("%d\n", $1->eval(nullptr)); }
    | Block                             {}
    | IF '(' Cond ')' Stmt %prec IFX    {}
    | IF '(' Cond ')' Stmt ELSE Stmt    {}
    | WHILE '(' Cond ')' Stmt           {}
    | BREAK ';'                         {}
    | CONTINUE ';'                      {}
    | RETURN ';'                        {}
    | RETURN Exp ';'                    {}
    ;
Exp: AddExp
   ;
Cond: LOrExp    {}
    ;
LVal: IDENT ArrayBlock  {}
    ;
ArrayBlock:                         {}
          | '[' Exp ']'             {}
          | ArrayBlock '[' Exp ']'  {}
          ;
PrimaryExp: '(' Exp ')'
          | LVal        {}
          | Number
          ;
Number: INT_CONST   { $$ = new NumberAST($1); };
UnaryExp: PrimaryExp
        | IDENT '(' ')'             {}
        | IDENT '(' FuncRParams ')' {}
        | ADD UnaryExp              { $$ = new UnaryAST($1, $2); }
        | SUB UnaryExp              { $$ = new UnaryAST($1, $2); }
        | LNOT UnaryExp             { $$ = new UnaryAST($1, $2); }
        ;
FuncRParams: Exp                    {}
           | FuncRParams ',' Exp    {}
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
ConstExp: AddExp    {}
        ;