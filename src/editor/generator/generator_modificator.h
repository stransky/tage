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

#ifndef __GENERATOR_MODIFICATOR_H__
#define __GENERATOR_MODIFICATOR_H__

/*
 * Types of modificators
 */
typedef enum {
  
  MODIFICATOR_NONE = 0,

  // Global generator launcher
  GENERATOR_LAUNCHER_TYPE,
    
  // Basic object parameters
  GENERATOR_MESH_TYPE,
  
  // Targets & settings
  MESH_PARAMETERS_TYPE,
  MATERIAL_PARAMETERS_TYPE,
  TEXTURE_PARAMETERS_TYPE,  
  AUX_PARAMETERS_TYPE,
    
  // Modificators  
  MODIFICATOR_GENERATOR_MESH_TYPE,
  MODIFICATOR_COORDINATE_TYPE,
  MODIFICATOR_MASK_TYPE,
  MODIFICATOR_POINT_SINGLE_TYPE,
    MODIFICATOR_POINT_OPBOX_TYPE,
    MODIFICATOR_POINT_OPBOX_INIT_TYPE,
  MODIFICATOR_POINT_EXTENDED_TYPE,
  MODIFICATOR_RECTANGLE_TYPE,
  MODIFICATOR_LINE_TYPE,
  MODIFICATOR_LINE_LEAF_TYPE,
  MODIFICATOR_BUNCH_TYPE,
  MODIFICATOR_BRICK_TYPE,
  MODIFICATOR_HEIGHTMAP_TYPE,
  MODIFICATOR_BITMAP_TYPE,
  MODIFICATOR_FRACTAL_TYPE,
  MODIFICATOR_PERLIN_TYPE,
  MODIFICATOR_CRACK_TYPE,
  MODIFICATOR_LIGHT_TYPE,
  MODIFICATOR_FILTER_TYPE,
  MODIFICATOR_FROM_TARGET_TYPE,
  
} MODIFICATOR_TYPE;

/*
 * Types of modificators
 */
typedef class modificator_type_item
{
  
public:
  
  MODIFICATOR_TYPE  modificator_type;
  
public:

  MODIFICATOR_TYPE  modificator_type_get(void)
  {
    return(modificator_type);
  }

  const char * modificator_type_name_get(void)
  {
    return(type_translate(modificator_type));
  }
  
  const char * modificator_name_get(void)
  {
    return(type_translate_name(modificator_type));
  }
  
public:
  
  static MODIFICATOR_TYPE  type_translate(const char *p_type);
  static const char *      type_translate(MODIFICATOR_TYPE type);
  static const char *      type_translate_name(MODIFICATOR_TYPE type);

public:  
  
  modificator_type_item(MODIFICATOR_TYPE type)
  {
    modificator_type = type;
  }
    
} MODIFICATOR_TYPE_ITEM;

// Load helpers
ARITMETIC_OPERATOR      aritmetic_operation_translate(const char *p_type);
RANDOM_GENERATOR_TYPE   random_generator_type_translate(const char *p_type);
COORD_MODIFICATOR       coord_modificator_translate(const char *p_type);

/*
 * Mask reference
 *
 * This class acts like an interface to MODIFICATOR_MASK generator
 * and allows any other generators to access to MODIFICATOR_MASK modificator
 */
typedef class modificator_mask_reference
  : public mask_reference
{
  // This pattern "owns" the slave_mask -> 
  // it creates it for each coordinate and removes
  // when it's finished
  MODIFICATOR_MASK   *p_mask_slave;
  
public:  
  
  void mask_slave_set(MODIFICATOR_MASK *p_mask)
  {
    this->p_mask_slave = p_mask;
  }
    
  void mask_slave_remove(void);
  void mask_slave_create(tpos dx, tpos dy);

public:

  bool load(DATA_FILE_SECTION *p_section, GENERATOR *p_generator);
  
public:

  modificator_mask_reference(void)
  {
    p_mask_slave = NULL;
  }
  
} MODIFICATOR_MASK_REFERENCE;

