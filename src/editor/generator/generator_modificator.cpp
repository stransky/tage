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

#include "editor.h"

typedef struct {
  
  char              name[MAX_NAME];
  MODIFICATOR_TYPE  type;
  
} SECTION_NAME;

const SECTION_NAME generator_type_names[] = 
{
  {"MODIFICATOR_NONE",            MODIFICATOR_NONE},
  
  {"GENERATOR_LAUNCHER",          GENERATOR_LAUNCHER_TYPE},
    
  {"GENERATOR_MESH",              GENERATOR_MESH_TYPE},
  {"MESH_PARAMS",                 MESH_PARAMETERS_TYPE},
  {"MATERIAL_PARAMS",             MATERIAL_PARAMETERS_TYPE},
  {"TEXTURE_PARAMS",              TEXTURE_PARAMETERS_TYPE},
  {"AUX_PARAMETERS",              AUX_PARAMETERS_TYPE},
      
  //{"TEXTURE_LIGHT",TEXTURE_LIGHT},
    
  //{"MESH_MODIFICATOR_HEIGHTMAP",MESH_MODIFICATOR_HEIGHTMAP},
  //{"MESH_MODIFICATOR_FRACTAL",MESH_MODIFICATOR_FRACTAL},
  
  {"MODIFICATOR_GENERATOR_MESH",  MODIFICATOR_GENERATOR_MESH_TYPE},
  {"MODIFICATOR_COORDINATE",      MODIFICATOR_COORDINATE_TYPE},
  {"MODIFICATOR_MASK",            MODIFICATOR_MASK_TYPE},  
  {"MODIFICATOR_POINT_SINGLE",    MODIFICATOR_POINT_SINGLE_TYPE},
  {"MODIFICATOR_POINT_OPBOX",     MODIFICATOR_POINT_OPBOX_TYPE,},  
  {"MODIFICATOR_POINT_OPBOX_INIT",MODIFICATOR_POINT_OPBOX_INIT_TYPE,},
  {"MODIFICATOR_POINT_EXTENDED",  MODIFICATOR_POINT_EXTENDED_TYPE},
  {"MODIFICATOR_RECT",            MODIFICATOR_RECTANGLE_TYPE},
  {"MODIFICATOR_LINE",            MODIFICATOR_LINE_TYPE},
  {"MODIFICATOR_LINE_LEAF",       MODIFICATOR_LINE_LEAF_TYPE},
  {"MODIFICATOR_BUNCH",           MODIFICATOR_BUNCH_TYPE},
  {"MODIFICATOR_BRICK",           MODIFICATOR_BRICK_TYPE},
  {"MODIFICATOR_HEIGHT_MAP",      MODIFICATOR_HEIGHTMAP_TYPE},
  {"MODIFICATOR_BITMAP",          MODIFICATOR_BITMAP_TYPE},
  {"MODIFICATOR_FRACTAL",         MODIFICATOR_FRACTAL_TYPE},
  {"MODIFICATOR_CRACK",           MODIFICATOR_CRACK_TYPE},
  {"MODIFICATOR_LIGHT",           MODIFICATOR_LIGHT_TYPE},
  {"MODIFICATOR_FILTER",          MODIFICATOR_FILTER_TYPE},
  {"MODIFICATOR_PERLIN",          MODIFICATOR_PERLIN_TYPE},
  {"MODIFICATOR_TARGET",          MODIFICATOR_FROM_TARGET_TYPE},
};

MODIFICATOR_TYPE modificator_type_item::type_translate(const char *p_type)
{ 
  int i;
  for(i = 0; 
      i < (int)(sizeof(generator_type_names)/sizeof(generator_type_names[0])); 
      i++) 
  {
    if(!strncasecmp(p_type,generator_type_names[i].name,MAX_NAME))
      return(generator_type_names[i].type);
  }
  return(MODIFICATOR_NONE);
}

const char * modificator_type_item::type_translate(MODIFICATOR_TYPE type)
{
  int i;
  for(i = 0; 
      i < (int)(sizeof(generator_type_names)/sizeof(generator_type_names[0])); 
      i++) 
  {
    if(generator_type_names[i].type == type)
      return(generator_type_names[i].name);
  }
  
  return(generator_type_names[0].name);
}

const SECTION_NAME generator_names[] = 
{
  {"NONE",                        MODIFICATOR_NONE},
  
  {"GENERATOR_LAUNCHER",          GENERATOR_LAUNCHER_TYPE},
    
  {"MESH_PARAMS",                 MESH_PARAMETERS_TYPE},
  {"MESH_MATERIAL",               MATERIAL_PARAMETERS_TYPE},
  {"MESH_TEXTURE",                TEXTURE_PARAMETERS_TYPE},
  
  {"GENERATOR_MESH",              MODIFICATOR_GENERATOR_MESH_TYPE},
  {"COORDINATE",                  MODIFICATOR_COORDINATE_TYPE},
  {"POINT_SINGLE",                MODIFICATOR_POINT_SINGLE_TYPE},
  {"POINT_OPBOX",                 MODIFICATOR_POINT_OPBOX_TYPE},
  {"POINT_OPBOX_INIT",            MODIFICATOR_POINT_OPBOX_INIT_TYPE},
  {"POINT_EXTENDED",              MODIFICATOR_POINT_EXTENDED_TYPE},
  {"RECTANGLE",                   MODIFICATOR_RECTANGLE_TYPE},
  {"LINE",                        MODIFICATOR_LINE_TYPE},
  {"LINE_LEAF",                   MODIFICATOR_LINE_LEAF_TYPE},
  {"BUNCH",                       MODIFICATOR_BUNCH_TYPE},
  {"BRICK",                       MODIFICATOR_BRICK_TYPE},
  {"HEIGHTMAP",                   MODIFICATOR_HEIGHTMAP_TYPE},
  {"BITMAP",                      MODIFICATOR_BITMAP_TYPE},
  {"FRACTAL",                     MODIFICATOR_FRACTAL_TYPE},
  {"CRACK",                       MODIFICATOR_CRACK_TYPE},
  {"LIGHT",                       MODIFICATOR_LIGHT_TYPE},
  {"FILTER",                      MODIFICATOR_FILTER_TYPE},
  {"PERLIN",                      MODIFICATOR_PERLIN_TYPE},
  {"TARGET",                      MODIFICATOR_FROM_TARGET_TYPE},
};

const char * modificator_type_item::type_translate_name(MODIFICATOR_TYPE type)
{
  int i;
  for(i = 0; 
      i < (int)(sizeof(generator_names)/sizeof(generator_names[0])); 
      i++) 
  {
    if(generator_names[i].type == type)
      return(generator_names[i].name);
  }
  
  return(generator_names[0].name);
}

#define SIMPLE_SECTION_TRANSLATOR(name, type) \
                                              \
type name##_translate(const char *p_type)     \
{                                             \
  int i;                                      \
  for(i = 0;                                  \
      i < (int)(sizeof(name)/sizeof(name[0]));\
      i++)                                    \
  {                                           \
    if(!strncasecmp(p_type,name[i],MAX_NAME)) \
      return((type)i);                        \
  }                                           \
  return((type)0);                            \
}

#define SIMPLE_SECTION_TRANSLATOR_ERROR(name, type, error) \
                                              \
type name##_translate(const char *p_type, type def = error) \
{                                             \
  int i;                                      \
  for(i = 0;                                  \
      i < (int)(sizeof(name)/sizeof(name[0]));\
      i++)                                    \
  {                                           \
    if(!strncasecmp(p_type,name[i],MAX_NAME)) \
      return((type)i);                        \
  }                                           \
  return((type)def);                          \
}

const char aritmetic_operation_names[][MAX_NAME] = 
{
  "SET",
  "ADD",
  "SUB",
  "MODULATE",
  "MODULATE2X",
};

SIMPLE_SECTION_TRANSLATOR(aritmetic_operation_names, ARITMETIC_OPERATOR);

const char random_generator_type[][MAX_NAME] = 
{
  "GENERATOR_GAUSS",
  "GENERATOR_RAND"
};

SIMPLE_SECTION_TRANSLATOR(random_generator_type, RANDOM_GENERATOR_TYPE);

/*
 * Coordinate modificators
 */
const char coord_modificator[][MAX_NAME] = 
{
  "COORD_CURRENT",
  "COORD_LAST_START",
  "COORD_LAST_SIZE",
  "COORD_LAST_START_SIZE",
};

SIMPLE_SECTION_TRANSLATOR(coord_modificator, COORD_MODIFICATOR);

/*
 * Mask type
 */
const char mask_type[][MAX_NAME] = 
{
  "MASK_BOOL",
  "MASK_COLOR",
  "MASK_HEIGHT",
};

SIMPLE_SECTION_TRANSLATOR(mask_type, MASK_TYPE);

/*
 * Filter targets
 */
const char filter_target[][MAX_NAME] = 
{
  "HEIGHT",
  "COLOR"
};

SIMPLE_SECTION_TRANSLATOR(filter_target, FILTER_TARGET);

/*
 * Filter type
 */
const char filter_type[][MAX_NAME] = 
{
  "INTERPOLATE",
  "BLUR"
};

SIMPLE_SECTION_TRANSLATOR(filter_type, FILTER_TYPE);

/*
 * Interpolation type
 */
const char interpolation_type[][MAX_NAME] = 
{
  "MID_POINT",
  "LINE_MIN",
  "LINE_MAX",
  "LINE_CENTER",
  "LINE_PRIORITY_HIGH",
  "LINE_PRIORITY_LOW",
  "LINE_RANGE_HIGH",
  "LINE_RANGE_LOW",
  "LINE_RANDOM"
};

SIMPLE_SECTION_TRANSLATOR(interpolation_type, SQUARE_INTERPOLATION_TYPE);

/*
 * Target types
 */
const char target_type[][MAX_NAME] = 
{
  "TARGET_NONE",
  "TARGET_TEXTURE",
  "TARGET_GEOMETRY",
  "TARGET_GENERATOR",
  "TARGET_AUX",
  "TARGET_MASK",
};

SIMPLE_SECTION_TRANSLATOR_ERROR(target_type, MODIFICATOR_TARGET_TYPE, MODIFICATOR_TARGET_AUX_TYPE);

/*
 * Crack types
 */
const char crack_type[][MAX_NAME] = 
{
  "DEFAULT",
  "CENTER",
};

SIMPLE_SECTION_TRANSLATOR_ERROR(crack_type, CRACK_TYPE, CRACK_DEFAULT);

/*
 * Other helpers
 */
inline int size_interpolate(int size_start, int size_stop, int size_length, int position)
{
  return((int)(size_start+(size_stop - size_start)*((float)position/(float)size_length)));
}

inline float * size_array_float_variate(int size_start, int size_stop, int size_length, float theshold = 1.0f, float factor = 1.0f, float variator = 0.0f)
{
  float *p_array = (float *)mmalloc(sizeof(float)*size_length);
  
  int i;
  for(i = 0; i < size_length; i++) {
    p_array[i] = size_interpolate(size_start, size_stop, size_length, i);
    if(random_generator::generator_rand() > theshold)
      variator += random_generator::generator_gauss_0()*factor;
    p_array[i] -= fabs(variator);
  }
  
  return(p_array);
}

inline int * size_array_int_variate(int size_start, int size_stop, int size_length, float theshold = 0.0f, float factor = 1.0f, float variator = 0.0f)
{
  int *p_array = (int *)mmalloc(sizeof(int)*size_length);
  
  int i;
  for(i = 0; i < size_length; i++) {
    p_array[i] = size_interpolate(size_start, size_stop, size_length, i);
    if(random_generator::generator_rand() < theshold)
      variator += random_generator::generator_gauss_0()*factor;
    p_array[i] -= (int)(fabs(variator));
  }
  
  return(p_array);
}

/*
  start_index = 0
  start_x = 0
  start_y = 0
  
  center_index = 0
  center_x = 0
  center_y = 0
  center_dx = 0
  center_dy = 0
  
  size_index = 0
  size_x = 0
  size_y = 0
  
  end_index = 0
  end_x = 0
  end_y = 0
*/
bool modificator::load_coord(DATA_FILE_SECTION *p_section, COORDINATE *p_coord, int *p_index)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line) {    
    return(FALSE);
  }    
  
  VECT2DI   coordinates_1_vect(0);
  bool      coordinates_1_vect_active = FALSE;
  
  POLAR2DI  coordinates_1_polar(0);
  bool      coordinates_1_polar_active = FALSE;
  
  VECT2DI   coordinates_2_vect(0);
  bool      coordinates_2_vect_active = FALSE;
  
  POLAR2DI  coordinates_2_polar(0);
  bool      coordinates_2_polar_active = FALSE;
  
  int       coordinates_index = 0;
  
  do {
    if(p_line->is_line()) {
      load_vect2di_active(coordinates_1_vect,"start");
      load_polar2di_active(coordinates_1_polar,"start");
      
      load_vect2di_active(coordinates_2_vect,"size");
      load_polar2di_active(coordinates_2_polar,"size");
      
      load_int(coordinates_index,"index");
    }    
  } while((p_line = p_line->section_next()));
  
  if(coordinates_1_vect_active) {
    p_coord->start_set(coordinates_1_vect);
  }
  if(coordinates_1_polar_active) {
    p_coord->start_set(coordinates_1_polar);
  }
  
  if(coordinates_2_vect_active) {
    p_coord->size_set(coordinates_2_vect);
  }
  if(coordinates_2_polar_active) {
    p_coord->size_set(coordinates_2_polar);
  }  
  
  *p_index = coordinates_index;
  
  return(coordinates_1_vect_active ||
         coordinates_1_polar_active ||
         coordinates_2_vect_active ||
         coordinates_2_polar_active);
}
    
void modificator::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line) {
    pdebug(DEBUG_GENERATOR,"Modificator '%s' : no configuration line!",name_get());
    return;
  }

  // Load modificator mask
  modificator_mask_reference::load(p_section, generator_get());
  
  // Load coordinates configuration
  coordinates_get()->load(p_section);
  
  // Load target modificator
  do {
    if(p_line->is_line()) {
      
      // Load slave modificator
      if(p_line->match_name("modificator_slave")) {
        GENERATOR *p_gen = generator_get();
        MODIFICATOR *p_slave = p_gen->modificator_find(p_line->line_value_string_get());
        if(!p_slave) {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Modificator '%s' : Can't find a slave modificator '%s'",
                                 name_get(),p_line->line_value_string_get());
        }
        modificator_slave_add(p_slave);
        p_line->line_mark_loaded();
        continue;
      }
      
      // Load pre-modificator
      if(p_line->match_name("modificator_pre")) {
        GENERATOR *p_gen = generator_get();
        MODIFICATOR *p_pre = p_gen->modificator_find(p_line->line_value_string_get());
        if(!p_pre) {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Modificator '%s' : Can't find a pre modificator '%s'",
                                 name_get(),p_line->line_value_string_get());
        }
        modificator_pre_add(p_pre);
        p_line->line_mark_loaded();
        continue;
      }
      
      // Load post-modificator
      if(p_line->match_name("modificator_post")) {
        GENERATOR *p_gen = generator_get();
        MODIFICATOR *p_post = p_gen->modificator_find(p_line->line_value_string_get());
        if(!p_post) {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Modificator '%s' : Can't find a post modificator '%s'",
                                 name_get(),p_line->line_value_string_get());
        }
        modificator_post_add(p_post);
        p_line->line_mark_loaded();
        continue;
      }
    }
    
    if(p_line->is_section()) {
      MODIFICATOR_TYPE type = modificator_type_item::type_translate(p_line->section_type_get());
      switch(type) {
        case MODIFICATOR_COORDINATE_TYPE:
        {
          COORDINATE coord;
          int index;
          if(load_coord(p_line, &coord, &index)) {
            COORDINATE *p_coord = coordinates_get()->coord_local_fixed_get(index);
            *p_coord = coord;
          }
          break;
        }
        default:
          break;
      }
    }
  } while((p_line = p_line->section_next()));  
}

modificator::modificator(GENERATOR *p_gen, MODIFICATOR_TYPE type)
  : generator_reference(p_gen),
    modificator_type_item(type),
    object_list(NULL)
{
  memset(p_modificator_slave,0,sizeof(p_modificator_slave));
  memset(p_modificator_pre,0,sizeof(p_modificator_pre));
  memset(p_modificator_post,0,sizeof(p_modificator_post));
}

modificator::~modificator(void)
{
}

modificator_pixel_pattern::modificator_pixel_pattern(GENERATOR *p_gen, MODIFICATOR_TYPE type)
  : modificator(p_gen,type)
{
  pixel_step_x = 1;
  pixel_step_y = 1;
  pixel_step_random = FALSE;
  
  pixel_size = 1;
  
  area_inverted = FALSE;
  
  pixel_color_density = 1.0f;
    
  probability_fade = FALSE;
  probability_fade_start = 0.5f;
  probability_fade_stop = 1.0f;
  
  color_fade = FALSE;
  color_fade_start = 0.5f;
  color_fade_stop = 1.0f;
  
  erode_border = 0;
  erode_factor = 0;
  
  size_variator_theshold = 0.0f;
  size_variator_factor = 10.0f;
}

modificator_pixel_pattern::~modificator_pixel_pattern(void)
{
}

int modificator_pixel_pattern::pixel_step_random_get(void)
{
  return(pixel_step_random_min+(int)((pixel_step_random_max-pixel_step_random_min)*random_generator::generator_rand()));
}

// get the pixel step
int modificator_pixel_pattern::pixel_step_x_get(void)
{
  return(pixel_step_random ? pixel_step_random_get() : pixel_step_x);
}

int modificator_pixel_pattern::pixel_step_y_get(void)
{
  return(pixel_step_random ? pixel_step_random_get() : pixel_step_y);
}

void modificator_pixel_pattern::pixel_step_set(int step)
{
  pixel_step_x = pixel_step_y = step;
}

int modificator_pixel_pattern::pixel_step_get(void)
{
  return(MAX(pixel_step_x,pixel_step_y));
}

