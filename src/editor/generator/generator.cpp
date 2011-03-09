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
  Simple object generator
  
  Generates meshes (ground) based on a height map
  or a fractal
  
  Generator scheme:

  MESH_PARAMS
      ||
      ||
      \/      
     Mesh
  
  Mesh + MESH_MODIFICATOR
      ||
      ||
      \/
     Mesh
  
  Maps:
    - height-map      - sets height of the terrain
    - terrain-map     - sets type of terrain (sand/dirt/cliff/grass)
    - items-map       - sets properties located on the ground (tree/water/grass/stones)
*/

#include "editor.h"

/*
 * Error handler
 */
void generator_loader_error(char *p_file, int line, const char *p_text,...)
{
  char      text[2000];
  va_list   arguments;  

  va_start(arguments,p_text);
  vsnprintf(text,2000,p_text,arguments);
  va_end(arguments);

  fprintf(stderr,"Generator load error, file %s: line %d\n%s\n",p_file,line,text);  
  abort();
}

generator_config::generator_config(void)
{
  max_texture_size = 2048;
  strcpy(output_test_file, "text_out.bmp");
}

MODIFICATOR * modificator_new(GENERATOR *p_gen, MODIFICATOR_TYPE type)
{
  MODIFICATOR *p_tmp = NULL;
    
  switch(type) {
    case MODIFICATOR_GENERATOR_MESH_TYPE:
      p_tmp = new MODIFICATOR_GENERATOR_MESH(p_gen);
      break;
    case MODIFICATOR_MASK_TYPE:
      p_tmp = new MODIFICATOR_MASK(p_gen);
      break;
    case MODIFICATOR_POINT_SINGLE_TYPE:
      p_tmp = new MODIFICATOR_POINT_SINGLE(p_gen);
      break;
    case MODIFICATOR_POINT_EXTENDED_TYPE:
      p_tmp = new MODIFICATOR_POINT_EXTENDED(p_gen);
      break;
    case MODIFICATOR_RECTANGLE_TYPE:
      p_tmp = new MODIFICATOR_RECTANGLE(p_gen);
      break;
    case MODIFICATOR_LINE_TYPE:
      p_tmp = new MODIFICATOR_LINE(p_gen);
      break;
    case MODIFICATOR_LINE_LEAF_TYPE:
      p_tmp = new MODIFICATOR_LINE_LEAF(p_gen);
      break;
    case MODIFICATOR_BUNCH_TYPE:
      p_tmp = new MODIFICATOR_BUNCH(p_gen);
      break;
    case MODIFICATOR_BRICK_TYPE:
      p_tmp = new MODIFICATOR_BRICK(p_gen);
      break;
    case MODIFICATOR_HEIGHTMAP_TYPE:
      p_tmp = new MODIFICATOR_HEIGHT_MAP(p_gen);
      break;
    case MODIFICATOR_FRACTAL_TYPE:
      p_tmp = new MODIFICATOR_FRACTAL(p_gen);
      break;
    case MODIFICATOR_CRACK_TYPE:
      p_tmp = new MODIFICATOR_CRACK(p_gen);
      break;
    case MODIFICATOR_LIGHT_TYPE:
      p_tmp = new MODIFICATOR_LIGHT(p_gen);
      break;
    case MODIFICATOR_FILTER_TYPE:
      p_tmp = new MODIFICATOR_FILTER(p_gen);
      break;
    case MODIFICATOR_BITMAP_TYPE:
      p_tmp = new MODIFICATOR_BITMAP(p_gen);
      break;
    case MODIFICATOR_PERLIN_TYPE:
      p_tmp = new MODIFICATOR_PERLIN(p_gen);
      break;
    case MODIFICATOR_FROM_TARGET_TYPE:
      p_tmp = new MODIFICATOR_FROM_TARGET(p_gen);
      break;
    default:
      assert(0);
      break;
  }
  
  return(p_tmp);
}

MODIFICATOR * generator::modificator_create(MODIFICATOR_TYPE type, const char *p_name)
{
  MODIFICATOR *p_new = modificator_new(this, type);
  p_new->name_set(p_name);
  modificators.object_list_insert(p_new);  
  return(p_new);
}

void generator::modificator_remove(MODIFICATOR *p_mod)
{
  modificators.object_list_remove(p_mod);
  delete (MODIFICATOR *)p_mod;
}

MODIFICATOR * generator::modificator_get_first(void)
{
  return((MODIFICATOR *)modificators.object_list_get_first());
}

MODIFICATOR * generator::modificator_find(const char *p_name)
{
  return((MODIFICATOR *)modificators.object_list_find(p_name));
}

