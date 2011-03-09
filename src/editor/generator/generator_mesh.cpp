/*
 *        .þÛÛþ þ    þ þÛÛþ.     þ    þ þÛÛÛþ.  þÛÛÛþ .þÛÛþ. þ    þ
 *       .þ   Û Ûþ.  Û Û   þ.    Û    Û Û    þ  Û.    Û.   Û Ûþ.  Û
 *       Û    Û Û Û  Û Û    Û    Û   þ. Û.   Û  Û     Û    Û Û Û  Û
 *     .þþÛÛÛÛþ Û  Û Û þÛÛÛÛþþ.  þþÛÛ.  þþÛÛþ.  þÛ    Û    Û Û  Û Û
 *    .Û      Û Û  .þÛ Û      Û. Û   Û  Û    Û  Û.    þ.   Û Û  .þÛ
 *    þ.      þ þ    þ þ      .þ þ   .þ þ    .þ þÛÛÛþ .þÛÛþ. þ    þ
 *
 * Berusky (C) AnakreoN
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

/*
 * Mesh types
 */
const char mesh_type[][MAX_NAME] = 
{
  "MESH_LAND",
  "MESH_BUNCH",
  "MESH_GRASS",
  "MESH_BUSH",
};

MESH_TYPE mesh_type_translate(const char *p_type)
{
  int i;
  for(i = 0;
      i < (int)(sizeof(mesh_type)/sizeof(mesh_type[0]));
      i++)
  {
    if(!strncasecmp(p_type,mesh_type[i],MAX_NAME))
      return((MESH_TYPE)i);
  }
  return(MESH_LAND);
}

/*
 * Generator config
 */  
MESH_HELPER * generator_mesh::mesh_create(SCENE *p_scene)
{
  // Init the target cache
  target_cache.init();
  
  // Launch modificators
  run();
  
  // Flush the results
  MESH_HELPER *p_helper = target_cache.flush(p_scene);
  
  return(p_helper);
}

bool generator_mesh::mesh_remove(SCENE *p_scene)
{
  GAME_MESH *p_mesh = p_scene->mesh_mark_first(MARK_CREATED_BY_GENERATOR);
  while(p_mesh) {
    GAME_MESH *p_tmp = p_mesh;
    p_mesh = (GAME_MESH *)p_mesh->object_list_next_mark(MARK_CREATED_BY_GENERATOR);        
    p_scene->mesh_remove(p_tmp);
  }
  return(TRUE);
}

/*
 * Launch this mesh generator
 */
void generator_mesh::run(void)
{
  GENERATOR *p_gen = generator_get();
  
  pprintf("Run generator mesh %s:",name_get());
  
  int i;
  for(i = 0; i < MAX_MODIFICATORS; i++) {
    GENERATOR_MESH_MODIFICATOR_CONFIG *p_conf = config.modificator_get(i);    
    if(!p_conf)
      break;
    
    MODIFICATOR *p_mod = p_gen->modificator_find(p_conf->modificator_name);
    if(!p_mod) {
      generator_loader_error("",0,
                             "Can't find modificator %s",
                             p_conf->modificator_name);
    }
    
    pprintf("  Modificator %s, type %s:",
            p_mod->name_get(), p_mod->modificator_type_name_get());
    
    MODIFICATOR_TARGET_STACK target_stack;
    RECT2DI first_target_size(0); 
    
    int j;
    for(j = 0; j < MAX_MODIFICATOR_TARGETS; j++) {
      GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG *p_tconfig = p_conf->target_config_get(j);
      if(!p_tconfig)
        break;
                
      MODIFICATOR_TARGET *p_target = target_cache.target_get(p_tconfig->target_type,
                                                             p_tconfig->target_name);
      if(j == 0) {
        // Get size of first target
        first_target_size = p_target->target_area_get();
        assert(first_target_size.dx > 0 && first_target_size.dy > 0);
      } else {
        // Set scale to all targets along the first one
        p_target->target_scale_set(first_target_size.size_get());
      }
      
      // Set target mask, if it's active
      if(p_tconfig->mask.mask_active() ||
         p_tconfig->mask.mask_create_active())
      {
        p_target->mask_set(&p_tconfig->mask);
      }
      
      // Add found target to target stack, it's going to be used
      // as a target for this modificator run
      target_stack.push(p_target);
      
      pprintf("    target %s, target name '%s'",
              modificator_target_translate(p_tconfig->target_type),
              p_tconfig->target_name);
    }
    
    // Check if we have any target available. Get a target which was 
    // put to target stack as the firts one
    MODIFICATOR_TARGET *p_target = target_stack.get_first();
    if(!p_target) {
      perror("    Missing target! You have to define at least one.");
    }
    
    // Launch the circus
    MODIFICATOR_PARAMS p;
    RECT2DI pos = p_target->target_area_get();
    p_mod->coordinates_top_set(pos);
    p_mod->init(&target_stack, pos, &p);
    p_mod->apply(&target_stack, &p);
          
    // Reset scale on all targets
    int t, target_num;
    MODIFICATOR_TARGET **p_targets = target_stack.get_all(target_num);
    for(t = 0; t < target_num; t++) {
      p_targets[t]->target_scale_clear();
    }
  }  
}

