%{
#include <string>
#include <errno.h>
#include "ast.h"
#include "parser.hpp"
extern int yylex_destroy();
extern void yyerror(const char *s);

int text2num(char *text){
    char *end_pos;
    int num = std::strtol(text, &end_pos, 0);
    if (*end_pos){ yyerror("invalid number"); }
    if (errno == ERANGE){ yyerror("number out of range"); }
    return num;
}
%}
%option noyywrap
%option yylineno
%x COMMENT
%%
"/*"            BEGIN COMMENT;
<COMMENT>"*/"   BEGIN INITIAL;
<COMMENT>.|\n   ;
"//"[^\n]*      ;
([1-9][0-9]*)|(0[0-7]*)|(0(x|X)[0-9a-fA-F]+)   yylval.num = text2num(yytext); return INT_CONST;
"const"     return yylval.token = CONST;
"int"       return yylval.token = INT;
"void"      return yylval.token = VOID;
"if"        return yylval.token = IF;
"else"      return yylval.token = ELSE;
"while"     return yylval.token = WHILE;
"break"     return yylval.token = BREAK;
"continue"  return yylval.token = CONTINUE;
"return"    return yylval.token = RETURN;
[A-Za-z_][A-Za-z_0-9]*  yylval.ident = new IdentAST(new std::string(yytext)); return IDENT;
"+"     return yylval.token = ADD;
"-"     return yylval.token = SUB;
"*"     return yylval.token = MUL;
"/"     return yylval.token = DIV;
"%"     return yylval.token = MOD;
"=="    return yylval.token = EQ;
"!="    return yylval.token = NEQ;
"<"     return yylval.token = LESS;
">"     return yylval.token = GREAT;
"<="    return yylval.token = LESSEQ;
">="    return yylval.token = GREATEQ;
"!"     return yylval.token = LNOT;
"&&"    return yylval.token = LAND;
"||"    return yylval.token = LOR;
[\(\)\[\]\{\},;=]    return yytext[0];
[ \t\n]             ;
.                   yyerror("unknown token");
%%