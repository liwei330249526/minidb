/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 125 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
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

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_BY = 4,                         /* BY  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_DROP = 6,                       /* DROP  */
  YYSYMBOL_GROUP = 7,                      /* GROUP  */
  YYSYMBOL_TABLE = 8,                      /* TABLE  */
  YYSYMBOL_TABLES = 9,                     /* TABLES  */
  YYSYMBOL_INDEX = 10,                     /* INDEX  */
  YYSYMBOL_CALC = 11,                      /* CALC  */
  YYSYMBOL_SELECT = 12,                    /* SELECT  */
  YYSYMBOL_ORDER = 13,                     /* ORDER  */
  YYSYMBOL_ASC = 14,                       /* ASC  */
  YYSYMBOL_DESC = 15,                      /* DESC  */
  YYSYMBOL_SHOW = 16,                      /* SHOW  */
  YYSYMBOL_SYNC = 17,                      /* SYNC  */
  YYSYMBOL_INSERT = 18,                    /* INSERT  */
  YYSYMBOL_DELETE = 19,                    /* DELETE  */
  YYSYMBOL_UPDATE = 20,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 21,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 22,                    /* RBRACE  */
  YYSYMBOL_LSQBRACE = 23,                  /* LSQBRACE  */
  YYSYMBOL_RSQBRACE = 24,                  /* RSQBRACE  */
  YYSYMBOL_COMMA = 25,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 26,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 27,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 28,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 29,                     /* INT_T  */
  YYSYMBOL_STRING_T = 30,                  /* STRING_T  */
  YYSYMBOL_DATE_T = 31,                    /* DATE_T  */
  YYSYMBOL_FLOAT_T = 32,                   /* FLOAT_T  */
  YYSYMBOL_VECTOR_T = 33,                  /* VECTOR_T  */
  YYSYMBOL_HELP = 34,                      /* HELP  */
  YYSYMBOL_EXIT = 35,                      /* EXIT  */
  YYSYMBOL_DOT = 36,                       /* DOT  */
  YYSYMBOL_INTO = 37,                      /* INTO  */
  YYSYMBOL_VALUES = 38,                    /* VALUES  */
  YYSYMBOL_FROM = 39,                      /* FROM  */
  YYSYMBOL_WHERE = 40,                     /* WHERE  */
  YYSYMBOL_AND = 41,                       /* AND  */
  YYSYMBOL_SET = 42,                       /* SET  */
  YYSYMBOL_ON = 43,                        /* ON  */
  YYSYMBOL_INNER = 44,                     /* INNER  */
  YYSYMBOL_JOIN = 45,                      /* JOIN  */
  YYSYMBOL_SUM = 46,                       /* SUM  */
  YYSYMBOL_COUNT = 47,                     /* COUNT  */
  YYSYMBOL_AVG = 48,                       /* AVG  */
  YYSYMBOL_MAX = 49,                       /* MAX  */
  YYSYMBOL_MIN = 50,                       /* MIN  */
  YYSYMBOL_LOAD = 51,                      /* LOAD  */
  YYSYMBOL_DATA = 52,                      /* DATA  */
  YYSYMBOL_INFILE = 53,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 54,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 55,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 56,                    /* FORMAT  */
  YYSYMBOL_EQ = 57,                        /* EQ  */
  YYSYMBOL_LT = 58,                        /* LT  */
  YYSYMBOL_GT = 59,                        /* GT  */
  YYSYMBOL_LE = 60,                        /* LE  */
  YYSYMBOL_GE = 61,                        /* GE  */
  YYSYMBOL_NE = 62,                        /* NE  */
  YYSYMBOL_LIKE = 63,                      /* LIKE  */
  YYSYMBOL_NOT = 64,                       /* NOT  */
  YYSYMBOL_PERCENT = 65,                   /* PERCENT  */
  YYSYMBOL_UNDERSCORE = 66,                /* UNDERSCORE  */
  YYSYMBOL_L2_DISTANCE = 67,               /* L2_DISTANCE  */
  YYSYMBOL_COSINE_DISTANCE = 68,           /* COSINE_DISTANCE  */
  YYSYMBOL_INNER_PRODUCT = 69,             /* INNER_PRODUCT  */
  YYSYMBOL_IN = 70,                        /* IN  */
  YYSYMBOL_EXISTS = 71,                    /* EXISTS  */
  YYSYMBOL_NUMBER = 72,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 73,                     /* FLOAT  */
  YYSYMBOL_ID = 74,                        /* ID  */
  YYSYMBOL_SSS = 75,                       /* SSS  */
  YYSYMBOL_76_ = 76,                       /* '+'  */
  YYSYMBOL_77_ = 77,                       /* '-'  */
  YYSYMBOL_78_ = 78,                       /* '*'  */
  YYSYMBOL_79_ = 79,                       /* '/'  */
  YYSYMBOL_UMINUS = 80,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 81,                  /* $accept  */
  YYSYMBOL_commands = 82,                  /* commands  */
  YYSYMBOL_command_wrapper = 83,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 84,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 85,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 86,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 87,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 88,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 89,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 90,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 91,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 92,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 93,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 94,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 95,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 96,             /* attr_def_list  */
  YYSYMBOL_attr_def = 97,                  /* attr_def  */
  YYSYMBOL_number = 98,                    /* number  */
  YYSYMBOL_type = 99,                      /* type  */
  YYSYMBOL_insert_stmt = 100,              /* insert_stmt  */
  YYSYMBOL_value_list = 101,               /* value_list  */
  YYSYMBOL_value = 102,                    /* value  */
  YYSYMBOL_vector_literal = 103,           /* vector_literal  */
  YYSYMBOL_storage_format = 104,           /* storage_format  */
  YYSYMBOL_delete_stmt = 105,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 106,              /* update_stmt  */
  YYSYMBOL_select_stmt = 107,              /* select_stmt  */
  YYSYMBOL_valuelist_expr_y = 108,         /* valuelist_expr_y  */
  YYSYMBOL_subquery = 109,                 /* subquery  */
  YYSYMBOL_join_list = 110,                /* join_list  */
  YYSYMBOL_join_relation = 111,            /* join_relation  */
  YYSYMBOL_calc_stmt = 112,                /* calc_stmt  */
  YYSYMBOL_expression_list = 113,          /* expression_list  */
  YYSYMBOL_expression = 114,               /* expression  */
  YYSYMBOL_vector_function = 115,          /* vector_function  */
  YYSYMBOL_rel_attr = 116,                 /* rel_attr  */
  YYSYMBOL_id_list_y = 117,                /* id_list_y  */
  YYSYMBOL_relation = 118,                 /* relation  */
  YYSYMBOL_rel_list = 119,                 /* rel_list  */
  YYSYMBOL_where = 120,                    /* where  */
  YYSYMBOL_condition_list = 121,           /* condition_list  */
  YYSYMBOL_condition = 122,                /* condition  */
  YYSYMBOL_comp_op = 123,                  /* comp_op  */
  YYSYMBOL_group_by = 124,                 /* group_by  */
  YYSYMBOL_order_by_clause = 125,          /* order_by_clause  */
  YYSYMBOL_order_by_item_list = 126,       /* order_by_item_list  */
  YYSYMBOL_order_by_item = 127,            /* order_by_item  */
  YYSYMBOL_sort_direction = 128,           /* sort_direction  */
  YYSYMBOL_load_data_stmt = 129,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 130,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 131,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 132             /* opt_semicolon  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  75
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   327

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  128
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  249

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   331


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    78,    76,     2,    77,     2,    79,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    80
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   234,   234,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   265,   271,   276,   282,   288,   294,   300,
     307,   313,   321,   340,   350,   374,   377,   390,   398,   408,
     411,   412,   413,   414,   415,   432,   456,   459,   464,   475,
     479,   483,   488,   497,   502,   507,   516,   528,   531,   538,
     550,   589,   632,   641,   649,   652,   665,   679,   688,   693,
     704,   707,   710,   713,   716,   720,   723,   733,   739,   742,
     746,   750,   754,   758,   762,   774,   779,   784,   799,   804,
     814,   820,   833,   838,   843,   857,   860,   866,   869,   874,
     881,   890,   899,   908,   916,   928,   929,   930,   931,   932,
     933,   934,   935,   936,   937,   947,   954,   957,   964,   970,
     983,   996,   999,  1003,  1010,  1023,  1031,  1041,  1042
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "BY",
  "CREATE", "DROP", "GROUP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT",
  "ORDER", "ASC", "DESC", "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE",
  "LBRACE", "RBRACE", "LSQBRACE", "RSQBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "DATE_T", "FLOAT_T",
  "VECTOR_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE",
  "AND", "SET", "ON", "INNER", "JOIN", "SUM", "COUNT", "AVG", "MAX", "MIN",
  "LOAD", "DATA", "INFILE", "EXPLAIN", "STORAGE", "FORMAT", "EQ", "LT",
  "GT", "LE", "GE", "NE", "LIKE", "NOT", "PERCENT", "UNDERSCORE",
  "L2_DISTANCE", "COSINE_DISTANCE", "INNER_PRODUCT", "IN", "EXISTS",
  "NUMBER", "FLOAT", "ID", "SSS", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "$accept", "commands", "command_wrapper", "exit_stmt", "help_stmt",
  "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "drop_index_stmt", "create_table_stmt",
  "attr_def_list", "attr_def", "number", "type", "insert_stmt",
  "value_list", "value", "vector_literal", "storage_format", "delete_stmt",
  "update_stmt", "select_stmt", "valuelist_expr_y", "subquery",
  "join_list", "join_relation", "calc_stmt", "expression_list",
  "expression", "vector_function", "rel_attr", "id_list_y", "relation",
  "rel_list", "where", "condition_list", "condition", "comp_op",
  "group_by", "order_by_clause", "order_by_item_list", "order_by_item",
  "sort_direction", "load_data_stmt", "explain_stmt", "set_variable_stmt",
  "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,    43,    45,    42,    47,
     331
};
#endif

#define YYPACT_NINF (-231)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-48)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     248,     8,    11,    50,    50,   -68,    26,  -231,     0,    33,
     -35,  -231,  -231,  -231,  -231,  -231,     9,    49,   248,    88,
      99,  -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,
    -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,
    -231,    35,    38,    39,    40,    50,   -61,    86,    89,    94,
      95,   100,   105,   108,   110,  -231,  -231,    98,  -231,    50,
    -231,  -231,  -231,   154,  -231,  -231,    81,  -231,  -231,    61,
      66,   101,    92,    97,  -231,  -231,  -231,  -231,   120,   111,
    -231,   122,   -17,   148,   149,   127,    50,    50,    50,    50,
      50,    50,    50,    50,   118,  -231,    50,    50,    50,    50,
      50,   137,   162,   135,   138,   183,   139,   141,   143,   151,
    -231,   -61,   -61,  -231,    -2,     2,    60,    84,   126,   160,
     166,   172,  -231,  -231,   -56,   -56,  -231,  -231,  -231,   176,
     169,   195,   109,  -231,   165,  -231,   186,     1,   199,   213,
    -231,  -231,  -231,  -231,  -231,  -231,  -231,  -231,    50,    50,
      50,   137,   190,   135,   169,    50,    17,   257,   234,  -231,
     200,   183,   232,  -231,  -231,  -231,  -231,  -231,   225,   141,
     230,   173,   131,   142,   150,  -231,   137,  -231,  -231,   235,
     239,  -231,  -231,  -231,  -231,  -231,  -231,  -231,   -55,  -231,
     109,   121,   109,   135,   188,   197,   199,   215,   246,   250,
    -231,  -231,  -231,   236,   260,  -231,  -231,  -231,  -231,  -231,
     -51,    17,  -231,  -231,   -51,  -231,  -231,  -231,  -231,   255,
    -231,   222,  -231,   173,  -231,   109,   276,  -231,   259,   -15,
    -231,   227,  -231,  -231,   211,  -231,   183,   212,    36,  -231,
     262,  -231,   263,  -231,  -231,  -231,  -231,   211,  -231
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     127,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    88,    52,     0,
      78,    76,    67,    68,    84,    77,     0,    31,    30,     0,
       0,     0,     0,     0,   125,     1,   128,     2,     0,     0,
      29,     0,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
      74,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    69,    70,    71,    72,    73,    92,    93,
      64,     0,    97,    59,     0,   126,     0,     0,    35,     0,
      33,    56,    55,    79,    80,    81,    82,    83,     0,     0,
       0,     0,     0,    95,    64,     0,     0,     0,     0,    96,
      98,     0,     0,    40,    41,    42,    43,    44,    38,     0,
       0,     0,     0,     0,     0,    94,     0,   115,    65,     0,
       0,   105,   106,   107,   108,   109,   110,   111,     0,   113,
       0,     0,    97,    95,     0,     0,    35,    57,    90,     0,
      85,    86,    87,     0,   116,    45,    63,   112,   114,   102,
     101,    46,   103,   100,   104,    99,    60,   124,    39,     0,
      36,     0,    34,     0,    32,    97,     0,    61,     0,    76,
      37,     0,    91,    66,     0,    62,    46,     0,   121,   117,
     118,    48,    47,    58,   122,   123,   120,     0,   119
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -231,  -231,   271,  -231,  -231,  -231,  -231,  -231,  -231,  -231,
    -231,  -231,  -231,  -231,  -231,   104,   132,  -231,  -231,  -231,
      67,  -103,   -42,  -231,  -231,  -231,  -143,  -231,  -134,   152,
    -231,  -231,    -3,   -45,  -231,  -230,    82,   133,   156,  -144,
    -189,  -231,   164,  -231,  -231,    76,  -231,  -231,  -231,  -231,
    -231,  -231
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,   170,   138,   219,   168,    33,
     228,    61,    85,   222,    34,    35,    36,   212,   157,   153,
     154,    37,    62,    63,    64,    65,   199,   129,   130,   133,
     159,   160,   190,   204,   227,   239,   240,   246,    38,    39,
      40,    77
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,    66,   135,   215,   238,   110,    67,   -47,   207,   177,
     236,    83,    84,   180,    95,   208,    41,   238,    42,    43,
     143,    44,    99,   100,   144,    97,    98,    99,   100,     4,
     163,   164,   165,   166,   167,    68,   233,    69,    45,    71,
      46,   114,   115,   116,   117,   118,   119,   120,   121,   216,
     244,   245,   124,   125,   126,   127,   209,   213,   193,    97,
      98,    99,   100,    47,    48,    49,    50,    51,   180,   141,
     142,    45,    70,    46,    97,    98,    99,   100,    97,    98,
      99,   100,   145,    72,    52,    53,    54,   158,    75,    55,
      56,    57,    58,   123,    59,    60,    47,    48,    49,    50,
      51,    73,    76,   172,   173,   174,   146,    86,   229,    78,
      87,    82,    79,    80,    81,    88,    89,    52,    53,    54,
     101,    90,    55,    56,    57,    58,    91,    59,    60,    92,
     156,    93,    46,   242,    94,   102,    97,    98,    99,   100,
     103,   107,   211,   104,    46,   210,   214,   158,   147,   105,
     106,   113,   179,   200,   108,    47,    48,    49,    50,    51,
      97,    98,    99,   100,   201,   109,    82,    47,    48,    49,
      50,    51,   202,   111,   112,   132,    52,    53,    54,    96,
     158,    55,    56,    57,    58,   148,    59,    60,    52,    53,
      54,   149,   122,    55,    56,    57,    58,   150,    59,    60,
     131,   151,    97,    98,    99,   100,    46,    97,    98,    99,
     100,   128,   134,   152,   136,   137,   155,   139,    97,    98,
      99,   100,   161,   162,   169,   140,    97,    98,    99,   100,
      97,    98,    99,   100,   171,   176,    97,    98,    99,   100,
     194,   192,    97,    98,    99,   100,   195,   198,    97,    98,
      99,   100,   197,     1,     2,    55,    56,   205,    58,     3,
       4,   206,   217,     5,     6,     7,     8,     9,    10,   218,
     221,   223,   224,   226,    11,    12,    13,   230,   231,   225,
     234,   235,    14,    15,   237,    57,   243,   247,   236,    74,
      16,   181,   182,   183,   184,   185,   186,   187,   188,    17,
     220,   196,    18,   241,   189,   232,   178,   175,     0,   203,
      97,    98,    99,   100,   181,   182,   183,   184,   185,   186,
     187,   188,   191,   248,     0,     0,     0,   189
};

static const yytype_int16 yycheck[] =
{
      45,     4,   105,   192,   234,    22,    74,    22,    63,   153,
      25,    72,    73,   156,    59,    70,     8,   247,    10,     8,
      22,    10,    78,    79,    22,    76,    77,    78,    79,    12,
      29,    30,    31,    32,    33,     9,   225,    37,    21,    74,
      23,    86,    87,    88,    89,    90,    91,    92,    93,   193,
      14,    15,    97,    98,    99,   100,   190,   191,   161,    76,
      77,    78,    79,    46,    47,    48,    49,    50,   211,   111,
     112,    21,    39,    23,    76,    77,    78,    79,    76,    77,
      78,    79,    22,    74,    67,    68,    69,   132,     0,    72,
      73,    74,    75,    96,    77,    78,    46,    47,    48,    49,
      50,    52,     3,   148,   149,   150,    22,    21,   211,    74,
      21,   156,    74,    74,    74,    21,    21,    67,    68,    69,
      39,    21,    72,    73,    74,    75,    21,    77,    78,    21,
      21,    21,    23,   236,    36,    74,    76,    77,    78,    79,
      74,    21,    21,    42,    23,   190,   191,   192,    22,    57,
      53,    24,   155,    22,    43,    46,    47,    48,    49,    50,
      76,    77,    78,    79,    22,    43,   211,    46,    47,    48,
      49,    50,    22,    25,    25,    40,    67,    68,    69,    25,
     225,    72,    73,    74,    75,    25,    77,    78,    67,    68,
      69,    25,    74,    72,    73,    74,    75,    25,    77,    78,
      38,    25,    76,    77,    78,    79,    23,    76,    77,    78,
      79,    74,    74,    44,    75,    74,    21,    74,    76,    77,
      78,    79,    57,    37,    25,    74,    76,    77,    78,    79,
      76,    77,    78,    79,    21,    45,    76,    77,    78,    79,
       8,    41,    76,    77,    78,    79,    21,    74,    76,    77,
      78,    79,    22,     5,     6,    72,    73,    22,    75,    11,
      12,    22,    74,    15,    16,    17,    18,    19,    20,    72,
      55,    25,    22,    13,    26,    27,    28,    22,    56,    43,
       4,    22,    34,    35,    57,    74,    74,    25,    25,    18,
      42,    57,    58,    59,    60,    61,    62,    63,    64,    51,
     196,   169,    54,   236,    70,   223,   154,   151,    -1,   176,
      76,    77,    78,    79,    57,    58,    59,    60,    61,    62,
      63,    64,   158,   247,    -1,    -1,    -1,    70
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,    11,    12,    15,    16,    17,    18,    19,
      20,    26,    27,    28,    34,    35,    42,    51,    54,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,   100,   105,   106,   107,   112,   129,   130,
     131,     8,    10,     8,    10,    21,    23,    46,    47,    48,
      49,    50,    67,    68,    69,    72,    73,    74,    75,    77,
      78,   102,   113,   114,   115,   116,   113,    74,     9,    37,
      39,    74,    74,    52,    83,     0,     3,   132,    74,    74,
      74,    74,   114,    72,    73,   103,    21,    21,    21,    21,
      21,    21,    21,    21,    36,   114,    25,    76,    77,    78,
      79,    39,    74,    74,    42,    57,    53,    21,    43,    43,
      22,    25,    25,    24,   114,   114,   114,   114,   114,   114,
     114,   114,    74,   113,   114,   114,   114,   114,    74,   118,
     119,    38,    40,   120,    74,   102,    75,    74,    97,    74,
      74,   103,   103,    22,    22,    22,    22,    22,    25,    25,
      25,    25,    44,   110,   111,    21,    21,   109,   114,   121,
     122,    57,    37,    29,    30,    31,    32,    33,    99,    25,
      96,    21,   114,   114,   114,   119,    45,   120,   110,   113,
     107,    57,    58,    59,    60,    61,    62,    63,    64,    70,
     123,   123,    41,   102,     8,    21,    97,    22,    74,   117,
      22,    22,    22,   118,   124,    22,    22,    63,    70,   109,
     114,    21,   108,   109,   114,   121,   120,    74,    72,    98,
      96,    55,   104,    25,    22,    43,    13,   125,   101,   102,
      22,    56,   117,   121,     4,    22,    25,    57,   116,   126,
     127,   101,   102,    74,    14,    15,   128,    25,   126
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    81,    82,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    96,    97,    97,    98,
      99,    99,    99,    99,    99,   100,   101,   101,   101,   102,
     102,   102,   102,   103,   103,   103,   103,   104,   104,   105,
     106,   107,   108,   109,   110,   110,   111,   112,   113,   113,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   115,   115,   115,   116,   116,
     117,   117,   118,   119,   119,   120,   120,   121,   121,   121,
     122,   122,   122,   122,   122,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   124,   125,   125,   126,   126,
     127,   128,   128,   128,   129,   130,   131,   132,   132
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     8,     5,     8,     0,     3,     5,     2,     1,
       1,     1,     1,     1,     1,     7,     0,     1,     3,     1,
       1,     3,     1,     1,     1,     3,     3,     0,     4,     4,
       7,     8,     3,     3,     0,     2,     5,     2,     1,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     1,     4,
       4,     4,     4,     4,     1,     6,     6,     6,     1,     3,
       1,     3,     1,     1,     3,     0,     2,     0,     1,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     0,     0,     3,     1,     3,
       2,     0,     1,     1,     7,     2,     4,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

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
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (sql_string);
  YYUSE (sql_result);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (sql_string);
  YYUSE (sql_result);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 235 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1831 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 265 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1840 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 271 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1848 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 276 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1856 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 282 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1864 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 288 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1872 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 294 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1880 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 300 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1890 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 307 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1898 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 313 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1908 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE id_list_y RBRACE  */
