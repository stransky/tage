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

editor_state_selection::editor_state_selection(EDITOR *p_editor)
  : editor_reference(p_editor)
{
  p_texture_selected = NULL;
  p_material_selected = NULL;
  p_generator_mesh_modificator_selected = NULL;
  p_generator_mesh_selected = NULL;
}

/*
 * Active mesh modificator
 */
 /*
MESH_MODIFICATOR_ITEM * editor_state::generator_mesh_modificator_active_get(void)
{
  return(p_generator_mesh_modificator_active);
}

void editor_state::generator_mesh_modificator_active_set(MESH_MODIFICATOR_ITEM *p_modificator = NULL)
{
  if(p_generator_mesh_modificator_active != p_modificator) {
    p_generator_mesh_modificator_active = p_modificator;
    generator_mesh_modificator_active_changed = TRUE;
  }
}

bool editor_state::generator_mesh_modificator_active_change_get(void)
{
  return(generator_mesh_modificator_active_changed);
}

void editor_state::generator_mesh_modificator_active_change_clear(void)
{
  generator_mesh_modificator_active_changed = FALSE;
}
*/

/*
 * Active mesh generator
 */
 /*
GENERATOR_MESH  * editor_state::generator_mesh_active_get(void)
{
  return(p_generator_mesh_active);
}

void editor_state::generator_mesh_active_set(GENERATOR_MESH  *p_generator = NULL)
{
  if(p_generator_mesh_active != p_generator) {
    p_generator_mesh_active = p_generator;      
    generator_mesh_active_changed = TRUE;
    
    // Reset active mesh modificator
    generator_mesh_modificator_active_set();
  }
}

bool editor_state::generator_mesh_active_change_get(void)
{
  return(generator_mesh_active_changed);
}

void editor_state::generator_mesh_active_change_clear(void)
{
  generator_mesh_active_changed = FALSE;
}
*/

void gui_state::toolbox_user_data_set(TOOLBOX_ITEM tool, void *p_data)
{
  assert(tool < TOOLBOX_NUM);
  toolbox[tool].user_data_set(p_data);
}

void * gui_state::toolbox_user_data_get(TOOLBOX_ITEM tool)
{
  assert(tool < TOOLBOX_NUM);
  return(toolbox[tool].user_data_get());
}

// Set active toolbox
bool gui_state::toolbox_active_set(TOOLBOX_ITEM active)
{   
  return(TRUE);
}

TOOLBOX_ITEM gui_state::toolbox_active_get(void)
{
  return(toolbox_active);
}

GUI_ELEMENT * gui_state::toolbox_scene_panel_get(SCENE_PANEL scene_element)
{
  switch(scene_element) {   
    case SCENE_PANEL_GEOMETRY:
      return(&scene_geometry_list);
    case SCENE_PANEL_MATERIALS:
      return(&scene_material_list);
    default:
      return(NULL);
  }
}

void gui_state::toolbox_scene_panel_set(SCENE_PANEL scene_element)
{
  
}

void gui_state::change_start(EDITOR_PART editor_part)
{
  change_part[editor_part] = TRUE;
  
  switch(editor_part) {    
    case EDITOR_PART_TEXTURE_LIST:
      break;
    case EDITOR_PART_MATERIAL_LIST:
      scene_material_list.update_start();
      break;
    case EDITOR_PART_MESH_LIST:
      scene_geometry_list.update_start();
      break;
    case EDITOR_PART_GENERATOR_MESH_LIST:
      gen_mesh_list.update_start();
      break;
    case EDITOR_PART_GENERATOR_MESH_MODIFICATOR_LIST:
      gen_mesh_modificator_list.update_start();
      break;
    default:
      assert(0);
      break;
  }  
}

void gui_state::change_stop(EDITOR_PART editor_part)
{
  change_part[editor_part] = TRUE;
  
  switch(editor_part) {    
    case EDITOR_PART_TEXTURE_LIST:      
      break;
    case EDITOR_PART_MATERIAL_LIST:
      scene_material_list.update_stop();
      break;
    case EDITOR_PART_MESH_LIST:
      scene_geometry_list.update_stop();
      break;
    case EDITOR_PART_GENERATOR_MESH_LIST:
      gen_mesh_list.update_stop();
      break;
    case EDITOR_PART_GENERATOR_MESH_MODIFICATOR_LIST:
      gen_mesh_modificator_list.update_stop();
      break;
    default:
      assert(0);
      break;
  }  
}

gui_state::gui_state(EDITOR *p_editor)

  : editor_reference(p_editor),
    editor_state_selection(p_editor)
{  
  int i;
  for(i = 0; i < TOOLBOX_NUM; i++)
    toolbox[i].gui_state_set(this);  
}