bool modificator_pixel_pattern::probability_pixel_fade_get(float pixel_distance, float pixel_distance_max)
{
  if(!probability_fade)
    return(TRUE);
  
  // find relative pixel position (0...1)
  float normalized_pixel_distance = pixel_distance / pixel_distance_max;
  
  // Is the pixel completely faded?
  if(normalized_pixel_distance > probability_fade_stop)
    return(FALSE);
  
  // Is the pixel in the fade-range?
  if(normalized_pixel_distance > probability_fade_start) {
    // Pixel probability is from 1 to 0
    float fade_range = probability_fade_stop - probability_fade_start;
    float pixel_probability = 1.0f - ((normalized_pixel_distance-probability_fade_start) / fade_range);
    if(random_generator::generator_rand() > pixel_probability)
      return(FALSE);
  }
  
  return(TRUE);
}

bool modificator_pixel_pattern::pixel_density_get(void)
{
  return(pixel_color_density >= 1.0f || random_generator::generator_rand() < pixel_color_density);
}

bool modificator_pixel_pattern::pixel_erosion_get(float pixel_distance, float pixel_distance_max)
{
  // Calculate border erosion
  if(erode_factor > 0.0f) {
    if(pixel_distance_max - pixel_distance < erode_border)
    {
      if(random_generator::generator_rand_0() < erode_factor) {
        // Pixel is eroded
        return(FALSE);
      }
    }
  }
  // Pixel is not eroded
  return(TRUE);
}

float modificator_pixel_pattern::size_variation_get(float pixel_distance_max)
{
  if(random_generator::generator_rand() < size_variator_theshold)
    return(random_generator::generator_gauss_0()*pixel_distance_max*size_variator_factor);
  else
    return(0.0f);
}

/*
  - Color pattern (border/fade/pixel steps):

    area_inverted = 0
    
    pixel_size = 1
    
    pixel_step_x = 1
    pixel_step_y = 1
    
    pixel_step_random = 0
    pixel_step_random_min = 0
    pixel_step_random_max = 0
    
    pixel_color_density = 1.0
  
    probability_fade = 0
    probability_fade_start = 0.5
    probability_fade_stop = 1.0

    color_fade = 0
    color_fade_start = 0
    color_fade_stop = 0
  
    erode_border = 0.0
    erode_factor = 0.0
    
    size_variator_theshold = 0.0
    size_variator_factor = 0.0
*/
void modificator_pixel_pattern::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator::load(p_section);
  
  int pixel_step = 0;
  
  do {
    if(p_line->is_line()) {
      load_int(area_inverted,"area_inverted");
      load_int(pixel_size,"pixel_size");
      
      load_int(pixel_step,"pixel_step");
      load_int(pixel_step_x,"pixel_step_x");
      load_int(pixel_step_y,"pixel_step_y");
  
      load_int(pixel_step_random,"pixel_step_random");
      load_int(pixel_step_random_min,"pixel_step_random_min");
      load_int(pixel_step_random_max,"pixel_step_random_max");
      
      load_float(pixel_color_density,"pixel_color_density");
      
      load_int(probability_fade,"probability_fade");
      load_float(probability_fade_start,"probability_fade_start");
      load_float(probability_fade_stop,"probability_fade_stop");
  
      load_int(color_fade,"color_fade");
      load_float(color_fade_start,"color_fade_start");
      load_float(color_fade_stop,"color_fade_stop");
        
      load_int(erode_border,"erode_border");
      load_float(erode_factor,"erode_factor");
      
      load_float(size_variator_theshold,"size_variator_theshold");
      load_float(size_variator_factor,"size_variator_factor");
    }
  } while((p_line = p_line->section_next()));
  
  if(pixel_step) {
    pixel_step_x = pixel_step_y = pixel_step;
  }
}

/*
 * Loads the table
 */
bool modificator_color_table::load(char *p_name)
{ 
  SURFACE_SDL bitmap(p_name);
  if(bitmap.loaded()) {
    clear();
    p_hist = histogram_new(&bitmap);
  }
  return(p_hist != NULL);
}

void modificator_color_table::clear(void)
{
  if(p_hist) {
    delete p_hist;
    p_hist = NULL;
  }
}

modificator_color_table::modificator_color_table(void)
{
  p_hist = NULL;
}

modificator_color_table::~modificator_color_table(void)
{
  clear();
}

/*
tbox_color::tbox_color(void)
{
  color_op.op = OPERATION_ADD;
    
  color_center = RGBAF(0,0,0,0);
  color_center_scale = 1.0f;
  color_center_delta = RGBAF(0,0,0,0);
  color_center_current = RGBAF(0,0,0,0);
  
  color_delta.set(0.0f,0.0f,0.0f,0.0f);
  color_delta_scale = 1.0f;
}

void tbox_color::init(float scatter, MODIFICATOR_PARAMS *p_params,
                      MODIFICATOR_COLOR_TABLE *p_table)
{
  PIXEL_PARAMS par(scatter, p_params);
  float center_diff = color_center_parameter.execute(&par);
  
  if(p_table) {    
    color_center_current = RGBAB(p_table->color_get(center_diff));
  }
  else {  
    RGBAF tmp = color_center_delta*center_diff;
    color_center_current = color_center*color_center_scale;
    color_center_current += tmp;
  }
}

RGBAF tbox_color::apply(RGBAF top, float scatter, MODIFICATOR_PARAMS *p_params, 
                        MODIFICATOR_COLOR_TABLE *p_table)
{
  //color = color (OP) ( color_center (OP) color_delta*scatter )
  PIXEL_PARAMS par(scatter, p_params);
  float color_diff = color_delta_parameter.execute(&par);
  color_diff *= color_delta_scale;
    
  RGBAF delta;
  if(p_table) {
    delta = RGBAB(p_table->color_get(color_diff));
  }
  else {
    delta = color_delta*color_diff;
  }
    
  if(color_blend_param.loaded()) {      
     color_op.blend_factor = color_blend_param.execute(&par);
  }
  
  RGBAF current = blend(color_op, color_center_current, delta);
    
  if(final_blend_param.loaded()) {
    final_op.blend_factor = final_blend_param.execute(&par);
  }
  
  return(blend(final_op, top, current));
}

tbox_height::tbox_height(void)
{  
  height_op.op = OPERATION_ADD;
  final_op.op = OPERATION_SET;
  
  height_center = 0.0f;
  height_delta = 1.0f;   
}

float tbox_height::apply(float top, float scatter, MODIFICATOR_PARAMS *p_params)
{
  //color = color (OP) ( color_center (OP) color_delta*scatter )
  PIXEL_PARAMS par(scatter, p_params);
  float height_diff = height_parameter.execute(&par);
  
  float tmp = height_delta*height_diff;
  tmp = blend(height_op, height_center, tmp);
  return(blend(final_op, top, tmp));
}
*/
/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_POINT_SINGLE

  color_apply = 1
  
  # Base colors for the texture
  color_center = 0xffffff
  
  # Color vector for scattering
  # target_color = target_color_center +- target_color_delta
  color_delta = 0xffdfdf
  
  # Is the delta color vector bidirectional? 
  # (i.e. bidirectional      => target_color_center +- target_color_delta)
  # (i.e. non bidirectional  => target_color_center +  target_color_delta)
  delta_bidirectional = 1
  
  fade_color_enabled
  fade_color_center
  fade_color_delta
  fade_delta_bidirectional

  height_apply
  
  height_center
  height_delta
}
*/

void operation_box::init(MODIFICATOR_PARAMS *p_params, 
                         RECT2DI pos, RECT2DI target_area,
                         MODIFICATOR_COLOR_TABLE *p_table)
{   
  symbol_table_set_active(&stable);

  // Set pixel area
  SYMBOL_TABLE_NODE *p_area = symbol_table_get_int(symbol_table_get_active(), PIXEL_AREA);  
  assert(p_area && p_area->value.type == VARIABLE_RECT2DI);
  p_area->value.vrect = pos;
  
  // Set target area
  SYMBOL_TABLE_NODE *p_target_area = symbol_table_get_int(symbol_table_get_active(), TARGET_AREA);
  assert(p_target_area && p_target_area->value.type == VARIABLE_RECT2DI);
  p_target_area->value.vrect = target_area;
  
  PIXEL_PARAMS par(p_params);
  params_init.execute(&par);
  
  symbol_table_set_active(NULL);
}

static char * operation_box_name_compose(char *out, const char *base_name, int num)
{
  sprintf(out,"%s_%d",base_name,num+1);
  return(out);
}

void operation_box::apply(RGBAF *p_color, float *p_height, VECT3DF *p_normal,
                          int targets,
                          RECT2DI pos,
                          MODIFICATOR_PARAMS *p_params,
                          MODIFICATOR_COLOR_TABLE *p_table)
{
  symbol_table_set_active(&stable);
  
  SYMBOL_TABLE_NODE *p_color_variables[MAX_MODIFICATOR_TARGETS];
  SYMBOL_TABLE_NODE *p_height_variables[MAX_MODIFICATOR_TARGETS];
  
  //Load variables from target surface
  for(int i = 0; i < targets; i++) {
    char tmp[MAX_NAME];
        
    p_color_variables[i] = symbol_table_get_int(symbol_table_get_active(),
                                 operation_box_name_compose(tmp, PIXEL_COLOR_FINAL, i));
    assert(p_color_variables[i] && p_color_variables[i]->value.type == VARIABLE_RGBAF);
    p_color_variables[i]->value.vcolor = p_color[i];
    
    p_height_variables[i] = symbol_table_get_int(symbol_table_get_active(),
                                 operation_box_name_compose(tmp, PIXEL_HEIGHT_FINAL, i));
    assert(p_height_variables[i] && p_height_variables[i]->value.type == VARIABLE_FLOAT);
    p_height_variables[i]->value.vfloat = p_height[i];
    
    SYMBOL_TABLE_NODE *p_tmp;
    p_tmp = symbol_table_get_int(symbol_table_get_active(),
                                 operation_box_name_compose(tmp, PIXEL_NORMAL, i));
    assert(p_tmp && p_tmp->value.type == VARIABLE_VECT3DF);
    p_tmp->value.vvector = p_normal[i];
  }
  
  SYMBOL_TABLE_NODE *p_coordinate = symbol_table_get_int(symbol_table_get_active(), PIXEL_COORDINATE);
  assert(p_coordinate && p_coordinate->value.type == VARIABLE_VECT3DF);
  p_coordinate->value.vvector.x = pos.x;
  p_coordinate->value.vvector.y = pos.y;
  p_coordinate->value.vvector.z = 0;
    
  PIXEL_PARAMS par(p_params);
  params.execute(&par);

  // Write back results
  for(int i = 0; i < targets; i++) {
    p_color[i] = p_color_variables[i]->value.vcolor;
    p_height[i] = p_height_variables[i]->value.vfloat;
  }
  
  // undet global symbol table
  symbol_table_set_active(NULL);
}

// Load init section
void operation_box::load_init(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  do {
    if(p_line->is_line_raw()) {      
      params_init.parse_line(p_line);
      p_line->line_mark_loaded();
    }
  } while((p_line = p_line->section_next()));
}

void operation_box::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
      
  do {    
    // Load raw lines for basic parser (apply)
    if(p_line->is_line_raw()) {      
      params.parse_line(p_line);
      p_line->line_mark_loaded();
    }
    
    // Load init section
    if(p_line->is_section()) {
      MODIFICATOR_TYPE type = modificator_type_item::type_translate(p_line->section_type_get());
      switch(type) {
        case MODIFICATOR_POINT_OPBOX_INIT_TYPE:
        {          
          load_init(p_line);
          break;
        }
        default:
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Unknown section name '%s', type '%s'",
                                 p_line->name_get(),p_line->section_type_get());
          break;
      }
    }
    
  } while((p_line = p_line->section_next()));  
}

operation_box::operation_box(void)
  : params_init(&stable),
    params(&stable)
{
  symbol_table_init(&stable, NULL);
}

void modificator_point_single::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator_pixel_pattern::load(p_section);
  
  do {    
    if(p_line->is_line()) {
      
      /*      
       * bool                        color_borders;
       * RGBAF                       color_border_min;
       * RGBAF                       color_border_max;
       */
      load_int(color_borders, "color_borders");
      load_rgbaf(color_border_min, "color_border_min");
      load_rgbaf(color_border_max, "color_border_max");
      
      /*      
       * COLOR_OPERATION             height_op;
       */
      //load_aritmetic_operation(height_op, "height_operation");

      /*
       * Color tables
       */
      if(p_line->match_name("color_table")) {
        if(!color_table.load(p_line->line_value_string_get())) {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Modificator '%s' : Can't load colortable from '%s'",
                                 p_line->name_get(),p_line->line_value_string_get());
        }        
        p_line->line_mark_loaded();
        continue;
      }
      if(p_line->match_name("color_table_center")) {
        if(!color_table_center.load(p_line->line_value_string_get())) {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Modificator '%s' : Can't load colortable from '%s'",
                                 p_line->name_get(),p_line->line_value_string_get());
        }
        p_line->line_mark_loaded();
        continue;
      }
      if(p_line->match_name("color_table_delta")) {
        if(color_table_delta.load(p_line->line_value_string_get())) {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Modificator '%s' : Can't load colortable from '%s'",
                                 p_line->name_get(),p_line->line_value_string_get());          
        }
        p_line->line_mark_loaded();
        continue;
      }
    }
    
    if(p_line->is_section()) {
      MODIFICATOR_TYPE type = modificator_type_item::type_translate(p_line->section_type_get());
      switch(type) {
        case MODIFICATOR_POINT_OPBOX_TYPE:
        {
          OPERATION_BOX *p_box = operation_box_create();
          p_box->load(p_line);
          break;
        }
        case MODIFICATOR_COORDINATE_TYPE:
        {
          // handled by modificator_pixel_pattern::load()
          break;
        }
        default:
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Unknown section name '%s', type '%s'",
                                 p_line->name_get(),p_line->section_type_get());
          break;
      }
    }
    
  } while((p_line = p_line->section_next()));
}

/*
  Emit single sub-pixel:
  color = target_color_center + target_color_delta*scattering
*/
modificator_point_single::modificator_point_single(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_POINT_SINGLE_TYPE)
{    
  color_borders = FALSE;
  color_border_min = RGBAF(0,0,0,0);
  color_border_max = RGBAF(1,1,1,1);
}

modificator_point_single::~modificator_point_single(void)
{
  
}

OPERATION_BOX * modificator_point_single::operation_box_create(void)
{
  OPERATION_BOX *p_box = new OPERATION_BOX;
  operation_box_list.list_insert(p_box);
  return(p_box);
}

void modificator_point_single::operation_box_remove(OPERATION_BOX *p_box)
{
  operation_box_list.list_remove(p_box);
  delete p_box;
}

OPERATION_BOX * modificator_point_single::operation_box_get_first(void)
{
  return((OPERATION_BOX *)operation_box_list.list_get_first());
}

void modificator_point_single::apply(RGBAF *p_color,
                                     float *p_height,
                                     VECT3DF *p_normal,
                                     int targets,
                                     RECT2DI pos,
                                     MODIFICATOR_PARAMS *p_params)
{
  OPERATION_BOX *p_box = operation_box_get_first();
  if(p_box) {
    
    while(p_box) {
      MODIFICATOR_COLOR_TABLE *p_table = color_table_delta.loaded() ?
                                         &color_table_delta : NULL;
      p_box->apply(p_color, p_height, p_normal, targets, pos, p_params, p_table);
      p_box = (OPERATION_BOX *)p_box->list_next();
    }
    
    if(color_table.loaded() || color_borders) {
      for(int i = 0; i < targets; i++) {    
        if(color_table.loaded()) {
          float a = p_color[i].a;
          p_color[i] = RGBAF(color_table.color_get(RGBB(p_color[i])));
          p_color[i].a = a;
        }
    
        if(color_borders) {
          p_color[i].norm_color(color_border_max,color_border_min);
        }
      }
    }
  }
}

bool modificator_point_single::target_apply_pixel(MODIFICATOR_TARGET_STACK *p_target,
                                                  RECT2DI pos,
                                                  MODIFICATOR_PARAMS *p_params)
{
  RGBAF color[MAX_MODIFICATOR_TARGETS];
  float height[MAX_MODIFICATOR_TARGETS];
  VECT3DF normal[MAX_MODIFICATOR_TARGETS];

  int i,target_num;
  MODIFICATOR_TARGET **p_list = p_target->get_all(target_num);
  
  // Load to variables
  for(i = 0; i < target_num; i++) {    
    color[i] = p_list[i]->color_get(pos.x, pos.y);
    height[i] = p_list[i]->height_get(pos.x, pos.y);
    normal[i] = p_list[i]->normal_get(pos.x, pos.y);
  }

  // Apply op box
  apply(color, height, normal, target_num, pos, p_params);
  
  // Store back
  for(i = 0; i < target_num; i++) {    
    p_list[i]->color_set(pos.x, pos.y, color[i]);
    p_list[i]->height_set(pos.x, pos.y, height[i]);
  }
    
  return(TRUE);
}

bool modificator_point_single::target_apply(MODIFICATOR_TARGET_STACK *p_target,
                                            RECT2DI pos,
                                            MODIFICATOR_PARAMS *p_params)
{
  if(pixel_size == 1) {
    target_apply_pixel(p_target, pos, p_params);
  }
  else if(pixel_size == 2) {
    target_apply_pixel(p_target, RECT2DI(pos.x, pos.y), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x+1, pos.y), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x, pos.y+1), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x+1, pos.y+1), p_params);      
  } 
  else if(pixel_size == 3) {
    target_apply_pixel(p_target, RECT2DI(pos.x-1, pos.y-1), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x  , pos.y-1), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x+1, pos.y-1), p_params);
    
    target_apply_pixel(p_target, RECT2DI(pos.x-1, pos.y  ), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x  , pos.y  ), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x+1, pos.y  ), p_params);

    target_apply_pixel(p_target, RECT2DI(pos.x-1, pos.y+1), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x  , pos.y+1), p_params);
    target_apply_pixel(p_target, RECT2DI(pos.x+1, pos.y+1), p_params);    
  }
  return(TRUE);
}

