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

#include <errno.h>
#include <ctype.h>
 
#include "editor.h"

struct token_table_type {
  
  char       token_string[MAX_NAME];
  int        token_length;
  PARAM_NODE token;
  
} token_table[] = {
  {"+",                        1,  { NODE_OPERATOR,  NODE_OP_ADD }},
  {"-",                        1,  { NODE_OPERATOR,  NODE_OP_SUB }},
  {"*",                        1,  { NODE_OPERATOR,  NODE_OP_MULT}},
  {"/",                        1,  { NODE_OPERATOR,  NODE_OP_DIV}},
  {"(",                        1,  { NODE_OPERATOR,  NODE_OP_LB}},
  {")",                        1,  { NODE_OPERATOR,  NODE_OP_RB}},
  {"=",                        1,  { NODE_OPERATOR,  NODE_OP_EQ}},
  {",",                        1,  { NODE_OPERATOR,  NODE_OP_CM}},
  {"sin",                      3,  { NODE_FUNC1,     NODE_FUNC_SIN}},
  {"cos",                      3,  { NODE_FUNC1,     NODE_FUNC_COS}},
  {"sqrt",                     4,  { NODE_FUNC1,     NODE_FUNC_SQRT}},
  {"norm",                     4,  { NODE_FUNC1,     NODE_FUNC_NORM}},
  {"abs",                      3,  { NODE_FUNC1,     NODE_FUNC_ABS}},
  {"rgb",                      3,  { NODE_FUNC1,     NODE_FUNC_RGB}},
  {"blend",                    5,  { NODE_FUNC3,     NODE_FUNC_BLEND}},  
  {"PARAM_PREV_0",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_0}},
  {"PARAM_PREV_1",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_1}},
  {"PARAM_PREV_2",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_2}},
  {"PARAM_PREV_3",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_3}},
  {"PARAM_PREV_4",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_4}},
  {"PARAM_PREV_5",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_5}},
  {"PARAM_PREV_6",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_6}},
  {"PARAM_PREV_7",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_7}},
  {"PARAM_PREV_8",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_8}},
  {"PARAM_PREV_9",            12,  { NODE_PARAMETER, NODE_PARAM_PREV_9}},
  {"PARAM_RAND",              10,  { NODE_PARAMETER, NODE_PARAM_RAND}},
  {"PARAM_RAND_HALF",         15,  { NODE_PARAMETER, NODE_PARAM_RAND_HALF}},
  {"PARAM_GAUSS",             11,  { NODE_PARAMETER, NODE_PARAM_GAUSS}},
  {"PARAM_GAUSS_HALF",        16,  { NODE_PARAMETER, NODE_PARAM_GAUSS_HALF}},
};

char token_delim[][2] = {
  "+",
  "-",
  "*",
  "/",
  "(",
  ")",
  "=",
  ";",
  ",",
};

char constant_table[][MAX_NAME] = {
  "CONSTANT/INT",
  "CONSTANT/FLOAT",
  "CONSTANT/RGBAF",
  "CONSTANT/VECT3DF",
  "CONSTANT/RECT2DF"
};

char variable_table[][MAX_NAME] = {
  "VARIABLE",
  "VARIABLE_R",
  "VARIABLE_G",
  "VARIABLE_B",
  "VARIABLE_X",
  "VARIABLE_Y",
  "VARIABLE_Z",
  "VARIABLE_DX",
  "VARIABLE_DY",
};

/*
 * Token table helpers
 */
static char * get_token_name(PARAM_NODE &node)
{
  if(node.node_type == NODE_CONSTANT) {
    int pos = node.content_type-NODE_CONSTANT_INT;
    assert(pos >= 0 && pos < (int)(sizeof(constant_table)/sizeof(constant_table[0])));
    return(constant_table[pos]);
  }
  else if(node.node_type == NODE_VARIABLE) {
    int pos = node.content_type-NODE_VAR_VARIABLE;
    assert(pos >= 0 && pos < (int)(sizeof(variable_table)/sizeof(variable_table[0])));
    return(variable_table[pos]);
  } 
  else {
    int i;
    int num = sizeof(token_table)/sizeof(token_table[0]);
    for(i = 0; i < num; i++) {
      if(token_table[i].token.node_type == node.node_type &&
         token_table[i].token.content_type == node.content_type)
      {
        return(token_table[i].token_string);
      }
    }
    return(NULL);
  }
}

static bool is_token_delim(char c)
{
  int i;
  int num = sizeof(token_delim)/sizeof(token_delim[0]);
  for(i = 0; i < num; i++) {
    if(token_delim[i][0] == c) {
        return(TRUE);
    }
  }
  return(FALSE);
}