#line 322 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-5].string);
      create_index.relation_name = (yyvsp[-3].string);
      if ((yyvsp[-1].string_list) != nullptr) {
        // 因为是从右往左解析的，所以需要反转
        create_index.attribute_name.swap(*(yyvsp[-1].string_list));
        std::reverse(create_index.attribute_name.begin(), create_index.attribute_name.end());
        delete (yyvsp[-1].string_list);
      }

      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
    }
#line 1928 "yacc_sql.cpp"
    break;

  case 33: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 341 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 1940 "yacc_sql.cpp"
    break;

  case 34: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 351 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      if ((yyvsp[0].string) != nullptr) {
        create_table.storage_format = (yyvsp[0].string);
        free((yyvsp[0].string));
      }
    }
#line 1965 "yacc_sql.cpp"
    break;

  case 35: /* attr_def_list: %empty  */
#line 374 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 1973 "yacc_sql.cpp"
    break;

  case 36: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 378 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 1987 "yacc_sql.cpp"
    break;

  case 37: /* attr_def: ID type LBRACE number RBRACE  */
#line 391 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number) * 4;  // float 字节是4， 有number个 float
      free((yyvsp[-4].string));
    }
#line 1999 "yacc_sql.cpp"
    break;

  case 38: /* attr_def: ID type  */