/*
 * Mesh loaders - generic
 */
/*
  Load geometry target

{
  name = ***
  type = MESH_PARAMS

  mesh_start_x = 0
  mesh_start_y = 0
  mesh_start_z = 0

  mesh_size_x = 50
  mesh_size_y = 0
  mesh_size_z = 50

  mesh_diff_x = 1
  mesh_diff_y = 0
  mesh_diff_z = 1
}
*/
void generator_mesh::load_section_mesh_params(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  VECT    start(0);
  bool    start_active = FALSE;
  VECT    diff(0);
  bool    diff_active = FALSE;
  VECT3DI size(0);
  bool    size_active = FALSE;
  
  do {
    if(p_line->is_line()) {
      load_mesh_type(geometry_desc.type,"mesh_type");
      
      load_vect3df_active(start,"start");
      load_vect3df_active(diff,"diff");
      load_vect3di_active(size,"size");
      
      load_interval_int(geometry_desc.bunch_slice_num,"bunch_slice_num");
      load_interval_int(geometry_desc.bunch_slice_segments,"bunch_slice_segments");
      
      load_interval_float(geometry_desc.bunch_slice_x_offset,"bunch_slice_x_offset");
      load_interval_float(geometry_desc.bunch_slice_z_offset,"bunch_slice_z_offset");
      
      load_interval_float(geometry_desc.bunch_slice_width_start,"bunch_slice_width_start");
      load_interval_float(geometry_desc.bunch_slice_width_end,"bunch_slice_width_end");

      load_interval_float(geometry_desc.bunch_slice_height_start,"bunch_slice_height_start");
      load_interval_float(geometry_desc.bunch_slice_height_end,"bunch_slice_height_end");
      
      load_interval_angle(geometry_desc.bunch_slice_falling,"bunch_slice_falling");
      load_interval_angle(geometry_desc.bunch_segment_falling,"bunch_segment_falling");
      
      load_int(geometry_desc.bunch_slice_rotation_incemental,"bunch_slice_rotation_incemental");
      load_interval_angle(geometry_desc.bunch_slice_rotation_range,"bunch_slice_rotation_range");
      load_interval_angle(geometry_desc.bunch_slice_rotation_step,"bunch_slice_rotation_step");
    }
  } while((p_line = p_line->section_next()));
  
  if(start_active)
    geometry_desc.start = start;
  if(diff_active)
    geometry_desc.diff = diff;
  if(size_active)
    geometry_desc.size = size;
}

/*
  Load material parameters

{
  name = 
  type = MATERIAL_PARAMS
}
*/
void generator_mesh::load_section_material_params(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  do {
    if(p_line->is_line()) {
      load_int(material_desc.transparent, "transparent");
      load_int(material_desc.double_side, "double_side");
    }
  } while((p_line = p_line->section_next()));
}

/*
  Load texture target
{
  name = 
  type = TEXTURE_PARAMS

  texture_size_x = 512
  texture_size_y = 512
}
*/
void generator_mesh::load_section_texture_params(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
    
  do {
    if(p_line->is_line()) {
      load_vect2di(texture_desc.texture_size, "texture_size");
      load_int(texture_desc.texture_height, "texture_height");
      load_rgbaf(texture_desc.texture_background_color, "background_color");
      load_int(texture_desc.texture_alpha, "texture_alpha");
    }
  } while((p_line = p_line->section_next()));
}