static bool is_token(char *p_line, int line_length, PARAM_NODE &token)
{ 
  int i;
  int num = sizeof(token_table)/sizeof(token_table[0]);
  for(i = 0; i < num; i++) {
    if(token_table[i].token_length == line_length) {
      if(!strncasecmp(token_table[i].token_string, p_line, line_length)) {
        token = token_table[i].token;
        return(TRUE);
      }
    }
  }
  return(FALSE);
}

static bool is_number(char *p_line, int line_length, PARAM_NODE &token)
{ 
  #define MAX_NUMBER 100
  char tmp[MAX_NUMBER+1];
  
  if(line_length > MAX_NUMBER)
    line_length = MAX_NUMBER;
  
  strncpy(tmp, p_line, line_length);
  tmp[line_length] = '\0';
  
  int   int_value;
  float float_value;
  bool  int_loaded = false;
  bool  float_loaded = false;
  
  // is it a hexadecimal color?
  if(tmp[0] == '#') {
    int_loaded = (sscanf(tmp+1,"%x",&int_value) == 1);
    if(int_loaded) {
      token.node_type = NODE_CONSTANT;
      token.content_type = NODE_CONSTANT_RGBAF;
      int_value = rb_switch(int_value);
      rgba_float(int_value, &token.ccolor.r, &token.ccolor.g,
                 &token.ccolor.b, &token.ccolor.a);
    }
  }
  // is it a hexadecimal number?
  else if(tmp[0] == '0' && tolower(tmp[1]) == 'x') {
    int_loaded = (sscanf(tmp+2,"%x",&int_value) == 1);
    if(int_loaded) {
      token.node_type = NODE_CONSTANT;
      token.content_type = NODE_CONSTANT_INT;
      token.cint = int_value;
    }
  }
  // a normal number
  else if(isdigit(tmp[0])) {
    float_loaded = (sscanf(tmp,"%f",&float_value) == 1);
    if(float_loaded) {
      token.node_type = NODE_CONSTANT;
      token.content_type = NODE_CONSTANT_FLOAT;
      token.cfloat = float_value;
    }
  }
    
  return(int_loaded || float_loaded);
}

#ifdef __cplusplus
extern "C" {
#endif

void print_node(PARAM_NODE *p_node, int ident)
{
  switch(p_node->content_type) {
    case NODE_CONSTANT_INT:
      pprintf("%*cNODE_CONSTANT_INT(%d, 0x%x)",ident*2,' ',p_node->cint,p_node->cint);
      break;    
    case NODE_CONSTANT_FLOAT:
      pprintf("%*cNODE_CONSTANT_FLOAT(%d)",ident*2,' ',p_node->cfloat);
      break;    
    default:
      char *p_token_name = get_token_name(*p_node);
      if(!p_token_name) {
        p_token_name = (char *)"UNKNOWN NODE!";
        assert(0);
      }
      pprintf("%*c%s",ident*2,' ',p_token_name);
      break;
  } 
}

void print_tree(PARAM_NODE_LEAF *p_tree, int ident)
{
  print_node(&p_tree->node, ident);
  if(p_tree->p_child[0]) {
    print_tree(p_tree->p_child[0],ident+1);
  }
  if(p_tree->p_child[1]) {
    print_tree(p_tree->p_child[1],ident+1);
  }
}

#ifdef __cplusplus
}
#endif

#define MAX_PARAM_LINE 4000

typedef enum {
  
  STATE_TOKEN_START,
  STATE_TOKEN_IN,
  STATE_TOKEN_END,
  STATE_STRING_END,
   
} TOKEN_STATE;