/*
  Base class for all patterns
  
  Interface:

    coordinates_set(x,y);
    apply(MODIFICATOR_PARAMS);

*/
typedef class modificator : 
  public generator_reference,
  public modificator_type_item,
  public modificator_mask_reference,
  public object_list
{
  
  MODIFICATOR_COORDINATES  coordinates;
  
public:
  
  // Return current coordinates
  MODIFICATOR_COORDINATES * coordinates_get(void)
  {
    return(&coordinates);
  }  
  MODIFICATOR_COORDINATES * coordinates_local_set(RECT2DI area)
  {
    coordinates.coord_local_fixed_set(area,0);
    return(&coordinates);
  }    
  MODIFICATOR_COORDINATES * coordinates_top_set(RECT2DI area)
  {
    coordinates.coord_top_set(area);
    return(&coordinates);
  }    
  
  /*
   * Slave modificators
   */
private:

  #define MODIFICATOR_SLAVE_NUM   5

  // Type of the sub-pattern (what this pattent emits)
  class modificator * p_modificator_slave[MODIFICATOR_SLAVE_NUM];
      
public:
  
  class modificator * modificator_slave_get(int num = 0)
  {
    assert(num < MODIFICATOR_SLAVE_NUM);
    return(p_modificator_slave[num]);
  }

  class modificator * modificator_slave_add(class modificator *p_src)
  {
    int i;
    for(i = 0; i < MODIFICATOR_SLAVE_NUM; i++) {
      if(!p_modificator_slave[i]) {
        p_modificator_slave[i] = p_src;
        return(p_modificator_slave[i]);
      }
    }
    return(FALSE);
  }
  
public:
  
  bool target_apply(MODIFICATOR_TARGET_STACK *p_target, 
                    RECT2DI pos, MODIFICATOR_PARAMS *p_params)
  {
    bool ret = FALSE;
    
    if(mask_get(pos)) {    
      int i;
      for(i = 0; i < MODIFICATOR_SLAVE_NUM; i++) {
        if(p_modificator_slave[i]) {
          p_modificator_slave[i]->coordinates.coord_top_set(pos);
          if(p_modificator_slave[i]->apply(p_target, p_params)) {
            ret = TRUE;
          }
        }
      }
    }
    
    return(ret);
  }  
 
  bool target_init(MODIFICATOR_TARGET_STACK *p_target, 
                   RECT2DI pos, MODIFICATOR_PARAMS *p_params)
  {
    int i;
    for(i = 0; i < MODIFICATOR_SLAVE_NUM; i++) {
      if(p_modificator_slave[i]) {        
         p_modificator_slave[i]->init(p_target, pos, p_params);
      }
    }
    return(TRUE);
  }  

private:
  
  #define MODIFICATOR_PRE_NUM     5
  #define MODIFICATOR_POST_NUM    5
  
  class modificator * p_modificator_pre[MODIFICATOR_PRE_NUM];
  class modificator * p_modificator_post[MODIFICATOR_POST_NUM];

public:
  
  class modificator * modificator_pre_get(int num = 0)
  {
    assert(num < MODIFICATOR_PRE_NUM);
    return(p_modificator_pre[num]);
  }

  class modificator * modificator_pre_add(class modificator *p_src, int num = 0)
  {
    int i;
    for(i = 0; i < MODIFICATOR_PRE_NUM; i++) {
      if(!p_modificator_pre[i]) {
        p_modificator_pre[i] = p_src;
        return(p_modificator_pre[i]);
      }
    }
    return(FALSE);
  }

  class modificator * modificator_post_get(int num = 0)
  {
    assert(num < MODIFICATOR_POST_NUM);
    return(p_modificator_post[num]);
  }

  class modificator * modificator_post_add(class modificator *p_src, int num = 0)
  {
    int i;
    for(i = 0; i < MODIFICATOR_POST_NUM; i++) {
      if(!p_modificator_post[i]) {
        p_modificator_post[i] = p_src;
        return(p_modificator_post[i]);
      }
    }
    return(FALSE);
  }
  
public:

  bool target_pre_apply(MODIFICATOR_TARGET_STACK *p_target, 
                        RECT2DI pos, MODIFICATOR_PARAMS *p_params)
  {
    // Create slave mask is there's any
    modificator_mask_reference::mask_slave_create(pos.dx, pos.dy);

    // Call all pre modificators
    bool ret = FALSE;
    
    int i;
    for(i = 0; i < MODIFICATOR_PRE_NUM; i++) {
      if(p_modificator_pre[i]) {
        p_modificator_pre[i]->init(p_target, pos, p_params);
        p_modificator_pre[i]->coordinates.coord_top_set(pos);
        if(p_modificator_pre[i]->apply(p_target, p_params)) {
          ret = TRUE;
        }
      }
    }
    
    return(ret);
  }

  bool target_post_apply(MODIFICATOR_TARGET_STACK *p_target,
                         RECT2DI pos, MODIFICATOR_PARAMS *p_params)
  {
    // Call all post modificators
    bool ret = FALSE;
    
    int i;
    for(i = 0; i < MODIFICATOR_POST_NUM; i++) {
      if(p_modificator_post[i]) {
        p_modificator_post[i]->init(p_target, pos, p_params);
        p_modificator_post[i]->coordinates.coord_top_set(pos);
        if(p_modificator_post[i]->apply(p_target, p_params)) {
          ret = TRUE;
        }
      }
    }
    
    // Remove slave mask if there's any
    modificator_mask_reference::mask_slave_remove();
    
    return(ret);
  }
  
public:
  
  virtual bool  init(MODIFICATOR_TARGET_STACK *p_target, 
                     RECT2DI pos, MODIFICATOR_PARAMS *p_params)
  {
    // Create mask (if requested)
    modificator_mask_reference::mask_create(pos);
    
    // Set mask_size in modificator_mask_reference
    modificator_mask_reference::mask_size_set(pos);
    
    return(TRUE);
  }
  
  virtual bool  apply(MODIFICATOR_TARGET_STACK *p_target, 
                      MODIFICATOR_PARAMS *p_params) = 0;

public:
  
  bool          load_coord(DATA_FILE_SECTION *p_section, COORDINATE *p_coord, int *p_index);
  virtual void  load(DATA_FILE_SECTION *p_section);
  
public:
  
  modificator(GENERATOR *p_gen, MODIFICATOR_TYPE type);
  virtual ~modificator(void);

} MODIFICATOR;

