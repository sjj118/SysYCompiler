%{
#include <cstdio>
#include <string>
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
    int num;
};
%token <num> INT_CONST
%token <ident> IDENT
%token CONST INT VOID IF ELSE WHILE BREAK CONTINUE RETURN
%token ADD SUB MUL DIV MOD EQ NEQ LESS GREAT LESSEQ GREATEQ LNOT LAND LOR ASSIGN
%type <num> Number

%nonassoc IFX
%nonassoc ELSE

%start PrimaryExp
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
    | Exp ';'                           {}
    | Block                             {}
    | IF '(' Cond ')' Stmt %prec IFX    {}
    | IF '(' Cond ')' Stmt ELSE Stmt    {}
    | WHILE '(' Cond ')' Stmt           {}
    | BREAK ';'                         {}
    | CONTINUE ';'                      {}
    | RETURN ';'                        {}
    | RETURN Exp ';'                    {}
    ;
Exp: AddExp     {}
   ;
Cond: LOrExp    {}
    ;
LVal: IDENT ArrayBlock  {}
    ;
ArrayBlock:                         {}
          | '[' Exp ']'             {}
          | ArrayBlock '[' Exp ']'  {}
          ;
PrimaryExp: '(' Exp ')' {}
          | LVal        {}
          | Number      {}
          ;
Number: INT_CONST;
UnaryExp: PrimaryExp                {}
        | IDENT '(' ')'             {}
        | IDENT '(' FuncRParams ')' {}
        | UnaryOp UnaryExp          {}
        ;
UnaryOp: ADD    {}
       | SUB    {}
       | LNOT   {}
       ;
FuncRParams: Exp                    {}
           | FuncRParams ',' Exp    {}
           ;
MulExp: UnaryExp            {}
      | MulExp MUL UnaryExp {}
      | MulExp DIV UnaryExp {}
      | MulExp MOD UnaryExp {}
      ;
AddExp: MulExp              {}
      | AddExp ADD MulExp   {}
      | AddExp SUB MulExp   {}
      ;
RelExp: AddExp                  {}
      | RelExp LESS AddExp      {}
      | RelExp GREAT AddExp     {}
      | RelExp LESSEQ AddExp    {}
      | RelExp GREATEQ AddExp   {}
      ;
EqExp: RelExp           {}
     | EqExp EQ RelExp  {}
     | EqExp NEQ RelExp {}
     ;
LAndExp: EqExp              {}
       | LAndExp LAND EqExp {}
       ;
LOrExp: LAndExp             {}
      | LOrExp LOR LAndExp  {}
      ;
ConstExp: AddExp    {}
        ;