// delit to bez tech mezer
int parametrizer::tokens_get(DATA_FILE_SECTION *p_line, array<PARAM_NODE> &nodes)
{  
  assert(p_line);    
  
  char *p_start = p_line->line_raw_string_get();
  char *p_current = p_line->line_raw_string_get();
  TOKEN_STATE state = STATE_TOKEN_START;
  
  do {
    switch(state) {
      case STATE_TOKEN_START:
      {
        assert(p_start == p_current);
        
        char c = *p_start;
        if(c == '\0') {
          state = STATE_STRING_END;
        } 
        else if(isspace(c)) {
          p_start++;
        }
        else if(is_token_delim(c)) {
          state = STATE_TOKEN_END;
        } else {
          state = STATE_TOKEN_IN;
        }
        p_current++;
        
        break;
      }
      
      case STATE_TOKEN_IN:
      {
        char c = *p_current;
        if(c == '\0' || is_token_delim(c) || isspace(c)) {
          state = STATE_TOKEN_END;
        }
        else {
          p_current++;
        }
        break;
      }
      
      case STATE_TOKEN_END:
      { 
        PARAM_NODE token;        
        memset(&token,0,sizeof(token));
        
        char c = *p_start;
        int length = p_current - p_start;
        
        assert(c != '\0' && !isspace(c));        
        assert(length != 0);
        
        if(is_token(p_start, length, token) ||
           is_number(p_start, length, token)) {
          // It's a number or token
          nodes.append(token);
        }
        else {
          if(!is_token_delim(p_start[0])) {
            // We have a variable here
            if(p_start[length-2] == '.') {
              // it's a part of color variable (.r .g .b .a) 
              // or a vector variable (.x .y .z)
              switch(p_start[length-1]) {
                case 'r':
                  token.content_type = NODE_VAR_VARIABLE_R;
                  length -= 2;
                  break;
                case 'g':
                  token.content_type = NODE_VAR_VARIABLE_G;
                  length -= 2;
                  break;
                case 'b':
                  token.content_type = NODE_VAR_VARIABLE_B;
                  length -= 2;
                  break;
                case 'a':
                  token.content_type = NODE_VAR_VARIABLE_A;
                  length -= 2;
                  break;
                
                case 'x':
                  token.content_type = NODE_VAR_VARIABLE_X;
                  length -= 2;
                  break;
                case 'y':
                  token.content_type = NODE_VAR_VARIABLE_Y;
                  length -= 2;
                  break;
                case 'z':
                  token.content_type = NODE_VAR_VARIABLE_Z;
                  length -= 2;
                  break;
                
                default:
                  p_line->ppline();
                  pperror(TRUE, "wrong tail .%c",p_start[length-1]);
                  break;
              }
            }
            else {
              token.content_type = NODE_VAR_VARIABLE;
            }
                      
            SYMBOL_TABLE_NODE *p_variable = symbol_table_get(symbol_table_get_active(), p_start, length);
            if(!p_variable) {
              p_variable = symbol_table_add(symbol_table_get_active(), p_start, length);
            }
            
            if(token.content_type == NODE_VAR_VARIABLE_R ||
               token.content_type == NODE_VAR_VARIABLE_G ||
               token.content_type == NODE_VAR_VARIABLE_B ||
               token.content_type == NODE_VAR_VARIABLE_A)
            {
              if(p_variable->value.type == VARIABLE_UNSET) {
                p_variable->value.type = VARIABLE_RGBAF;
              }
              
              if(p_variable->value.type != VARIABLE_RGBAF) {
                p_line->ppline();
                pperror(TRUE, "variable %s has to be a color type! (is %s)", 
                              p_variable->name, 
                              variable_type_translate(p_variable->value.type));
              }
            }
            else if(token.content_type == NODE_VAR_VARIABLE_X ||
                   token.content_type == NODE_VAR_VARIABLE_Y ||
                   token.content_type == NODE_VAR_VARIABLE_Z)
            {
              if(p_variable->value.type == VARIABLE_UNSET) {
                p_variable->value.type = VARIABLE_VECT3DF;
              }
              
              if(p_variable->value.type != VARIABLE_VECT3DF) {
                p_line->ppline();
                pperror(TRUE, "variable %s has to be a vector type! (is %s)", 
                              p_variable->name, 
                              variable_type_translate(p_variable->value.type));
              }
            }
            
            token.node_type = NODE_VARIABLE;
            token.p_cvariable = p_variable;
            
            // Append it
            nodes.append(token);
          }
        }
        
        p_start = p_current;
        state = STATE_TOKEN_START;
        break;
      }
      default:
        break;
    }
  } while(state != STATE_STRING_END);
  
  return(nodes.num_get());
}