#line 399 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = 4;
      free((yyvsp[-1].string));
    }
#line 2011 "yacc_sql.cpp"
    break;

  case 39: /* number: NUMBER  */
#line 408 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2017 "yacc_sql.cpp"
    break;

  case 40: /* type: INT_T  */
#line 411 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2023 "yacc_sql.cpp"
    break;

  case 41: /* type: STRING_T  */
#line 412 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2029 "yacc_sql.cpp"
    break;

  case 42: /* type: DATE_T  */
#line 413 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2035 "yacc_sql.cpp"
    break;

  case 43: /* type: FLOAT_T  */
#line 414 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2041 "yacc_sql.cpp"
    break;

  case 44: /* type: VECTOR_T  */
#line 415 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2047 "yacc_sql.cpp"
    break;

  case 45: /* insert_stmt: INSERT INTO ID VALUES LBRACE expression_list RBRACE  */
#line 433 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-4].string);

//      if ($7 != nullptr) {
//        $$->insertion.values.swap(*$7);
//        delete $7;
//      }
//      $$->insertion.values.emplace_back(*$6);
//      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
//      delete $6;

      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.sql_node)->insertion.expressions.swap(*(yyvsp[-1].expression_list));  // expression 表达式
        delete (yyvsp[-1].expression_list);
      }

      free((yyvsp[-4].string));
    }
