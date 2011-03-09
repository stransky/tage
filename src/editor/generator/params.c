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

#include <stdio.h>
#include "mem_alloc.h"
#include "defs.h"
#include "params.h"
#include "y.tab.h"

char type_table[][10] = 
{
  "UNSET",
  "ALIAS",
  "INTEGER",
  "FLOAT",
  "COLOR",
  "VECTOR",
  "RECTANGLE",
};

char * variable_type_translate(VARIABLE_TYPE type)
{
  return(type_table[type]);
}

static SYMBOL_TABLE *p_global_symbol_table = NULL;

void symbol_table_set_active(SYMBOL_TABLE *p_table)
{
  if(p_table) {
    p_global_symbol_table = p_table;
  }
  else {
    p_global_symbol_table = NULL;
  }
}

SYMBOL_TABLE * symbol_table_get_active(void)
{
  return(p_global_symbol_table);
}

static char * operation_box_name_compose(char *out, const char *base_name, int num)
{
  sprintf(out,"%s_%d",base_name,num+1);
  return(out);
}

void symbol_table_init(SYMBOL_TABLE *p_table, SYMBOL_TABLE *p_parent)
{
  int i;
  
  // top symbol table
  p_table->p_parent = p_parent;
  
  // Set the table
  p_table->length = MAX_SYMBOL_TABLE_ITEMS;
  memset(&p_table->table, 0, sizeof(p_table->table));
  p_table->current = 0;
  
  // Add predefined target variables
  SYMBOL_TABLE_NODE *p_color_0 = symbol_table_add_type(p_table, PIXEL_COLOR_FINAL, VARIABLE_NAME_LENGTH, VARIABLE_ALIAS);
  SYMBOL_TABLE_NODE *p_height_0 = symbol_table_add_type(p_table, PIXEL_HEIGHT_FINAL, VARIABLE_NAME_LENGTH, VARIABLE_ALIAS);
  SYMBOL_TABLE_NODE *p_norm_0 = symbol_table_add_type(p_table, PIXEL_NORMAL, VARIABLE_NAME_LENGTH, VARIABLE_ALIAS);
  
  for(i = 0; i < MAX_MODIFICATOR_TARGETS; i++) {
    char tmp[MAX_NAME];
    SYMBOL_TABLE_NODE *p_color, *p_height, *p_norm;
    
    p_color = symbol_table_add_type(p_table, operation_box_name_compose(tmp, PIXEL_COLOR_FINAL, i), 
                                    VARIABLE_NAME_LENGTH, VARIABLE_RGBAF);
    p_height = symbol_table_add_type(p_table, operation_box_name_compose(tmp, PIXEL_HEIGHT_FINAL, i),
                                     VARIABLE_NAME_LENGTH, VARIABLE_FLOAT);
    p_norm = symbol_table_add_type(p_table, operation_box_name_compose(tmp, PIXEL_NORMAL, i),
                                   VARIABLE_NAME_LENGTH, VARIABLE_VECT3DF);
    
    // Set aliases for first target values
    if(!i) {
      p_color_0->value.p_alias = p_color;
      p_height_0->value.p_alias = p_height;
      p_norm_0->value.p_alias = p_norm;
    }
  }
      
  symbol_table_add_type(p_table, PIXEL_AREA, VARIABLE_NAME_LENGTH, VARIABLE_RECT2DI);
  symbol_table_add_type(p_table, PIXEL_COORDINATE, VARIABLE_NAME_LENGTH, VARIABLE_VECT3DF);
  symbol_table_add_type(p_table, TARGET_AREA, VARIABLE_NAME_LENGTH, VARIABLE_RECT2DI);
}

SYMBOL_TABLE_NODE * symbol_table_simple_get(SYMBOL_TABLE *p_table, const char *p_name, int length)
{
  // Look up in local scope
  SYMBOL_TABLE_NODE *p_node = p_table->table;
  if(p_node) {
    int i;
    for(i = 0; i < p_table->length; i++) {
      if(!strncmp(p_node->name, p_name, length)) {
        return(p_node);
      }
      p_node++;
    }
  }
  
  // Move up
  if(p_table->p_parent) {
    return(symbol_table_simple_get(p_table->p_parent,p_name,length));
  }
  
  return(NULL);
}

SYMBOL_TABLE_NODE * symbol_table_get(SYMBOL_TABLE *p_table, const char *p_name, int length)
{
  SYMBOL_TABLE_NODE *p_node = symbol_table_simple_get(p_table, p_name, length);
  if(p_node && p_node->value.type == VARIABLE_ALIAS) {
    p_node = p_node->value.p_alias;
  }
  return(p_node);
}

SYMBOL_TABLE_NODE * symbol_table_get_int(SYMBOL_TABLE *p_table, const char *p_name)
{
  return(symbol_table_get(p_table, p_name, strlen(p_name)));
}

SYMBOL_TABLE_NODE * symbol_table_add(SYMBOL_TABLE *p_table, const char *p_name, int length)
{
  if(p_table && p_table->current < p_table->length) {
    SYMBOL_TABLE_NODE *p_node = p_table->table + p_table->current;
    strncpy(p_node->name, p_name, length);
    memset(&p_node->value,0,sizeof(p_node->value));
    p_node->value.type = VARIABLE_UNSET;
    return(p_table->table+p_table->current++);
  }
  else {
    return(NULL);
  }
}