/*
*/
typedef class modificator_pixel_pattern :
  public modificator
{
  
public:  
  // The target area is inverted (pixels are filled oudside)
  bool                        area_inverted;

  // If target_pixel_size > 1 pixels are emited
  // as square area around the pixel center
  int                         pixel_size;

  // Pixels are incremented by this step
  // if pixel_step > 1, pixels are emitted in grid (unless is random)
  int                         pixel_step_x;
  int                         pixel_step_y;

  // Random pixel steps  
  bool                        pixel_step_random;
  int                         pixel_step_random_min;
  int                         pixel_step_random_max;

  // get the pixel step
  void pixel_step_set(int step);
  int  pixel_step_random_get(void);
  int  pixel_step_x_get(void);
  int  pixel_step_y_get(void);
  int  pixel_step_get(void);
    
  /*
   * How "solid" is the color shape?
   * 0 - minimal density (no filled)
   * 1 - maximal density (filled completely)
   */  
  // (0...1) -> density of color
  float                       pixel_color_density;
  
  void pixel_density_set(float density);
  bool pixel_density_get(void);

  /*
   * Area fading
   */  
  // Is the area probability faded?  
  bool                        probability_fade;
  float                       probability_fade_start; // floats from 0...1
  float                       probability_fade_stop;  // floats from 0...1

  // Calculate if the pixel is rejected because of the pixel fade
  bool probability_pixel_fade_get(float pixel_distance, float pixel_distance_max);

  // Is the area faded by color?
  bool                        color_fade;
  float                       color_fade_start; // floats from 0...1
  float                       color_fade_stop;  // floats from 0...1

  // Calculate if a pixel color is modified because of the pixel fade
  float color_pixel_fade_get(float pixel_distance, float pixel_distance_max);
  
  /*
   * Area border erosion
   */
  // Size of the erosion
  float                       erode_border;
  
  // If target_pixel_size, pixels can be "eroded" on the borders
  // 1 = no erosion
  // 0 = maximal erosion
  float                       erode_factor;
  
  // Calculate pixel erosion
  bool pixel_erosion_get(float pixel_distance, float pixel_distance_max);
  
  // Size variator
  // Theshold for the size variation (0 - minimal probability)
  //                                 (1 - maximal probability)
  float                       size_variator_theshold;
  // Variator factor -> how big is the single change
  float                       size_variator_factor;

  // Calculate new size
  float size_variation_get(float pixel_distance_max);


public:

  virtual bool apply(MODIFICATOR_TARGET_STACK *p_target, 
                     MODIFICATOR_PARAMS *p_params) = 0;

public:

  void  load(DATA_FILE_SECTION *p_section);
  
public:

  modificator_pixel_pattern(GENERATOR *p_gen, MODIFICATOR_TYPE type);
  virtual ~modificator_pixel_pattern(void);

} MODIFICATOR_PIXEL_PATTERN;

/*
 * Generates color-table from given bitmap
 */
typedef class modificator_color_table
{ 
  HISTOGRAM *p_hist;
 
public:
  
  /*
   * Returns color based on loaded histogram
   * index is <0,1>
   */
  RGBB  color_get(float index)
  {
    if(p_hist) {
      return(p_hist->color_get(index));
    }
    return(RGBB(0));
  }

  /*
   * Look-up near color
   */
  RGBB  color_get(RGBB color)
  {
    if(p_hist) {
      return(p_hist->color_get(color));
    }
    return(RGBB(0));
  }
  
public:

  bool loaded(void)
  {
    return(p_hist != NULL);
  }
   
  void clear(void);
  bool load(char *p_name);

public:
  
  modificator_color_table(void);
  ~modificator_color_table(void);

} MODIFICATOR_COLOR_TABLE;

/*
typedef struct tbox_color
  : public llist_item
{
  BLEND_OPERATION             color_op;
  PARAMETRIZER                color_blend_param;
    
  RGBAF                       color_center;
  float                       color_center_scale;
  RGBAF                       color_center_delta;
  PARAMETRIZER                color_center_parameter;
    
  // For static colors:
  // color_center_current = (color_center*color_center_scale)+
  //                        (color_center_delta*color_center_parameter)
  // For color table:
  // color_center_current = (color_table.get(color_center_delta*color_center_parameter))  
  RGBAF                       color_center_current;
  
  RGBAF                       color_delta;
  float                       color_delta_scale;
  PARAMETRIZER                color_delta_parameter;
  
  // For static colors:
  // color_final =          (color_center_current)+
  //                        (color_delta*color_delta_scale*color_delta_parameter)
  // For color table:
  // color_final =          (color_center_current)+
  //                        color_table.get(delta*color_delta_scale*color_delta_parameter)
    
  // Final color operation
public:  
  
  BLEND_OPERATION             final_op;
  PARAMETRIZER                final_blend_param;
  
public:

  void  init(float scatter, MODIFICATOR_PARAMS *p_params, 
             MODIFICATOR_COLOR_TABLE *p_table);

  RGBAF apply(RGBAF top, float scatter, MODIFICATOR_PARAMS *p_params,
              MODIFICATOR_COLOR_TABLE *p_table);

public:
  
  void load(DATA_FILE_SECTION *p_section);

public:
  
  tbox_color(void);

} TBOX_COLOR;

typedef struct tbox_height
  : public llist_item
{
  
  BLEND_OPERATION             final_op;
  BLEND_OPERATION             height_op;
  
  float                       height_center;
  float                       height_delta;
  
  PARAMETRIZER                height_parameter;
  
public:

  float apply(float top, float scatter, MODIFICATOR_PARAMS *p_params);
  
public:

  void load(DATA_FILE_SECTION *p_section);

public:

  tbox_height(void);

} TBOX_HEIGHT;
*/

