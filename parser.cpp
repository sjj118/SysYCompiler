/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 8 "/Users/sjj118/SysYCompiler/src/front/parser.y"

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

#line 89 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 131 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"

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

#line 189 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_USERS_SJJ118_SYSYCOMPILER_CMAKE_BUILD_DEBUG_PARSER_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   215

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  38
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  161

#define YYUNDEFTOK  2
#define YYMAXUTOK   283

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      36,    37,     2,     2,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    29,
       2,    31,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    34,     2,    35,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,    33,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    67,    67,    69,    70,    71,    72,    74,    75,    77,
      79,    80,    81,    83,    85,    87,    88,    90,    91,    93,
      94,    95,    97,    98,   100,   101,   102,   105,   106,   107,
     108,   110,   111,   113,   114,   117,   119,   120,   121,   122,
     123,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   140,   142,   144,   146,   147,   148,   150,   151,
     152,   153,   154,   155,   156,   158,   159,   161,   162,   163,
     164,   166,   167,   168,   170,   171,   172,   173,   174,   176,
     177,   178,   180,   181,   183,   184
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_CONST", "IDENT", "CONST", "INT",
  "VOID", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "RETURN", "ADD",
  "SUB", "MUL", "DIV", "MOD", "EQ", "NEQ", "LESS", "GREAT", "LESSEQ",
  "GREATEQ", "LNOT", "LAND", "LOR", "IFX", "';'", "','", "'='", "'{'",
  "'}'", "'['", "']'", "'('", "')'", "$accept", "CompUnit", "DeclDefs",
  "Decl", "ConstDecl", "BType", "ConstDefs", "ConstDef", "VarDecl",
  "VarDefs", "VarDef", "InitVal", "InitVals", "ArrayBlock", "FuncDef",
  "FuncFParams", "FuncFParam", "Block", "BlockItems", "Stmt", "Exp",
  "Cond", "LVal", "PrimaryExp", "Number", "UnaryExp", "FuncRParams",
  "MulExp", "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    59,
      44,    61,   123,   125,    91,    93,    40,    41
};
# endif