GENERATOR_MESH * generator::mesh_generator_create(const char *p_name)
{
  GUI_STATE *p_state = gui_state_get();    
  p_state->change_start(EDITOR_PART_GENERATOR_MESH_LIST);  
  
  GENERATOR_MESH *p_gen = mesh_generators.create(p_name);
  assert(p_gen);
  
  p_state->change_stop(EDITOR_PART_GENERATOR_MESH_LIST);
  return(p_gen);
}

void generator::mesh_generator_remove(GENERATOR_MESH *p_gen)
{
  GUI_STATE *p_state = gui_state_get();  
  
  p_state->change_start(EDITOR_PART_GENERATOR_MESH_LIST);  
  mesh_generators.destroy(p_gen);  
  p_state->change_stop(EDITOR_PART_GENERATOR_MESH_LIST);
}

GENERATOR_MESH * generator::mesh_generator_find(const char *p_name)
{
  return((GENERATOR_MESH *)mesh_generators.object_list_find(p_name));
}

GENERATOR_MESH * generator::mesh_generator_get_first(void)
{
  return((GENERATOR_MESH *)mesh_generators.object_list_get_first());
}

/*
 * Mesh loaders - specific
 */

/*
void generator::load_texture_modificator_coordinates(TEXTURE_MODIFICATOR_ITEM *p_modificator, DATA_FILE_SECTION *p_section)
{ 
  COORDINATE tmp;
  int        index;
  
  if(load_modificator_coordinates(p_section, &tmp, &index)) {
    COORDINATE *p_coord = (p_modificator->coordinates_get())->coord_fixed_get(index);
    *p_coord = tmp;
  }
}
*/

/*
{
  name = 
  type = MESH_MODIFICATOR_HEIGHTMAP

  height_bitmap = "/home/komat/Projekty/draha.png"

  height_bitmap_colormask = 0xfffff
  height_bitmap_colormask_r = 2
  height_bitmap_colormask_g = 3
  height_bitmap_colormask_b = 4

  height_multiplier = -2.0
  height_shift = -2.0
}
*/
/*
void generator::load_section_mesh_heightmap(DATA_FILE_SECTION *p_section, MESH_MODIFICATOR_HEIGHT_MAP *p_map)
{  
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  load_modificator_mask(p_section, generator_get(), (MODIFICATOR_MASK *)p_map);  
  load_modificator_coordinates(p_section, p_map->coordinates_get());
  
  do {
    if(p_line->is_line()) {
      if(p_line->match_name("height_bitmap")) {
        p_map->heightmap_set(NULL, p_line->line_value_string_get());
        p_line->line_mark_loaded();
        continue;
      }
      
      load_aritmetic_operation(p_map->height_operator, "height_operation");
      
      load_float(p_map->height_multiplier,"height_multiplier");
      load_float(p_map->height_shift,"height_shift");
      
      load_int(p_map->height_filter_active, "height_filter_active");
      load_int(p_map->height_filter_min, "height_filter_min");
      load_int(p_map->height_filter_max, "height_filter_max");
      
    }
  } while((p_line = p_line->section_next()));
}

void generator::load_section_mesh_heightmap(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
 
  MESH_MODIFICATOR_HEIGHT_MAP *p_map = (MESH_MODIFICATOR_HEIGHT_MAP *)mesh.height_map_new();
  p_map->name_set(p_section->name_get());

  load_section_mesh_heightmap(p_section, p_map);
}
*/

/*
{
  name = 
  type = MESH_MODIFICATOR_FRACTAL

  fractal_base = 0
  fractal_delta = 100
  fractal_hurst = 0.6
}
*/
/*
void generator::load_section_mesh_fractal(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  MESH_MODIFICATOR_HEIGHT_FRACTAL *p_frac = (MESH_MODIFICATOR_HEIGHT_FRACTAL *)mesh.height_fractal_new();
  p_frac->name_set(p_section->name_get());
  
  load_section_mesh_heightmap(p_section, p_frac);
  
  do {
    if(p_line->is_line()) {
      load_float(p_frac->height_config.generator_hurst,"fractal_hurst");
      
      load_float(p_frac->height_config.generator_delta,"fractal_delta");
      load_float(p_frac->height_config.generator_center,"fractal_base");      
    }
  } while((p_line = p_line->section_next()));  
}
*/

/*
 * File loaders
 */
void generator::load_generator_launcher(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  do {
    if(p_line->is_line()) {
      if(p_line->match_name("generator_mesh")) {
      
        GENERATOR_LAUNCHER tmp;
        tmp.type = GENERATOR_MESH_TYPE;
        strncpy(tmp.name,p_line->line_value_string_get(),MAX_NAME);
        launcher_add(&tmp);
        
        p_line->line_mark_loaded();
        continue;
      }
    }
  } while((p_line = p_line->section_next()));
}