bool modificator_point_single::init(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos,
                                    MODIFICATOR_PARAMS *p_params)
{
  OPERATION_BOX *p_color_box = operation_box_get_first();
  while(p_color_box) {    
    MODIFICATOR_COLOR_TABLE *p_table = color_table_center.loaded() ? 
                                       &color_table_center : NULL;
    MODIFICATOR_TARGET *p_tg = p_target->get_first();
    p_color_box->init(p_params, pos, p_tg->target_area_get(), p_table);
    p_color_box = (OPERATION_BOX *)p_color_box->list_next();
  }
  return(modificator::init(p_target,pos,p_params));
}

bool modificator_point_single::apply(MODIFICATOR_TARGET_STACK *p_target,
                                     MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  bool ret = FALSE;
  
  RECT2DI center;
  p_coord->coord_final_get_start();
  while(p_coord->coord_final_get_next(&center)) {
    target_pre_apply(p_target, center, p_params);
    
    target_init(p_target, center, p_params);
    if(target_apply(p_target, center, p_params))
      ret = TRUE;
    
    target_post_apply(p_target, center, p_params);
  }
  
  return(ret);
}

/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_POINT_EXTENDED
}
*/
void modificator_point_extended::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator_pixel_pattern::load(p_section);
  //load_texture_light_pattern(p_point, p_section);

  do {    
    if(p_line->is_line()) {
  
      // Is the point located in center or not?
      load_int(point_center, "point_center");
    }
  } while((p_line = p_line->section_next())); 
}

/*
  Emit single pixel:
*/
bool modificator_point_extended::apply_pixel(MODIFICATOR_TARGET_STACK *p_target,
                                             VECT2DI *p_center, 
                                             float pixel_distance_max, 
                                             int x, int y, 
                                             float *p_size_variator, 
                                             MODIFICATOR_PARAMS *p_params)
{    
  bool  pixel_result = pixel_density_get();
  float pixel_distance = 0;
  
  if(pixel_result) {
    
    // Check if the pixes is in alowed range
    VECT2DI act(x,y);
    VECT2DI tmp = act - *p_center;
    
    *p_size_variator += size_variation_get(pixel_distance_max);
    pixel_distance_max -= fabs(*p_size_variator);
    
    pixel_distance = tmp.size();    
    if(pixel_distance > pixel_distance_max)
      pixel_result = FALSE;
  }
  
  // Calculate border erosion
  if(pixel_result)
    pixel_result = pixel_erosion_get(pixel_distance, pixel_distance_max);
  
  // Calculate fade 
  if(pixel_result)
    pixel_result = probability_pixel_fade_get(pixel_distance, pixel_distance_max);
  
  if(area_inverted)
    pixel_result = !pixel_result;
  
  // Skip this pixel?
  if(!pixel_result)
    return(FALSE);

  return(target_apply(p_target, RECT2DI(x, y), p_params));
}

/*
  Apply point
*/
bool modificator_point_extended::apply(MODIFICATOR_TARGET_STACK *p_target, 
                                       MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  bool ret = FALSE;
  
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
        
    VECT2DI tcenter;
    VECT2DI tsize;
    float   size;
    
    if(point_center) {
      tcenter = pos.center_get();
      tsize.set(pos.dx/2, pos.dy/2);
    } else {
      tcenter = pos.start_get();
      tsize.set(pos.dx, pos.dy);
    }
    
    size = (float)(MIN(tsize.x, tsize.y));
    float size_variator = 0;
    int x,y;
    int sx = tcenter.x-tsize.x,
        sy = tcenter.y-tsize.y;
    int ex = tcenter.x+tsize.x,
        ey = tcenter.y+tsize.y;
    
    for(y = sy; y < ey; y += pixel_step_y_get()) {
      for(x = sx; x < ex; x += pixel_step_x_get()) {
        if(apply_pixel(p_target, &tcenter, size, x, y, &size_variator, p_params))
          ret = TRUE;
      }
    }
    
    target_post_apply(p_target, pos, p_params);
  }
  return(ret);
}

modificator_point_extended::modificator_point_extended(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_POINT_EXTENDED_TYPE)
{  
  point_center = TRUE;
}

modificator_point_extended::~modificator_point_extended(void)
{ 
}

/*
 * Pattern area
 */
/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_RECTANGLE
}
*/
void modificator_rectangle::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator_pixel_pattern::load(p_section);
  //load_texture_light_pattern(p_point, p_section);
}

bool modificator_rectangle::apply_pixel(MODIFICATOR_TARGET_STACK *p_target,
                                        tpos x, tpos y,
                                        int pixel_distance,
                                        int pixel_distance_max,
                                        MODIFICATOR_PARAMS *p_params)
{
  // Inside??
  bool pixel_result = (pixel_distance < pixel_distance_max);
   
  // Check the basic pixel probability (based on density)
  if(pixel_result)
  {
    pixel_result = pixel_density_get();
  }
  
  // Calculate border erosion
  if(pixel_result)
    pixel_result = pixel_erosion_get(pixel_distance, pixel_distance_max);
  
  // Calculate fade 
  if(pixel_result)
    pixel_result = probability_pixel_fade_get(pixel_distance, pixel_distance_max);
  
  if(area_inverted)
    pixel_result = !pixel_result;
  
  // Skip this pixel?
  if(!pixel_result)
    return(FALSE);
    
  return(target_apply(p_target, RECT2DI(x, y), p_params));
}

bool modificator_rectangle::apply_rect_y(MODIFICATOR_TARGET_STACK *p_target,
                                         int sy, int ey,
                                         int cx, int x_direction,
                                         int border_size,
                                         int *p_size_array, 
                                         MODIFICATOR_PARAMS *p_params)
{
  int i,y;
  bool ret = FALSE;
  
  for(y = sy; y < ey; y += pixel_step_y_get()) {
    int border_distance = MIN(y-sy,ey-y);
    for(i = 0; i < p_size_array[y-sy]; i += pixel_step_x_get()) {
      int pixel_distance = i;
      if(soft_corners && border_distance < border_size) {
        int y_dist = border_size-border_distance;
        pixel_distance = (int)sqrtf(pixel_distance*pixel_distance + y_dist*y_dist);
      }
      if(apply_pixel(p_target, cx+i*x_direction,y,pixel_distance,p_size_array[y-sy],p_params))
        ret = TRUE;
    }
  }  
  
  return(ret);
}

bool modificator_rectangle::apply_rect_x(MODIFICATOR_TARGET_STACK *p_target,
                                         int sx, int ex,
                                         int cy, int y_direction,
                                         int border_size,
                                         int *p_size_array, 
                                         MODIFICATOR_PARAMS *p_params)
{
  int i,x;
  bool ret = FALSE;
  
  for(x = sx; x < ex; x += pixel_step_x_get()) {
    int border_distance = MIN(x-sx,ex-x);
    for(i = 0; i < p_size_array[x-sx]; i += pixel_step_y_get()) {
      int pixel_distance = i;
      if(soft_corners && border_distance < border_size) {
        int x_dist = border_size-border_distance;
        pixel_distance = (int)sqrtf(pixel_distance*pixel_distance + x_dist*x_dist);
      }      
      if(apply_pixel(p_target,x,cy+i*y_direction,pixel_distance,p_size_array[x-sx],p_params))
        ret = TRUE;
    }
  }
  
  return(ret);
}

bool modificator_rectangle::apply_rect(MODIFICATOR_TARGET_STACK *p_target,
                                       RECT2DI pos,
                                       MODIFICATOR_PARAMS *p_params,
                                       int direction)
{
  int x,y;
  
  int sx = pos.x,
      sy = pos.y;
  int dx = pos.dx,
      dy = pos.dy;
  
  if(!direction) {
    for(y = 0; y < dy; y += pixel_step_y_get()) {
      for(x = 0; x < dx; x += pixel_step_x_get()) {
        int index = p_params->push((float)x/(float)dx);
        apply_pixel(p_target,sx+x,sy+y,0,1,p_params);
        p_params->clear(index);
      }
    }
  } else {
    for(x = 0; x < dx; x += pixel_step_x_get()) {
      for(y = 0; y < dy; y += pixel_step_y_get()) {
        int index = p_params->push((float)y/(float)dy);
        apply_pixel(p_target,sx+x,sy+y,0,1,p_params);
        p_params->clear(index);
      }
    }
  }
  
  return(TRUE);
}

/*
 Apply area

 Center of the point is:

 area.x
 area.y

 Size of the point is:

 MIN(area.dx*2, area.dy*2)
*/
bool modificator_rectangle::apply(MODIFICATOR_TARGET_STACK *p_target,
                                  MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start, size;  
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target,pos,p_params);
    apply_rect(p_target,pos,p_params,0);
    
/*    
    int sx = pos.x,
        sy = pos.y;
    int ex = sx+pos.dx,
        ey = sy+pos.dy;
    int dx = pos.dx/2,
        dy = pos.dy/2;
  
    int *p_sizes = NULL;
        
    if(dx > dy) {
      p_sizes = size_array_int_variate(dy, dy, dx*2,
                                       size_variator_theshold, size_variator_factor);
      apply_rect_x(p_target, sx, ex, sx+dx, -1, dy, p_sizes, p_params);
      ffree(p_sizes);
      
      p_sizes = size_array_int_variate(dy, dy, dx*2, 
                                       size_variator_theshold, size_variator_factor);
      apply_rect_x(p_target, sx, ex, sx+dx,  1, dy, p_sizes, p_params);
      ffree(p_sizes);
    }
    else {
      p_sizes = size_array_int_variate(dx, dx, dy*2, 
                                       size_variator_theshold, size_variator_factor);
      apply_rect_y(p_target, sy, ey, sx+dx,-1, dx, p_sizes, p_params);
      ffree(p_sizes);
      
      p_sizes = size_array_int_variate(dx, dx, dy*2,
                                       size_variator_theshold, size_variator_factor);
      apply_rect_y(p_target, sy, ey, sx+dx, 1, dx, p_sizes, p_params);
      ffree(p_sizes);
    }
*/    
    target_post_apply(p_target, pos, p_params);    
  }
  return(TRUE);
}

modificator_rectangle::modificator_rectangle(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_RECTANGLE_TYPE)
{
  soft_corners = FALSE;
}

modificator_rectangle::~modificator_rectangle(void)
{
  
}

/*
 * Pattern line
 */
typedef bool (*LINE_FUNC)(tpos x, tpos y, float pos, void *p_param);

void line_emit(VECT2DI p1, VECT2DI p2, LINE_FUNC target, void *p_param)
{
  int sx = p1.x;
  int sy = p1.y;
  
  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
    
  // dx is bigger, iterate it
  if(abs(dx) > abs(dy)) {
    
    float k = (float)dy/(float)dx;
    float line_step = 1.0f/dx;
    int x_step = dx > 0 ? 1 : -1;
    
    int x;
    for(x = p1.x; x != p2.x; x += x_step) {
      int rx = x-sx;
      float index = line_step*rx;
      if(!target(x, sy+(int)roundf(k*rx), index, p_param)) {
        return;
      }
    }
  }
  else {
    
    float k = (float)dx/(float)dy;
    float line_step = 1.0f/dy;
    int y_step = dy > 0 ? 1 : -1;
    
    int y;
    for(y = p1.y; y != p2.y; y += y_step) {
      int ry = y-sy;
      float index = line_step*ry;
      if(!target(sx+(int)roundf(k*ry), y, index, p_param)) {
        return;
      }
    }
  }
}

void spline_emit(VECT2DI p0, VECT2DI p1,
                 VECT2DI p2, VECT2DI p3,
                 float bias,
                 float continuity,
                 float tension,
                 LINE_FUNC target,
                 void *p_param)
{
  VECT2DF v0 = ferguson_tangent(VECT2DF(p0), VECT2DF(p1), VECT2DF(p2),
                                CLASS_OUT, bias,continuity,tension);
  VECT2DF v1 = ferguson_tangent(VECT2DF(p1), VECT2DF(p2), VECT2DF(p3),
                                CLASS_IN, bias,continuity,tension);
  
  VECT2DF tmp = p2 - p1;
  
  // Basic step is 2/size
  float   step = 1.0f/(tmp.size()*2);
  float   t = 0;
  VECT2DI current(INT_UNDEFINED);
  while(t < 1) {
    VECT2DI next = ferguson_interpolate(v0, VECT2DF(p1), v1, VECT2DF(p2), t);
    VECT2DI diff = next-current;
    if(abs(diff.x) > 0 || abs(diff.y) > 0) {
      target(next.x, next.y, t, p_param);
      current = next;
    }
    t += step;
  }
}

/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_LINE
}
*/
void modificator_line::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_pixel_pattern::load(p_section);
  
  do {
    if(p_line->is_line()) {
      load_int(line_width, "line_width");
      load_int(line_smooth_end, "line_smooth_end");
    }
  } while((p_line = p_line->section_next()));
}

void modificator_line::line_emit(MODIFICATOR_TARGET_STACK *p_target,
                                 RECT2DI pos,
                                 MODIFICATOR_PARAMS *p_params)
{
  int sx = pos.x;
  int sy = pos.y;
  
  int dx = pos.dx;
  int dy = pos.dy;
    
  bool reversed = FALSE;
  
  // dx is bigger, iterate it
  if(abs(dx) > abs(dy)) {
        
    if(dx < 0) {
      sx += dx;
      sy += dy;
            
      dx = -dx;
      dy = -dy;
      
      reversed = TRUE;
    }
      
    float k = (float)dy/(float)dx;
    float line_step = 1.0f/dx;
    
    int x;
    for(x = 0; x <= dx; x += pixel_step_x_get()) {      
      int index = p_params->push(reversed ? 1.0f - line_step*x : line_step*x);
      target_apply(p_target, RECT2DI(sx+x, sy+(int)roundf(k*x)), p_params);
      p_params->clear(index);
    }
  }
  else {
  
    if(dy < 0) {
      sx += dx;
      sy += dy;
            
      dx = -dx;
      dy = -dy;
      
      reversed = TRUE;
    }
    
    float k = (float)dx/(float)dy;
    float line_step = 1.0f/dy;
    
    int y;
    for(y = 0; y <= dy; y += pixel_step_y_get()) {
      int index = p_params->push(reversed ? 1.0f - line_step*y : line_step*y);
      target_apply(p_target, RECT2DI(sx+(int)roundf(k*y), sy+y), p_params);
      p_params->clear(index);
    }
  }
}

// SURFACE_MASK
// line_width
void modificator_line::line_emit_width(MODIFICATOR_TARGET_STACK *p_target,
                                       RECT2DI pos,
                                       MODIFICATOR_PARAMS *p_params,
                                       int line_size)
{
  int sx = pos.x;
  int sy = pos.y;
  
  int dx = pos.dx;
  int dy = pos.dy;
  
  SURFACE_MASK *p_mask = NULL;
  bool reversed = FALSE;
  
  // dx is bigger, iterate it
  if(abs(dx) > abs(dy)) {
        
    if(dx < 0) {
      sx += dx;
      sy += dy;
            
      dx = -dx;
      dy = -dy;
      
      reversed = TRUE;
    }

    p_mask = new SURFACE_MASK(dx+2*line_size, dy+2*line_size);
    float k = (float)dy/(float)dx;
    
    int x;
    for(x = 0; x <= dx; x += pixel_step_x_get()) {      
      int cx = x;
      int cy = (int)roundf(k*x);
      p_mask->fill(cx, cy, line_size*2+1, line_size*2+1, TRUE);
    }
  }
  else {
  
    if(dy < 0) {
      sx += dx;
      sy += dy;
            
      dx = -dx;
      dy = -dy;
      
      reversed = TRUE;
    }
    
    p_mask = new SURFACE_MASK(dx+2*line_size, dy+2*line_size);
    float k = (float)dx/(float)dy;
    
    int y;
    for(y = 0; y <= dy; y += pixel_step_y_get()) {
      int cx = (int)roundf(k*y);
      int cy = y;
      p_mask->fill(cx, cy, line_size*2+1, line_size*2+1, TRUE);
    }
  }
  
  // Fill the area
  int mx,my;
  for(my = 0; my < dy+line_size*2; my++) {
    for(mx = 0; mx < dx+line_size*2; mx++) {
      if(p_mask->mask_get(mx, my)) {
        tpos tx = sx+mx-line_size;
        tpos ty = sy+my-line_size;
        float t, dist;        
        line_dist(pos.start_get(), pos.end_get(), VECT2DI(tx,ty), t, dist);        
        if(line_smooth_end) {
          if(t < 0) {
            dist = point_dist(pos.start_get(),VECT2DI(tx,ty));
          }
          else if(t > 1) {
            dist = point_dist(pos.end_get(),VECT2DI(tx,ty));
          }
        }
        if(dist < line_size) {
          int index = p_params->push(1.0f-(dist/line_size));
          target_apply(p_target, RECT2DI(tx, ty), p_params);
          p_params->clear(index);
        }
      }
    }
  }
  
  delete p_mask;
}

/*
 * Apply line
 */