#define YYPACT_NINF -100

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-100)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      78,    17,  -100,    54,    90,    78,  -100,  -100,    93,  -100,
    -100,    97,    82,  -100,  -100,  -100,    29,   -16,  -100,    85,
       5,  -100,    14,   118,    15,    -9,  -100,   116,    26,  -100,
      97,    91,   132,   -20,  -100,  -100,    75,   118,   118,   118,
     118,   110,  -100,  -100,  -100,  -100,   112,    59,    91,     1,
     155,   118,    85,  -100,   155,  -100,   136,  -100,   103,    17,
      91,    12,   119,  -100,  -100,  -100,   115,  -100,   118,   118,
     118,   118,   118,  -100,    91,    63,  -100,  -100,   125,  -100,
     126,   127,   135,   150,   179,  -100,  -100,   116,  -100,   102,
    -100,   152,   153,   151,  -100,  -100,  -100,  -100,    24,  -100,
    -100,  -100,  -100,   112,   112,  -100,  -100,  -100,    42,  -100,
     118,   118,  -100,  -100,  -100,   156,  -100,  -100,  -100,  -100,
     118,    85,   118,  -100,   155,  -100,   159,    59,    47,    61,
     164,   162,   160,  -100,   169,   119,  -100,  -100,   163,   118,
     118,   118,   118,   118,   118,   118,   118,   163,  -100,   191,
      59,    59,    59,    59,    47,    47,    61,   164,  -100,   163,
    -100
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    10,     0,     0,     2,     3,     7,     0,     8,
       4,     0,     0,     1,     5,     6,    24,     0,    15,    24,
       0,    11,     0,     0,     0,    17,    14,     0,     0,     9,
       0,     0,     0,     0,    31,    58,    24,     0,     0,     0,
       0,     0,    56,    59,    57,    67,    71,    52,     0,     0,
       0,     0,    24,    16,     0,    12,    36,    30,    33,     0,
       0,     0,    54,    62,    63,    64,     0,    25,     0,     0,
       0,     0,     0,    28,     0,     0,    18,    19,     0,    13,
       0,     0,     0,     0,     0,    42,    37,     0,    44,     0,
      38,     0,    56,     0,    32,    29,    60,    65,     0,    55,
      68,    69,    70,    72,    73,    27,    20,    22,     0,    26,
       0,     0,    48,    49,    50,     0,    35,    39,    40,    43,
       0,    24,     0,    61,     0,    21,     0,    74,    79,    82,
      84,    53,     0,    51,     0,    34,    66,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,    45,
      75,    76,    77,    78,    80,    81,    83,    85,    47,     0,
      46
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -100,  -100,  -100,     0,  -100,     3,  -100,   171,  -100,  -100,
     175,   -45,  -100,   -17,   198,   181,   147,   -24,  -100,   -83,
     -22,    96,   -56,  -100,  -100,    87,  -100,    22,   -99,   -57,
      64,    65,  -100
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,     6,     7,    32,    20,    21,     9,    17,
      18,    76,   108,    25,    10,    33,    34,    88,    89,    90,
      91,   126,    42,    43,    44,    45,    98,    46,    47,   128,
     129,   130,   131
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      92,    41,    28,     8,    11,    14,   118,    57,     8,    79,
      59,   127,   127,    26,    27,    35,    36,    60,    66,    62,
       2,     2,    50,     2,    73,    51,    37,    38,    77,    78,
     107,    59,    77,    92,    29,    30,    95,    39,    74,    97,
     150,   151,   152,   153,   127,   127,   127,   127,    40,    96,
     105,    31,    48,    77,   122,   149,    86,    54,    12,    87,
      51,   123,   115,    23,   158,    24,    35,    36,   139,   140,
     141,   142,   124,    71,    72,   125,   160,    37,    38,   137,
     143,   144,    92,     1,     2,     3,   154,   155,    39,   117,
      13,    92,    87,   103,   104,    75,   106,    16,   134,    40,
     136,    19,    77,    92,   135,    35,    36,     1,     2,    23,
      80,    61,    81,    82,    83,    84,    37,    38,    22,    23,
      52,    35,    36,    56,    63,    64,    65,    39,    68,    69,
      70,    85,    37,    38,    56,   116,    58,    93,    40,    35,
      36,     1,     2,    39,    80,    67,    81,    82,    83,    84,
      37,    38,    99,    51,    40,   100,   101,   102,    35,    36,
     109,    39,   110,   111,   112,    85,    35,    36,    56,    37,
      38,    80,    40,    81,    82,    83,    84,    37,    38,   113,
      39,   119,    35,    36,   120,   133,   121,    75,    39,   146,
     145,    40,    85,    37,    38,    56,   138,   147,   148,    40,
     159,    55,    53,    15,    39,    49,    94,   132,   114,   156,
       0,   157,     0,     0,     0,    40
};

