%{
#define  YYERROR_VERBOSE 1

#define  GRAMMAR_DEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "params.h"

int yylex(void);
void yyerror(char *s);

%}

%union {
    PARAM_NODE_LEAF *nPtr;      /* node pointer */
};

%token <nPtr> PARAM
%token <nPtr> CONSTANT
%token <nPtr> VARIABLE
%token <nPtr> FUNCTION1
%token <nPtr> FUNCTION2
%token <nPtr> FUNCTION3
%token <nPtr> FUNCTION4

%token <nPtr> OPERATOR_ADD
%token <nPtr> OPERATOR_SUB

%token <nPtr> OPERATOR_MULT
%token <nPtr> OPERATOR_DIV

%token <nPtr> OPERATOR_LB
%token <nPtr> OPERATOR_RB
%token <nPtr> OPERATOR_CM

%token <nPtr> OPERATOR_EQ

%left OPERATOR_ADD  OPERATOR_SUB
%left OPERATOR_MULT OPERATOR_DIV
%nonassoc UMINUS

%type <nPtr> expr

%%

program:
        function                                { }
        ;

function:
          function expr                         { param_tree_set($2); }
        | /* NULL */
        ;

expr:
          CONSTANT                              { $$ = $1; }
        | PARAM                                 { $$ = $1; }
        | VARIABLE                              { $$ = $1; }
        
        | VARIABLE  OPERATOR_EQ expr            { $$ = param_node_leaf_set_2($2,$1,$3); }
        
        | FUNCTION1 OPERATOR_LB expr OPERATOR_RB { $$ = param_node_leaf_set_1($1,$3); }
        | FUNCTION2 OPERATOR_LB expr OPERATOR_CM expr OPERATOR_RB { $$ = param_node_leaf_set_2($1,$3,$5); }
        | FUNCTION3 OPERATOR_LB expr OPERATOR_CM expr OPERATOR_CM expr OPERATOR_RB { $$ = param_node_leaf_set_3($1,$3,$5,$7); }
        | FUNCTION4 OPERATOR_LB expr OPERATOR_CM expr OPERATOR_CM expr OPERATOR_CM expr OPERATOR_RB { $$ = param_node_leaf_set_4($1,$3,$5,$7,$9); }
        
        | OPERATOR_SUB expr %prec UMINUS        { $$ = param_node_leaf_set_1($1,$2); }
        | expr OPERATOR_ADD  expr               { $$ = param_node_leaf_set_2($2,$1,$3); }
        | expr OPERATOR_SUB  expr               { $$ = param_node_leaf_set_2($2,$1,$3); }
        | expr OPERATOR_MULT expr               { $$ = param_node_leaf_set_2($2,$1,$3); }
        | expr OPERATOR_DIV  expr               { $$ = param_node_leaf_set_2($2,$1,$3); }
        | OPERATOR_LB expr OPERATOR_RB          { $$ = $2; }
        ;
%%

yylex()
{
  PARAM_NODE *p_node = parser_node_next();
  if(!p_node)
    return(0);

  yylval.nPtr = param_node_leaf_create(p_node);

#ifdef GRAMMAR_DEBUG
  printf("----------------------\n");
  printf("Node type = %d\n", p_node->node_type);
  printf("Content type = %d\n", p_node->content_type);
  print_node(p_node, 3);
  printf("----------------------\n");
#endif

  switch(p_node->content_type) {
    case NODE_PARAM_PREV_0:
    case NODE_PARAM_PREV_1:
    case NODE_PARAM_PREV_2:
    case NODE_PARAM_PREV_3:
    case NODE_PARAM_PREV_4:
    case NODE_PARAM_PREV_5:
    case NODE_PARAM_PREV_6:
    case NODE_PARAM_PREV_7:
    case NODE_PARAM_PREV_8:
    case NODE_PARAM_PREV_9:
    case NODE_PARAM_RAND:
    case NODE_PARAM_RAND_HALF:
    case NODE_PARAM_GAUSS:
    case NODE_PARAM_GAUSS_HALF:
      return(PARAM);

    case NODE_CONSTANT_INT:
    case NODE_CONSTANT_FLOAT:
    case NODE_CONSTANT_RGBAF:
      return(CONSTANT);

    case NODE_VAR_VARIABLE:
    case NODE_VAR_VARIABLE_R:
    case NODE_VAR_VARIABLE_G:
    case NODE_VAR_VARIABLE_B:
    case NODE_VAR_VARIABLE_A:
    case NODE_VAR_VARIABLE_X:
    case NODE_VAR_VARIABLE_Y:
    case NODE_VAR_VARIABLE_Z:
    case NODE_VAR_VARIABLE_DX:
    case NODE_VAR_VARIABLE_DY:
      return(VARIABLE);

    case NODE_OP_ADD:
      return(OPERATOR_ADD);
    case NODE_OP_SUB:
      return(OPERATOR_SUB);
    case NODE_OP_MULT:
      return(OPERATOR_MULT);
    case NODE_OP_DIV:
      return(OPERATOR_DIV);

    case NODE_OP_LB:
      return(OPERATOR_LB);
    case NODE_OP_RB:
      return(OPERATOR_RB);

    case NODE_OP_EQ:
      return(OPERATOR_EQ);

    case NODE_FUNC_SIN:
    case NODE_FUNC_COS:
    case NODE_FUNC_SQRT:
    case NODE_FUNC_NORM:
    case NODE_FUNC_ABS:
    case NODE_FUNC_RGB:
      return(FUNCTION1);
      
    case NODE_FUNC_BLEND:
      return(FUNCTION3);
  }
  assert(0);
}

void yyerror(char *s)
{
  fprintf(stdout, "Compiler failure: %s\n", s);
}

// 0 = ok
// 1 = error
int parser_parse(void)
{    
    int ret = yyparse();    
    return ret;
}