bool modificator_line::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{  
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start, end;
  RECT2DI pos;
  p_coord->coord_final_get_start();
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    
    if(line_width > 1) {
      line_emit_width(p_target, pos, p_params, line_width-1);
    }
    else {
      line_emit(p_target, pos, p_params);
    }
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

modificator_line::modificator_line(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_LINE_TYPE)
{  
  line_width = 1;
  line_smooth_end = TRUE;
}

modificator_line::~modificator_line(void)
{ 
}

bool modificator_poly::poly_pixel_mask_border_get(VECT2DI pixel)
{
  VECT2DI borders[] = 
  {
    VECT2DI(-1,-1),
    VECT2DI( 0,-1),
    VECT2DI( 1,-1),
    VECT2DI(-1,0),
    VECT2DI( 0,0),
    VECT2DI( 1,0),
    VECT2DI(-1,1),
    VECT2DI( 0,1),
    VECT2DI( 1,1),
  };
  
  int i;
  for(i = 0; i < (int)(sizeof(borders)/sizeof(borders[0])); i++) {
    if(poly_mask_get(pixel.x+borders[i].x, pixel.y+borders[i].y))
      return(TRUE);
  }
  return(FALSE);
}

// Returns distance from center to near border
float modificator_poly::poly_pixel_distance_get(VECT2DI pixel, VECT2DI center, int max_dist)
{
  if(pixel == center)
    return(0);
  
  VECT2DF dir(pixel - center);
  float pixel_dist = dir.norm();
  
  VECT2DF current = pixel;
  int i;
  for(i = pixel_dist; i <= max_dist; i++) {
    if(poly_pixel_mask_border_get(current)) {
      return(i+1);
    }
    current += dir;
  }
  return(max_dist);
}

// Returns distance from center to near border
float modificator_poly::poly_pixel_height_plane_get(PLANE   p,
                                                    VECT2DI pixel,
                                                    RECT2DI pos,
                                                    float   height)
{
  VECT2DF center = pos.center_get();
  VECT2DF sizes = pos.center_size_get();
  float   size_max = sizes.x > sizes.y ? sizes.x : sizes.y;
  VECT2DF relative = (VECT2DF(pixel) - center);
  
  relative = relative * (1.0f / size_max);
  
  // We want to know y
  // a*x + b*y + c*z + e = 0
  // y = -(a*x + c*z + e) / b
  float plane_height = -(p.x*relative.x + p.z*relative.y + p.e) / p.y;
  if(plane_height > 0) {
    plane_height = plane_height;
  }
  
  return(height+plane_height);
}

/*
  helper - poly modificator
  MODIFICATOR_POLY
*/
void modificator_poly::poly_fill(MODIFICATOR_TARGET_STACK *p_target, RECT2DI pos,
                                 MODIFICATOR_PARAMS *p_params, float height_maximal)
{
  VECT2DI center = pos.center_get();
  float   max_distance = (pos.center_size_get()).size();
  
  float current_height_center = height_center;
  float current_height_left = height_left;
  float current_height_top = height_top;
  
  PLANE p(VECT3DF(-1, current_height_left,   0),
          VECT3DF( 0, current_height_center, 0),
          VECT3DF( 0, current_height_top,   -1));
  
  tpos x,y;
  for(y = 0; y < pos.dy; y++) {
    
    bool inside = FALSE;
    bool set = FALSE;
    bool set_last = FALSE;
    
    bool border_change;
    int  border_change_num = 0;
    
    int  scanline_length = 0;
    int  scanline_start = 0;
    
    // Calculate scan-line start & length
    for(x = 0; 
        x < pos.dx && border_change_num < 3;
        x++) 
    {
      set = poly_mask_get(pos.x+x,pos.y+y);
            
      border_change = (set != set_last);
      set_last = set;
      
      if(border_change) {
        border_change_num++;
        
        // Scanline start
        if(border_change_num == 2) {
          inside = TRUE;
          scanline_start = x;
        }
        
        // Scanline stop
        else if(border_change_num == 3) {
          inside = FALSE;
        }
      }
      
      if(inside) {
        scanline_length++;
      }
    }
    
    // Draw scan line
    if(!inside || poly_mask_get(pos.x+pos.dx-1,pos.y+y)) {      
      for(x = 0; x < pos.dx ; x++) {
        float param = 0;
        if(x >= scanline_start && x < scanline_start+scanline_length) {
          VECT2DI pixel(pos.x+x, pos.y+y);
          
          float pixel_distance = (pixel - center).size();
          
          float linear_distance = 0;
          if(pixel_distance != 0) {
            float max_distance_border = poly_pixel_distance_get(pixel,center,max_distance);
            linear_distance = pixel_distance / max_distance_border;
            //linear_distance = pixel_distance/max_distance;
          }
  
          switch(height_profile) {
            case POLY_PROFILE_ELLIPSE:
              if(linear_distance < 1) {
                param = sqrtf(1-linear_distance*linear_distance);
                assert(param >= 0 && param <= 1);
              }
              break;
            case POLY_PROFILE_PYRAMID:
              if(linear_distance < 1) {
                param = (1.0f-linear_distance);
                assert(param >= 0 && param <= 1);
              }
              break;
          }
  
          // Height modifications regards to pixel position
          param = poly_pixel_height_plane_get(p, pixel, pos, param);
          if(param < 0) {
            param = 0;
            continue;
          }
        }
        
        if(border || param > 0) {
          int index = p_params->push(param*height_maximal);
          p_params->push(param);
          RECT2DI tmp(pos.x+x, pos.y+y);
          target_apply(p_target, tmp, p_params);
          p_params->clear(index);
        }
      }
    }
  }
}

/*
  helper - poly modificator
  MODIFICATOR_POLY
*/
void modificator_poly::poly_fill_mask(RECT2DI pos)
{
  tpos x,y;
  for(y = 0; y < pos.dy; y++) {
    
    bool inside = FALSE;
    bool set = FALSE;
    bool set_last = FALSE;
    
    bool border_change;
    int  border_change_num = 0;
    
    int  scanline_length = 0;
    int  scanline_start = 0;
        
    for(x = 0; 
        x < pos.dx && border_change_num < 3;
        x++) 
    {
      set = poly_mask_get(pos.x+x,pos.y+y);
            
      border_change = (set != set_last);
      set_last = set;
      
      if(border_change) {
        border_change_num++;
        
        // Scanline start
        if(border_change_num == 2) {
          inside = TRUE;
          scanline_start = x;
        }
        
        // Scanline stop
        else if(border_change_num == 3) {
          inside = FALSE;
        }
      }
      
      if(inside) {
        scanline_length++;
      }
    }
    
    // Draw scan line to mask
    if(!inside) {
      for(x = scanline_start; x < scanline_start+scanline_length; x++) {
        poly_mask_set(pos.x+x,pos.y+y);
      }
    }
  }
}

/*
 * Emit line to mask
 */
bool modificator_poly_poly_line_point(tpos x, tpos y, float pos, void *p_param)
{
  MODIFICATOR_POLY *p_poly = (MODIFICATOR_POLY *)p_param;
  p_poly->poly_mask_set(x,y);
  return(TRUE);
}

void modificator_poly::poly_line(VECT2DI p0, VECT2DI p1,
                                 VECT2DI p2, VECT2DI p3)
{
  if(!mask.loaded())
    return;
  
  float tension = -(corner_curvature - 0.5f)*2;
  spline_emit(p0, p1, p2, p3, 0, 0, tension, 
              modificator_poly_poly_line_point, this);
}

/*
 * Emit line to target
 */
typedef struct _poly_cont
{
  MODIFICATOR_POLY    *p_poly;
  MODIFICATOR_TARGET_STACK  *p_target;
  MODIFICATOR_PARAMS  *p_params;
  
} POLY_CONT;

bool modificator_poly_poly_line_point_target(tpos x, tpos y, float pos, void *p_param)
{
  POLY_CONT *p_cont = (POLY_CONT *)p_param;
  
  RECT2DI tmp(x,y);
  p_cont->p_poly->target_apply(p_cont->p_target, tmp, p_cont->p_params);
  return(TRUE);
}

void modificator_poly::poly_line(MODIFICATOR_TARGET_STACK *p_target,
                                 MODIFICATOR_PARAMS *p_params,
                                 VECT2DI p0, VECT2DI p1,
                                 VECT2DI p2, VECT2DI p3)
{
  POLY_CONT cont = { this, p_target, p_params };
  float tension = -(corner_curvature - 0.5f)*2;
  spline_emit(p0, p1, p2, p3, 0, 0, tension,
              modificator_poly_poly_line_point_target, &cont);
}

bool modificator_poly_poly_line_point_coord(tpos x, tpos y, float pos, void *p_param)
{
  RECT2DI *p_coord = (RECT2DI *)p_param;  
  p_coord->extend(x,y);
  return(TRUE);
}

void modificator_poly::poly_line_coord(RECT2DI *p_coord,
                                       VECT2DI p0, VECT2DI p1,
                                       VECT2DI p2, VECT2DI p3)
{
  float tension = -(corner_curvature - 0.5f)*2;
  spline_emit(p0, p1, p2, p3, 0, 0, tension,
              modificator_poly_poly_line_point_coord, p_coord);
}

void modificator_poly::poly_init(RECT2DI pos)
{
  mask_shift.x = pos.x;
  mask_shift.y = pos.y;
  mask.create(pos.dx, pos.dy);
}

void modificator_poly::poly_clear(void)
{
  mask.clear();
  mask_shift.x = mask_shift.y = 0;
}
  
modificator_poly::modificator_poly(GENERATOR *p_gen, MODIFICATOR_TYPE type)
  : modificator_pixel_pattern(p_gen, type)
{
  height_profile = POLY_PROFILE_ELLIPSE;
  corner_curvature = 0.4f;

  height_center = 0;
  height_left = 0;
  height_top = 0;
  border = FALSE;
  
  mask_shift.x = 0;
  mask_shift.y = 0;
}

/*
  Line leaf generator
*/

/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_LINE_LEAF
}
*/
void modificator_line_leaf::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator_pixel_pattern::load(p_section);
  //load_texture_light_pattern(p_pattern, p_section);
  
  do {
    if(p_line->is_line()) {      
      load_interval_float(leaf_start, "leaf_start");
      load_interval_float(leaf_stop, "leaf_stop");
      
      load_interval_float(leaf_width_start, "leaf_width_start");
      load_interval_float(leaf_width_end, "leaf_width_end");
      
      load_angle(leaf_thread_angle, "leaf_thread_angle");
      
      load_int(leaf_fill, "leaf_fill");      
    }
  } while((p_line = p_line->section_next()));
}

typedef struct modificator_line_leaf_coord
{
  MODIFICATOR_LINE_LEAF *p_poly;
  bool                   inside;
  int                    changes;
  VECT2DI                start;
  VECT2DI                end;
  
  modificator_line_leaf_coord(MODIFICATOR_LINE_LEAF *p_poly,
                              VECT2DI start, VECT2DI end)
  {
    this->p_poly = p_poly;    
    this->start = start;
    this->end = end;
    inside = FALSE;
    changes = 0;
  }
  
} MODIFICATOR_LINE_LEAF_COORD;

bool modificator_line_leaf_line_emit_check(tpos x, tpos y, float pos, void *p_param)
{
  MODIFICATOR_LINE_LEAF_COORD *p_crd = (MODIFICATOR_LINE_LEAF_COORD *)p_param;  
  
  bool set = p_crd->p_poly->poly_mask_get(x,y);
  if(!p_crd->inside) { // outside
    if(set) {
      p_crd->inside = TRUE;
      p_crd->start.set(x,y);
      p_crd->changes++;
    }
  }  
  else { //inside
    if(!set) {
      p_crd->inside = FALSE;
      p_crd->end.set(x,y);
      p_crd->changes++;
    }    
  }
  
  return(p_crd->changes < 3);
}

void modificator_line_leaf::line_emit(MODIFICATOR_TARGET_STACK *p_target,
                                      VECT2DI start,
                                      VECT2DI end,
                                      MODIFICATOR_PARAMS *p_params)
{
  // check the mask -> for maximal line lenght
  MODIFICATOR_LINE_LEAF_COORD crd(this, start, end);
  
  ::line_emit(start, end, modificator_line_leaf_line_emit_check, &crd);
/*  
  pprintf("   start = (%d,%d), end = (%d,%d) => start = (%d,%d), end = (%d,%d)",
              start.x, start.y, end.x, end.y, 
              crd.start.x, crd.start.y, crd.end.x, crd.end.y);
*/  
  target_apply(p_target, RECT2DI(crd.start, crd.end), p_params);
}

void modificator_line_leaf::apply_leaf_line(MODIFICATOR_TARGET_STACK *p_target,
                                            MODIFICATOR_PARAMS *p_params,
                                            VECT2DI start,                                            
                                            VECT2DF direction_l,
                                            VECT2DF direction_r,
                                            float line_length,
                                            float leaf_position)
{ 
  VECT2DI end_l = start + VECT2DI(direction_l*line_length);
  VECT2DI end_r = start + VECT2DI(direction_r*line_length);
    
  // Param_0 is relative position of this thread from 0 to 1
  int index = p_params->push(leaf_position);

  line_emit(p_target, start, end_l, p_params);
  line_emit(p_target, start, end_r, p_params);
/*  
  pprintf("start = (%d,%d), end_l = (%d,%d), end_r = (%d,%d)",
           start.x, start.y, end_l.x, end_l.y, end_r.x, end_r.y);
*/  
  p_params->clear(index);
}

/*
 * Apply leaf
 */
bool modificator_line_leaf::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  RECT2DI coord;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&coord)) {
    target_pre_apply(p_target, coord, p_params);
    
    target_init(p_target, coord, p_params);
    
    VECT2DI start(coord);
    VECT2DI end = coord.end_get();
    
    int dx = coord.dx;
    int dy = coord.dy;
    
    // Basic leaf paramaters
    VECT2DF leaf_direction(dx,dy);
    VECT2DF leaf_direction_norm(dx,dy);
    float   leaf_length = leaf_direction_norm.norm();
    VECT2DF leaf_center = VECT2DF(start) + leaf_direction/2;
    
    VECT2DF start_v = leaf_center-VECT2DF(start);
    VECT2DF end_v = VECT2DF(end)-leaf_center;
    
    start_v.norm();
    end_v.norm();
    
    VECT2DF start_l = start_v, end_l = end_v;
    VECT2DF start_r = start_v, end_r = end_v;
    VECT2DF sl, el;
    VECT2DF sr, er;

    float   angle = (PI/2);    
    start_l.rotate(-angle);
    start_r.rotate(angle);
    end_l.rotate(-angle);
    end_r.rotate(angle);
    
    // load from interval, relative width
    // from 0 to 1    
    float vect_size_start = leaf_length*leaf_width_start;
    float vect_size_end = leaf_length*leaf_width_end;
    
    sl = start_l*vect_size_start + VECT2DF(start);
    el = end_l*vect_size_end + VECT2DF(end);
        
    sr = start_r*vect_size_start + VECT2DF(start);
    er = end_r*vect_size_end + VECT2DF(end);
    
    RECT2DI tmp = coord;
    
    poly_line_coord(&coord,sl,start,end,el);
    poly_line_coord(&coord,sr,start,end,er);
    
/*
    pprintf("Extended (%d,%d)->(%d,%d) to (%d,%d)->(%d,%d)",
            tmp.x,tmp.y,tmp.dx,tmp.dy,
            coord.x,coord.y,coord.dx,coord.dy);
*/

    // We need a reserve for filling
    coord.extend(2);

    // Generate a mask
    poly_init(coord);
    assert(poly_mask_loaded());

    poly_line(sl,start,end,el);
    poly_line(sr,start,end,er);
    
    poly_fill_mask(coord);
    
    if(leaf_fill) {
      tpos x,y;
      for(y = 0; y < coord.dy; y += pixel_step_y_get()) {
        for(x = 0; x < coord.dx; x += pixel_step_x_get()) {
          if(poly_mask_get(coord.x+x,coord.y+y)) {
            target_apply(p_target, RECT2DI(coord.x+x, coord.y+y), p_params);
          }
        }
      }      
    }
    else {
      float pixel_lenght;
      int   pixel_step;
  
      // dx is bigger, iterate it
      if(abs(dx) > abs(dy)) {
        pixel_lenght = fabs(dx);
        pixel_step = pixel_step_x_get();
      }
      else {
        pixel_lenght = fabs(dy);
        pixel_step = pixel_step_y_get();
      }
  
      float step = 1.0f/pixel_lenght;
      
      int pos;
      for(pos = 0; pos <= pixel_lenght; pos += pixel_step) {
        
        // Current position in the leaf <0,1>
        float current_leaf_position = step*pos;
        float abs_leaf_position = fabs(current_leaf_position);
        if(abs_leaf_position < leaf_start || abs_leaf_position > leaf_stop)
          continue;
  
        VECT2DI start_line(VECT2DF(leaf_direction*current_leaf_position));
        start_line += start;
        
        VECT2DF direction_l(leaf_direction_norm);
        VECT2DF direction_r(leaf_direction_norm);
    
        direction_l.rotate(leaf_thread_angle);
        direction_r.rotate(-leaf_thread_angle);
  
        apply_leaf_line(p_target, p_params, 
                        start_line, direction_l, direction_r,
                        leaf_length, abs_leaf_position);
      }
    }
    poly_clear();  
    
    target_post_apply(p_target, coord, p_params);
  }
  
  return(TRUE);
}

modificator_line_leaf::modificator_line_leaf(GENERATOR *p_gen)
  : modificator_poly(p_gen, MODIFICATOR_LINE_LEAF_TYPE)
{    
  leaf_start = -0.4f;
  leaf_stop = 0.9f;
  
  leaf_width_start = 1;
  leaf_width_end = 1;
  
  leaf_thread_angle = PI/4; // 45 degrees
}

modificator_line_leaf::~modificator_line_leaf(void)
{
  
}

/*
 * Apply bunch
 */
/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_BUNCH
}
*/
void modificator_bunch::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator_pixel_pattern::load(p_section);

  do {
    if(p_line->is_line()) {
      
      load_float(bunch_height,"height");
      
      load_float(height_center,"height_correction_center");
      load_float(height_left,"height_correction_left");
      load_float(height_top,"height_correction_top");      
      
      load_interval_float(corner_curvature,"corner_curvature");
      
      load_interval_int(bunch_points, "points");
      load_interval_float(bunch_length, "length");
      
      load_float(bunch_angle, "angle");
      load_int(border, "border");
    }
  } while((p_line = p_line->section_next()));
}

//point_num
int get_point(int point, int point_num)
{
  assert(point_num > 2);
  
  if(point < 0)
    point += point_num;
  if(point > (point_num-1))
    point -= point_num;
  
  return(point);
}