// Table of variables, we support two types:
// RGBAF and floats
/*
  RGBAF                       color_delta;
  float                       color_delta_scale;
  PARAMETRIZER                color_delta_parameter;
*/

typedef struct operation_box
  : public llist_item
{
  // shared symbol table
  SYMBOL_TABLE stable;
  
  // parsed tree for init section
  PARAMETRIZER params_init;

  // parsed tree for basic section
  PARAMETRIZER params;  

public:

  void init(MODIFICATOR_PARAMS *p_params, 
            RECT2DI pos, RECT2DI target_area,
            MODIFICATOR_COLOR_TABLE *p_table);

  void apply(RGBAF *p_color, float *p_height, VECT3DF *p_normal,
             int targets, RECT2DI pos,
             MODIFICATOR_PARAMS *p_params,
             MODIFICATOR_COLOR_TABLE *p_table);

public:

  void load_init(DATA_FILE_SECTION *p_section);
  void load(DATA_FILE_SECTION *p_section);

public:

  operation_box(void);

} OPERATION_BOX;

typedef class modificator_point_single :
  public modificator_pixel_pattern  
{  
  
public:

  /*
   * Color borders
   */
  bool                        color_borders;
  RGBAF                       color_border_min;
  RGBAF                       color_border_max;
  
  /*
   * Colors for the texture
   */
private:
  
  MODIFICATOR_COLOR_TABLE     color_table;
  MODIFICATOR_COLOR_TABLE     color_table_center;
  MODIFICATOR_COLOR_TABLE     color_table_delta;
  
  /*
   * Op boxes
   */
  LLIST_HEAD                  operation_box_list;

public:
  
  OPERATION_BOX             * operation_box_create(void);
  void                        operation_box_remove(OPERATION_BOX *p_box);
  OPERATION_BOX             * operation_box_get_first(void);
  
private:
  
  bool target_apply(MODIFICATOR_TARGET *p_target, RECT2DI pos,
                    MODIFICATOR_PARAMS *p_params);
  bool target_apply(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, 
                    MODIFICATOR_PARAMS *p_params);
  bool target_apply_pixel(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, 
                          MODIFICATOR_PARAMS *p_params);

private:

  bool init(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params);

  void apply(RGBAF *p_color, float *p_height, VECT3DF *p_normal, int targets,
             RECT2DI pos, MODIFICATOR_PARAMS *p_params);
  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);  

public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  modificator_point_single(GENERATOR *p_gen);
  virtual ~modificator_point_single(void);

} MODIFICATOR_POINT_SINGLE;

/*
  extended pixel pattern:
  COLOR_PATTERN_POINT_EXTENDED
*/
typedef class modificator_point_extended : 
  public modificator_pixel_pattern
{

  /*
   * Point is drawn in center
   */
  bool point_center;
  
  /*
   * Apply it!
   */
private:
  
  bool apply_pixel(MODIFICATOR_TARGET_STACK *p_target, VECT2DI *p_center,
                   float pixel_distance_max, int x, int y,
                   float *p_size_variator,
                   MODIFICATOR_PARAMS *p_params);

public:

  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  modificator_point_extended(GENERATOR *p_gen);
  virtual ~modificator_point_extended(void);

} MODIFICATOR_POINT_EXTENDED;

/*
  area pattern:
  MODIFICATOR_COLOR_PATTERN_RECTANGLE
*/
typedef class modificator_rectangle :
  public modificator_pixel_pattern
{  
  
  /*
   * Soft corners
   */
  bool          soft_corners;
    
  /*
   * Apply it!
   */
private:

  bool apply_pixel(MODIFICATOR_TARGET_STACK *p_target, tpos x, tpos y, int pixel_distance, int pixel_distance_max, MODIFICATOR_PARAMS *p_params);
  bool apply_rect_y(MODIFICATOR_TARGET_STACK *p_target, int sy, int ey, int cx, int x_direction, int border_size, int *p_size_array, MODIFICATOR_PARAMS *p_params);
  bool apply_rect_x(MODIFICATOR_TARGET_STACK *p_target, int sx, int ex, int cy, int y_direction, int border_size, int *p_size_array, MODIFICATOR_PARAMS *p_params);
  bool apply_rect(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params, int direction);
  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:  
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  modificator_rectangle(GENERATOR *p_gen);
  virtual ~modificator_rectangle(void);

} MODIFICATOR_RECTANGLE;

/*
  area pattern:
  MODIFICATOR_COLOR_PATTERN_LINE
*/
typedef class modificator_line : 
  public modificator_pixel_pattern
{   
  // line size
  int     line_width;
  
  // draw smooth ends
  bool    line_smooth_end;
  
  /*
   * Apply it!
   */
private:
  
  void    line_emit(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, 
                    MODIFICATOR_PARAMS *p_params);
  void    line_emit_width(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, 
                          MODIFICATOR_PARAMS *p_params, int line_size);
  
  bool    apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:  
  
  void    load(DATA_FILE_SECTION *p_section);

public:

  modificator_line(GENERATOR *p_gen);
  virtual ~modificator_line(void);

} MODIFICATOR_LINE;

