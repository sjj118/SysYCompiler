%{
#include <string>
#include <errno.h>
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
"const"     return CONST;
"int"       return INT;
"void"      return VOID;
"if"        return IF;
"else"      return ELSE;
"while"     return WHILE;
"break"     return BREAK;
"continue"  return CONTINUE;
"return" return RETURN;
[A-Za-z_][A-Za-z_0-9]*  yylval.ident = new std::string(yytext); return IDENT;
"+"     return ADD;
"-"     return SUB;
"*"     return MUL;
"/"     return DIV;
"%"     return MOD;
"=="    return EQ;
"!="    return NEQ;
"<"     return LESS;
">"     return GREAT;
"<="    return LESSEQ;
">="    return GREATEQ;
"!"     return LNOT;
"&&"    return LAND;
"||"    return LOR;
"="     return ASSIGN;
[\(\)\[\]\{\},;]    return yytext[0];
[ \t\n]             ;
.                   yyerror("unknown token");
%%