/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_USERS_SJJ118_SYSYCOMPILER_CMAKE_BUILD_DEBUG_PARSER_HPP_INCLUDED
# define YY_YY_USERS_SJJ118_SYSYCOMPILER_CMAKE_BUILD_DEBUG_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "/Users/sjj118/SysYCompiler/src/front/parser.y"

#include <string>
#include <vector>
#include "sysy.h"

std::string op2str();

#line 56 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.hpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT_CONST = 258,
    IDENT = 259,
    CONST = 260,
    INT = 261,
    VOID = 262,
    IF = 263,
    ELSE = 264,
    WHILE = 265,
    BREAK = 266,
    CONTINUE = 267,
    RETURN = 268,
    ADD = 269,
    SUB = 270,
    MUL = 271,
    DIV = 272,
    MOD = 273,
    EQ = 274,
    NEQ = 275,
    LESS = 276,
    GREAT = 277,
    LESSEQ = 278,
    GREATEQ = 279,
    LNOT = 280,
    LAND = 281,
    LOR = 282,
    IFX = 283
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "/Users/sjj118/SysYCompiler/src/front/parser.y"

    int num;
    int token;
    std::string *ident;
    SysYExpression *exp;
    std::vector<SysYExpression *> *exp_list;
    SysYLVal *lval;
    SysYStatement *stmt;
    std::vector<SysYBlockItem *> *block_list;
    SysYDefine *def;
    std::vector<SysYDefine *> *def_list;
    SysYVarDef *vardef;
    std::vector<SysYVarDef *> *vardef_list;
    SysYInitVal *init;
    std::vector<SysYInitVal *> *init_list;
    SysYCompUnit *comp_unit;

#line 114 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_USERS_SJJ118_SYSYCOMPILER_CMAKE_BUILD_DEBUG_PARSER_HPP_INCLUDED  */