/*
  helper - poly modificator
  MODIFICATOR_POLY
*/
typedef enum
{
  POLY_PROFILE_ELLIPSE = 0,
  POLY_PROFILE_PYRAMID,
  
} MODIFICATOR_POLY_PROFILE;

typedef class modificator_poly : 
  public modificator_pixel_pattern
{

public:
  
   // Height profile
  MODIFICATOR_POLY_PROFILE  height_profile;
  float                     height_maximal;
  
  // corner_curvature, 
  // <0,1>, 0 - sharp, 1 - round
  interval<float>           corner_curvature;
  
  // Poly height modification (from <0,1> ranges)
  interval<float>           height_center;
  interval<float>           height_left;
  interval<float>           height_top;

  // Borders
  bool                      border;
  
private:

  // bitmask
  SURFACE_MASK              mask;
  VECT2DI                   mask_shift;
 
private:
  
  bool    poly_pixel_mask_border_get(VECT2DI pixel);
  float   poly_pixel_distance_get(VECT2DI pixel, VECT2DI center, int max_dist);
  float   poly_pixel_height_plane_get(PLANE p, VECT2DI pixel, RECT2DI pos, float height);
  
public:

  void    poly_fill(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, 
                    MODIFICATOR_PARAMS *p_params, float height_maximal);
  void    poly_fill_mask(RECT2DI pos);

  void    poly_line_coord(RECT2DI *p_coord,
                          VECT2DI p0, VECT2DI p1,
                          VECT2DI p2, VECT2DI p3);
  void    poly_line(MODIFICATOR_TARGET_STACK *p_target,
                    MODIFICATOR_PARAMS *p_params,
                    VECT2DI p0, VECT2DI p1,
                    VECT2DI p2, VECT2DI p3);
  void    poly_line(VECT2DI p0, VECT2DI p1,
                    VECT2DI p2, VECT2DI p3);

public:
  
  void poly_mask_set(tpos x, tpos y)
  {    
    mask.pixel_set(x-mask_shift.x, y-mask_shift.y, TRUE);
  }
  bool poly_mask_get(tpos x, tpos y)
  {
    return(mask.pixel_get(x-mask_shift.x, y-mask_shift.y));
  }

  bool poly_mask_loaded(void)
  {
    return(mask.loaded());
  }
  
public:
  
  void    poly_init(RECT2DI pos);
  void    poly_clear(void);
  
public:
  
  modificator_poly(GENERATOR *p_gen, MODIFICATOR_TYPE type);

} MODIFICATOR_POLY;

/*
  area pattern:
  MODIFICATOR_COLOR_PATTERN_LINE_LEAF
*/
typedef class modificator_line_leaf : 
  public modificator_poly
{
  
public:
  
  // start and stop of the leaf
  // It's relative to leaf length, from 0 to 1
  interval<float> leaf_start;
  interval<float> leaf_stop;
    
  // Width of the leaf (start, end)
  // It's relative to leaf length, from 0 to 1
  interval<float> leaf_width_start;
  interval<float> leaf_width_end;
  
  // Angle of leaf threads (lines)
  float           leaf_thread_angle;

  // Fill the leaf area by solid pattern
  bool            leaf_fill;

  /*
   * Apply it!
   */
private:
  
  void    line_emit(MODIFICATOR_TARGET_STACK *p_target, 
                    VECT2DI start, VECT2DI end,
                    MODIFICATOR_PARAMS *p_params);
  void    apply_leaf_line(MODIFICATOR_TARGET_STACK *p_target, 
                          MODIFICATOR_PARAMS *p_params, VECT2DI start, 
                          VECT2DF leaf_dot_direction_l,
                          VECT2DF leaf_dot_direction_r,
                          float maximal_width,
                          float leaf_position);

  bool    apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:  
  
  void    load(DATA_FILE_SECTION *p_section);

public:

  modificator_line_leaf(GENERATOR *p_gen);
  virtual ~modificator_line_leaf(void);

} MODIFICATOR_LINE_LEAF;

/*
  area pattern:
  MODIFICATOR_COLOR_PATTERN_BUNCH
*/
typedef class modificator_bunch : 
  public modificator_poly
{ 
  // Number of bunch border points
  interval<int>   bunch_points;
  
  // Distance of each point from bunch center
  interval<float> bunch_length;
  
  // Height of whole bunch
  // it's a scale factor
  interval<float> bunch_height;
  
  // Randomize angle netween points?
  float           bunch_angle;
  
  /*
   * Apply it!
   */
private:

  void apply_bunch(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos,
                   MODIFICATOR_PARAMS *p_params);
  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  modificator_bunch(GENERATOR *p_gen);
  virtual ~modificator_bunch(void);

} MODIFICATOR_BUNCH;

/*
  area pattern:
  MODIFICATOR_COLOR_PATTERN_NET

  This modificator generates net of lines...
*/
typedef class modificator_brick :
  public modificator_pixel_pattern
{

public:  
  
  /*
   * Brick sizes
   */  
  int     brick_width;
  int     brick_height;
  
  float   brick_width_scatter;
  float   brick_height_scatter;

  /*
   * Brick configuration - max brick sizes
   */
  int     brick_width_max;
  int     brick_height_max;
  
  /*
   * Brick configuration - size configuration
   */
  int     brick_corners;

  // Zipped bricks
  bool    brick_width_zip;
  bool    brick_height_zip;

  // Bricks join probabilities
  float   brick_width_join_pobability;
  float   brick_width_join_pobability_multiplier;
  
  float   brick_height_join_pobability;
  float   brick_height_join_pobability_multiplier;  
  
  /*
   * Apply it!
   */
private:

  void    apply_net(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params);
  bool    apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:  
  
  void    load(DATA_FILE_SECTION *p_section);

public:

  modificator_brick(GENERATOR *p_gen);
  virtual ~modificator_brick(void);

} MODIFICATOR_BRICK;

