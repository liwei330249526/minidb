/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    BY = 259,                      /* BY  */
    CREATE = 260,                  /* CREATE  */
    DROP = 261,                    /* DROP  */
    GROUP = 262,                   /* GROUP  */
    TABLE = 263,                   /* TABLE  */
    TABLES = 264,                  /* TABLES  */
    INDEX = 265,                   /* INDEX  */
    CALC = 266,                    /* CALC  */
    SELECT = 267,                  /* SELECT  */
    ORDER = 268,                   /* ORDER  */
    ASC = 269,                     /* ASC  */
    DESC = 270,                    /* DESC  */
    SHOW = 271,                    /* SHOW  */
    SYNC = 272,                    /* SYNC  */
    INSERT = 273,                  /* INSERT  */
    DELETE = 274,                  /* DELETE  */
    UPDATE = 275,                  /* UPDATE  */
    LBRACE = 276,                  /* LBRACE  */
    RBRACE = 277,                  /* RBRACE  */
    LSQBRACE = 278,                /* LSQBRACE  */
    RSQBRACE = 279,                /* RSQBRACE  */
    COMMA = 280,                   /* COMMA  */
    TRX_BEGIN = 281,               /* TRX_BEGIN  */
    TRX_COMMIT = 282,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 283,            /* TRX_ROLLBACK  */
    INT_T = 284,                   /* INT_T  */
    STRING_T = 285,                /* STRING_T  */
    DATE_T = 286,                  /* DATE_T  */
    FLOAT_T = 287,                 /* FLOAT_T  */
    VECTOR_T = 288,                /* VECTOR_T  */
    HELP = 289,                    /* HELP  */
    EXIT = 290,                    /* EXIT  */
    DOT = 291,                     /* DOT  */
    INTO = 292,                    /* INTO  */
    VALUES = 293,                  /* VALUES  */
    FROM = 294,                    /* FROM  */
    WHERE = 295,                   /* WHERE  */
    AND = 296,                     /* AND  */
    SET = 297,                     /* SET  */
    ON = 298,                      /* ON  */
    INNER = 299,                   /* INNER  */
    JOIN = 300,                    /* JOIN  */
    SUM = 301,                     /* SUM  */
    COUNT = 302,                   /* COUNT  */
    AVG = 303,                     /* AVG  */
    MAX = 304,                     /* MAX  */
    MIN = 305,                     /* MIN  */
    LOAD = 306,                    /* LOAD  */
    DATA = 307,                    /* DATA  */
    INFILE = 308,                  /* INFILE  */
    EXPLAIN = 309,                 /* EXPLAIN  */
    STORAGE = 310,                 /* STORAGE  */
    FORMAT = 311,                  /* FORMAT  */
    EQ = 312,                      /* EQ  */
    LT = 313,                      /* LT  */
    GT = 314,                      /* GT  */
    LE = 315,                      /* LE  */
    GE = 316,                      /* GE  */
    NE = 317,                      /* NE  */
    LIKE = 318,                    /* LIKE  */
    NOT = 319,                     /* NOT  */
    PERCENT = 320,                 /* PERCENT  */
    UNDERSCORE = 321,              /* UNDERSCORE  */
    L2_DISTANCE = 322,             /* L2_DISTANCE  */
    COSINE_DISTANCE = 323,         /* COSINE_DISTANCE  */
    INNER_PRODUCT = 324,           /* INNER_PRODUCT  */
    IN = 325,                      /* IN  */
    EXISTS = 326,                  /* EXISTS  */
    NUMBER = 327,                  /* NUMBER  */
    FLOAT = 328,                   /* FLOAT  */
    ID = 329,                      /* ID  */
    SSS = 330,                     /* SSS  */
    UMINUS = 331                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 139 "yacc_sql.y"
   // %union 用于定义一个联合体（union），表示语法规则中符号的语义值（semantic value）可以存储的不同类型。 每个符号（终结符或非终结符）可以有一个语义值，%union 定义了这些语义值的可能类型。
  ParsedSqlNode *                            sql_node;
  ConditionSqlNode *                         condition;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  std::vector<std::unique_ptr<Expression>> * expression_list;
  JoinRelationNode*                          join_node;  // %union 定义了所有可能的语义值类型。
  std::vector<JoinRelationNode> *            join_node_list; // join 链表
  std::vector<Value> *                       value_list;
  std::vector<ConditionSqlNode> *            condition_list;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  std::vector<std::string> *                 relation_list;
  bool                                       bool_u;
  char *                                     string;
  int                                        number;
  float                                      floats;
  VectorFunctionExpr *                       vector_function_expr;  // 向量函数表达式
  std::vector<float> *                       vector_value;  // 新增：用于存储向量数据
  SubqueryExpr *                             subquery_expr;  // 新增：子查询表达式
  ValueListExpr *                            valuelist_expr;  // 新增
  std::vector<std::string>*                  string_list;
  std::vector<std::pair<RelAttrSqlNode, bool>> *     order_by_item_list_t;
  std::pair<RelAttrSqlNode, bool> *     order_by_item_t;

#line 169 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);

#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