static const yytype_int16 yycheck[] =
{
      56,    23,    19,     0,     1,     5,    89,    31,     5,    54,
      30,   110,   111,    29,    30,     3,     4,    37,    40,    36,
       6,     6,    31,     6,    48,    34,    14,    15,    50,    51,
      75,    30,    54,    89,    29,    30,    60,    25,    37,    61,
     139,   140,   141,   142,   143,   144,   145,   146,    36,    37,
      74,    37,    37,    75,    30,   138,    56,    31,     4,    56,
      34,    37,    84,    34,   147,    36,     3,     4,    21,    22,
      23,    24,    30,    14,    15,    33,   159,    14,    15,   124,
      19,    20,   138,     5,     6,     7,   143,   144,    25,    89,
       0,   147,    89,    71,    72,    32,    33,     4,   120,    36,
     122,     4,   124,   159,   121,     3,     4,     5,     6,    34,
       8,    36,    10,    11,    12,    13,    14,    15,    36,    34,
       4,     3,     4,    32,    37,    38,    39,    25,    16,    17,
      18,    29,    14,    15,    32,    33,     4,    34,    36,     3,
       4,     5,     6,    25,     8,    35,    10,    11,    12,    13,
      14,    15,    37,    34,    36,    68,    69,    70,     3,     4,
      35,    25,    36,    36,    29,    29,     3,     4,    32,    14,
      15,     8,    36,    10,    11,    12,    13,    14,    15,    29,
      25,    29,     3,     4,    31,    29,    35,    32,    25,    27,
      26,    36,    29,    14,    15,    32,    37,    37,    29,    36,
       9,    30,    27,     5,    25,    24,    59,   111,    29,   145,
      -1,   146,    -1,    -1,    -1,    36
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,     7,    39,    40,    41,    42,    43,    46,
      52,    43,     4,     0,    41,    52,     4,    47,    48,     4,
      44,    45,    36,    34,    36,    51,    29,    30,    51,    29,
      30,    37,    43,    53,    54,     3,     4,    14,    15,    25,
      36,    58,    60,    61,    62,    63,    65,    66,    37,    53,
      31,    34,     4,    48,    31,    45,    32,    55,     4,    30,
      37,    36,    51,    63,    63,    63,    58,    35,    16,    17,
      18,    14,    15,    55,    37,    32,    49,    58,    58,    49,
       8,    10,    11,    12,    13,    29,    41,    43,    55,    56,
      57,    58,    60,    34,    54,    55,    37,    58,    64,    37,
      63,    63,    63,    65,    65,    55,    33,    49,    50,    35,
      36,    36,    29,    29,    29,    58,    33,    41,    57,    29,
      31,    35,    30,    37,    30,    33,    59,    66,    67,    68,
      69,    70,    59,    29,    58,    51,    58,    49,    37,    21,
      22,    23,    24,    19,    20,    26,    27,    37,    29,    57,
      66,    66,    66,    66,    67,    67,    68,    69,    57,     9,
      57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    38,    39,    40,    40,    40,    40,    41,    41,    42,
      43,    44,    44,    45,    46,    47,    47,    48,    48,    49,
      49,    49,    50,    50,    51,    51,    51,    52,    52,    52,
      52,    53,    53,    54,    54,    55,    56,    56,    56,    56,
      56,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    58,    59,    60,    61,    61,    61,    62,    63,
      63,    63,    63,    63,    63,    64,    64,    65,    65,    65,
      65,    66,    66,    66,    67,    67,    67,    67,    67,    68,
      68,    68,    69,    69,    70,    70
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     4,
       1,     1,     3,     4,     3,     1,     3,     2,     4,     1,
       2,     3,     1,     3,     0,     3,     4,     6,     5,     6,
       5,     1,     3,     2,     5,     3,     0,     1,     1,     2,
       2,     4,     1,     2,     1,     5,     7,     5,     2,     2,
       2,     3,     1,     1,     2,     3,     1,     1,     1,     1,
       3,     4,     2,     2,     2,     1,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 67 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { root = (yyval.comp_unit) = new SysYCompUnit((yyvsp[0].def_list)); }
#line 1404 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 3:
#line 69 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def_list) = new std::vector<SysYDefine *>(); (yyval.def_list)->insert((yyval.def_list)->end(), (yyvsp[0].vardef_list)->begin(), (yyvsp[0].vardef_list)->end()); delete (yyvsp[0].vardef_list); }
#line 1410 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 4:
#line 70 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def_list) = new std::vector<SysYDefine *>(); (yyval.def_list)->push_back((yyvsp[0].def)); }
#line 1416 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 5:
#line 71 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def_list) = (yyvsp[-1].def_list); (yyval.def_list)->insert((yyval.def_list)->end(), (yyvsp[0].vardef_list)->begin(), (yyvsp[0].vardef_list)->end()); delete (yyvsp[0].vardef_list); }
#line 1422 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 6:
#line 72 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def_list) = (yyvsp[-1].def_list); (yyval.def_list)->push_back((yyvsp[0].def)); }
#line 1428 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 9:
#line 77 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = (yyvsp[-1].vardef_list); }
#line 1434 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 11:
#line 80 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = new std::vector<SysYVarDef *>; (yyval.vardef_list)->push_back((yyvsp[0].vardef)); }
#line 1440 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 12:
#line 81 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = (yyvsp[-2].vardef_list); (yyval.vardef_list)->push_back((yyvsp[0].vardef)); }
#line 1446 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 13:
#line 83 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef) = new SysYVarDef(true, (yyvsp[-3].ident), (yyvsp[-2].exp_list), (yyvsp[0].init)); }
#line 1452 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 14:
#line 85 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = (yyvsp[-1].vardef_list); }
#line 1458 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 15:
#line 87 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = new std::vector<SysYVarDef *>; (yyval.vardef_list)->push_back((yyvsp[0].vardef)); }
#line 1464 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 16:
#line 88 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = (yyvsp[-2].vardef_list); (yyval.vardef_list)->push_back((yyvsp[0].vardef)); }
#line 1470 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 17:
#line 90 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef) = new SysYVarDef(false, (yyvsp[-1].ident), (yyvsp[0].exp_list)); }
#line 1476 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 18:
#line 91 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef) = new SysYVarDef(false, (yyvsp[-3].ident), (yyvsp[-2].exp_list), (yyvsp[0].init)); }
#line 1482 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 19:
#line 93 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.init) = new SysYInitVal((yyvsp[0].exp)); }
#line 1488 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 20:
#line 94 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.init) = new SysYInitVal(new std::vector<SysYInitVal *>); }
#line 1494 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 21:
#line 95 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.init) = new SysYInitVal((yyvsp[-1].init_list)); }
#line 1500 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 22:
#line 97 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.init_list) = new std::vector<SysYInitVal *>; (yyval.init_list)->push_back((yyvsp[0].init)); }
#line 1506 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 23:
#line 98 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.init_list) = (yyvsp[-2].init_list); (yyval.init_list)->push_back((yyvsp[0].init)); }
#line 1512 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 24:
#line 100 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp_list) = new std::vector<SysYExpression *>; }
#line 1518 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 25:
#line 101 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp_list) = new std::vector<SysYExpression *>; (yyval.exp_list)->push_back((yyvsp[-1].exp)); }
#line 1524 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 26:
#line 102 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp_list) = (yyvsp[-3].exp_list); (yyval.exp_list)->push_back((yyvsp[-1].exp)); }
#line 1530 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 27:
#line 105 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def) = new SysYFuncDef(false, (yyvsp[-4].ident), (yyvsp[-2].vardef_list), (SysYBlockStmt *)(yyvsp[0].stmt)); }
#line 1536 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 28:
#line 106 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def) = new SysYFuncDef(false, (yyvsp[-3].ident), new std::vector<SysYVarDef *> , (SysYBlockStmt *)(yyvsp[0].stmt)); }
#line 1542 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 29:
#line 107 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def) = new SysYFuncDef(true, (yyvsp[-4].ident), (yyvsp[-2].vardef_list), (SysYBlockStmt *)(yyvsp[0].stmt)); }
#line 1548 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 30:
#line 108 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.def) = new SysYFuncDef(true, (yyvsp[-3].ident), new std::vector<SysYVarDef *> , (SysYBlockStmt *)(yyvsp[0].stmt)); }
#line 1554 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 31:
#line 110 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = new std::vector<SysYVarDef *>; (yyval.vardef_list)->push_back((yyvsp[0].vardef)); }
#line 1560 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 32:
#line 111 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef_list) = (yyvsp[-2].vardef_list); (yyval.vardef_list)->push_back((yyvsp[0].vardef)); }
#line 1566 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 33:
#line 113 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.vardef) = new SysYVarDef(false, (yyvsp[0].ident), new std::vector<SysYExpression *>); }
#line 1572 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 34:
#line 114 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyvsp[0].exp_list)->insert((yyvsp[0].exp_list)->begin(), new SysYNumber(0)); (yyval.vardef) = new SysYVarDef(false, (yyvsp[-3].ident), (yyvsp[0].exp_list)); }
#line 1578 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 35:
#line 117 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYBlockStmt((yyvsp[-1].block_list)); }
#line 1584 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 36:
#line 119 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.block_list) = new std::vector<SysYBlockItem *>; }
#line 1590 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 37:
#line 120 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.block_list) = new std::vector<SysYBlockItem *>; (yyval.block_list)->insert((yyval.block_list)->end(), (yyvsp[0].vardef_list)->begin(), (yyvsp[0].vardef_list)->end()); delete (yyvsp[0].vardef_list); }
#line 1596 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 38:
#line 121 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.block_list) = new std::vector<SysYBlockItem *>; (yyval.block_list)->push_back((yyvsp[0].stmt)); }
#line 1602 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 39:
#line 122 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.block_list) = (yyvsp[-1].block_list); (yyval.block_list)->insert((yyval.block_list)->end(), (yyvsp[0].vardef_list)->begin(), (yyvsp[0].vardef_list)->end()); delete (yyvsp[0].vardef_list); }
#line 1608 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 40:
#line 123 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.block_list) = (yyvsp[-1].block_list); (yyval.block_list)->push_back((yyvsp[0].stmt)); }
#line 1614 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 41:
#line 128 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYAssignStmt((yyvsp[-3].lval), (yyvsp[-1].exp)); }
#line 1620 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 42:
#line 129 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYExpStmt(); }
#line 1626 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 43:
#line 130 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYExpStmt((yyvsp[-1].exp)); }
#line 1632 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 45:
#line 132 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYIfStmt((yyvsp[-2].exp), (yyvsp[0].stmt)); }
#line 1638 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 46:
#line 133 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYIfStmt((yyvsp[-4].exp), (yyvsp[-2].stmt), (yyvsp[0].stmt)); }
#line 1644 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 47:
#line 134 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYWhileStmt((yyvsp[-2].exp), (yyvsp[0].stmt)); }
#line 1650 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 48:
#line 135 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYControlStmt((yyvsp[-1].token)); }
#line 1656 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 49:
#line 136 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYControlStmt((yyvsp[-1].token)); }
#line 1662 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 50:
#line 137 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYControlStmt((yyvsp[-1].token)); }
#line 1668 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 51:
#line 138 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.stmt) = new SysYControlStmt((yyvsp[-2].token), (yyvsp[-1].exp)); }
#line 1674 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 54:
#line 144 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.lval) = new SysYLVal((yyvsp[-1].ident), (yyvsp[0].exp_list)); }
#line 1680 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 55:
#line 146 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = (yyvsp[-1].exp); }
#line 1686 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 58:
#line 150 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYNumber((yyvsp[0].num)); }
#line 1692 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 60:
#line 152 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYFunCall((yyvsp[-2].ident), new std::vector<SysYExpression *>); }
#line 1698 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 61:
#line 153 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYFunCall((yyvsp[-3].ident), (yyvsp[-1].exp_list)); }
#line 1704 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 62:
#line 154 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYUnary((yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1710 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 63:
#line 155 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYUnary((yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1716 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 64:
#line 156 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYUnary((yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1722 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 65:
#line 158 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp_list) = new std::vector<SysYExpression *>; (yyval.exp_list)->push_back((yyvsp[0].exp)); }
#line 1728 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 66:
#line 159 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp_list) = (yyvsp[-2].exp_list); (yyval.exp_list)->push_back((yyvsp[0].exp)); }
#line 1734 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 68:
#line 162 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1740 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 69:
#line 163 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1746 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 70:
#line 164 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1752 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 72:
#line 167 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1758 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 73:
#line 168 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1764 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 75:
#line 171 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1770 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 76:
#line 172 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1776 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 77:
#line 173 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1782 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 78:
#line 174 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1788 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 80:
#line 177 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1794 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 81:
#line 178 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1800 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 83:
#line 181 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1806 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;

  case 85:
#line 184 "/Users/sjj118/SysYCompiler/src/front/parser.y"
    { (yyval.exp) = new SysYBinary((yyvsp[-2].exp), (yyvsp[-1].token), (yyvsp[0].exp)); }
#line 1812 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"
    break;


#line 1816 "/Users/sjj118/SysYCompiler/cmake-build-debug/parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