/*
 * Heightmap Filter - uses a bitmap for the height filtering
 *
 * Heightmap are applied as:
 * 
 * final_height = (color * color_multiplier + color_shift) HEIGHT_OPERATION current_height
 *
 * Where:
 *
 *    final_height      - final vertex height
 *    current_height    - current pixel height
 *    color             - bitmap color (from range 0...1)
 *    color_multiplier  - a constant (default 1)
 *    color_shift       - a constant (default 0)
 *    HEIGHT_OPERATION  - given operation from HEIGHT_OPERATOR class
 *                        (HEIGHT_SET is the default one)
 *
 *    If color mask is active, vertex with color from 
 *    color_mask_low - color_mask_top range are ignored.
 */
typedef struct modificator_height_map :
  public modificator_pixel_pattern,
  public phong_lighting_intensity
{
  /*
   *  Height map surface
   */
protected:  
  
  SURFACE_HEIGHT  heightmap;

public:
  
  SURFACE_HEIGHT * heightmap_get(void)
  {
    heightmap_create();
    return(&heightmap);
  }

protected:  

  /*
   *  Heightmap size (for created maps)
   */
  VECT2DI     heightmap_size;
  
  /*
   *  Mask file (for masks loaded from files)
   */  
  char        heightmap_file[MAX_FILENAME];

  /*
   *  For heightmaps derived from targets
   */
  MODIFICATOR_TARGET_TYPE   target_type;
  char                      target_name[MAX_NAME];
  
public:  
  
  // Height publication ranges
  float height_multiplier;
  float height_shift;

  float height_range_min;
  float height_range_max;

  // Intensity publication ranges
  float intensity_multiplier;
  float intensity_shift;

  float intensity_range_min;
  float intensity_range_max;

public:

  bool  height_match(float height);
  float height_translate(float height);

  bool  intensity_match(float intensity);
  float intensity_translate(float intensity);

public:

  // Scale heightmap?
  bool  heightmap_scale;

  // Scale heightmap to target
  bool  heightmap_scale_target;

  // Scale heightmap to given size
  tpos  height_scale_width;
  tpos  height_scale_height;

public:

  float height_get(tpos x, tpos y);
  bool  height_set(tpos x, tpos y, float height);
  
public:
  
  // Mask is waiting for load
  bool  heightmap_load_waiting(void);

  // Is heighmap loaded?
  bool  heightmap_loaded(void);

  // Load heighmap from file
  void  heightmap_load(char *p_file);

  // Load heighmap from target
  void  heightmap_load(char *p_target, char *p_name);

  // Load heighmap from surface
  void  heightmap_load(SURFACE_HEIGHT *p_surf);

  // Create a new one
  void  heightmap_load(tpos width, tpos height);

protected:

  bool  heightmap_create(char *p_file);
  bool  heightmap_create(MODIFICATOR_TARGET_AUX *p_map);
  bool  heightmap_create(MODIFICATOR_TARGET_GEOMETRY *p_map);  
  bool  heightmap_create(MODIFICATOR_TARGET_TYPE type, char *p_name);
  bool  heightmap_create(SURFACE_HEIGHT *p_surf);
  bool  heightmap_create(tpos width, tpos height);

  // Scale heightmap to this target
  void  heightmap_target_scale(MODIFICATOR_TARGET *p_target);
  
  bool  heightmap_create(void);

public:
  
  void  heightmap_remove(void);

  // Remove mask if it was loaded from target
  void  heightmap_remove_target(void);

public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  bool  apply_heighmap(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params);
  bool  apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  modificator_height_map(GENERATOR *p_gen);
  ~modificator_height_map(void);

} MODIFICATOR_HEIGHT_MAP;

/*
 * Bitmap modificator
 */
typedef struct modificator_bitmap : 
  public modificator_pixel_pattern
{
  /*
   *  Terrain height map
   */
protected:  
  
  SURFACE_SW                bitmap;
  MODIFICATOR_TARGET_TYPE   target_type;
  char                      target_name[MAX_NAME];
  
public:

  // Load heighmap from file
  bool  bitmap_load(char *p_file);

  // Load heighmap from target
  bool  bitmap_load(char *p_target, char *p_name);

  // Load heighmap from surface
  bool  bitmap_load(SURFACE_SW *p_surf);

private:

  bool  bitmap_load(MODIFICATOR_TARGET_AUX *p_map);  
  bool  bitmap_load(MODIFICATOR_TARGET_TEXTURE *p_map);

  // Set-up it
  bool  bitmap_load(void);

public:

  void  bitmap_create(tpos width, tpos height);
  void  bitmap_destroy(void);

public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  bool  apply_bitmap(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params);
  bool  apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  modificator_bitmap(GENERATOR *p_gen);
  ~modificator_bitmap(void);

} MODIFICATOR_BITMAP;