void modificator_bunch::apply_bunch(MODIFICATOR_TARGET_STACK *p_target,
                                    RECT2DI pos,
                                    MODIFICATOR_PARAMS *p_params)
{
  VECT2DF center = pos.center_get();
  VECT2DF direction(-1,1);
  direction.norm();
  
  //pprintf("Pos = %d x %d -> %d x %d",pos.x, pos.y, pos.dx, pos.dy);
  
  int points = bunch_points;
  float angle = (2*PI)/points;
  int length = (VECT2DF(pos.dx/2,pos.dy/2)).size();
  
  VECT2DI *p_points = new VECT2DI[points];
  
  int i;
  for(i = 0; i < points; i++) {
    if(bunch_angle != 0) {
      angle += 2*PI*random_generator::generator_gauss_0()*bunch_angle;
    }
    direction.rotate(angle);
    
    // Reduce points num
    if(angle >= 2*PI) {
      points = i+1;
      break;
    }
    
    p_points[i] = center + (direction*(length*bunch_length));
  }
  
  // Generate the mask
  poly_init(pos);
  assert(poly_mask_loaded());
  
  for(i = 0; i < points; i++) {
    int p0 = get_point(i-1, points);
    int p1 = get_point(i,   points);
    int p2 = get_point(i+1, points);
    int p3 = get_point(i+2, points);
    
    poly_line(p_points[p0],
              p_points[p1],
              p_points[p2],
              p_points[p3]);
  }
  
  // Transformate local height range to the global one  
  int size = (VECT2DF(pos.dx,pos.dy)).size();
  float height_maximal = (size*bunch_height)/(p_target->get_first())->target_height_get();
  
  poly_fill(p_target, pos, p_params, height_maximal);
  
  poly_clear();
  
  delete p_points;
}

bool modificator_bunch::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{  
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  RECT2DI pos;
  p_coord->coord_final_get_start();
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    apply_bunch(p_target, pos, p_params);
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

modificator_bunch::modificator_bunch(GENERATOR *p_gen)
  : modificator_poly(p_gen, MODIFICATOR_BUNCH_TYPE)
{
  height_profile = POLY_PROFILE_ELLIPSE;
    
  corner_curvature = 0.4f;  
  
  bunch_points = 4;
  bunch_length = 0.75f;
  bunch_angle = 0.01;
  bunch_height = 1;
}

modificator_bunch::~modificator_bunch(void)
{
  
}

/*
 * Pattern net
 */
class brick_generator;
typedef void (brick_generator::*AREA_OPERATOR)(int *p_brick, int param);

typedef class brick_generator
{
/*  
  Not used yet
  
  #define BRICK_ID_GET(b)       (b&0xffffff)
  #define BRICK_ID_SET(b,id)    ((b&0xffffff) | id)
  
  #define BRICK_MASK_GET(b)     (b&0xff000000)
  #define BRICK_MASK_SET(b,m)   ((b&0xff000000) | m)
*/  
  
  int *p_bricks;
  
  int  nx;
  int  ny;
   
  int  max_size_x;
  int  max_size_y;
  
private:
  
  float join_probability;
  float join_probability_coeficient;

public:
  
  void  join_config(float probability, float probability_coeficient)
  {
    join_probability = probability;
    join_probability_coeficient = probability_coeficient;    
  }
  
private:
  
  bool  zip;

public:
  
  void  zip_config(bool enabled)
  {
    zip = enabled;
  }

public:
  
  // Bricks of the wall
  bool  valid(int x, int y)
  {
    return(x >= 0 && y >= 0 && x < nx && y < ny);
  }
  
  int * get(int x, int y)
  {
    return(valid(x,y) ? p_bricks+y*nx+x : NULL);
  }
  
  int id_get(int x, int y)
  {
    int *p_id = get(x,y);
    return(p_id ? *p_id : -1);
  }
  
  bool  is_different(int brick, int x2, int y2)
  {
    int *p_b2 = get(x2,y2);

    if(p_b2 == NULL)
      return(TRUE);
    
    return(brick != *p_b2);
  }
  
  bool  is_different(int x1, int y1, int x2, int y2)
  {
    int *p_b1 = get(x1,y1);
    int *p_b2 = get(x2,y2);
    
    if(p_b1 == NULL || p_b2 == NULL)
      return(TRUE);
    
    return(*p_b1 != *p_b2);
  }
  
  bool  is_different(VECT2DI p1, VECT2DI p2)
  {
    return(is_different(p1.x,p1.y,p2.x,p2.y));
  }
  
  bool brick_process(int brick_base,
                     int x, int y, 
                     AREA_OPERATOR op = NULL, int param = 0)
  {
    int *p_brick = get(x,y);
    if(!p_brick)
      return(FALSE);
    
    bool end = is_different(brick_base,x,y);
    
    if(end)
      return(FALSE);
    
    if(op)
      (this->*op)(p_brick, param);
    
    return(TRUE);
  }
  
  int area_process(int x, int y, AREA_OPERATOR op = NULL, int param = 0)
  {
    int *p_brick = get(x,y);
    if(!p_brick) {
      return(0);
    }
    
    int base = *p_brick;
    
    int ax,sx = x - max_size_x;
    int ay,sy = y - max_size_y;
    int size = 0;
    
    for(ay = 0; ay < max_size_y*2; ay++) {
      for(ax = 0; ax < max_size_x*2; ax++) {
        p_brick = get(sx + ax, 
                      sy + ay);
        if(p_brick && *p_brick == base) {
          size++;
          if(op)
            (this->*op)(p_brick,param);
        }
      }
    }
    
    return(size);
  }  
  
  bool can_join(int x1, int y1, int x2, int y2)
  {
    int *p_b1 = get(x1,y1);
    int *p_b2 = get(x2,y2);
    
    return(p_b1 != NULL && p_b2 != NULL);
  }
  
  void join_operator(int *p_brick, int param)
  {
    *p_brick = param;
  }
  
  bool join_allowed(int size1, int size2)
  {
    int size = size1+size2-2;
    assert(size >= 0);
    float probability = join_probability*(size ? join_probability_coeficient*size : 1);
    return(random_generator::generator_rand() < probability);
  }
  
  void area_join(int x1, int y1, int x2, int y2)
  {
    // Can we join the two bricks?
    if(!can_join(x1,y1,x2,y2))
      return;
    
    // Are they different?
    if(!is_different(x1,y1,x2,y2))
      return;
    
    int size1 = area_process(x1,y1);
    int size2 = area_process(x2,y2);
    
    // join area1 and area2
    if(join_allowed(size1, size2))
      area_process(x2,y2,&brick_generator::join_operator,*(get(x1,y1)));
  }
  
  // process given area
  // search_x / search_y - search directions
  int  line_process(int x, int y, 
                    bool search_x, bool search_y, 
                    bool op_different = FALSE, AREA_OPERATOR op = NULL, int param = 0)
  {
    int *p_brick = get(x,y);
    
    if(!p_brick) {
      return(0);
    }
    
    int brick_base = *p_brick;
    int size = 0;
    int i;
    
    // search +-x
    if(search_x) {
      for(i = x+1; ; i++) {
        if(!brick_process(brick_base,i,y,op,param))
          break;
        size++;
      }      
      for(i = x-1; i >= 0; i--) {
        if(!brick_process(brick_base,i,y,op,param))
          break;
        size++;
      }
    }
        
    // search +-y
    if(search_y) {
      for(i = y+1; ; i++) {
        if(!brick_process(brick_base,x,i,op,param))
          break;
        size++;
      }
      for(i = y-1; i >= 0; i--) {
        if(!brick_process(brick_base,x,i,op,param))
          break;
        size++;
      }
    }
    
    if(op)
      (this->*op)(p_brick, param);
    size++;
    
    return(size);
  }

  void line_join(int x1, int y1, int x2, int y2, bool search_x, bool search_y)
  {
    // Can we join the two bricks?
    if(!can_join(x1,y1,x2,y2))
      return;
    
    // Are they different?
    if(!is_different(x1,y1,x2,y2))
      return;
    
    // Process given lines
    int size1 = line_process(x1,y1,search_x,search_y);
    int size2 = line_process(x2,y2,search_x,search_y);
    
    // join area1 and area2
    if(join_allowed(size1, size2))
      line_process(x2,y2,search_x,search_y,TRUE,&brick_generator::join_operator,*(get(x1,y1)));
  }
  
public:

  void print(void)
  {
    int x,y;
    for(y = 0; y < ny; y++) {
      for(x = 0; x < nx; x++) {
        printf("%d ",id_get(x,y));
      }
      printf("\n");
    }
  }
  
public:

  #define DEFAULT_MAX_BRICK_SIZE 10

  brick_generator(int width, int height,
                  int width_max = DEFAULT_MAX_BRICK_SIZE,
                  int height_max = DEFAULT_MAX_BRICK_SIZE)
  {
    nx = width;
    ny = height;
    
    max_size_x = width_max;
    max_size_y = height_max;
    
    p_bricks = (int *)mmalloc(sizeof(int)*ny*nx);
    int i;
    for(i = 0; i < nx*ny; i++)
      p_bricks[i] = i;
  }
  
  ~brick_generator(void)
  {
    ffree(p_bricks);
  }

} BRICK_GENERATOR;


void modificator_brick::apply_net(MODIFICATOR_TARGET_STACK *p_target,
                                  RECT2DI pos,
                                  MODIFICATOR_PARAMS *p_params)
{
  // Check the values
  assert(brick_width > 0);
  assert(brick_height > 0);
  assert(pos.dx > 0);
  assert(pos.dy > 0);
  
  int bx = pos.dx / brick_width;
  int by = pos.dy / brick_height;
  
  BRICK_GENERATOR bricks(bx,by,brick_width_max,brick_height_max);
  
  if(brick_width_max > 1) {
    bricks.join_config(brick_width_join_pobability,
                       brick_width_join_pobability_multiplier);
    bricks.zip_config(brick_width_zip);
    int x,y;
    for(y = 0; y < by; y++) {
      for(x = 1; x < bx; x++) {
        bricks.line_join(x-1, y, x, y, TRUE, FALSE);
      }
    }
  }
  
  if(brick_height_max > 1) {
    bricks.join_config(brick_height_join_pobability,
                       brick_height_join_pobability_multiplier);    
    bricks.zip_config(brick_height_zip);
    int x,y;
    for(x = 0; x < bx; x++) {
      for(y = 1; y < by; y++) {
        bricks.line_join(x, y-1, x, y, FALSE, TRUE);
      }
    }
  }
  
  //bricks.print();
  
  /*
   *  Vizualize the bricks
   */
  // Generate corner points
  int px = bx+1;
  int py = by+1;
  
  VECT2DI *p_points = (VECT2DI *)mmalloc(sizeof(VECT2DI)*py*px);

  // Border points of every brick
  #define POINT(x,y)      (p_points+(y)*px+(x))
  #define POINT_UNDEFINED INT_UNDEFINED
  
  // Initialize points
  int x,y;
  for(x = 0; x < px; x++) {
    for(y = 0; y < py; y++) {
      // Initial indexes of bricks (each brick is 1x1)
      VECT2DI *p_point = POINT(x,y);
      p_point->x = pos.x+x*brick_width;
      p_point->y = pos.y+y*brick_height;
      
      // Apply scattering
      p_point->x += random_generator::generator_gauss_0()*brick_width*brick_width_scatter;
      p_point->y += random_generator::generator_gauss_0()*brick_height*brick_height_scatter;
    }
  }
  
  // Draw borders
  for(x = 1; x < px; x++) {
    VECT2DI *p1 = POINT(x-1,0);
    VECT2DI *p2 = POINT(  x,0);
    target_apply(p_target, RECT2DI(*p1,*p2),p_params);
    
    VECT2DI *p3 = POINT(x-1,py-1);
    VECT2DI *p4 = POINT(  x,py-1);
    target_apply(p_target, RECT2DI(*p3,*p4),p_params);
  }
  
  for(y = 1; y < py; y++) {
    VECT2DI *p1 = POINT(0,y-1);
    VECT2DI *p2 = POINT(0,y);
    target_apply(p_target, RECT2DI(*p1,*p2),p_params);
    
    VECT2DI *p3 = POINT(px-1,y-1);
    VECT2DI *p4 = POINT(px-1,y);
    target_apply(p_target, RECT2DI(*p3,*p4),p_params);
  }

  // Fill bricks in horizontal orientation
  for(y = 0; y < by; y++) {
    for(x = 1; x < bx; x++) {
      if(bricks.is_different(x-1,y,x,y)) {
        VECT2DI *p1 = POINT(x,y);
        VECT2DI *p2 = POINT(x,y+1);
        target_apply(p_target, RECT2DI(*p1,*p2),p_params);
      }
    }
  }
  
  // Fill bricks in vertical orientation
  for(x = 0; x < bx; x++) {
    for(y = 1; y < by; y++) {
      if(bricks.is_different(x,y-1,x,y)) {
        VECT2DI *p1 = POINT(x,y);
        VECT2DI *p2 = POINT(x+1,y);
        target_apply(p_target, RECT2DI(*p1,*p2),p_params);
      }
    }
  }
  
  ffree(p_points);
}

/*
 * Apply net
 */
bool modificator_brick::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{  
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    apply_net(p_target, pos, p_params);
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

/*
{
  name = some_modificator_name
  type = TEXTURE_MODIFICATOR_PATTERN_NET
}
*/
void modificator_brick::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_pixel_pattern::load(p_section);
  //  load_texture_light_pattern(p_net, p_section);
  
  do {
    if(p_line->is_line()) {
      
      load_int(brick_corners, "brick_corners");
      
      load_int(brick_width, "brick_width");
      load_int(brick_height, "brick_height");
      
      load_float(brick_width_scatter, "brick_width_scatter");
      load_float(brick_height_scatter, "brick_height_scatter");
      
      load_int(brick_width_max, "brick_width_max");
      load_int(brick_height_max, "brick_height_max");
      
      load_int(brick_width_zip, "brick_width_zip");
      load_int(brick_height_zip, "brick_height_zip");
      
      load_float(brick_width_join_pobability,
                "brick_width_join_pobability");
      load_float(brick_width_join_pobability_multiplier,
                "brick_width_join_pobability_multiplier");
      
      load_float(brick_height_join_pobability, 
                "brick_height_join_pobability");
      load_float(brick_height_join_pobability_multiplier,
                "brick_height_join_pobability_multiplier");
    }
  } while((p_line = p_line->section_next()));
}

modificator_brick::modificator_brick(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_BRICK_TYPE)
{
  brick_corners = 4;
  
  // Some reasonable brick sizes (in pixels)
  brick_width = 50;
  brick_height = 30;
  
  brick_width_scatter = 0.0f;
  brick_height_scatter = 0.0f;
  
  brick_width_max = DEFAULT_MAX_BRICK_SIZE;
  brick_height_max = DEFAULT_MAX_BRICK_SIZE;
  
  brick_width_zip = FALSE;
  brick_height_zip = FALSE;
  
  brick_width_join_pobability = 0.0f;
  brick_width_join_pobability_multiplier = 0.0f;
  
  brick_height_join_pobability = 0.0f;
  brick_height_join_pobability_multiplier = 0.0f;
}

modificator_brick::~modificator_brick(void)
{
}


/*
  area pattern:
  MODIFICATOR_HEIGHTMAP
*/
bool modificator_height_map::heightmap_load_waiting(void)
{
  // Mask is already loaded
  if(heightmap_loaded())
    return(FALSE);

  // File is set
  if(heightmap_file[0])
    return(TRUE);

  // Mask target is set
  if(target_type != MODIFICATOR_TARGET_NONE)
    return(TRUE);
  
  // Mask size is set
  if(heightmap_size.x > 0 && heightmap_size.y > 0)
    return(TRUE);
  
  // Mask is not loaded and it's not configured
  return(FALSE);
}

// Is heighmap loaded?
bool modificator_height_map::heightmap_loaded(void)
{
  return(heightmap.loaded());
}

/*  
*/
void modificator_height_map::heightmap_load(char *p_file)
{
  strncpy(heightmap_file, p_file, MAX_FILENAME);
}

// Set target_type & target_name and leave the heightmap for
// delayed loading
void modificator_height_map::heightmap_load(char *p_target, char *p_name)
{  
  target_type = target_type_translate(p_target, MODIFICATOR_TARGET_NONE);
  
  if(p_name) {
    strncpy(target_name, p_name, MAX_NAME);
  }
  else {
    target_name[0] = '\0';
  }
}

void modificator_height_map::heightmap_load(tpos width, tpos height)
{
  heightmap_size.x = width;
  heightmap_size.y = height;
}

bool modificator_height_map::heightmap_create(SURFACE_HEIGHT *p_surf)
{
  if(p_surf->loaded()) {
    heightmap.move(p_surf);
    return(TRUE);
  }
  return(FALSE);
}

bool modificator_height_map::heightmap_create(char *p_file)
{
  SURFACE_HEIGHT bitmap;
  bitmap.load(p_file);
  return(heightmap_create(&bitmap));
}

bool modificator_height_map::heightmap_create(MODIFICATOR_TARGET_AUX *p_map)
{ 
  if(p_map->heightmap_loaded()) {
    SURFACE_HEIGHT *p_surf = p_map->heightmap_get();
    if(p_surf->loaded()) {    
      heightmap.copy(p_surf);
      return(TRUE);
    }
  }
  return(FALSE);
}

bool modificator_height_map::heightmap_create(MODIFICATOR_TARGET_GEOMETRY *p_map)
{
  SURFACE_HEIGHT *p_surf = p_map->heightmap_get();
  if(p_surf->loaded()) {
    heightmap.copy(p_surf);
    return(TRUE);
  }
  return(FALSE);
}

bool modificator_height_map::heightmap_create(tpos width, tpos height)
{
  heightmap_remove();
  heightmap.create(width, height);
  return(heightmap.loaded());
}

// Load the heightmap, set scale 
bool modificator_height_map::heightmap_create(MODIFICATOR_TARGET_TYPE type, char *p_name)
{ 
  if(heightmap.loaded())
    return(TRUE); 
  
  // We don't have to load it
  if(target_type == MODIFICATOR_TARGET_NONE)
    return(TRUE);
  
  MODIFICATOR_TARGET_CACHE *p_cache = generator_get()->current_target_cache_get();
  
  // if p_cache == NULL it's called too early
  assert(p_cache != NULL);
      
  MODIFICATOR_TARGET *p_map = p_cache->target_find(target_type, target_name);
  if(!p_map) {
    return(FALSE);
  }
      
  switch(target_type) {
    case MODIFICATOR_TARGET_TEXTURE_TYPE:
      return(heightmap_create((MODIFICATOR_TARGET_AUX *)p_map));
    case MODIFICATOR_TARGET_GEOMETRY_TYPE:
      return(heightmap_create((MODIFICATOR_TARGET_GEOMETRY *)p_map));
    case MODIFICATOR_TARGET_GENERATOR_TYPE:      
      break;
    case MODIFICATOR_TARGET_AUX_TYPE:
      return(heightmap_create((MODIFICATOR_TARGET_AUX *)p_map));
    default:
      break;
  }
  
  return(FALSE);
}

bool modificator_height_map::heightmap_create(void)
{
  if(heightmap.loaded())
    return(TRUE);
     
  // Priorities are:  
  // 1) From file
  if(heightmap_file[0]) {
    return(heightmap_create(heightmap_file));
  }
  
  // 2) From target
  if(target_type != MODIFICATOR_TARGET_NONE) {
    return(heightmap_create(target_type, target_name));
  }

  // 3) From coordinates
  if(heightmap_size.x != 0 && heightmap_size.y != 0) {
    return(heightmap_create(heightmap_size.x, heightmap_size.y));
  }
  
  // Should not happen
  pperror(TRUE, "MODIFICATOR_HEIGHTMAP(%s): Unable to load heightmap, no source is defined!", name_get());
  return(FALSE);
}

void modificator_height_map::heightmap_remove_target(void)
{
  // Is it loaded from target?
  if(target_type == MODIFICATOR_TARGET_NONE)
    return;
  
  // Free height data because we want a fresh one
  // for next run
  heightmap_remove();
}

void modificator_height_map::heightmap_target_scale(MODIFICATOR_TARGET *p_target)
{ 
  // Is scale imported from target?
  if(heightmap_scale_target) {    
    RECT2DI target = p_target->target_area_get();
    
    // Scale heightmap to target size
    height_scale_width = target.dx;
    height_scale_height = target.dy;    
    heightmap_scale = TRUE;
  }  
}

bool modificator_height_map::height_set(tpos x, tpos y, float height)
{    
  if(!heightmap_create())
    return(FALSE);
  
  if(heightmap_scale) {
    assert(height_scale_width > 0 && height_scale_height > 0);
    x = x*((float)heightmap.width_get()/(float)height_scale_width);
    y = y*((float)heightmap.height_get()/(float)height_scale_height);
  }  
  
  heightmap.height_set(x, y, height);
  return(TRUE);
}

float modificator_height_map::height_get(tpos x, tpos y)
{  
  float height = HEIGHT_UNSET;
  
  if(heightmap_create()) {  
    if(heightmap_scale) {
      assert(height_scale_width > 0 && height_scale_height > 0);
      x = x*((float)heightmap.width_get()/(float)height_scale_width);
      y = y*((float)heightmap.height_get()/(float)height_scale_height);
    }
    height = heightmap.height_get(x, y, HEIGHT_UNSET);
  }
  
  return(height);
}

void modificator_height_map::heightmap_remove(void)
{  
  heightmap.clear();
}

bool modificator_height_map::height_match(float height)
{ 
  if(height_range_min != FLOAT_UNDEFINED && height < height_range_min)
    return(FALSE);
  if(height_range_max != FLOAT_UNDEFINED && height > height_range_max)
    return(FALSE);
  
  return(TRUE);
}
  
bool modificator_height_map::intensity_match(float intensity)
{
  if(intensity_range_min != FLOAT_UNDEFINED && intensity < intensity_range_min)
    return(FALSE);
  if(intensity_range_max != FLOAT_UNDEFINED && intensity > intensity_range_max)
    return(FALSE);
  
  return(TRUE);
}

float modificator_height_map::height_translate(float height)
{
  if(height_range_min != FLOAT_UNDEFINED && height_range_max != FLOAT_UNDEFINED) {
    float height_translated = (height - height_range_min) / (height_range_max - height_range_min);
    height = height_translated*height_multiplier + height_shift;
  }
  return(height);
}

float modificator_height_map::intensity_translate(float intensity)
{
  if(intensity_range_min != FLOAT_UNDEFINED && intensity_range_max != FLOAT_UNDEFINED) {
    float intensity_translated = (intensity - intensity_range_min) / (intensity_range_max - intensity_range_min);
    intensity = intensity_translated*intensity_multiplier + intensity_shift;
  }
  return(intensity);
}

bool modificator_height_map::apply_heighmap(MODIFICATOR_TARGET_STACK *p_target,
                                            RECT2DI pos,
                                            MODIFICATOR_PARAMS *p_params)                                            
{  
  int sx = pos.x;
  int sy = pos.y;
  int map_width = pos.dx;
  int map_height = pos.dy;

  float texture_height = heightmap.height_range_get();
  
  VECT3DF light_vector;
  VECT3DF light(0, 1, 0);
  VECT3DF center(0, 0, 0);
  light_vector = light - center;
  light_vector.norm();
  
  // Populate height & colors
  int x,y;
  for(y = 0; y < map_height; y += pixel_step_y) {
    for(x = 0; x < map_width; x += pixel_step_x) {
      if(!mask_get(x,y)) {
        continue;
      }
      
      float height = height_get(x,y);
            
      // Skip unset pixels
      if(height == HEIGHT_UNSET) {
        continue;
      }
      
      if(!height_match(height)) {
        continue;
      }
      
      float pixel_height = height*texture_height;
      VECT pixel_position(x,pixel_height,y);
      VECT pixel_normal = heightmap.height_normal_get(x, y,
                          mask_active() ? this : NULL, sx, sy);
      float intensity = light_intensity_get(&light_vector, &pixel_normal);
      
      if(!intensity_match(intensity)) {
        continue;
      }            
      
      int index = p_params->push(intensity);
      p_params->push(height);
      p_params->push(intensity_translate(intensity));
      p_params->push(height_translate(height));
      target_apply(p_target, RECT2DI(pos.x+x,pos.y+y),p_params);
      p_params->clear(index);
    }
  }

  return(TRUE);
}

bool modificator_height_map::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  // Load the heightmap from target if requested
  if(!heightmap_create())
    return(FALSE);
  
  // Scale heightmap to first target
  heightmap_target_scale(p_target->get_first());
  
  // Apply it
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start,size;
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    apply_heighmap(p_target, pos, p_params);
    
    target_post_apply(p_target, pos, p_params);
  }
  
  // Free the heightmap loaded from target
  heightmap_remove_target();
  
  return(TRUE);
}

