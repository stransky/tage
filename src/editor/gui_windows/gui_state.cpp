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

gui_config::gui_config(void)
{
  gui_enabled = TRUE;
  script_file[0] = '\0';
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
  if(toolbox_active == active)
    return(TRUE);
  
  if(toolbox_active < TOOLBOX_NUM)
    toolbox[toolbox_active].destroy();
  
  toolbox_active = active;
  toolbox[toolbox_active].create();

  
  GUI_INTERFACE *p_interface = gui_interface_get();
  GtkWidget *combobox_toolbox_type = lookup_widget(GTK_WIDGET(p_interface->gtk_main_window_get()),"combobox_tool_type");
  assert(combobox_toolbox_type != NULL);
  
  int active_item = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox_toolbox_type));
  if(active_item != (int)active) {
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_toolbox_type),(int)active);
  }
  
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
  if(scene_element != SCENE_PANEL_NONE && toolbox_scene_panel_active == scene_element)
    return;
  
  if(toolbox_scene_panel_active != SCENE_PANEL_NONE) {
    GUI_ELEMENT *p_old = toolbox_scene_panel_get(toolbox_scene_panel_active);
    p_old->destroy();
  }
  
  toolbox_scene_panel_active = (scene_element != SCENE_PANEL_NONE) ? scene_element : SCENE_PANEL_GEOMETRY;
  GUI_ELEMENT *p_new = toolbox_scene_panel_get(toolbox_scene_panel_active);
  p_new->create();

  GtkWidget *p_object_toolbox = (GtkWidget *)toolbox_user_data_get(TOOLBOX_SCENE);
  if(p_object_toolbox) {
    GtkWidget *p_combo = lookup_widget_directly(GTK_WIDGET(p_object_toolbox),"combobox3");
    assert(p_combo);
    
    gtk_combo_box_set_active(GTK_COMBO_BOX(p_combo),(int)toolbox_scene_panel_active);
  }
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
    editor_state_selection(p_editor),
        
    scene_geometry_list(this),
    scene_material_list(this),
    gen_mesh_list(this),
    gen_mesh_modificator_list(this)

{  
  int i;
  for(i = 0; i < TOOLBOX_NUM; i++)
    toolbox[i].gui_state_set(this);
  
  toolbox_active = TOOLBOX_NONE;
  toolbox_scene_panel_active = SCENE_PANEL_NONE;
  
  /*
   * Main tool panel
   */
  toolbox[TOOLBOX_SCENE].create_func = gui_scene_create_func;
  toolbox[TOOLBOX_SCENE].destroy_func = gui_scene_destroy_func;  
  
  toolbox[TOOLBOX_MODIFICATOR].create_func = gui_modificators_create_func;
  toolbox[TOOLBOX_MODIFICATOR].destroy_func = gui_modificators_destroy_func;
    
  scene_geometry_list.create_func = gui_scene_geometry_create_func;
  scene_geometry_list.update_func = gui_scene_geometry_update_func;
  
  scene_material_list.create_func = gui_scene_material_create_func;
  scene_material_list.update_func = gui_scene_material_update_func;
    
  gen_mesh_list.update_func = gui_generator_mesh_list_update;

  gen_mesh_modificator_list.create_func = gui_generator_modificator_list_create;
  gen_mesh_modificator_list.update_func = gui_generator_modificator_list_update;
}