/*
  Load aux target
{
  name = test_aux
  type = AUX_PARAMS

  aux_size.x = 512
  aux_size.y = 512
}
*/
void generator_mesh::load_section_aux_params(DATA_FILE_SECTION *p_section)
{
  TEXTURE_DESCRIPTION aux;
  
  DATA_FILE_SECTION *p_line = p_section->section_child_get();    
  if(p_line) {      
    do {
      if(p_line->is_line()) {
        load_vect2di(aux.texture_size, "aux_size");
        load_int(aux.texture_height, "aux_height");
        load_rgbaf(aux.texture_background_color, "background_color");
        load_int(aux.texture_alpha, "aux_alpha");
      }
    } while((p_line = p_line->section_next()));
  }
  
  MODIFICATOR_TARGET_AUX *p_target =
  (MODIFICATOR_TARGET_AUX *)target_cache.target_get(MODIFICATOR_TARGET_AUX_TYPE,
                                                    p_section->line_name_get());
  p_target->init(aux.texture_size, aux.texture_height);
  
  SURFACE_SW *p_bitmap = p_target->bitmap_get();
  p_bitmap->fill(aux.texture_background_color);
}

/*
{
  name = ***
  type = GENERATOR_MESH_CONFIG
}
*/
void generator_mesh::load_section_generator_config(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line)
    return;
  
  // Set name of this generator
  name_set(p_section->name_get());
  
  int modificator_index = -1;
  int target_index = -1;
  do {
    if(p_line->is_line()) {
      
      if(p_line->match_name("modificator")) {
        modificator_index++;
        target_index = -1;
        GENERATOR_MESH_MODIFICATOR_CONFIG *p_modificator = config.modificator_ref(modificator_index);
        strncpy(p_modificator->modificator_name, p_line->line_value_string_get(), MAX_NAME);
        p_line->line_mark_loaded();        
        continue;
      }
      else if(modificator_index >= 0) {        
        GENERATOR_MESH_MODIFICATOR_CONFIG *p_modificator = config.modificator_ref(modificator_index);

        // modificator target - for unique targets (TEXTURE/MESH)
        if(p_line->match_name("modificator_target")) {
          target_index++;
          GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG *p_target = p_modificator->target_config_ref(target_index);
          p_target->target_type = modificator_target_translate(p_line->line_value_string_get());
          if(p_target->target_type == MODIFICATOR_TARGET_NONE) {            
            ppset(p_line->source_file_get(), p_line->source_line_get());
            pperror(TRUE, "'%s' is not a valid modificator target!",p_line->line_value_string_get());
            ppclear();
          }
          p_line->line_mark_loaded();
          continue;
        }
        else {
          GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG *p_target = p_modificator->target_config_ref(target_index);
          
          // For other targets (BITMAP/HEIGHTMAP/AUX)
          if(p_line->match_name("modificator_target_name")) {
            strncpy(p_target->target_name, p_line->line_value_string_get(), MAX_NAME);
            p_line->line_mark_loaded();
            continue;
          }
      
          // Load repeat
          load_int(p_target->repeat,"modificator_repeat");
                    
          // Load mask
          p_target->mask.load_line(p_line, generator_get());
        }
      }
    }
  } while((p_line = p_line->section_next()));
}


void generator_mesh::load_section(DATA_FILE_SECTION *p_section)
{
  // Load all lines from section
  MODIFICATOR_TYPE type = modificator_type_item::type_translate(p_section->section_type_get());
  switch(type) {
    case MESH_PARAMETERS_TYPE:
      load_section_mesh_params(p_section);
      break;
    case MATERIAL_PARAMETERS_TYPE:
      load_section_material_params(p_section);
      break;
    case TEXTURE_PARAMETERS_TYPE:
      load_section_texture_params(p_section);
      break;
    case AUX_PARAMETERS_TYPE:
      load_section_aux_params(p_section);
      break;    
    case GENERATOR_MESH_TYPE:
      load_section_generator_config(p_section);
      break;
    default:
      generator_loader_error(p_section->source_file_get(), 
                             p_section->source_line_get(), 
                             "Unknown section name %s, type %s",
                             p_section->name_get(),
                             p_section->section_type_get());
      break;
  }
}

void generator_mesh::create(void)
{
  
}

void generator_mesh::clear(void)
{  
}

void generator_mesh::destroy(void)
{
  clear();
}

generator_mesh::generator_mesh(GENERATOR *p_generator, const char *p_name)
  : generator_reference(p_generator),
    object_list(NULL),
    target_cache(this)
{
  clear();  
  name_set(p_name);
}

generator_mesh::~generator_mesh(void)
{
  destroy();
}