/*
{
  name = some_modificator_name
  type = MODIFICATOR_HEIGHT_MAP
}
*/
void modificator_height_map::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_pixel_pattern::load(p_section);  
//  load_texture_light_pattern(p_mid, p_section);
    
  char file[MAX_FILENAME] = "";  
  char target_type[MAX_NAME] = "";
  char target_type_name[MAX_NAME] = "";
  VECT2DI size(0);
  
  do {
    if(p_line->is_line()) {
      
      // Load heighmap from file
      if(p_line->match_name("height_bitmap")) {
        strncpy(file, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();
        continue;
      }
      
      // Load heighmap from generator (from target)
      if(p_line->match_name("height_source")) {
        strncpy(target_type, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();
        continue;
      }      
      if(p_line->match_name("height_source_name")) {
        strncpy(target_type_name, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();
        continue;
      }
      
      // Size of the heightmap for run-time creation
      load_vect2di(size, "height_size");
      load_vect2di(size, "heightmap_size");
     
      int tmp;
      load_int(tmp, "heightmap_intensity");
      
      load_float(height_multiplier, "height_multiplier");
      load_float(height_shift, "height_shift");
      load_float(height_range_min, "height_range_min");
      load_float(height_range_max, "height_range_max");
      
      load_float(height_multiplier, "intensity_multiplier");
      load_float(height_shift, "intensity_shift");
      load_float(height_range_min, "intensity_range_min");
      load_float(height_range_max, "intensity_range_max");
            
      load_int(heightmap_scale_target, "scale_target");
      load_int(height_scale_width, "scale_width");
      load_int(height_scale_height, "scale_height");
    }
  } while((p_line = p_line->section_next()));
  
  if(height_scale_width > 0 && height_scale_height > 0)
  {
    heightmap_scale = TRUE;
  }  
  
  if(height_range_min != FLOAT_UNDEFINED && height_range_max == FLOAT_UNDEFINED)
    height_range_max = 1.0f;
  if(height_range_max != FLOAT_UNDEFINED && height_range_min == FLOAT_UNDEFINED)
    height_range_min = 0.0f;
  
  if(intensity_range_min != FLOAT_UNDEFINED && intensity_range_max == FLOAT_UNDEFINED)
    intensity_range_max = 1.0f;
  if(intensity_range_max != FLOAT_UNDEFINED && intensity_range_min == FLOAT_UNDEFINED)
    intensity_range_min = 0.0f;
  
  // Try to load the mask
  if(file[0]) {
    heightmap_load(file);
  } else if(target_type[0]) {
    heightmap_load(target_type, target_type_name);
  } else if(size.x != 0 && size.y != 0) {
    heightmap_load(size.x, size.y);
  }  
}

modificator_height_map::modificator_height_map(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_HEIGHTMAP_TYPE)
{ 
  
  target_type = MODIFICATOR_TARGET_NONE;
  target_name[0] = '\0';  
  
  height_multiplier = 1.0f;
  height_shift = 0.0f;
  
  intensity_multiplier = 1.0f;
  intensity_shift = 0.0f;

  // Publication ranges
  height_range_min  = FLOAT_UNDEFINED;
  height_range_max  = FLOAT_UNDEFINED;
  intensity_range_min  = FLOAT_UNDEFINED;
  intensity_range_max  = FLOAT_UNDEFINED;
  
  heightmap_scale = FALSE;
  height_scale_width = 0;
  height_scale_height = 0;
}

modificator_height_map::~modificator_height_map(void)
{
  heightmap_remove();
}

/*
  area pattern:
  MODIFICATOR_BITMAP
*/
bool modificator_bitmap::bitmap_load(SURFACE_SW *p_surf)
{
  if(p_surf->loaded()) {    
    bitmap.move(p_surf);
    return(TRUE);
  }
  return(FALSE);
}

bool modificator_bitmap::bitmap_load(char *p_file)
{
  SURFACE_SW bitmap;
  bitmap.load(p_file);
  return(bitmap_load(&bitmap));
}

bool modificator_bitmap::bitmap_load(char *p_target, char *p_name)
{  
  target_type = target_type_translate(p_target);
  
  if(p_name) {
    strncpy(target_name, p_name, MAX_NAME);
  }
  else {
    target_name[0] = '\0';
  }
  
  return(TRUE);
}

bool modificator_bitmap::bitmap_load(MODIFICATOR_TARGET_AUX *p_map)
{ 
  if(p_map->bitmap_loaded()) {
    SURFACE_SW *p_surf = p_map->bitmap_get();
    if(p_surf->loaded()) {    
      bitmap.copy(p_surf);
      return(TRUE);
    }
  }
  return(FALSE);
}

bool modificator_bitmap::bitmap_load(MODIFICATOR_TARGET_TEXTURE *p_map)
{   
  SURFACE_SW *p_surf = p_map->bitmap_get();
  if(p_surf->loaded()) {    
    bitmap.copy(p_surf);
    return(TRUE);
  }
  return(FALSE);
}

// Load the heightmap, set scale 
bool modificator_bitmap::bitmap_load(void)
{ 
  /*
  if(bitmap.loaded()) {
    return(TRUE);
  }
  
  MODIFICATOR_TARGET_CACHE *p_cache = generator_get()->current_target_cache_get();
  
  // if p_cache == NULL it's called too early
  assert(p_cache != NULL);
      
  MODIFICATOR_TARGET *p_map = p_cache->target_find(target_type, target_name);  
  if(!p_map) {
    return(FALSE);
  }
  
  switch(target_type) {
    case MODIFICATOR_TARGET_TEXTURE_TYPE:
      return(bitmap_load((MODIFICATOR_TARGET_TEXTURE *)p_map));
    case MODIFICATOR_TARGET_GEOMETRY_TYPE:
      return(bitmap_load((MODIFICATOR_TARGET_GEOMETRY *)p_map));
      break;
    case MODIFICATOR_TARGET_GENERATOR_TYPE:      
      break;
    case MODIFICATOR_TARGET_AUX_TYPE:
      return(bitmap_load((MODIFICATOR_TARGET_AUX *)p_map));
    default:
      break;
  }
  */
  return(FALSE);
}

/*
float modificator_bitmap::height_get(tpos x, tpos y)
{  
  float height = HEIGHT_UNSET;
  
  if(heightmap_scale) {
    assert(height_scale_width > 0 && height_scale_height > 0);
    x = x*((float)heightmap.width_get()/(float)height_scale_width);
    y = y*((float)heightmap.height_get()/(float)height_scale_height);
  }
  
  height = heightmap.height_get(x, y, HEIGHT_UNSET);  
  return(height);
}
*/

void modificator_bitmap::bitmap_create(tpos width, tpos height)
{
  bitmap_destroy();
  bitmap.create(width, height);
}

void modificator_bitmap::bitmap_destroy(void)
{  
  bitmap.clear();
}

bool modificator_bitmap::apply_bitmap(MODIFICATOR_TARGET_STACK *p_target,
                                      RECT2DI pos,
                                      MODIFICATOR_PARAMS *p_params)
{  
  /*
  int sx = pos.x;
  int sy = pos.y;
  int width = pos.dx;
  int height = pos.dy;

  // Populate height & colors
  int x,y;
  for(y = 0; y < height; y += pixel_step_y) {
    for(x = 0; x < width; x += pixel_step_x) {
      if(!mask_get(x,y)) {
        continue;
      }
  
      float local_height = height_get(x,y);
      
      // Skip unset pixels
      if(local_height == HEIGHT_UNSET) {
        continue;
      }
      
      if(!height_match(local_height)) {
        continue;
      }

      int index = 0;
      
      if(heightmap_intensity) {
        float pixel_height = local_height*texture_height;
        VECT pixel_position(x,pixel_height,y);
        VECT pixel_normal = heightmap.height_normal_get(x, y,
                            mask_active() ? this : NULL, sx, sy);

        float intensity = light_intensity_get(&light_vector, &pixel_normal);
        
        index = p_params->push(intensity);
        p_params->push(local_height);
      }
      else {
        index = p_params->push(local_height);
      }
            
      p_params->push(height_translate(local_height));
      target_apply(p_target, RECT2DI(pos.x+x,pos.y+y),p_params);
      p_params->clear(index);

    }
  }
  */
  return(TRUE);
}

bool modificator_bitmap::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  // Load the heightmap if requested
  if(!bitmap_load())
    return(FALSE);
  
  // Apply it
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start,size;
  RECT2DI pos;
  p_coord->coord_final_get_start();
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    apply_bitmap(p_target, pos, p_params);
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

/*
{
  name = some_modificator_name
  type = MODIFICATOR_HEIGHT_MAP
}
*/
void modificator_bitmap::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_pixel_pattern::load(p_section);
    
  do {
    if(p_line->is_line()) {
      
      // Load heighmap from file
      if(p_line->match_name("height_bitmap")) {
        bitmap_load(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      
      // Load heighmap from generator (from target)
      if(p_line->match_name("height_source")) {
        bitmap_load(p_line->line_value_string_get(), NULL);
        p_line->line_mark_loaded();
        continue;
      }
    }
  } while((p_line = p_line->section_next()));  
}

modificator_bitmap::modificator_bitmap(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_BITMAP_TYPE)
{ 
}

modificator_bitmap::~modificator_bitmap(void)
{
  bitmap_destroy();
}

/*
  area pattern:
  MODIFICATOR_COLOR_TYPE_MID_POINT
*/
bool modificator_fractal::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start,size;
  RECT2DI pos;
  p_coord->coord_final_get_start();
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    
    VECT2DI size(pos.dx, pos.dy);
    
    heightmap_load(size.x, size.y);
    heightmap_create();
    heightmap.fill(HEIGHT_UNSET);
        
    heightmap.generate_mid_point(&height_config);
    
    modificator_height_map::apply_heighmap(p_target, pos, p_params);
        
    heightmap_remove();
    
    target_post_apply(p_target, pos, p_params);
  }
  return(TRUE);
}

/*
{
  type = TEXTURE_MODIFICATOR_PATTERN_MID_POINT
}
*/
void modificator_fractal::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_height_map::load(p_section);
//  load_texture_light_pattern(p_mid, p_section);
  
  do {
    if(p_line->is_line()) {
      load_float(height_config.generator_hurst,"fractal_hurst");

      load_float(height_config.generator_delta,"fractal_delta");
      load_float(height_config.generator_center,"fractal_center");
      
      load_float(height_config.correction_center, "correction_center");
      load_float(height_config.correction_border, "correction_border");
      
      load_int(height_config.pixel_filter_back, "filter_back");
      load_float(height_config.border_start_height, "border_start");
      load_float(height_config.perturbation, "perturbation");
      
      load_int(height_config.pixel_fill, "pixel_fill");
      load_int(height_config.pixel_distance, "pixel_distance");
      load_int(height_config.pixel_filter, "pixel_filter");
      load_int(height_config.pixel_filter_num, "pixel_filter_num");

      if(p_line->match_name("interpolation")) {        
        height_config.ifirst = height_config.isecond = 
        interpolation_type_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      if(p_line->match_name("interpolation_first")) {        
        height_config.ifirst = interpolation_type_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      if(p_line->match_name("interpolation_second")) {
        height_config.isecond = interpolation_type_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      load_int(height_config.iborder, "interpolation_border");      
      load_int(height_config.gborder, "generation_border");
    }
  } while((p_line = p_line->section_next()));
}

modificator_fractal::modificator_fractal(GENERATOR *p_gen)
  : modificator_height_map(p_gen)
{ 
  modificator_type = MODIFICATOR_FRACTAL_TYPE;

  /*
  INTERPOLATION_LINE_MAX;
  INTERPOLATION_LINE_RANGE_HIGH;
  */
  
  height_config.ifirst = INTERPOLATION_LINE_MAX;
  height_config.isecond = INTERPOLATION_LINE_MAX;
  height_config.iborder = 0;
  height_config.gborder = 0;
  
  height_config.pixel_fill = FALSE;
  height_config.pixel_distance = 1;
  
  height_config.pixel_filter = FALSE;
  height_config.pixel_filter_num = 1;
}

modificator_fractal::~modificator_fractal(void)
{
  
}

/*
  area pattern:
  MODIFICATOR_FRACTAL_TYPE
*/
bool modificator_perlin::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start,size;
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    
    VECT2DI size(pos.dx, pos.dy);
    
    heightmap_load(size.x, size.y);
    heightmap_create();
    
    heightmap.fill(HEIGHT_UNSET);
    
    pconfig.rand(size.x, size.y);
    heightmap.generate_perlin(&pconfig);
    pconfig.release();
    
    modificator_height_map::apply_heighmap(p_target, pos, p_params);
        
    heightmap_remove();
    
    target_post_apply(p_target, pos, p_params);
  }
  return(TRUE);
}

/*
{
  type = TEXTURE_MODIFICATOR_PATTERN_MID_POINT
}
*/
void modificator_perlin::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_height_map::load(p_section);
  
  do {
    if(p_line->is_line()) {
      load_int(pconfig.octaves,"perlin_octaves");
      load_int(pconfig.octaves_start,"perlin_octaves_start");
      load_float(pconfig.persistence,"perlin_persistence");
    }
  } while((p_line = p_line->section_next()));
}