void generator::load_section(DATA_FILE_SECTION *p_section)
{
  // Load all lines from section
  MODIFICATOR_TYPE type = modificator_type_item::type_translate(p_section->section_type_get());
  switch(type) {
    
    case GENERATOR_LAUNCHER_TYPE:
      load_generator_launcher(p_section);
      break;
    case GENERATOR_MESH_TYPE:
    {
      GENERATOR_MESH *p_new = mesh_generator_create(p_section->name_get());
      p_new->load_section(p_section);
      break;
    }
    
    case MESH_PARAMETERS_TYPE:
    case MATERIAL_PARAMETERS_TYPE:
    case TEXTURE_PARAMETERS_TYPE:
    case AUX_PARAMETERS_TYPE:
    {
      GENERATOR_MESH *p_last = mesh_generators.get_last_created();      
      if(!p_last) {
        generator_loader_error(p_section->source_file_get(), 
                               p_section->source_line_get(), 
                              "You need to define GENERATOR_MESH before %s (with name '%s')",
                               p_section->section_type_get(),
                               p_section->name_get());
      }  
      p_last->load_section(p_section);
      break;
    }
    
    case MODIFICATOR_GENERATOR_MESH_TYPE:
    case MODIFICATOR_MASK_TYPE:
    case MODIFICATOR_POINT_SINGLE_TYPE:
    case MODIFICATOR_POINT_EXTENDED_TYPE:
    case MODIFICATOR_RECTANGLE_TYPE:
    case MODIFICATOR_LINE_TYPE:
    case MODIFICATOR_LINE_LEAF_TYPE:
    case MODIFICATOR_BUNCH_TYPE:
    case MODIFICATOR_BRICK_TYPE:
    case MODIFICATOR_HEIGHTMAP_TYPE:
    case MODIFICATOR_BITMAP_TYPE:
    case MODIFICATOR_FRACTAL_TYPE:
    case MODIFICATOR_PERLIN_TYPE:      
    case MODIFICATOR_CRACK_TYPE:
    case MODIFICATOR_LIGHT_TYPE:
    case MODIFICATOR_FILTER_TYPE:
    case MODIFICATOR_FROM_TARGET_TYPE:
      {
        MODIFICATOR *p_mod = modificator_create(type, p_section->name_get());
        p_mod->load(p_section);
        break;
      }
    default:
      generator_loader_error(p_section->source_file_get(), 
                             p_section->source_line_get(), 
                             "Unknown section type %s (with name '%s')",
                             p_section->section_type_get(),
                             p_section->name_get());
      break;
  }
  
  // Check if all lines are loaded
  load_section_check(p_section);
}

bool generator::load(const char *p_file, bool safe, bool clear_content)
{   
  DATA_FILE file;
  
  if(!file.load(p_file, safe))
    return(FALSE);

  file.print();
  
  if(clear_content)
    clear();
  
  // Load a new one
  DATA_FILE_SECTION *p_section = file.section_get_first();
  while(p_section) {
    load_section(p_section);
    p_section = p_section->section_next();
  }  
  
  return(TRUE);
}

void generator::launcher_add(GENERATOR_LAUNCHER *p_new)
{
  launcher[launcher_last++] = *p_new;
}

bool generator::launcher_run(GENERATOR_LAUNCHER *p_launcher, SCENE *p_scene)
{
  switch(p_launcher->type) {
    case GENERATOR_MESH_TYPE:
      {
        GENERATOR_MESH *p_mesh_generator = mesh_generator_find(p_launcher->name);
        if(!p_mesh_generator) {
          pperror(TRUE, "Unable to find generator %s", p_launcher->name);
          return(FALSE);
        }
        
        p_mesh_generator->mesh_remove(p_scene);
        
        p_current_target_cache = &p_mesh_generator->target_cache;
        p_mesh_generator->mesh_create(p_scene);
        p_current_target_cache = NULL;
      }
      break;
      
    default:
      break;
  }
  
  return(TRUE);
}

void generator::run(SCENE *p_scene)
{
  int i;
  for(i = 0; i < launcher_last; i++) {
    launcher_run(launcher+i, p_scene);
  }
}

generator::generator(EDITOR *p_editor)
  : editor_reference(p_editor),
    mesh_generators(this,NULL)
{
  launcher_last = 0;
  p_current_target_cache = NULL;
}

generator::~generator(void)
{
  
}

void load_section_check(DATA_FILE_SECTION *p_section)
{  
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  while(p_line) {
    // Check if the line has been loaded
    if(p_line->is_line() && !p_line->line_value_is_loaded())
    {
      generator_loader_error(p_line->source_file_get(), 
                             p_line->source_line_get(), 
                             "Section: '%s' - wrong line '%s'",
                             p_section->name_get(), p_line->name_get());
    }
    
    // Recurse to next section
    if(p_line->is_section()) {
      load_section_check(p_section->section_child_get());
    }
    
    p_line = p_line->section_next();
  }
}