VARIABLE_NODE line_tree::variable_node_add_float(float v1, float v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_FLOAT;
  ret.vfloat = v1+v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_add_color(RGBAF v1, RGBAF v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_RGBAF;
  ret.vcolor = v1+v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_add(VARIABLE_NODE ch1, VARIABLE_NODE ch2)
{
  if(ch1.type == VARIABLE_FLOAT && ch2.type == VARIABLE_FLOAT) {
    return(variable_node_add_float(ch1.vfloat, ch2.vfloat));
  }
  else if(ch1.type == VARIABLE_RGBAF && ch2.type == VARIABLE_RGBAF) {
    return(variable_node_add_color(ch1.vcolor, ch2.vcolor));
  }
  else {
    ppline();
    pperror(TRUE, "type mismatch - can't add types %s and %s", 
                  variable_type_translate(ch1.type),
                  variable_type_translate(ch2.type));
  }  
}

VARIABLE_NODE line_tree::variable_node_sub_float(float v1, float v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_FLOAT;
  ret.vfloat = v1-v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_sub_color(RGBAF v1, RGBAF v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_RGBAF;
  ret.vcolor = v1-v2;
  return(ret); 
}

VARIABLE_NODE line_tree::variable_node_sub(VARIABLE_NODE ch1, VARIABLE_NODE ch2)
{
  if(ch1.type == VARIABLE_FLOAT && ch2.type == VARIABLE_FLOAT) {
    return(variable_node_sub_float(ch1.vfloat, ch2.vfloat));
  }
  else if(ch1.type == VARIABLE_RGBAF && ch2.type == VARIABLE_RGBAF) {
    return(variable_node_sub_color(ch1.vcolor, ch2.vcolor));
  }
  else {
    ppline();
    pperror(TRUE, "type mismatch - can't substract types %s and %s", 
                  variable_type_translate(ch1.type),
                  variable_type_translate(ch2.type));
  }
}

VARIABLE_NODE line_tree::variable_node_mult_float(float v1, float v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_FLOAT;
  ret.vfloat = v1*v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_mult_color(RGBAF v1, RGBAF v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_RGBAF;
  ret.vcolor = v1*v2;
  return(ret); 
}

VARIABLE_NODE line_tree::variable_node_mult_color_float(RGBAF v1, float v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_RGBAF;
  ret.vcolor = v1*v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_mult(VARIABLE_NODE ch1, VARIABLE_NODE ch2)
{
  if(ch1.type == VARIABLE_FLOAT && ch2.type == VARIABLE_FLOAT) {
    return(variable_node_mult_float(ch1.vfloat, ch2.vfloat));
  }
  else if(ch1.type == VARIABLE_RGBAF && ch2.type == VARIABLE_RGBAF) {
    return(variable_node_mult_color(ch1.vcolor, ch2.vcolor));
  }
  else {
    if(ch1.type == VARIABLE_RGBAF) {
      return(variable_node_mult_color_float(ch1.vcolor, ch2.vfloat));
    }
    else {
      return(variable_node_mult_color_float(ch2.vcolor, ch1.vfloat));
    }
  }
  VARIABLE_NODE tmp;
  return(tmp);
}

VARIABLE_NODE line_tree::variable_node_div_float(float v1, float v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_FLOAT;
  ret.vfloat = v1/v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_div_color(RGBAF v1, RGBAF v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_RGBAF;
  ret.vcolor = v1/v2;
  return(ret); 
}

VARIABLE_NODE line_tree::variable_node_div_color_float(RGBAF v1, float v2)
{
  VARIABLE_NODE ret;
  ret.type = VARIABLE_RGBAF;
  ret.vcolor = v1/v2;
  return(ret);
}

VARIABLE_NODE line_tree::variable_node_div(VARIABLE_NODE ch1, VARIABLE_NODE ch2)
{
  if(ch1.type == VARIABLE_FLOAT && ch2.type == VARIABLE_FLOAT) {
    return(variable_node_div_float(ch1.vfloat, ch2.vfloat));
  }
  else if(ch1.type == VARIABLE_RGBAF && ch2.type == VARIABLE_RGBAF) {
    return(variable_node_div_color(ch1.vcolor, ch2.vcolor));
  }
  else {
    if(ch1.type == VARIABLE_RGBAF && ch2.type == VARIABLE_FLOAT) {
      return(variable_node_div_color_float(ch1.vcolor, ch2.vfloat));
    }
    else {
      ppline();
      pperror(TRUE, "type mismatch - can't divide types %s and %s", 
                    variable_type_translate(ch1.type),
                    variable_type_translate(ch2.type));
    }
  }
}

// source is float
bool line_tree::variable_node_convert_float(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result)
{
  if(target == VARIABLE_INT) {
    p_result->type = VARIABLE_INT;
    p_result->vint = roundf(ch.vfloat);
    return(TRUE);
  }
  else {
    ppline();
    pperror(TRUE, "type mismatch - can't convert type %s to %s", 
                  variable_type_translate(ch.type),
                  variable_type_translate(target));
  }
  return(FALSE);
}

// source is int
bool line_tree::variable_node_convert_int(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result)
{
  if(target == VARIABLE_FLOAT) {
    p_result->type = VARIABLE_FLOAT;
    p_result->vfloat = (float)ch.vint;
    return(TRUE);
  }
  else if(target == VARIABLE_RGBAF) {
    p_result->type = VARIABLE_RGBAF;
    rgba_float(ch.vint, &p_result->vcolor.r, &p_result->vcolor.g,
               &p_result->vcolor.b, &p_result->vcolor.a);
    return(TRUE);
  }
  else {
    ppline();
    pperror(TRUE, "type mismatch - can't convert type %s to %s", 
                  variable_type_translate(ch.type),
                  variable_type_translate(target));
  }
  return(FALSE);
}

// source is rgbaf
bool line_tree::variable_node_convert_color(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result)
{
  if(target == VARIABLE_INT) {
    p_result->type = VARIABLE_INT;
    p_result->vint = float_rgba(p_result->vcolor.r, p_result->vcolor.g, 
                                p_result->vcolor.b, p_result->vcolor.a);
    return(TRUE);
  }
  else {
    ppline();
    pperror(TRUE, "type mismatch - can't convert type %s to %s", 
                  variable_type_translate(ch.type),
                  variable_type_translate(target));
  }
  return(FALSE);
}

bool line_tree::variable_node_convert(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result)
{
  if(ch.type == VARIABLE_FLOAT) {
    // convert from float
    return(variable_node_convert_float(ch, target, p_result));
  }
  else if(ch.type == VARIABLE_INT) {
    // convert from int
    return(variable_node_convert_int(ch, target, p_result));
  }
  else if(ch.type == VARIABLE_RGBAF) {
    // convert from color
    return(variable_node_convert_color(ch, target, p_result));
  }
  else {
    ppline();
    pperror(TRUE, "type mismatch - can't convert type %s to %s", 
                  variable_type_translate(ch.type),
                  variable_type_translate(target));
    return(FALSE);
  }
}

float line_tree::variable_content_get(VARIABLE_NODE *p_var, NODE_CONTENT_TYPE content_type)
{  
  assert(p_var->type == VARIABLE_RGBAF || 
         p_var->type == VARIABLE_VECT3DF || 
         p_var->type == VARIABLE_RECT2DI);
    
  switch(content_type) {
    
    case NODE_VAR_VARIABLE_R:
      return(p_var->vcolor.r);
    case NODE_VAR_VARIABLE_G:
      return(p_var->vcolor.g);
    case NODE_VAR_VARIABLE_B:
      return(p_var->vcolor.b);
    case NODE_VAR_VARIABLE_A:
      return(p_var->vcolor.a);
    
    case NODE_VAR_VARIABLE_X:
      if(p_var->type == VARIABLE_VECT3DF) {
        return(p_var->vvector.x);
      }
      else {
        return(p_var->vrect.x);      
      }
    case NODE_VAR_VARIABLE_Y:
      if(p_var->type == VARIABLE_VECT3DF) {
        return(p_var->vvector.y);
      }
      else {
        return(p_var->vrect.y);
      }
      
    case NODE_VAR_VARIABLE_Z:
      return(p_var->vvector.z);
    case NODE_VAR_VARIABLE_DX:
      return(p_var->vrect.dx);
    case NODE_VAR_VARIABLE_DY:
      return(p_var->vrect.dy);    
    
    default:
      assert(0);
      return(0);
  }
}

void line_tree::variable_content_set(VARIABLE_NODE *p_var, NODE_CONTENT_TYPE content_type, float val)
{  
  assert(p_var->type == VARIABLE_RGBAF || 
         p_var->type == VARIABLE_VECT3DF || 
         p_var->type == VARIABLE_RECT2DI);
  
  switch(content_type) {
    
    case NODE_VAR_VARIABLE_R:
      p_var->vcolor.r = val;
      break;
    case NODE_VAR_VARIABLE_G:
      p_var->vcolor.g = val;
      break;
    case NODE_VAR_VARIABLE_B:
      p_var->vcolor.b = val;
      break;
    case NODE_VAR_VARIABLE_A:
      p_var->vcolor.a = val;
      break;
    
    case NODE_VAR_VARIABLE_X:
      if(p_var->type == VARIABLE_VECT3DF) {
        p_var->vvector.x = val;
      }
      else {
        p_var->vrect.x = val;
      }
    case NODE_VAR_VARIABLE_Y:
      if(p_var->type == VARIABLE_VECT3DF) {
        p_var->vvector.y = val;
      }
      else {
        p_var->vrect.y = val;
      }
      
    case NODE_VAR_VARIABLE_Z:
      p_var->vvector.z = val;
    case NODE_VAR_VARIABLE_DX:
      p_var->vrect.dx = val;
    case NODE_VAR_VARIABLE_DY:
      p_var->vrect.dy = val;
    
    default:
      assert(0);
      break;
  }
}

VARIABLE_NODE line_tree::execute_leaf(PARAM_NODE_LEAF *p_leaf, PIXEL_PARAMS *p_params)
{
  VARIABLE_NODE ret;
  
  PARAM_NODE *p_node = &p_leaf->node;
  switch(p_node->content_type) {
    
    // Parameters from previous modificator
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
    {
      assert(p_params);
      MODIFICATOR_PARAMS *p_par = p_params->p_params;
      int parameter = p_node->content_type - NODE_PARAM_PREV_0;
      if(p_par->is_index(parameter)) {
        ret.type = VARIABLE_FLOAT;
        ret.vfloat = p_par->get(parameter);
      } 
      else {
        ppline();
        pperror(TRUE, "parametrizer::execute_leaf() - missing parameter %d", parameter);
        ret.type = VARIABLE_INT;
        ret.vint = FALSE;
      }
      break;
    }
    
    // random numbers
    case NODE_PARAM_RAND:
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = random_generator::generator_rand_0();
      break;
    case NODE_PARAM_RAND_HALF:
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = random_generator::generator_rand();
      break;
    case NODE_PARAM_GAUSS:
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = random_generator::generator_gauss_0();
      break;
    case NODE_PARAM_GAUSS_HALF:
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = fabs(random_generator::generator_gauss_0());
      break;
    
    case NODE_CONSTANT_INT:
      ret.type = VARIABLE_INT;
      ret.vint = p_node->cint;
      break;
    case NODE_CONSTANT_FLOAT:
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = p_node->cfloat;
      break;
    case NODE_CONSTANT_RGBAF:
      ret.type = VARIABLE_RGBAF;
      ret.vcolor = p_node->ccolor;
      break;
    case NODE_CONSTANT_VECT3DF:
      ret.type = VARIABLE_VECT3DF;
      ret.vvector = p_node->cvector;
      break;
    case NODE_CONSTANT_RECT2DF:
      ret.type = VARIABLE_RECT2DI;
      ret.vrect = p_node->crect;
      break;

    case NODE_VAR_VARIABLE:
      ret = p_node->p_cvariable->value;
      break;
    
    case NODE_VAR_VARIABLE_R:
    case NODE_VAR_VARIABLE_G:
    case NODE_VAR_VARIABLE_B:
    case NODE_VAR_VARIABLE_A:
    {
      assert(p_node->p_cvariable->value.type == VARIABLE_RGBAF);
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = variable_content_get(&p_node->p_cvariable->value, p_node->content_type);
      break;
    }     
    case NODE_VAR_VARIABLE_X:
    case NODE_VAR_VARIABLE_Y:
    case NODE_VAR_VARIABLE_Z:
    {
      assert(p_node->p_cvariable->value.type == VARIABLE_VECT3DF);
      ret.type = VARIABLE_FLOAT;
      ret.vfloat = variable_content_get(&p_node->p_cvariable->value, p_node->content_type);
      break;
    } 
    case NODE_VAR_VARIABLE_DX:
    case NODE_VAR_VARIABLE_DY:
    {
      ppline();
      pperror(TRUE, "Rectangles are not yet implemented.");
    }
    
    case NODE_OP_ADD:  // +
    {
      assert(p_leaf->p_child[0] && p_leaf->p_child[1]);
      VARIABLE_NODE ch1 = execute_leaf(p_leaf->p_child[0],p_params);
      VARIABLE_NODE ch2 = execute_leaf(p_leaf->p_child[1],p_params);
      ret = variable_node_add(ch1,ch2);
      break;
    }
    case NODE_OP_SUB:  // -
    {
      assert(p_leaf->p_child[0] && p_leaf->p_child[1]);
      VARIABLE_NODE ch1 = execute_leaf(p_leaf->p_child[0],p_params);
      VARIABLE_NODE ch2 = execute_leaf(p_leaf->p_child[1],p_params);
      ret = variable_node_sub(ch1,ch2);
      break;
    }
    case NODE_OP_MULT: // *
    {
      assert(p_leaf->p_child[0] && p_leaf->p_child[1]);
      VARIABLE_NODE ch1 = execute_leaf(p_leaf->p_child[0],p_params);
      VARIABLE_NODE ch2 = execute_leaf(p_leaf->p_child[1],p_params);
      ret = variable_node_mult(ch1,ch2);
      break;
    }
    case NODE_OP_DIV:  // /
    {
      assert(p_leaf->p_child[0] && p_leaf->p_child[1]);
      VARIABLE_NODE ch1 = execute_leaf(p_leaf->p_child[0],p_params);
      VARIABLE_NODE ch2 = execute_leaf(p_leaf->p_child[1],p_params);
      ret = variable_node_div(ch1,ch2);
      break;
    }
    case NODE_OP_EQ:  // =
    {
      assert(p_leaf->p_child[0] && p_leaf->p_child[1]);
      assert(p_leaf->p_child[0]->node.node_type == NODE_VARIABLE);
      
      PARAM_NODE *p_target_node = &p_leaf->p_child[0]->node;
      VARIABLE_NODE *p_target = &(p_target_node->p_cvariable->value);
      VARIABLE_NODE source = execute_leaf(p_leaf->p_child[1],p_params);

      // Set whole variable
      if(p_target_node->content_type == NODE_VAR_VARIABLE) {
        if(p_target->type == VARIABLE_UNSET || p_target->type == source.type) {
          *p_target = source;
        }
        else {
          bool ret = variable_node_convert(source, p_target->type, p_target);
          if(!ret) {
            ppline();
            pperror(TRUE, "Type mismatch: can't convert %s to %s",
                    variable_type_translate(source.type), 
                    variable_type_translate(p_target->type));
          }
        }
      }
      else if(p_target_node->content_type == NODE_VAR_VARIABLE_R ||
              p_target_node->content_type == NODE_VAR_VARIABLE_G ||
              p_target_node->content_type == NODE_VAR_VARIABLE_B ||
              p_target_node->content_type == NODE_VAR_VARIABLE_A)        
      {        
        VARIABLE_NODE target;
        if(source.type == VARIABLE_FLOAT) {
          target = source;
        }
        else {
          bool ret = variable_node_convert(source, VARIABLE_FLOAT, &target);
          if(!ret) {            
            ppline();
            pperror(TRUE, "Type mismatch: can't convert %s to %s",
                    variable_type_translate(source.type),
                    variable_type_translate(VARIABLE_FLOAT));
          }
        }
        target.type = VARIABLE_FLOAT;
        variable_content_set(p_target, p_target_node->content_type, target.vfloat);
      }
      else {
        ppline();
        pperror(TRUE, "Assignment target is not a variable!");
      }
      
      ret.type = VARIABLE_INT;
      ret.vint = TRUE;
      break;
    }
      
    case NODE_FUNC_SIN:   // sin()
    {
      assert(p_leaf->p_child[0]);
      ret = execute_leaf(p_leaf->p_child[0],p_params);
      if(ret.type != VARIABLE_FLOAT) {
        ppline();
        pperror(TRUE, "Type mismatch: sin() needs a parameter type FLOAT, not %s",
                variable_type_translate(ret.type));
      }
      ret.vfloat = sinf(ret.vfloat);
      break;
    }
    case NODE_FUNC_COS:   // cos()
    {
      assert(p_leaf->p_child[0]);
      ret = execute_leaf(p_leaf->p_child[0],p_params);
      if(ret.type != VARIABLE_FLOAT) {
        ppline();
        pperror(TRUE, "Type mismatch: cos() needs a parameter type FLOAT, not %s",
                variable_type_translate(ret.type));
      }
      ret.vfloat = cosf(ret.vfloat);
      break;
    }
    case NODE_FUNC_SQRT:  // sqrt()
    {
      assert(p_leaf->p_child[0]);
      ret = execute_leaf(p_leaf->p_child[0],p_params);
      if(ret.type != VARIABLE_FLOAT) {
        ppline();
        pperror(TRUE, "Type mismatch: sqrt() needs a parameter type FLOAT, not %s",
                variable_type_translate(ret.type));
      }
      ret.vfloat = sqrtf(ret.vfloat);
      break;
    }
    case NODE_FUNC_NORM:  // norm()
    {
      assert(p_leaf->p_child[0]);
      ret = execute_leaf(p_leaf->p_child[0],p_params);
      if(ret.type != VARIABLE_FLOAT) {
        ppline();
        pperror(TRUE, "Type mismatch: norm() needs a parameter type FLOAT, not %s",
                variable_type_translate(ret.type));
      }
      if(ret.vfloat > 1) {
        ret.vfloat = 1.0f;
      }
      else if(ret.vfloat < 0) {
        ret.vfloat = 0.0f;
      }      
      break;
    }
    case NODE_FUNC_ABS: // abs()
    {
      assert(p_leaf->p_child[0]);
      ret = execute_leaf(p_leaf->p_child[0],p_params);
      if(ret.type != VARIABLE_FLOAT) {
        ppline();
        pperror(TRUE, "Type mismatch: abs() needs a parameter type FLOAT, not %s",
                variable_type_translate(ret.type));
      }
      ret.vfloat = fabs(ret.vfloat);
      break;
    }
    case NODE_FUNC_RGB: // rgb()
    {
      assert(p_leaf->p_child[0]);
      ret = execute_leaf(p_leaf->p_child[0],p_params);
      if(ret.type != VARIABLE_FLOAT) {
        ppline();
        pperror(TRUE, "Type mismatch: rgb() needs a parameter type FLOAT, not %s",
                variable_type_translate(ret.type));
      }
      
      if(ret.vfloat < 0)
        ret.vfloat = 0;
      else if(ret.vfloat > 255)
        ret.vfloat = 1;
      else
        ret.vfloat = ret.vfloat/255;
      
      break;
    }    
    case NODE_FUNC_BLEND:  // blend()
    {
      assert(p_leaf->p_child[0]);
      assert(p_leaf->p_child[1]);
      assert(p_leaf->p_child[2]);
      VARIABLE_NODE ch1 = execute_leaf(p_leaf->p_child[0],p_params);
      VARIABLE_NODE ch2 = execute_leaf(p_leaf->p_child[1],p_params);
      VARIABLE_NODE ch3 = execute_leaf(p_leaf->p_child[2],p_params);
      
      if(ch1.type != VARIABLE_RGBAF ||
         ch2.type != VARIABLE_FLOAT ||
         ch3.type != VARIABLE_RGBAF)
      {      
        if(ret.type != VARIABLE_FLOAT) {
          ppline();
          pperror(TRUE, "Type mismatch: blend() takes types COLOR, FLOAT, COLOR, but it got %s %s %s",
                  variable_type_translate(ch1.type),
                  variable_type_translate(ch2.type),
                  variable_type_translate(ch3.type));
                  
        }      
      }
      
      float alpha_dest = ch2.vfloat;
      float alpha_src = 1.0f - ch2.vfloat;
      
      RGBAF dest(ch1.vcolor);
      RGBAF src(ch3.vcolor);
      
      ret.type = VARIABLE_RGBAF;
      ret.vcolor = dest*alpha_dest + src*alpha_src;
      break;
    }
    
    default:
      ret.type = VARIABLE_INT;
      ret.vint = FALSE;
      break;
  }
  
  return(ret);
}

bool line_tree::execute(PIXEL_PARAMS *p_params)
{
  if(!p_top) {
    return(FALSE);
  }
  else {
    VARIABLE_NODE ret = execute_leaf(p_top, p_params);
    return(ret.vint);
  }
}

void line_tree::ppline(void)
{
  ppset(p_param->parse_file_get(), source_line_number);
  pperror(FALSE, p_source_line);
  ppclear();
}

static array<PARAM_NODE> parser_nodes;

bool parametrizer::parse_line(DATA_FILE_SECTION *p_line)
{   
  parse_file_set(p_line->source_file_get());
  
  // reset global parser cache and top
  // of parsed tree
  parser_nodes.reset();
  param_tree_set(NULL);

  // Set active symbol table from this parametrizer
  symbol_table_set_active(p_table);
  
  // Load all tokend  
  if(!tokens_get(p_line, parser_nodes))
    return(FALSE);
  
  // compose the tree
  if(parser_parse()) {    
    // Print the wrong line
    p_line->ppline();
    abort();
  }
    
  LINE_TREE *p_top = new LINE_TREE(this,
                                   param_tree_get(),
                                   p_line->line_raw_string_get(),
                                   p_line->source_line_get());
  lines.list_insert_last(p_top);
  
  // parsed tree is now owned by LINE_TREE object
  param_tree_set(NULL);
  
  // unset active symbol table
  symbol_table_set_active(NULL);
  
  return(TRUE);
}

void parametrizer::clear(void)
{  
  LINE_TREE *p_tmp = (LINE_TREE *)lines.list_get_first();
  while(p_tmp) {
    LINE_TREE *p_rm = p_tmp;
    p_tmp = (LINE_TREE *)p_tmp->list_next();
    p_rm->clear();
    delete p_rm;
  }
  lines.list_clear();
}

void parametrizer::print(void)
{  
  LINE_TREE *p_tmp = (LINE_TREE *)lines.list_get_first();
  while(p_tmp) {
    p_tmp->print();
    p_tmp = (LINE_TREE *)p_tmp->list_next();
  }
}

bool parametrizer::execute(PIXEL_PARAMS *p_pixel)
{    
  // Execute loaded params
  LINE_TREE *p_tmp = (LINE_TREE *)lines.list_get_first();
  while(p_tmp) {
    p_tmp->execute(p_pixel);
    p_tmp = (LINE_TREE *)p_tmp->list_next();
  }
  return(TRUE);
}

void parser_nodes_get(array<PARAM_NODE> &nodes)
{
  nodes = parser_nodes;
}

pixel_params::pixel_params(MODIFICATOR_PARAMS *p_par)
    : p_params(p_par)      
{
}

#ifdef __cplusplus
extern "C" {
#endif

void parser_node_reset(void)
{
  parser_nodes.reset();
}

PARAM_NODE * parser_node_next(void)
{
  if(parser_nodes.is_empty())
    return(NULL);
  
  return(parser_nodes.fetch_ref());
}

#ifdef __cplusplus
}
#endif