SYMBOL_TABLE_NODE * symbol_table_add_type(SYMBOL_TABLE *p_table, const char *p_name, int length, VARIABLE_TYPE type)
{
  SYMBOL_TABLE_NODE *p_node = symbol_table_add(p_table, p_name, length);
  if(p_node) {
    p_node->value.type = type;
  }
  return(p_node);
}

PARAM_NODE_LEAF * param_node_leaf_create(PARAM_NODE *p_node)
{
  PARAM_NODE_LEAF *p_new = mmalloc(sizeof(PARAM_NODE_LEAF));
  p_new->node = *p_node;
  return(p_new);
}

void param_tree_release(PARAM_NODE_LEAF *p_top)
{
  if(p_top->p_child[0]) {
    param_tree_release(p_top->p_child[0]);
  }
  if(p_top->p_child[1]) {
    param_tree_release(p_top->p_child[1]);
  }
  ffree(p_top);
}

PARAM_NODE_LEAF * param_node_leaf_set_1(PARAM_NODE_LEAF *p_leaf, PARAM_NODE_LEAF *p_child)
{
#ifdef GRAMMAR_DEBUG  
  printf("param_node_leaf_set_1\n");
  printf("  Leaf %p:\n",p_leaf);
  print_node(&p_leaf->node, 2);  
  printf("  Child 1 %p:\n",p_child1);
  print_node(&p_child1->node, 3);
#endif
  p_leaf->p_child[0] = p_child;
  return(p_leaf);
}

PARAM_NODE_LEAF * param_node_leaf_set_2(PARAM_NODE_LEAF *p_leaf, PARAM_NODE_LEAF *p_child1, PARAM_NODE_LEAF *p_child2)
{

#ifdef GRAMMAR_DEBUG  
  printf("param_node_leaf_set_2\n");
  printf("  Leaf %p:\n",p_leaf);
  print_node(&p_leaf->node, 2);  
  printf("  Child 1 %p:\n",p_child1);
  print_node(&p_child1->node, 3);  
  printf("  Child 2 %p:\n",p_child2);
  print_node(&p_child2->node, 3);
#endif
  
  p_leaf->p_child[0] = p_child1;
  p_leaf->p_child[1] = p_child2;
  return(p_leaf);
}

PARAM_NODE_LEAF   * param_node_leaf_set_3(PARAM_NODE_LEAF *p_leaf,
                                          PARAM_NODE_LEAF *p_child1,
                                          PARAM_NODE_LEAF *p_child2,
                                          PARAM_NODE_LEAF *p_child3)
{
#ifdef GRAMMAR_DEBUG  
  printf("param_node_leaf_set_3\n");
  printf("  Leaf %p:\n",p_leaf);
  print_node(&p_leaf->node, 2);  
  printf("  Child 1 %p:\n",p_child1);
  print_node(&p_child1->node, 3);  
  printf("  Child 2 %p:\n",p_child2);
  print_node(&p_child2->node, 3);
  printf("  Child 3 %p:\n",p_child3);
  print_node(&p_child3->node, 3);
#endif
  
  p_leaf->p_child[0] = p_child1;
  p_leaf->p_child[1] = p_child2;
  p_leaf->p_child[2] = p_child3;
  return(p_leaf); 
}

PARAM_NODE_LEAF   * param_node_leaf_set_4(PARAM_NODE_LEAF *p_leaf,
                                          PARAM_NODE_LEAF *p_child1,
                                          PARAM_NODE_LEAF *p_child2,
                                          PARAM_NODE_LEAF *p_child3,
                                          PARAM_NODE_LEAF *p_child4)
{
#ifdef GRAMMAR_DEBUG  
  printf("param_node_leaf_set_3\n");
  printf("  Leaf %p:\n",p_leaf);
  print_node(&p_leaf->node, 2);  
  printf("  Child 1 %p:\n",p_child1);
  print_node(&p_child1->node, 3);  
  printf("  Child 2 %p:\n",p_child2);
  print_node(&p_child2->node, 3);
  printf("  Child 3 %p:\n",p_child3);
  print_node(&p_child3->node, 3);
  printf("  Child 4 %p:\n",p_child4);
  print_node(&p_child4->node, 3);
#endif
  
  p_leaf->p_child[0] = p_child1;
  p_leaf->p_child[1] = p_child2;
  p_leaf->p_child[2] = p_child3;
  p_leaf->p_child[3] = p_child4;
  return(p_leaf); 
}


/*
 * Global variables
 */
static PARAM_NODE_LEAF *p_tree;

void param_tree_set(PARAM_NODE_LEAF *p_top)
{
  p_tree = p_top;
  
#ifdef GRAMMAR_DEBUG  
  if(p_top) {
    printf("param_tree_set\n");
    printf("  Top %p:\n",p_top);
    print_node(&p_top->node, 0);
  }
#endif
}

PARAM_NODE_LEAF * param_tree_get(void)
{
  return(p_tree);
}

/*
  PARAMETRIZER par;
  char line[] = "1+2+3+4";
  bool ret = par.load(line);
  par.print();  
  exit(0);
*/