#line 2071 "yacc_sql.cpp"
    break;

  case 46: /* value_list: %empty  */
#line 456 "yacc_sql.y"
  {
    (yyval.value_list) = nullptr;
  }
#line 2079 "yacc_sql.cpp"
    break;

  case 47: /* value_list: value  */
#line 459 "yacc_sql.y"
          {
    (yyval.value_list) = new std::vector<Value>;
    (yyval.value_list)->emplace_back(*(yyvsp[0].value));
    delete (yyvsp[0].value);
  }
#line 2089 "yacc_sql.cpp"
    break;

  case 48: /* value_list: value COMMA value_list  */
#line 464 "yacc_sql.y"
                            {
    if ((yyvsp[0].value_list) != nullptr) {
      (yyval.value_list) = (yyvsp[0].value_list);
    } else {
      (yyval.value_list) = new std::vector<Value>;
    }
    (yyval.value_list)->emplace_back(*(yyvsp[-2].value));
    delete (yyvsp[-2].value);
  }
#line 2103 "yacc_sql.cpp"
    break;

  case 49: /* value: NUMBER  */
#line 475 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2112 "yacc_sql.cpp"
    break;

  case 50: /* value: FLOAT  */
#line 479 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2121 "yacc_sql.cpp"
    break;

  case 51: /* value: LSQBRACE vector_literal RSQBRACE  */
