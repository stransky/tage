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

/*
  Parses modificator parameters
  proceed parameters
*/

#ifndef __GENERATOR_MODIFICATOR_PARAMS_H__
#define __GENERATOR_MODIFICATOR_PARAMS_H__

/*
  single pixel pattern:
  COLOR_PATTERN_POINT_SINGLE

  // Is the delta color vector bidirectional?
  // (i.e. bidirectional      => target_color_center +- target_color_delta)
  // (i.e. non bidirectional  => target_color_center +  target_color_delta)
  // Base colors for the texture
  // Color vector for scattering
  // target_color = target_color_center +- target_color_delta

  scatter = <-1,1> or <0,1>
  param   = parameter from previous modificator

  color = color (OP) ( color_center (OP) color_delta*scatter )

  Number of color ops. it 10?

  ....
  color = color (OP) ( color_param  (OP) color_param_delta*param )
  ....
  

  texture = color (OP) texture
*/

/*
 * Parameters transfered between master and slave modificators
 */
typedef class stack<float> MODIFICATOR_PARAMS;

typedef struct pixel_params {
  
  MODIFICATOR_PARAMS *p_params;
  
public:
  
  pixel_params(MODIFICATOR_PARAMS *p_par);

} PIXEL_PARAMS;

class parametrizer;
typedef class parametrizer PARAMETRIZER;

/*
  This structure contais pointer to a top of parsed tree for one line from
  an input file.
*/
typedef struct line_tree
  : public llist_item
{   
  PARAM_NODE_LEAF *p_top;
    
  PARAMETRIZER    *p_param;
  char            *p_source_line;
  int              source_line_number;
  
public:

  void clear(void)
  {
    if(p_top) {
      param_tree_release(p_top);
      p_top = NULL;
    }
  }

  void print(void)
  { 
    if(p_top) {
      print_tree(p_top, 0);
    }
  }
  
  bool execute(PIXEL_PARAMS *p_pixel);

private:  
  
  VARIABLE_NODE   variable_node_add_float(float v1, float v2);
  VARIABLE_NODE   variable_node_add_color(RGBAF v1, RGBAF v2);
  VARIABLE_NODE   variable_node_add(VARIABLE_NODE ch1, VARIABLE_NODE ch2);
  VARIABLE_NODE   variable_node_sub_float(float v1, float v2);
  VARIABLE_NODE   variable_node_sub_color(RGBAF v1, RGBAF v2);
  VARIABLE_NODE   variable_node_sub(VARIABLE_NODE ch1, VARIABLE_NODE ch2);
  VARIABLE_NODE   variable_node_mult_float(float v1, float v2);
  VARIABLE_NODE   variable_node_mult_color(RGBAF v1, RGBAF v2);
  VARIABLE_NODE   variable_node_mult_color_float(RGBAF v1, float v2);
  VARIABLE_NODE   variable_node_mult(VARIABLE_NODE ch1, VARIABLE_NODE ch2);
  VARIABLE_NODE   variable_node_div_float(float v1, float v2);
  VARIABLE_NODE   variable_node_div_color(RGBAF v1, RGBAF v2);
  VARIABLE_NODE   variable_node_div_color_float(RGBAF v1, float v2);
  VARIABLE_NODE   variable_node_div(VARIABLE_NODE ch1, VARIABLE_NODE ch2);
  bool            variable_node_convert_float(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result);
  bool            variable_node_convert_int(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result);
  bool            variable_node_convert_color(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result);
  bool            variable_node_convert(VARIABLE_NODE ch, VARIABLE_TYPE target, VARIABLE_NODE *p_result);
  float           variable_content_get(VARIABLE_NODE *p_var, NODE_CONTENT_TYPE content_type);
  void            variable_content_set(VARIABLE_NODE *p_var, NODE_CONTENT_TYPE content_type, float val);

private:
    
  VARIABLE_NODE   execute_leaf(PARAM_NODE_LEAF *p_leaf, PIXEL_PARAMS *p_params);
  
private:

  void ppline(void);

public:
  
  line_tree(PARAMETRIZER *p_par, PARAM_NODE_LEAF *p_tree, char *p_line, int line_number)
    : p_top(NULL),
      p_param(p_par),
      p_source_line(NULL),
      source_line_number(0)
  {
    p_top = p_tree;
    
    if(p_line)
      p_source_line = strdup(p_line);
    
    source_line_number = line_number;
  }
  
  ~line_tree(void)
  {
    if(p_source_line) {
      free(p_source_line);
      p_source_line = NULL;
    }
  }
  
} LINE_TREE;

typedef class parametrizer {
  
  // List of trees for all parsed lines
  LLIST_HEAD          lines;
  SYMBOL_TABLE       *p_table;

  // Source filename
  char                filename[MAX_FILENAME];
  
public:

  void parse_file_set(char *p_name)
  {
    strncpy(filename, p_name, MAX_FILENAME);
  }
  char * parse_file_get(void)
  {
    return(filename);
  }
  
public:  
  
  bool loaded(void)
  {
    return(!lines.is_empty());
  }  
  
  void clear(void);
  void print(void);
  
public:  
  
  bool parse_line(DATA_FILE_SECTION *p_line);
  bool execute(PIXEL_PARAMS *p_params);
  
private:
  
  int  tokens_get(DATA_FILE_SECTION *p_line, array<PARAM_NODE> &nodes);

public:

  parametrizer(SYMBOL_TABLE *p_symbol_table)
  {   
    p_table = p_symbol_table;    
    filename[0] = '\0';
  }

} PARAMETRIZER;

void parser_nodes_get(array<PARAM_NODE> &nodes);

#ifdef __cplusplus
extern "C" {
#endif

void print_node(PARAM_NODE *p_node, int ident);
void print_tree(PARAM_NODE_LEAF *p_tree, int ident);

#ifdef __cplusplus
}
#endif

#endif // __GENERATOR_MODIFICATOR_PARAMS_H__