modificator_perlin::modificator_perlin(GENERATOR *p_gen)
  : modificator_height_map(p_gen)
{ 
  modificator_type = MODIFICATOR_PERLIN_TYPE;  
}

modificator_perlin::~modificator_perlin(void)
{
  
}

/*
{
  type = MODIFICATOR_CRACK
}
*/
void modificator_crack::apply_crack_line(MODIFICATOR_TARGET_STACK *p_target,
                                         VECT2DF start, VECT2DF direction, int length,
                                         MODIFICATOR_PARAMS *p_params)
{  
  float step = 1.0f/length;
  
  int i;
  for(i = 0; i < length; i++) {    
    float rnd = random_generator::generator_rand();
    if(rnd < direction_treshold) {
      float rotation = random_generator::generator_gauss_0()*2*PI*direction_angle_range;
      direction.rotate(rotation);
    }
        
    int index = p_params->push(step*i);
    target_apply(p_target, RECT2DI(start.x, start.y), p_params);
    p_params->clear(index);
    
    start += direction;    
  }
}

void modificator_crack::apply_crack(MODIFICATOR_TARGET_STACK *p_target,
                                    RECT2DI pos,
                                    MODIFICATOR_PARAMS *p_params)
{
  // Get the branches num
  int branches = crack_branches;

  switch(type) {
    case CRACK_DEFAULT:
      {
        VECT2DI start(pos.x,pos.y);
        VECT2DF direction(pos.dx,pos.dy);
        int length = direction.norm();
            
        int i;
        for(i = 0; i < branches; i++) {
          apply_crack_line(p_target, start, direction, length, p_params);          
        }
      }
      break;
      
    case CRACK_CENTER:
      {
        VECT2DI center = pos.center_get();
        VECT2DF direction(-1,1);
        direction.norm();
        
        float angle = (2*PI)/branches;
        int length = pos.dx > pos.dy ? pos.dy/2 : pos.dx/2;
            
        int i;
        for(i = 0; i < branches; i++) {
          if(crack_angle_random) {
            angle = 2*PI*random_generator::generator_rand();
          }
          direction.rotate(angle);
          apply_crack_line(p_target, center, direction, length, p_params);
        }
      }
      break;
  }
}

bool modificator_crack::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start, end;
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);
    apply_crack(p_target, pos, p_params);
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

void modificator_crack::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_pixel_pattern::load(p_section);
  
  do {
    if(p_line->is_line()) {

      if(p_line->match_name("crack_type")) {
        type = crack_type_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
            
      load_int(crack_branches, "crack_branches");
      load_int(crack_angle_random, "crack_angle_random");
      load_float(direction_angle_range, "direction_angle_range");
      load_float(direction_treshold, "direction_treshold");
    }
  } while((p_line = p_line->section_next()));
}

modificator_crack::modificator_crack(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_CRACK_TYPE)
{  
  type = CRACK_DEFAULT;
  
  crack_branches = 1;
  crack_angle_random = FALSE;
  
  // <0,1>, it's transfered to <0,PI>
  direction_angle_range = 0.1f;
  
  // <0,1>
  // 0 - no change at all
  // 1 - change every iteration
  direction_treshold = 0.1f;
}

modificator_crack::~modificator_crack(void)
{
  
}

/*
  Generator mask
*/
bool modificator_mask::mask_type_set(MASK_TYPE type)
{
  assert(!p_mask);
  
  if(!p_mask) {
    mask_type = type;
    return(TRUE);
  }
  
  return(FALSE);
}

MASK_TYPE modificator_mask::mask_type_get(void)
{
  return(mask_type);
}

tpos modificator_mask::width_get(void)
{
  mask_create();
  return(p_mask->width_get());
}

tpos modificator_mask::height_get(void)
{
  mask_create();
  return(p_mask->height_get());
}

VECT2DI modificator_mask::mask_size_get(void)
{
  mask_create();
  return(VECT2DI(p_mask->width_get(), p_mask->height_get()));
}

void modificator_mask::mask_size_set(VECT2DI size)
{
  mask_size = size;
}

RGBB  modificator_mask::mask_color_get(VECT2DI pos)
{
  mask_create();
  
  RGBB color(0);
  
  if(mask_type == MASK_COLOR) {
    color = RGBB(((SURFACE_SW *)p_mask)->pixel_get(pos.x,pos.y));
  } 
  
  return(color);
}

void  modificator_mask::mask_color_set(VECT2DI pos, RGBB mask)
{
  mask_create();
  
  if(mask_type == MASK_COLOR) {
    ((SURFACE_SW *)p_mask)->pixel_set(pos.x,pos.y,mask);
  }
}

bool  modificator_mask::mask_bool_get(VECT2DI pos)
{
  mask_create();
  
  bool mask = FALSE;
  
  if(mask_type == MASK_BOOL) {
    mask = ((SURFACE_MASK *)p_mask)->pixel_get(pos.x,pos.y);
  } 
  
  return(mask); 
}

void  modificator_mask::mask_bool_set(VECT2DI pos, bool mask)
{
  mask_create();
  
  if(mask_type == MASK_BOOL) {
    ((SURFACE_MASK *)p_mask)->pixel_set(pos.x,pos.y,mask);
  }
}

float  modificator_mask::mask_height_get(VECT2DI pos)
{
  mask_create();
  
  float mask = 0.0f;
  
  if(mask_type == MASK_HEIGHT) {
    mask = ((SURFACE_HEIGHT *)p_mask)->height_get(pos.x,pos.y);
  } 
  
  return(mask); 
}

void  modificator_mask::mask_height_set(VECT2DI pos, float mask)
{
  mask_create();
  
  if(mask_type == MASK_HEIGHT) {
    ((SURFACE_HEIGHT *)p_mask)->height_set(pos.x,pos.y,mask);
  }
}

VECT3DF modificator_mask::mask_normal_get(VECT2DI pos)
{
  mask_create();  
  if(mask_type == MASK_HEIGHT) {
    return((SURFACE_HEIGHT *)p_mask)->height_normal_get(pos.x,pos.y);
  }   
  return(VECT3DF(0)); 
}

// Fuzzy mask support - searches for near value in given range
// result is from <0,1>, where
//  0 = out or range, 1 = full mask
//  
bool  modificator_mask::mask_color_get(VECT2DI pos, RGBB mask, int range, float &result)
{
  result = 0;
  
  mask_create();
  
  if(mask_type != MASK_COLOR) {
    return(FALSE);
  }

  assert(p_mask->surface_class_get() == SW_SURFACE);
  SURFACE_SW *p_bitmap = (SURFACE_SW *)p_mask;
  
  int px,py;
  if(!p_bitmap->pixel_search(pos.x, pos.y, (tcolor)mask, range, px, py, result)) {
    result = 0;
    return(FALSE);
  }
    
  // Transform from range to mask intensity
  result = 1.0f - result;
  return(TRUE);
}

bool  modificator_mask::mask_bool_get(VECT2DI pos, bool mask, int range, float &result)
{
  mask_create();
  assert(0);
  return(FALSE);
}

bool modificator_mask::mask_create(tpos width, tpos height)
{
  assert(p_mask);
  assert(!p_mask->loaded());
  p_mask->create(width, height);
  return(TRUE);
}

// Load the mask from given file
bool modificator_mask::mask_create(char *p_file)
{  
  assert(p_mask);
  if(!p_mask->load(p_file)) {
    pperror(TRUE, "Can't load mask %s", p_file);
  }  
  return(TRUE);
}

bool modificator_mask::mask_create(MODIFICATOR_TARGET_AUX *p_target)
{ 
  assert(p_mask);
  switch(mask_type) {
    case MASK_BOOL:
      return(FALSE);
    case MASK_COLOR:
      if(p_target->bitmap_loaded()) {
        SURFACE_SW *p_surf = p_target->bitmap_get();
        if(!p_surf->loaded()) {
          return(FALSE);
        }
        
        RECT2DI tmp = p_surf->rect_get();
        mask_create(tmp.dx, tmp.dy);
        assert(p_mask);
        ((SURFACE_SW *)p_mask)->copy(p_surf);      
      }
      break;
    case MASK_HEIGHT:
      if(p_target->heightmap_loaded()) {
        SURFACE_HEIGHT *p_surf = p_target->heightmap_get();
        if(!p_surf->loaded()) {    
          return(FALSE);
        }
        
        RECT2DI tmp = p_surf->rect_get();
        mask_create(tmp.dx, tmp.dy);
        assert(p_mask);
        ((SURFACE_HEIGHT *)p_mask)->copy(p_surf);
      }
      break;
  }
  
  return(TRUE);
}

bool modificator_mask::mask_create(MODIFICATOR_TARGET_HEIGHT *p_target)
{
  SURFACE_HEIGHT * p_surf = NULL;
  
  switch(mask_type) {
    case MASK_HEIGHT:
    {
      p_surf = p_target->heightmap_get();
      if(!p_surf->loaded())
        return(FALSE);
      
      RECT2DI tmp = p_surf->rect_get();
      mask_create(tmp.dx, tmp.dy);
      
      assert(p_mask);
      ((SURFACE_HEIGHT *)p_mask)->copy(p_surf);
      
      return(TRUE);
    }
    default:
      break;
  }
  
  return(FALSE);
}

// Load the heightmap, set scale
bool modificator_mask::mask_create(MODIFICATOR_TARGET_TYPE type, char *p_name)
{ 
  MODIFICATOR_TARGET_CACHE *p_cache = generator_get()->current_target_cache_get();
  
  // if p_cache == NULL then mask_create() is called too early
  assert(p_cache != NULL);
      
  MODIFICATOR_TARGET *p_map = p_cache->target_find(type, p_name);
  if(!p_map) {
    return(FALSE);
  }
      
  switch(target_type) {
    case MODIFICATOR_TARGET_TEXTURE_TYPE:
      return(mask_create((MODIFICATOR_TARGET_AUX *)p_map));
    case MODIFICATOR_TARGET_GEOMETRY_TYPE:
      return(mask_create((MODIFICATOR_TARGET_HEIGHT *)p_map));
      break;
    case MODIFICATOR_TARGET_GENERATOR_TYPE:
      break;
    case MODIFICATOR_TARGET_AUX_TYPE:
      return(mask_create((MODIFICATOR_TARGET_AUX *)p_map));
    default:
      break;
  }
  
  return(FALSE);
}

// Set filename leave the mask for
// delayed loading
void modificator_mask::mask_load(const char *p_file)
{  
  strncpy(mask_file,p_file,MAX_FILENAME);    
  mask_size.set(0);
  target_type = MODIFICATOR_TARGET_NONE;  
}

// Set target_type & target_name and leave the heightmap for
// delayed loading
void modificator_mask::mask_load(char *p_target, char *p_name)
{  
  target_type = target_type_translate(p_target, MODIFICATOR_TARGET_NONE);
  
  if(p_name) {
    strncpy(target_name, p_name, MAX_NAME);
  }
  else {
    target_name[0] = '\0';
  }
  
  mask_file[0] = 0;
  mask_size.set(0);
}

void modificator_mask::mask_load(tpos width, tpos height)
{
  // Mark mask to create
  mask_size_set(VECT2DI(width,height));
  mask_file[0] = 0;
  target_type = MODIFICATOR_TARGET_NONE;
}

bool modificator_mask::mask_loaded(void)
{
  return(p_mask ? p_mask->loaded() : FALSE);
}

bool modificator_mask::mask_load_waiting(void)
{
  // Mask is already loaded
  if(mask_loaded())
    return(FALSE);

  // File is set
  if(mask_file[0])
    return(TRUE);

  // Mask target is set
  if(target_type != MODIFICATOR_TARGET_NONE)
    return(TRUE);
  
  // Mask size is set
  if(mask_size.x > 0 && mask_size.y > 0)
    return(TRUE);
  
  // Mask is not loaded and it's not configured
  return(FALSE);
}

void modificator_mask::mask_remove_target(void)
{
  // Is it loaded from target?
  if(target_type != MODIFICATOR_TARGET_NONE) {
    // Free height data because we want a fresh one
    // for next run
    mask_remove();
  }
}

bool modificator_mask::mask_create(void)
{
  if(p_mask && p_mask->loaded())
    return(TRUE);
  
  // Create a mask in requested format?
  if(!p_mask) {  
    switch(mask_type) {
      case MASK_BOOL:
        p_mask = new SURFACE_MASK;
        break;
      case MASK_COLOR:
        p_mask = new SURFACE_SW;
        break;
      case MASK_HEIGHT:
        p_mask = new SURFACE_HEIGHT;
        break;
    }
  }
   
  // Priorities are:
  
  // 1) From file
  if(mask_file[0]) {
    return(mask_create(mask_file));
  }
  
  // 2) From target
  if(target_type != MODIFICATOR_TARGET_NONE) {
    return(mask_create(target_type, target_name));
  }

  // 3) From coordinates
  if(mask_size.x != 0 && mask_size.y != 0) {
    return(mask_create(mask_size.x, mask_size.y));
  }
  
  // Should not happen
  pperror(TRUE, "MODIFICATOR_MASK(%s): Unable to load mask, no source is defined!", name_get());
  return(FALSE);
}

void modificator_mask::mask_remove(void)
{
  if(p_mask) {    
    delete p_mask;
    p_mask = NULL;
  }
}

void modificator_mask::mask_clear(bool value)
{
  mask_create();
    
  switch(mask_type) {
    case MASK_BOOL:
    {
      SURFACE_MASK *p_tmp = (SURFACE_MASK *)p_mask;
      p_tmp->fill(value ? mask_value_bool_true : mask_value_bool_false);
      break;
    }
    case MASK_COLOR:
    {
      SURFACE_SW *p_tmp = (SURFACE_SW *)p_mask;
      p_tmp->fill(value ? mask_value_color_true : mask_value_color_false);
      break;
    }
    case MASK_HEIGHT:
    {
      SURFACE_HEIGHT *p_tmp = (SURFACE_HEIGHT *)p_mask;
      p_tmp->fill(value ? mask_value_height_true : mask_value_height_false);
      break;
    }
  }
}

void modificator_mask::mask_shrink(int num)
{
  // unimplemented
  assert(0);
}