#line 484 "yacc_sql.y"
    {
      (yyval.value) = new Value(*(yyvsp[-1].vector_value));  // 通过一个std::vector  [1.5,2.3,3.3]，Value构造函数接受一个std::vector 构造一个向量类型的 Value
      delete((yyvsp[-1].vector_value));
    }
#line 2130 "yacc_sql.cpp"
    break;

  case 52: /* value: SSS  */
#line 488 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2141 "yacc_sql.cpp"
    break;

  case 53: /* vector_literal: NUMBER  */
#line 497 "yacc_sql.y"
           {   // 向量 vector 支持整数，
      (yyval.vector_value) = new std::vector<float>;
      (yyval.vector_value)->push_back(static_cast<float>(yyvsp[0].number));
    }
#line 2150 "yacc_sql.cpp"
    break;

  case 54: /* vector_literal: FLOAT  */
#line 503 "yacc_sql.y"
    {
      (yyval.vector_value) = new std::vector<float>;
      (yyval.vector_value)->push_back((yyvsp[0].floats));
    }
#line 2159 "yacc_sql.cpp"
    break;

  case 55: /* vector_literal: FLOAT COMMA vector_literal  */
#line 508 "yacc_sql.y"
    {
      if ((yyvsp[0].vector_value) != nullptr) {
        (yyval.vector_value) = (yyvsp[0].vector_value);
      } else {
        (yyval.vector_value) = new std::vector<float>;
      }
      (yyval.vector_value)->insert((yyval.vector_value)->begin(), (yyvsp[-2].floats));
    }
#line 2172 "yacc_sql.cpp"
    break;

  case 56: /* vector_literal: NUMBER COMMA vector_literal  */
#line 517 "yacc_sql.y"
    {
      if ((yyvsp[0].vector_value) != nullptr) {
        (yyval.vector_value) = (yyvsp[0].vector_value);
      } else {
        (yyval.vector_value) = new std::vector<float>;
      }
      (yyval.vector_value)->insert((yyval.vector_value)->begin(), static_cast<float>(yyvsp[-2].number));
    }
#line 2185 "yacc_sql.cpp"
    break;

  case 57: /* storage_format: %empty  */
#line 528 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2193 "yacc_sql.cpp"
    break;

  case 58: /* storage_format: STORAGE FORMAT EQ ID  */
#line 532 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2201 "yacc_sql.cpp"
    break;

  case 59: /* delete_stmt: DELETE FROM ID where  */
#line 539 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2215 "yacc_sql.cpp"
    break;

  case 60: /* update_stmt: UPDATE ID SET ID EQ value where  */
#line 551 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-5].string);
      (yyval.sql_node)->update.attribute_name = (yyvsp[-3].string);
      (yyval.sql_node)->update.value = *(yyvsp[-1].value);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
    }
#line 2232 "yacc_sql.cpp"
    break;

  case 61: /* select_stmt: SELECT expression_list FROM rel_list join_list where group_by order_by_clause  */