/*
 * Height Fractal Filter - uses a fractal for the height filtering
 *
 * Height fractal are applied as:
 * 
 * final_height = random_height HEIGHT_OPERATION current_height
 *
 * Where:
 *
 *    final_height      - final vertex height
 *    current_height    - current pixel height 
 *    HEIGHT_OPERATION  - given operation from HEIGHT_OPERATOR class 
 *                        (HEIGHT_SET is the default one)
 *
 */

typedef class modificator_fractal :
  public modificator_height_map
{
  /*
   * Height ranges of the generated mesh
   */
public:
  
  MID_POINT_CONFIG  height_config;

public:

  /*
   * Fractals are generated only to heihtmaps
   */
public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  bool  apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  modificator_fractal(GENERATOR *p_gen);
  virtual ~modificator_fractal(void);

} MODIFICATOR_FRACTAL;

/*
 * Perlin noise generator - uses a perlin noise generator for the height filtering
 */
typedef class modificator_perlin :
  public modificator_height_map
{
  /*
   * Height ranges of the generated mesh
   */
public:
  
  PERLIN_CONFIG  pconfig;

public:

  /*
   * Perlin is generated only to heihtmaps
   */
public:
  
  void  load(DATA_FILE_SECTION *p_section);

public:

  bool  apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  modificator_perlin(GENERATOR *p_gen);
  virtual ~modificator_perlin(void);

} MODIFICATOR_PERLIN;

/*
  area pattern:
  MODIFICATOR_CRACK
*/
typedef enum
{
  CRACK_DEFAULT,
  CRACK_CENTER
  
} CRACK_TYPE;

typedef class modificator_crack :
  public modificator_pixel_pattern
{
  // Crack type
  CRACK_TYPE    type;
  
  // "Branches" of the crack
  interval<int> crack_branches;
  
  // Crack angle direction
  float         crack_angle;
  bool          crack_angle_random;
  
  
  // Direction deviation treshold
  float         direction_treshold;  
  // Direction deviation 
  float         direction_angle_range;
  
  /*
   * Apply it!
   */
private:
  
  void apply_crack_line(MODIFICATOR_TARGET_STACK *p_target,
                        VECT2DF start,
                        VECT2DF direction,
                        int length,
                        MODIFICATOR_PARAMS *p_params);
  void apply_crack(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params);
  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:  
  
  void load(DATA_FILE_SECTION *p_section);

public:

  modificator_crack(GENERATOR *p_gen);
  virtual ~modificator_crack(void);

} MODIFICATOR_CRACK;

/*
 * Modificator mask
 * This modificator acts like a mask - it contains all mask data (what area
 * should be masked)
 * it can be used as generator or referenced from any other generator
 */
typedef enum
{
  MASK_BOOL = 0,
  MASK_COLOR,
  MASK_HEIGHT
  
} MASK_TYPE;

typedef class modificator_mask :
  public modificator
{
  MASK_TYPE   mask_type;
  
  /*
   *  Mask surface
   */
  SURFACE    *p_mask;
    
  /*
   *  Mask size (for created mask)
   */
  VECT2DI     mask_size;
  
  /*
   *  Mask file (for masks loaded from files)
   */  
  char        mask_file[MAX_FILENAME];    
  
  /*
   *  For maps derived from targets
   */
protected:  
  
  MODIFICATOR_TARGET_TYPE target_type;
  char                    target_name[MAX_NAME];
  
public:  

  bool        mask_value_bool_true;
  bool        mask_value_bool_false;

  RGBB        mask_value_color_true;
  RGBB        mask_value_color_false;

  float       mask_value_height_true;
  float       mask_value_height_false;
  
public:
  
  bool        mask_type_set(MASK_TYPE type);
  MASK_TYPE   mask_type_get(void);
  
  RGBB        mask_color_get(VECT2DI pos);
  void        mask_color_set(VECT2DI pos, RGBB mask);

  bool        mask_bool_get(VECT2DI pos);
  void        mask_bool_set(VECT2DI pos, bool mask); 
  
  float       mask_height_get(VECT2DI pos);
  void        mask_height_set(VECT2DI pos, float mask);

  VECT3DF     mask_normal_get(VECT2DI pos);


public:
  // Fuzzy mask support - searches for near value in given range
  bool        mask_color_get(VECT2DI pos, RGBB mask, int range, float &result);
  bool        mask_bool_get(VECT2DI pos, bool mask, int range, float &result);

public:
  
  tpos        width_get(void);
  tpos        height_get(void);
  VECT2DI     mask_size_get(void);

private:
  
  void        mask_size_set(VECT2DI size);

public:

  // Delayed loading from diven source
  void        mask_load(const char *p_file);
  void        mask_load(char *p_target, char *p_name);
  void        mask_load(tpos width, tpos height);

  // Mask is already loaded
  bool        mask_loaded(void);

  // Mask is waiting for load
  bool        mask_load_waiting(void);

  // Load/create mask from given source
private:

  bool        mask_create(char *p_file);
  bool        mask_create(MODIFICATOR_TARGET_AUX *p_target);
  bool        mask_create(MODIFICATOR_TARGET_HEIGHT *p_target);
  bool        mask_create(MODIFICATOR_TARGET_TYPE type, char *p_name);
  bool        mask_create(tpos width, tpos height);

protected:

  // Try to create a mask from requested source (file/target/size)
  bool        mask_create(void);

public:
  
  void        mask_remove(void);
  // Remove mask if it was loaded from target
  void        mask_remove_target(void);

public:
  
  void        mask_clear(bool value);
  void        mask_shrink(int num);

public:

  void        load(DATA_FILE_SECTION *p_section);

public:

  // Init mask
  bool init(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos, MODIFICATOR_PARAMS *p_params)
  {
    return(TRUE);
  }

  // Apply to mask - boolean mask is used
  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);
  