/*
{
  name = ***
  type = GENERATOR_MASK
}

  Load mask from selected target
*/
void modificator_mask::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  name_set(p_section->name_get());
  
  char file[MAX_FILENAME] = "";  
  char target_type[MAX_NAME] = "";
  char target_type_name[MAX_NAME] = "";
  VECT2DI size(0);
  
  do {
    if(p_line->is_line()) {
            
      if(p_line->match_name("mask_type")) {
        mask_type = mask_type_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      
      // Load mask from this bitmap?
      if(p_line->match_name("mask_bitmap")) {
        strncpy(file,p_line->line_value_string_get(),MAX_FILENAME);
        p_line->line_mark_loaded();
        continue;
      }
      
      // Load mask from this target?
      if(p_line->match_name("mask_source")) {        
        strncpy(target_type, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();
        continue;
      }
      if(p_line->match_name("mask_source_name")) {
        strncpy(target_type_name, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();
        continue;
      }
      
      // Size of the mask
      load_vect2di(size, "mask_size");
    }
  } while((p_line = p_line->section_next()));
  
  // Try to load the mask
  if(file[0]) {
    mask_load(file);
  } else if(target_type[0]) {
    mask_load(target_type, target_type_name);
  } else if(size.x != 0 && size.y != 0) {
    mask_load(size.x, size.y);
  }  
}

bool modificator_mask::apply(MODIFICATOR_TARGET_STACK *p_target,
                             MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  // Create the mask if it's empty
  mask_create();
  
  VECT2DI start, end;
  RECT2DI pos;
  p_coord->coord_final_get_start();
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    VECT2DI center = pos.center_get();
    
    switch(mask_type) {
      case MASK_BOOL:
        mask_bool_set(center, mask_value_bool_true);
        break;
      case MASK_COLOR:
        mask_color_set(center, mask_value_color_true);
        break;
      case MASK_HEIGHT:
        mask_height_set(center, mask_value_height_true);
        break;
    }
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

modificator_mask::modificator_mask(GENERATOR *p_gen)
  : modificator(p_gen, MODIFICATOR_MASK_TYPE)
{ 
  mask_type = MASK_BOOL;
  mask_file[0] = '\0';
  
  mask_size.set(0);
  p_mask = NULL;
  
  mask_value_bool_false = FALSE;
  mask_value_bool_true = TRUE;
  mask_value_color_false = RGBB(0,0,0);
  mask_value_color_true = RGBB(255,255,255);
  mask_value_height_false = 0.0f;
  mask_value_height_true = 1.0f;
}

modificator_mask::~modificator_mask(void)
{
  mask_remove();
}

/*
  Modificator mask 
*/
void modificator_mask_reference::mask_slave_create(tpos dx, tpos dy)
{
  if(p_mask_slave) {
    p_mask_slave->mask_load(dx,dy);    
  } 
}

void modificator_mask_reference::mask_slave_remove(void)
{
  if(p_mask_slave) {
    p_mask_slave->mask_remove();
  }  
}

bool modificator_mask_reference::load(DATA_FILE_SECTION *p_section, GENERATOR *p_generator)
{  
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line) {
    return(FALSE);
  }    
  
  // Load mask_references -> "mask" and so on
  mask_reference::load(p_section, p_generator);
    
  do {
    if(p_line->is_line()) {
      
      // Slave modificator mask
      if(p_line->match_name("mask_create_remove")) {
        MODIFICATOR *p_mask = p_generator->modificator_find(p_line->line_value_string_get());
        if(p_mask) {
          mask_slave_set((MODIFICATOR_MASK *)p_mask);
        }
        else {
          generator_loader_error(p_line->source_file_get(), 
                                 p_line->source_line_get(), 
                                 "Can't find GENERATOR_MASK %s", 
                                 p_line->line_value_string_get());
        }
        p_line->line_mark_loaded();
        continue;
      }      
      
    }
  } while((p_line = p_line->section_next()));
    
  return(TRUE);
}

/*
void modificator_light::apply_light_texture(MODIFICATOR_TARGET *p_target,
                                            RECT2DI pos,
                                            MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_TARGET_TEXTURE *p_text = (MODIFICATOR_TARGET_TEXTURE *)p_target;
  
  SURFACE_SW *p_bitmap = p_text->bitmap_get();
  SURFACE_HEIGHT *p_heightmap = p_text->heightmap_get();
  tpos texture_height = p_text->target_height_get();
  
  int sx = pos.x;
  int sy = pos.y;
  int width = pos.dx;
  int height = pos.dy;
  
  VECT3DF light(sx+roundf((light_position.x+1)*0.5f*width),
                light_position.y*texture_height,
                sy+roundf((light_position.z+1)*0.5f*height));
  
  p_heightmap->height_range_set(texture_height);
  
  VECT light_vector;
  if(light_planar) {
    VECT3DF center(width/2, 0, height/2);
    light_vector = light - center;
    light_vector.norm();
  }

  // Apply lighting  
  int x,y;
  for(y = 0; y < pos.dy; y++) {
    for(x = 0; x < pos.dx; x++) {
      if(mask_get(x,y)) {

        float pixel_height = p_heightmap->height_get(x,y)*texture_height;
        VECT pixel_position(x,pixel_height,y);            
        VECT pixel_normal = p_heightmap->height_normal_get(x, y, 
                            mask_active() ? this : NULL, sx, sy);
        
        if(!light_planar) {
          light_vector = light - pixel_position;
          light_vector.norm();
        }
      }
    }
  }
}
*/

void modificator_light::apply_light(MODIFICATOR_TARGET *p_target,
                                    RECT2DI pos,
                                    MODIFICATOR_PARAMS *p_params,
                                    SURFACE_HEIGHT *p_heightmap,
                                    SURFACE_SW *p_bitmap)
{
  float texture_height = p_heightmap->height_range_get();
  
  int sx = pos.x;
  int sy = pos.y;
  int width = pos.dx;
  int height = pos.dy;
  
  VECT3DF light(sx+roundf((light_position.x+1)*0.5f*width),
                light_position.y*texture_height,
                sy+roundf((light_position.z+1)*0.5f*height));
  
  VECT light_vector;
  if(light_planar) {
    VECT3DF center(width/2, 0, height/2);
    light_vector = light - center;
    light_vector.norm();
  }

  bool slave_target = modificator_slave_get();
  
  // Apply lighting
  int x,y;
  for(y = 0; y < pos.dy; y++) {
    for(x = 0; x < pos.dx; x++) {
      if(mask_get(x,y)) {

        float pixel_height = p_heightmap->height_get(x,y)*texture_height;
        VECT pixel_position(x,pixel_height,y);            
        VECT pixel_normal = p_heightmap->height_normal_get(x, y, 
                            mask_active() ? this : NULL, sx, sy);
        
        if(!light_planar) {
          light_vector = light - pixel_position;
          light_vector.norm();
        }
        
        if(slave_target) {
          float intensity = light_intensity_get(&light_vector, &pixel_normal);
          int index = p_params->push(intensity);
          MODIFICATOR_TARGET_STACK st(p_target);
          target_apply(&st, RECT2DI(pos.x+x,pos.y+y), p_params);
          p_params->clear(index);
        }
        else {
          RGBAF color(p_bitmap->pixel_get(x,y));
          phong_lighting_get(&color, &color, &light_vector, &pixel_normal);
          p_bitmap->pixel_set(x,y,color);          
        }
/*
        float pixel_height = p_heightmap->height_get(x,y);
        if(pixel_height != HEIGHT_UNSET) {
          RGBAF color(p_bitmap->pixel_get(x,y));
          color.set(pixel_height);
          p_bitmap->pixel_set(x,y,color);
        }
        else {
          RGBAF color(p_bitmap->pixel_get(x,y));
          color.set(1,0,0,0);
          p_bitmap->pixel_set(x,y,color);
        }
*/
      }
    }
  }
}

void modificator_light::apply_light(MODIFICATOR_TARGET *p_target,
                                    RECT2DI pos,
                                    MODIFICATOR_PARAMS *p_params)
{
  SURFACE_SW *p_bitmap = NULL;
  SURFACE_HEIGHT *p_src = NULL;
  
  // TODO - when we want to copy the surface?
  bool heightmap_copy = TRUE;
      
  if(p_target->type == MODIFICATOR_TARGET_TEXTURE_TYPE) {
    MODIFICATOR_TARGET_TEXTURE *p_text = (MODIFICATOR_TARGET_TEXTURE *)p_target;
    p_bitmap = p_text->bitmap_get();
    p_src = heightmap_copy ? new SURFACE_HEIGHT(p_text->heightmap_get()) : 
                             p_text->heightmap_get();
  }
  else if(p_target->type == MODIFICATOR_TARGET_AUX_TYPE) {
    MODIFICATOR_TARGET_AUX *p_text = (MODIFICATOR_TARGET_AUX *)p_target;
    p_src = heightmap_copy ? new SURFACE_HEIGHT(p_text->heightmap_get()) : 
                             p_text->heightmap_get();
  } else {
    // Not implemented
    assert(0);
  }
    
  apply_light(p_target,pos,p_params,p_src,p_bitmap);
  
  if(heightmap_copy) {
    delete p_src;
  }
  p_src = NULL;
}

void modificator_light::apply_light(MODIFICATOR_TARGET_STACK *p_target,
                                    RECT2DI pos,
                                    MODIFICATOR_PARAMS *p_params)
{
  int i,num;
  MODIFICATOR_TARGET **p_trg = p_target->get_all(num);
  for(i = 0; i < num; i++) {
    apply_light(p_trg[i],pos,p_params);
  }
}

bool modificator_light::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start, end;
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    target_init(p_target, pos, p_params);
    apply_light(p_target, pos, p_params);    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

/*
  - Lighting parameters (for this modificator)

    color_ambient = 0xffffff
  
    color_diffuse_active = 1
    color_diffuse = 0xffffff
  
    color_specular_active = 0
    color_specular = 0xffffff
    specular_shine = 0.5
*/
void modificator_light::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator::load(p_section);
  
  do {
    if(p_line->is_line()) {
      
      /*
       * Light configuration
       */
      load_int(self_shadows, "self_shadows");
      load_int(light_planar, "planar");
      
      load_vect3df(light_position, "position");
      load_vect3df(light_position, "light_position");
      
      /*
       * Light params
       */
      load_float(light_angle_min, "angle_min");
      load_float(light_angle_scale, "angle_scale");
      
      load_float(phong_light_color_specular_shine, "specular_shine");
      
      /*
       * Light color
       */
      load_rgbaf(phong_light_color_ambient, "color_ambient");
      
      load_int(phong_light_diffuse, "color_diffuse_active");
      load_rgbaf(phong_light_color_diffuse, "color_diffuse");
      
      load_int(phong_light_specular, "color_specular_active");
      load_rgbaf(phong_light_color_specular, "color_specular");
    }
  } while((p_line = p_line->section_next()));
}

modificator_light::modificator_light(GENERATOR *p_gen)
  : modificator(p_gen, MODIFICATOR_LIGHT_TYPE)
{ 
  light_position = VECT3DF(0,1,0);
  self_shadows = FALSE;
  light_planar = TRUE;
}

modificator_light::~modificator_light(void)
{
  
}

/*
 * Modificator filter
 */
modificator_filter::modificator_filter(GENERATOR *p_gen)
  : modificator(p_gen, MODIFICATOR_FILTER_TYPE)
{
  target = FILTER_HEIGHT;
  type = FILTER_INTERPOLATE;
  strength = 1;
}

modificator_filter::~modificator_filter(void)
{
  
}

void modificator_filter::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  modificator::load(p_section);
  
  do {
    if(p_line->is_line()) {
      
      /*
       * Filter configuration
       */
      if(p_line->match_name("filter_target")) {
        target = filter_target_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      
      if(p_line->match_name("filter_type")) {
        type = filter_type_translate(p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      
      load_int(strength, "filter_strength");
    }
  } while((p_line = p_line->section_next()));
}

/*
#define FILTER_VECTORS  25

static VECT2DI filter_matrix[FILTER_VECTORS] = 
{
  VECT2DI(-2,-2),
  VECT2DI(-1,-2),
  VECT2DI( 0,-2),
  VECT2DI( 1,-2),
  VECT2DI( 2,-2),
  
  VECT2DI(-2,-1),
  VECT2DI(-1,-1),
  VECT2DI( 0,-1),
  VECT2DI( 1,-1),
  VECT2DI( 2,-1),
  
  VECT2DI(-2, 0),
  VECT2DI(-1, 0),
  VECT2DI( 0, 0),
  VECT2DI( 1, 0),
  VECT2DI( 2, 0),
  
  VECT2DI(-2, 1),
  VECT2DI(-1, 1),
  VECT2DI( 0, 1),
  VECT2DI( 1, 1),
  VECT2DI( 2, 1),

  VECT2DI(-2, 2),
  VECT2DI(-1, 2),
  VECT2DI( 0, 2),
  VECT2DI( 1, 2),
  VECT2DI( 2, 2),
};
*/

#define FILTER_VECTORS  9

static VECT2DI filter_matrix[FILTER_VECTORS] = 
{
  VECT2DI( 0, 0),
  VECT2DI(-1,-1),
  VECT2DI( 0,-1),
  VECT2DI( 1,-1),
  VECT2DI(-1, 0),
  VECT2DI( 1, 0),
  VECT2DI(-1, 1),
  VECT2DI( 0, 1),
  VECT2DI( 1, 1)
};

int modificator_filter::filter_height_get(MODIFICATOR_TARGET *p_target,
                                          int x, int y,
                                          float *p_list_values)
{
  int values = 0;
  
  
  
  int i;
  for(i = 0;
      i < (int)(sizeof(filter_matrix)/sizeof(filter_matrix[0]));
      i++)
  {
    tpos ax = x+filter_matrix[i].x;
    tpos ay = y+filter_matrix[i].y;
    
    // The value is inside - ok, get it
    float height = filter_height_get(p_target, ax, ay);    
    if(height == HEIGHT_UNSET) {
      
      // Center of the matrix has to be always valid
      assert(i != 0);
      
      height = filter_height_get(p_target, x, y); 
      assert(height != HEIGHT_UNSET);
      
      // Okay, we have to interpolate the value
      tpos ix = x-filter_matrix[i].x;
      tpos iy = y-filter_matrix[i].y;        
      
      float opposite_height = filter_height_get(p_target, ix, iy);
      if(opposite_height != HEIGHT_UNSET) {
        height = 2*height - opposite_height;
      }
    }
    
    p_list_values[values] = height;
    values++;
  }

  return(values);
}

int modificator_filter::filter_color_get(MODIFICATOR_TARGET *p_target,
                                         int x, int y,
                                         RGBB *p_list_values)
{
  int values = 0;
    
  int i;
  for(i = 0;
      i < (int)(sizeof(filter_matrix)/sizeof(filter_matrix[0])); 
      i++)
  {
    tpos ax = x+filter_matrix[i].x;
    tpos ay = y+filter_matrix[i].y;
    
    // The value is inside 
    if(p_target->pixel_valid(ax, ay) && mask_get(ax,ay)) {
      RGBB color = RGBB(p_target->color_get(ax, ay));
      p_list_values[values] = color;
      values++;
      break;
    }
  }

  return(values);
}

static float filter_interpolate_height(float *p_values, int num)
{
  float av = p_values[0];
  
  int i;
  for(i = 1; i < num; i++)
    av += p_values[i];
  
  return(av/num);
}

static float filter_blur_height(float *p_values, int num)
{
  float av = p_values[0];
  
  int i;
  for(i = 1; i < num; i++)
    av += p_values[i];
  
  return(av/num);
}

bool modificator_filter::apply_height(MODIFICATOR_TARGET_STACK *p_target,
                                      RECT2DI pos,
                                      MODIFICATOR_PARAMS *p_params)
{
  FILTER_KERNEL_HEIGHT kernel = NULL;

  switch(type) {
    case FILTER_INTERPOLATE:
      kernel = filter_interpolate_height;
      break;
    case FILTER_BLUR:
      kernel = filter_blur_height;
      break;
  }
  
  assert(kernel);
  
  
  int i,num;
  MODIFICATOR_TARGET **p_trg = p_target->get_all(num);
  
  for(i = 0; i < num; i++) {
    int x,y;  
    for(y = 0; y < pos.dy; y++) {
      for(x = 0; x < pos.dx; x++) {
        tpos ax = pos.x+x;
        tpos ay = pos.y+y;
        if(p_trg[i]->pixel_valid(ax, ay) && mask_get(ax,ay)) {
          float values[FILTER_VECTORS];
          int num = filter_height_get(p_trg[i], ax, ay, values);
          assert(num == FILTER_VECTORS);
          if(num) {
            float height = kernel(values, num);
            p_trg[i]->height_set(ax, ay, height);
          }
        }
      }
    }
  }
  
  return(TRUE);
}

bool modificator_filter::apply_color(MODIFICATOR_TARGET_STACK *p_target,
                                     RECT2DI pos,
                                     MODIFICATOR_PARAMS *p_params)
{
  
  
  return(TRUE);
}

/*
 * Filters operate directly on targets, they don't call any slave modificators
 */
bool modificator_filter::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  
  VECT2DI start, end;
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    int i;
    for(i = 0; i < strength; i++) {
      switch(target) {
        case FILTER_HEIGHT:
          apply_height(p_target,pos,p_params);
          break;
        case FILTER_COLOR:
          apply_color(p_target,pos,p_params);
          break;
      }
    }
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}

/*
 * Modificator from target
 */
modificator_from_target::modificator_from_target(GENERATOR *p_gen)
  : modificator_pixel_pattern(p_gen, MODIFICATOR_FROM_TARGET_TYPE)
{
  target_name[0] = 0;
}

modificator_from_target::~modificator_from_target(void)
{
  
}

void modificator_from_target::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  modificator_pixel_pattern::load(p_section);
  
  do {
    if(p_line->is_line()) {      
      if(p_line->match_name("target_name")) {
        strncpy(target_name, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();
        continue;
      }
    }
  } while((p_line = p_line->section_next()));
}

/*
Params on parameter stack:

  [0] color.r <- TOP
  [1] color.g
  [2] color.b
  [3] color.a
  [4] height
  [5] normal.x
  [6] normal.y
  [7] normal.z
*/
bool modificator_from_target::apply_pixel(MODIFICATOR_TARGET_STACK *p_target,
                                          tpos dx, tpos dy,
                                          MODIFICATOR_TARGET *p_source,
                                          tpos sx, tpos sy,
                                          MODIFICATOR_PARAMS *p_params)
{
  if(!p_source->pixel_valid(sx, sy))
    return(FALSE);
  
  RGBAF color(p_source->color_get(sx, sy));
  float height(p_source->height_get(sx, sy));
  VECT3DF normal(p_source->normal_get(sx, sy));
  
  int index = p_params->push(normal.z);
  p_params->push(normal.y);
  p_params->push(normal.x);
  
  p_params->push(height);
  
  p_params->push(color.a);
  p_params->push(color.b);
  p_params->push(color.g);
  p_params->push(color.r);
  
  target_apply(p_target, RECT2DI(dx, dy), p_params);
  p_params->clear(index);

  return(TRUE);
}

/*
 * Filters operate directly on targets, they don't call any slave modificators
 */
bool modificator_from_target::apply(MODIFICATOR_TARGET_STACK *p_target, MODIFICATOR_PARAMS *p_params)
{  
  MODIFICATOR_TARGET_CACHE *p_cache = generator_get()->current_target_cache_get();
  
  // if p_cache == NULL it's called too early
  assert(p_cache != NULL);
      
  MODIFICATOR_TARGET *p_source = p_cache->target_find(MODIFICATOR_TARGET_NONE, target_name);
  assert(p_source);  
  
  MODIFICATOR_COORDINATES *p_coord = coordinates_get();
  VECT2DI start, end;
  RECT2DI pos;
  p_coord->coord_final_get_start();  
  while(p_coord->coord_final_get_next(&pos)) {
    target_pre_apply(p_target, pos, p_params);
    
    target_init(p_target, pos, p_params);

    int x,y;    
    for(y = pos.y; y < pos.y+pos.dy; y += pixel_step_y_get()) {
      for(x = pos.x; x < pos.x+pos.dx; x += pixel_step_x_get()) {
        apply_pixel(p_target, x, y, p_source, x, y, p_params);
      }
    }    
    
    target_post_apply(p_target, pos, p_params);
  }
  
  return(TRUE);
}