#line 590 "yacc_sql.y"
    {
     // 支持 join
     (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
     if ((yyvsp[-6].expression_list) != nullptr) {
       (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-6].expression_list));  // expression 表达式
       delete (yyvsp[-6].expression_list);
     }

     // 添加主表
      if ((yyvsp[-4].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-4].relation_list));
        delete (yyvsp[-4].relation_list);
      }

     // 处理 JOIN 表
     if ((yyvsp[-3].join_node_list) != nullptr) {
       for (const auto &join_relation : *(yyvsp[-3].join_node_list)) {
         (yyval.sql_node)->selection.relations.push_back(join_relation.right_table);  // join 表
         (yyval.sql_node)->selection.join_relations.push_back(join_relation); // join 表达式
       }
       delete (yyvsp[-3].join_node_list);
     }

     // 处理 WHERE 条件
     if ((yyvsp[-2].condition_list) != nullptr) {
       (yyval.sql_node)->selection.conditions.swap(*(yyvsp[-2].condition_list));
       delete (yyvsp[-2].condition_list);
     }

     // 处理 GROUP BY
     if ((yyvsp[-1].expression_list) != nullptr) {
       (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-1].expression_list));
       delete (yyvsp[-1].expression_list);
     }

     if ((yyvsp[0].order_by_item_list_t) != nullptr) {
       (yyval.sql_node)->selection.order_by = std::move(*(yyvsp[0].order_by_item_list_t));
       delete (yyvsp[0].order_by_item_list_t);
     }
   }
#line 2277 "yacc_sql.cpp"
    break;

  case 62: /* valuelist_expr_y: LBRACE value_list RBRACE  */
#line 633 "yacc_sql.y"
     {
         (yyval.valuelist_expr) = new ValueListExpr();
         (yyval.valuelist_expr)->values_.swap(*(yyvsp[-1].value_list));
         std::reverse((yyval.valuelist_expr)->values_.begin(), (yyval.valuelist_expr)->values_.end());
     }
#line 2287 "yacc_sql.cpp"
    break;

  case 63: /* subquery: LBRACE select_stmt RBRACE  */
#line 642 "yacc_sql.y"
    {
      (yyval.subquery_expr) = new SubqueryExpr((yyvsp[-1].sql_node));
      (yyval.subquery_expr)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2296 "yacc_sql.cpp"
    break;

  case 64: /* join_list: %empty  */
#line 649 "yacc_sql.y"
    {
      (yyval.join_node_list) = nullptr;
    }
#line 2304 "yacc_sql.cpp"
    break;

  case 65: /* join_list: join_relation join_list  */
#line 653 "yacc_sql.y"
    {
      if ((yyvsp[0].join_node_list) != nullptr) {  // 2 不为空，则将2赋值；
        (yyval.join_node_list) = (yyvsp[0].join_node_list);
      } else {
        (yyval.join_node_list) = new std::vector<JoinRelationNode>(); // 2 为空，则创建空的Vector
      }
      (yyval.join_node_list)->emplace_back(*(yyvsp[-1].join_node));
      delete (yyvsp[-1].join_node);
    }
#line 2318 "yacc_sql.cpp"
    break;

  case 66: /* join_relation: INNER JOIN relation ON condition_list  */
#line 666 "yacc_sql.y"
    {
      (yyval.join_node) = new JoinRelationNode();
      (yyval.join_node)->left_table = "";  // 左表名由上层规则提供
      (yyval.join_node)->right_table = (yyvsp[-2].string); // join 的右表
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.join_node)->conditions.swap(*(yyvsp[0].condition_list));  // 条件列表
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-2].string));
    }
#line 2333 "yacc_sql.cpp"
    break;

  case 67: /* calc_stmt: CALC expression_list  */
#line 680 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2343 "yacc_sql.cpp"
    break;

  case 68: /* expression_list: expression  */
#line 689 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>; // 给Vector
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2352 "yacc_sql.cpp"
    break;

  case 69: /* expression_list: expression COMMA expression_list  */
#line 694 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-2].expression));
    }
#line 2365 "yacc_sql.cpp"
    break;

  case 70: /* expression: expression '+' expression  */
#line 704 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2373 "yacc_sql.cpp"
    break;

  case 71: /* expression: expression '-' expression  */
#line 707 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2381 "yacc_sql.cpp"
    break;

  case 72: /* expression: expression '*' expression  */
#line 710 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2389 "yacc_sql.cpp"
    break;

  case 73: /* expression: expression '/' expression  */
#line 713 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2397 "yacc_sql.cpp"
    break;

  case 74: /* expression: LBRACE expression RBRACE  */
#line 716 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc))); // @$ 是 Yacc/Bison 中的一个特殊变量，它是一个 YYLTYPE 类型的结构体，用于存储当前规则匹配部分的位置信息，包括起始行、起始列、结束行和结束列。&@$ 表示取这个结构体的地址，将其作为参数传递给 token_name 函数，以便函数能知道具体的位置范围
    }
#line 2406 "yacc_sql.cpp"
    break;

  case 75: /* expression: '-' expression  */
