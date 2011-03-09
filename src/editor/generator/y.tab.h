
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PARAM = 258,
     CONSTANT = 259,
     VARIABLE = 260,
     FUNCTION1 = 261,
     FUNCTION2 = 262,
     FUNCTION3 = 263,
     FUNCTION4 = 264,
     OPERATOR_ADD = 265,
     OPERATOR_SUB = 266,
     OPERATOR_MULT = 267,
     OPERATOR_DIV = 268,
     OPERATOR_LB = 269,
     OPERATOR_RB = 270,
     OPERATOR_CM = 271,
     OPERATOR_EQ = 272,
     UMINUS = 273
   };
#endif
/* Tokens.  */
#define PARAM 258
#define CONSTANT 259
#define VARIABLE 260
#define FUNCTION1 261
#define FUNCTION2 262
#define FUNCTION3 263
#define FUNCTION4 264
#define OPERATOR_ADD 265
#define OPERATOR_SUB 266
#define OPERATOR_MULT 267
#define OPERATOR_DIV 268
#define OPERATOR_LB 269
#define OPERATOR_RB 270
#define OPERATOR_CM 271
#define OPERATOR_EQ 272
#define UMINUS 273




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{


    PARAM_NODE_LEAF *nPtr;      /* node pointer */



} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