public:
    
  modificator_mask(GENERATOR *p_gen);
  ~modificator_mask(void);
  
} MODIFICATOR_MASK;

/*
 * Modificator light
 */
typedef class modificator_light :
  public modificator,
  public phong_lighting
{ 
  // Relative light position (resp. from the texture)
  // <0,1> in all ranges
  VECT3DF light_position;
     
  // Are self-shadows enabled?
  bool    self_shadows;
  
  // Is it planar?
  bool    light_planar;
  
private:

  void apply_light(MODIFICATOR_TARGET *p_target, RECT2DI pos,
                   MODIFICATOR_PARAMS *p_params, 
                   SURFACE_HEIGHT *p_heightmap,
                   SURFACE_SW *p_bitmap);
  void apply_light(MODIFICATOR_TARGET *p_target, RECT2DI pos,
                   MODIFICATOR_PARAMS *p_params);
  void apply_light(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos,
                   MODIFICATOR_PARAMS *p_params);
public:

  void load(DATA_FILE_SECTION *p_section);

  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
    
  modificator_light(GENERATOR *p_gen);
  ~modificator_light(void);
  
} MODIFICATOR_LIGHT;

/*
 * Modificator filter
 */
typedef enum
{  
  FILTER_HEIGHT,
  FILTER_COLOR
  
} FILTER_TARGET;

typedef enum
{  
  FILTER_INTERPOLATE,
  FILTER_BLUR,
  
} FILTER_TYPE;

typedef float (* FILTER_KERNEL_HEIGHT)(float *p_values, int num);
typedef float (* FILTER_KERNEL_COLOR)(RGBB *p_values, int num);

typedef class modificator_filter :
  public modificator
{ 
  FILTER_TARGET   target;
  FILTER_TYPE     type;
  int             strength;
  
private:
  
  float filter_height_get(MODIFICATOR_TARGET *p_target, int x, int y)
  {
    float height = HEIGHT_UNSET;
    if(p_target->pixel_valid(x, y) && mask_get(x, y)) {
      height = p_target->height_get(x, y);
    }
    return(height);
  }

  int   filter_height_get(MODIFICATOR_TARGET *p_target, int x, int y, float *p_list_values);
  int   filter_color_get(MODIFICATOR_TARGET *p_target, int x, int y, RGBB *p_list_values);

public:

  void load(DATA_FILE_SECTION *p_section);

private:
  
  bool apply_height(MODIFICATOR_TARGET_STACK *p_target,
                    RECT2DI pos,
                    MODIFICATOR_PARAMS *p_params);

  bool apply_color(MODIFICATOR_TARGET_STACK *p_target,
                   RECT2DI pos,
                   MODIFICATOR_PARAMS *p_params);

public:
  
  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
    
  modificator_filter(GENERATOR *p_gen);
  ~modificator_filter(void);
  
} MODIFICATOR_FILTER;

/*
 * From target modificator
 *
 * Loads color/height from target (like AUX) 
 * and applies it to another target.
 */
typedef class modificator_from_target :
  public modificator_pixel_pattern
{   
  // Target name which is used as data source
  char    target_name[MAX_NAME];
  
private:  
  
  bool    apply_pixel(MODIFICATOR_TARGET_STACK *p_target, tpos dx, tpos dy,
                      MODIFICATOR_TARGET *p_source, tpos sx, tpos sy, 
                      MODIFICATOR_PARAMS *p_params);  
public:
  
  void  load(DATA_FILE_SECTION *p_section);
  bool  apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params);

public:
  
  modificator_from_target(GENERATOR *p_gen);
  virtual ~modificator_from_target(void);

} MODIFICATOR_FROM_TARGET;

/*
  extended pixel pattern:
  COLOR_PATTERN_POINT_EXTENDED
*/
typedef class modificator_generator_mesh :
  public modificator_pixel_pattern
{
  /*
   * Apply it!
   */
private:

  bool apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
  {
    return(TRUE);
  }

public:

  void  load(DATA_FILE_SECTION *p_section)
  {
  }

public:

  modificator_generator_mesh(GENERATOR *p_gen)
    : modificator_pixel_pattern(p_gen, MODIFICATOR_GENERATOR_MESH_TYPE)
  {
  }

  virtual ~modificator_generator_mesh(void)
  {
  }

} MODIFICATOR_GENERATOR_MESH;

/*
 * Base list of modificators
 */
typedef struct modificator_list :
  public object_list_head
{

  void clear(void)
  {
    object_list_clear();
  }
  
  modificator_list(void)
  : object_list_head(NULL)
  {
  }
    
} MODIFICATOR_LIST;

// New modificator
MODIFICATOR * modificator_new(MODIFICATOR_TYPE type);

// Helpers
ARITMETIC_OPERATOR aritmetic_operation_names_translate(const char *p_type);

#endif // __GENERATOR_MODIFICATOR_H__