#line 720 "yacc_sql.y"
                                  {  // %prec UMINUS：%prec 是 Yacc/Bison 中的一个特殊指令，用于指定当前规则的优先级。UMINUS 是之前在文件中通过 %nonassoc UMINUS 或类似声明定义的一个优先级标记，通常表示一元负号运算符的优先级。使用 %prec UMINUS 可以确保这个一元负号表达式的优先级与其他运算符的优先级正确匹配，避免出现运算顺序错误。
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2414 "yacc_sql.cpp"
    break;

  case 76: /* expression: value  */
#line 723 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2424 "yacc_sql.cpp"
    break;

  case 77: /* expression: rel_attr  */
#line 733 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2435 "yacc_sql.cpp"
    break;

  case 78: /* expression: '*'  */
#line 739 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 2443 "yacc_sql.cpp"
    break;

  case 79: /* expression: SUM LBRACE expression RBRACE  */
#line 742 "yacc_sql.y"
                                   {
      // 支持 sum (col) 语法
      (yyval.expression) = create_aggregate_expression("sum", (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2452 "yacc_sql.cpp"
    break;

  case 80: /* expression: COUNT LBRACE expression RBRACE  */
#line 746 "yacc_sql.y"
                                     {
      // 支持 count (col)  语法
      (yyval.expression) = create_aggregate_expression("count", (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2461 "yacc_sql.cpp"
    break;

  case 81: /* expression: AVG LBRACE expression RBRACE  */
#line 750 "yacc_sql.y"
                                   {
      // 支持 avg (col) 语法
      (yyval.expression) = create_aggregate_expression("avg", (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2470 "yacc_sql.cpp"
    break;

  case 82: /* expression: MAX LBRACE expression RBRACE  */
#line 754 "yacc_sql.y"
                                   {
      // 支持 max (col) 语法
      (yyval.expression) = create_aggregate_expression("max", (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2479 "yacc_sql.cpp"
    break;

  case 83: /* expression: MIN LBRACE expression RBRACE  */
#line 758 "yacc_sql.y"
                                   {
      // 支持 min (col) 语法
      (yyval.expression) = create_aggregate_expression("min", (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2488 "yacc_sql.cpp"
    break;

  case 84: /* expression: vector_function  */
#line 762 "yacc_sql.y"
                      {  // 新增：支持向量函数
      (yyval.expression) = (yyvsp[0].vector_function_expr);
    }
#line 2496 "yacc_sql.cpp"
    break;

  case 85: /* vector_function: L2_DISTANCE LBRACE expression COMMA expression RBRACE  */
#line 775 "yacc_sql.y"
    {
      (yyval.vector_function_expr) = new VectorFunctionExpr(VectorFunctionExpr::VectorFunctionType::L2_DISTANCE, (yyvsp[-3].expression), (yyvsp[-1].expression));
      (yyval.vector_function_expr)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2505 "yacc_sql.cpp"
    break;

  case 86: /* vector_function: COSINE_DISTANCE LBRACE expression COMMA expression RBRACE  */
#line 780 "yacc_sql.y"
    {
      (yyval.vector_function_expr) = new VectorFunctionExpr(VectorFunctionExpr::VectorFunctionType::COSINE_DISTANCE, (yyvsp[-3].expression), (yyvsp[-1].expression));
      (yyval.vector_function_expr)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2514 "yacc_sql.cpp"
    break;

  case 87: /* vector_function: INNER_PRODUCT LBRACE expression COMMA expression RBRACE  */
#line 785 "yacc_sql.y"
    {
      (yyval.vector_function_expr) = new VectorFunctionExpr(VectorFunctionExpr::VectorFunctionType::INNER_PRODUCT, (yyvsp[-3].expression), (yyvsp[-1].expression));
      (yyval.vector_function_expr)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2523 "yacc_sql.cpp"
    break;

  case 88: /* rel_attr: ID  */
#line 799 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2533 "yacc_sql.cpp"
    break;

  case 89: /* rel_attr: ID DOT ID  */
#line 804 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2545 "yacc_sql.cpp"
    break;

  case 90: /* id_list_y: ID  */
#line 815 "yacc_sql.y"
    {
      (yyval.string_list) = new std::vector<std::string>;
      (yyval.string_list)->emplace_back((yyvsp[0].string));
      free((yyvsp[0].string));
    }
#line 2555 "yacc_sql.cpp"
    break;

  case 91: /* id_list_y: ID COMMA id_list_y  */
#line 821 "yacc_sql.y"
    {
      if ((yyvsp[0].string_list) != nullptr) {
        (yyval.string_list) = (yyvsp[0].string_list);
      } else {
        (yyval.string_list) = new std::vector<std::string>;
      }
      (yyval.string_list)->emplace_back((yyvsp[-2].string));
      free((yyvsp[-2].string));
    }
#line 2569 "yacc_sql.cpp"
    break;

  case 92: /* relation: ID  */
#line 833 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2577 "yacc_sql.cpp"
    break;

  case 93: /* rel_list: relation  */
#line 838 "yacc_sql.y"
             {
      (yyval.relation_list) = new std::vector<std::string>();
      (yyval.relation_list)->push_back((yyvsp[0].string));
      free((yyvsp[0].string));
    }
#line 2587 "yacc_sql.cpp"
    break;

  case 94: /* rel_list: relation COMMA rel_list  */
#line 843 "yacc_sql.y"
                              {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->insert((yyval.relation_list)->begin(), (yyvsp[-2].string));
      free((yyvsp[-2].string));
    }
#line 2602 "yacc_sql.cpp"
    break;

  case 95: /* where: %empty  */
#line 857 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2610 "yacc_sql.cpp"
    break;

  case 96: /* where: WHERE condition_list  */
#line 860 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2618 "yacc_sql.cpp"
    break;

  case 97: /* condition_list: %empty  */
#line 866 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2626 "yacc_sql.cpp"
    break;

  case 98: /* condition_list: condition  */
#line 869 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2636 "yacc_sql.cpp"
    break;

  case 99: /* condition_list: condition AND condition_list  */
#line 874 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2646 "yacc_sql.cpp"
    break;

  case 100: /* condition: expression comp_op subquery  */
#line 882 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 2;    // 左边是表达式
      (yyval.condition)->left_expression = (yyvsp[-2].expression);
      (yyval.condition)->right_is_attr = 2;   // 右边是表达式
      (yyval.condition)->right_expression = (yyvsp[0].subquery_expr);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2659 "yacc_sql.cpp"
    break;

  case 101: /* condition: subquery comp_op expression  */
#line 891 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 2;    // 左边是表达式
      (yyval.condition)->left_expression = (yyvsp[-2].subquery_expr);
      (yyval.condition)->right_is_attr = 2;   // 右边是表达式
      (yyval.condition)->right_expression = (yyvsp[0].expression);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2672 "yacc_sql.cpp"
    break;

  case 102: /* condition: subquery comp_op subquery  */
#line 900 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 2;    // 左边是表达式
      (yyval.condition)->left_expression = (yyvsp[-2].subquery_expr);
      (yyval.condition)->right_is_attr = 2;   // 右边是表达式
      (yyval.condition)->right_expression = (yyvsp[0].subquery_expr);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2685 "yacc_sql.cpp"
    break;

  case 103: /* condition: expression comp_op valuelist_expr_y  */
#line 908 "yacc_sql.y"
                                           {
       (yyval.condition) = new ConditionSqlNode;
       (yyval.condition)->left_is_attr = 2;    // 左边是表达式
       (yyval.condition)->left_expression = (yyvsp[-2].expression);
       (yyval.condition)->right_is_attr = 2;
       (yyval.condition)->right_expression = (yyvsp[0].valuelist_expr); // 右边是值列表
       (yyval.condition)->comp = (yyvsp[-1].comp);
     }
#line 2698 "yacc_sql.cpp"
    break;

  case 104: /* condition: expression comp_op expression  */
#line 917 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_is_attr = 2;    // 左边是表达式
      (yyval.condition)->left_expression = (yyvsp[-2].expression);
      (yyval.condition)->right_is_attr = 2;   // 右边是表达式
      (yyval.condition)->right_expression = (yyvsp[0].expression);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2711 "yacc_sql.cpp"
    break;

  case 105: /* comp_op: EQ  */
#line 928 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2717 "yacc_sql.cpp"
    break;

  case 106: /* comp_op: LT  */
#line 929 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2723 "yacc_sql.cpp"
    break;

  case 107: /* comp_op: GT  */
#line 930 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2729 "yacc_sql.cpp"
    break;

  case 108: /* comp_op: LE  */
#line 931 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2735 "yacc_sql.cpp"
    break;

  case 109: /* comp_op: GE  */
#line 932 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2741 "yacc_sql.cpp"
    break;

  case 110: /* comp_op: NE  */
#line 933 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2747 "yacc_sql.cpp"
    break;

  case 111: /* comp_op: LIKE  */
#line 934 "yacc_sql.y"
           { (yyval.comp) = LIKE_OP; }
#line 2753 "yacc_sql.cpp"
    break;

  case 112: /* comp_op: NOT LIKE  */
#line 935 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE_OP; }
#line 2759 "yacc_sql.cpp"
    break;

  case 113: /* comp_op: IN  */
#line 936 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 2765 "yacc_sql.cpp"
    break;

  case 114: /* comp_op: NOT IN  */
#line 937 "yacc_sql.y"
             { (yyval.comp) = NOT_IN_OP; }
#line 2771 "yacc_sql.cpp"
    break;

  case 115: /* group_by: %empty  */
#line 947 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2779 "yacc_sql.cpp"
    break;

  case 116: /* order_by_clause: %empty  */
#line 954 "yacc_sql.y"
    {
        (yyval.order_by_item_list_t) = nullptr;
    }
#line 2787 "yacc_sql.cpp"
    break;

  case 117: /* order_by_clause: ORDER BY order_by_item_list  */
#line 958 "yacc_sql.y"
    {
        std::reverse((yyvsp[0].order_by_item_list_t)->begin(), (yyvsp[0].order_by_item_list_t)->end());  // 顺序调整
        (yyval.order_by_item_list_t) = (yyvsp[0].order_by_item_list_t);
    }
#line 2796 "yacc_sql.cpp"
    break;

  case 118: /* order_by_item_list: order_by_item  */
#line 965 "yacc_sql.y"
    {
        (yyval.order_by_item_list_t) = new std::vector<std::pair<RelAttrSqlNode, bool>>;
        (yyval.order_by_item_list_t)->emplace_back(*(yyvsp[0].order_by_item_t));
        delete (yyvsp[0].order_by_item_t);
    }
#line 2806 "yacc_sql.cpp"
    break;

  case 119: /* order_by_item_list: order_by_item COMMA order_by_item_list  */
#line 971 "yacc_sql.y"
    {
        if ((yyvsp[0].order_by_item_list_t) != nullptr) {
            (yyval.order_by_item_list_t) = (yyvsp[0].order_by_item_list_t);
        } else {
            (yyval.order_by_item_list_t) = new std::vector<std::pair<RelAttrSqlNode, bool>>;
        }
        (yyval.order_by_item_list_t)->emplace_back(*(yyvsp[-2].order_by_item_t));
        delete (yyvsp[-2].order_by_item_t);
    }
#line 2820 "yacc_sql.cpp"
    break;

  case 120: /* order_by_item: rel_attr sort_direction  */
#line 984 "yacc_sql.y"
    {
        auto *attr = (yyvsp[-1].rel_attr);
        // std::string attr_name = attr->relation_name.empty()? attr->attribute_name : attr->relation_name + "." + attr->attribute_name;
        //std::string direction = $2;
        //bool asc = (direction == "ASC") || (direction == "asc") || (direction == "0"); // $2 为0 表示默认升序
        (yyval.order_by_item_t) = new std::pair<RelAttrSqlNode, bool>(*attr, (yyvsp[0].bool_u));
        delete (yyvsp[-1].rel_attr);
    }
#line 2833 "yacc_sql.cpp"
    break;

  case 121: /* sort_direction: %empty  */
#line 996 "yacc_sql.y"
    {
        (yyval.bool_u) = true;
    }
#line 2841 "yacc_sql.cpp"
    break;

  case 122: /* sort_direction: ASC  */
#line 1000 "yacc_sql.y"
    {
        (yyval.bool_u) = true;
    }
#line 2849 "yacc_sql.cpp"
    break;

  case 123: /* sort_direction: DESC  */
#line 1004 "yacc_sql.y"
    {
        (yyval.bool_u) = false;
    }
#line 2857 "yacc_sql.cpp"
    break;

  case 124: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1011 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 2871 "yacc_sql.cpp"
    break;

  case 125: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1024 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 2880 "yacc_sql.cpp"
    break;

  case 126: /* set_variable_stmt: SET ID EQ value  */
#line 1032 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 2892 "yacc_sql.cpp"
    break;


#line 2896 "yacc_sql.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1044 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
