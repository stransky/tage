/*
 *        .þÛÛþ þ    þ þÛÛþ.     þ    þ þÛÛÛþ.  þÛÛÛþ .þÛÛþ. þ    þ
 *       .þ   Û Ûþ.  Û Û   þ.    Û    Û Û    þ  Û.    Û.   Û Ûþ.  Û
 *       Û    Û Û Û  Û Û    Û    Û   þ. Û.   Û  Û     Û    Û Û Û  Û
 *     .þþÛÛÛÛþ Û  Û Û þÛÛÛÛþþ.  þþÛÛ.  þþÛÛþ.  þÛ    Û    Û Û  Û Û
 *    .Û      Û Û  .þÛ Û      Û. Û   Û  Û    Û  Û.    þ.   Û Û  .þÛ
 *    þ.      þ þ    þ þ      .þ þ   .þ þ    .þ þÛÛÛþ .þÛÛþ. þ    þ
 *
 * AGE (C) AnakreoN
 * Martin Stransky <stransky@anakreon.cz> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PARAMS_H__
#define __PARAMS_H__

// Token types
typedef enum {
  
  NODE_PARAMETER = 0,   // node is a tbox parameter
  NODE_OPERATOR  = 1,   // node is an operator
  NODE_CONSTANT  = 2,   // node is a constant
  NODE_VARIABLE  = 3,   // node is a variable
  NODE_FUNC1     = 4,   // node is a function
  NODE_FUNC2     = 5,   // node is a function
  NODE_FUNC3     = 6,   // node is a function
  NODE_FUNC4     = 7,   // node is a function
  
} NODE_TYPE;

typedef enum {
    
  // Parameters from previous modificator
  NODE_PARAM_PREV_0,
  NODE_PARAM_PREV_1,
  NODE_PARAM_PREV_2,
  NODE_PARAM_PREV_3,
  NODE_PARAM_PREV_4,
  NODE_PARAM_PREV_5,
  NODE_PARAM_PREV_6,
  NODE_PARAM_PREV_7,
  NODE_PARAM_PREV_8,
  NODE_PARAM_PREV_9,
  
  // Parameters from random generator
  NODE_PARAM_RAND,
  NODE_PARAM_RAND_HALF,
  NODE_PARAM_GAUSS,
  NODE_PARAM_GAUSS_HALF,
  
  // Constant values
  NODE_CONSTANT_INT,
  NODE_CONSTANT_FLOAT,
  NODE_CONSTANT_RGBAF,
  NODE_CONSTANT_VECT3DF,
  NODE_CONSTANT_RECT2DF,
    
  // Variables
  NODE_VAR_VARIABLE,
  NODE_VAR_VARIABLE_R,
  NODE_VAR_VARIABLE_G,
  NODE_VAR_VARIABLE_B,
  NODE_VAR_VARIABLE_A,
  NODE_VAR_VARIABLE_X,
  NODE_VAR_VARIABLE_Y,
  NODE_VAR_VARIABLE_Z,
  NODE_VAR_VARIABLE_DX,
  NODE_VAR_VARIABLE_DY,
  
  NODE_OP_ADD,  // +
  NODE_OP_SUB,  // -
  NODE_OP_MULT, // *
  NODE_OP_DIV,  // /
  NODE_OP_LB,   // (
  NODE_OP_RB,   // )  
  NODE_OP_EQ,   // =  
  NODE_OP_CM,   // ,
    
  NODE_FUNC_SIN,   // sin()
  NODE_FUNC_COS,   // cos()
  NODE_FUNC_SQRT,  // sqrt()
  NODE_FUNC_NORM,  // norm()
  NODE_FUNC_ABS,   // abs()
  NODE_FUNC_RGB,   // rgb()
  NODE_FUNC_BLEND  // blend()
  
} NODE_CONTENT_TYPE;

typedef enum {

  VARIABLE_UNSET = 0, // unset, a default value
  VARIABLE_ALIAS,     // a alias for another variable
  VARIABLE_INT,
  VARIABLE_FLOAT,
  VARIABLE_RGBAF,
  VARIABLE_VECT3DF,
  VARIABLE_RECT2DI

} VARIABLE_TYPE;

char * variable_type_translate(VARIABLE_TYPE type);

#ifndef __cplusplus
typedef struct rgbaf_simple {

  float r,g,b,a;
  
} RGBAF_SIMPLE;

typedef struct vect3df_simple {

  float x,y,z;  

} VECT3DF_SIMPLE;

typedef struct rect2di_simple {

  float x,y;
  float dx,dy;

} RECT2DI_SIMPLE;
#endif

typedef struct variable_node
{
  VARIABLE_TYPE   type;
  union {
    void          *p_alias;
    float          vfloat;
    int            vint;
    RGBAF_SIMPLE   vcolor;
    VECT3DF_SIMPLE vvector;
    RECT2DI_SIMPLE vrect;
  };  
} VARIABLE_NODE;

#define VARIABLE_NAME_LENGTH 32

typedef struct symbol_table_node
{
  char            name[VARIABLE_NAME_LENGTH];
  VARIABLE_NODE   value;
  
} SYMBOL_TABLE_NODE;

// Symbol table for the lines
#define MAX_SYMBOL_TABLE_ITEMS 32

typedef struct symbol_table {
  
  // Parent symbol table, NULL if unset
  struct symbol_table *p_parent;
  
  // Local symbols
  SYMBOL_TABLE_NODE table[MAX_SYMBOL_TABLE_ITEMS];
  int               length;
  int               current;
  
} SYMBOL_TABLE;

typedef struct param_node
{  
  NODE_TYPE              node_type;
  NODE_CONTENT_TYPE      content_type;
  union {
    float                cfloat;
    int                  cint;
    RGBAF_SIMPLE         ccolor;
    VECT3DF_SIMPLE       cvector;
    RECT2DI_SIMPLE       crect;
    SYMBOL_TABLE_NODE   *p_cvariable;
  };
  
} PARAM_NODE;

#define PARAM_NODE_CHILDS 4

typedef struct param_node_leaf {

  struct param_node_leaf *p_child[PARAM_NODE_CHILDS];
  PARAM_NODE              node;

} PARAM_NODE_LEAF;

PARAM_NODE_LEAF   * param_node_leaf_create(PARAM_NODE *p_node);
PARAM_NODE_LEAF   * param_node_leaf_set_1(PARAM_NODE_LEAF *p_leaf,
                                          PARAM_NODE_LEAF *p_child);
PARAM_NODE_LEAF   * param_node_leaf_set_2(PARAM_NODE_LEAF *p_leaf,
                                          PARAM_NODE_LEAF *p_child1,
                                          PARAM_NODE_LEAF *p_child2);
PARAM_NODE_LEAF   * param_node_leaf_set_3(PARAM_NODE_LEAF *p_leaf,
                                          PARAM_NODE_LEAF *p_child1,
                                          PARAM_NODE_LEAF *p_child2,
                                          PARAM_NODE_LEAF *p_child3);
PARAM_NODE_LEAF   * param_node_leaf_set_4(PARAM_NODE_LEAF *p_leaf,
                                          PARAM_NODE_LEAF *p_child1,
                                          PARAM_NODE_LEAF *p_child2,
                                          PARAM_NODE_LEAF *p_child3,
                                          PARAM_NODE_LEAF *p_child4);

PARAM_NODE        * parser_node_next(void);

void                param_tree_set(PARAM_NODE_LEAF *p_top);
PARAM_NODE_LEAF   * param_tree_get(void);
void                param_tree_release(PARAM_NODE_LEAF *p_top);

void                parser_node_reset(void);
PARAM_NODE        * parser_node_next(void);

int                 parser_parse(void);

void                print_node(PARAM_NODE *p_node, int ident);
void                print_tree(PARAM_NODE_LEAF *p_tree, int ident);

// Symbol table interface
void                symbol_table_set_active(SYMBOL_TABLE *p_table);
SYMBOL_TABLE      * symbol_table_get_active(void);
void                symbol_table_init(SYMBOL_TABLE *p_table, SYMBOL_TABLE *p_parent);

SYMBOL_TABLE_NODE * symbol_table_simple_get(SYMBOL_TABLE *p_table, const char *p_name, int length);
SYMBOL_TABLE_NODE * symbol_table_get(SYMBOL_TABLE *p_table, const char *p_name, int length);
SYMBOL_TABLE_NODE * symbol_table_get_int(SYMBOL_TABLE *p_table, const char *p_name);
SYMBOL_TABLE_NODE * symbol_table_add(SYMBOL_TABLE *p_table, const char *p_name, int length);
SYMBOL_TABLE_NODE * symbol_table_add_type(SYMBOL_TABLE *p_table, const char *p_name, int length, VARIABLE_TYPE type);

// Predefined variables in modificator targets
#define PIXEL_COLOR_FINAL   "pixel_color_final"
#define PIXEL_HEIGHT_FINAL  "pixel_height_final"
#define PIXEL_AREA          "pixel_area"
#define PIXEL_COORDINATE    "pixel_coordinate"
#define PIXEL_NORMAL        "pixel_normal"
#define TARGET_AREA         "target_area"

// Modificator stack - holds references
// to targets which are passed between modificators
#define MAX_MODIFICATOR_TARGETS 5

#endif // __PARAMS_H__

#ifdef __cplusplus
}
#endif
