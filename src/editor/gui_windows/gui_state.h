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
 * Editor gui state
 */
#ifndef __GENERATOR_GUI_STATE_H__
#define __GENERATOR_GUI_STATE_H__

/*
  editor_state_selection
  
  - this class provides selection mechanism for the editor  

  Object selection is handled:

    editor <-> editor_state <-> age 

      All geometry objects that can be selected at age level:
        - meshes
        - lights

      All other objects are selected at editor level:
        - textures
        - materials
        - generators
*/

typedef class editor_state_selection
  : virtual public editor_reference
{
  
  MATERIAL_TEXT           *p_texture_selected;
  MESH_MATERIAL           *p_material_selected;
  GENERATOR_MESH          *p_generator_mesh_selected;
  MODIFICATOR             *p_generator_mesh_modificator_selected;
  
public:

  void selection_mesh_set(GAME_MESH *p_mesh);
  bool is_selected_mesh(GAME_MESH *p_mesh);
    
  void selection_texture_set(MATERIAL_TEXT *p_text)
  {
    p_texture_selected = p_text;
  }
  bool is_selected_texture(MATERIAL_TEXT *p_text)
  {
    return(p_texture_selected == p_text);
  }

  void selection_material_set(MESH_MATERIAL *p_mat)
  {
    p_material_selected = p_mat;
  }
  bool is_selected_material(MESH_MATERIAL *p_mat)
  {
    return(p_material_selected == p_mat);
  }
  
  void selection_generator_mesh_set(GENERATOR_MESH  *p_generator)
  {
    p_generator_mesh_selected = p_generator;
  }
  GENERATOR_MESH * selection_generator_mesh_get(void)
  {
    return(p_generator_mesh_selected);
  }
  bool is_selected_generator_mesh(GENERATOR_MESH *p_generator)
  {
    return(p_generator_mesh_selected == p_generator);
  }

  void selection_generator_mesh_modificator_set(MODIFICATOR *p_modificator)
  {
    p_generator_mesh_modificator_selected = p_modificator;
  }
  bool is_selected_generator_mesh_modificator(MODIFICATOR *p_modificator)
  {
    return(p_generator_mesh_modificator_selected == p_modificator);
  }

public:
  
  editor_state_selection(EDITOR *p_editor);

} EDITOR_STATE_SELECTION;

/*
 * Editor main gui config
 */

typedef enum
{  
  TOOLBOX_SCENE        = 0,
  TOOLBOX_MODIFICATOR  = 1,
  TOOLBOX_NUM          = 2,
  TOOLBOX_NONE,
  
} TOOLBOX_ITEM;

/*
typedef enum
{
  TOOL_OBJECT_GEOMETRY      = 0,
  TOOL_OBJECT_MATERIALS     = 1,
  TOOL_OBJECT_NUM           = 2,
  TOOL_OBJECT_NONE,
  
} TOOL_OBJECT_LIST;
*/

typedef bool (* GUI_ELEMENT_FUNC)(GUI_STATE *p_state);

typedef class gui_element
  : public class_user_data
{   
  // Used gui state
  GUI_STATE          *p_state;

public:

  void gui_state_set(GUI_STATE *p_state)
  {
    this->p_state = p_state;
  }

private:
  
  // This element is active (already created)
  bool                created;
  int                 update_state;
    
public:
  
  GUI_ELEMENT_FUNC    create_func;
  GUI_ELEMENT_FUNC    destroy_func;
  GUI_ELEMENT_FUNC    update_func;

public:  

  void update(void)
  {
    if(update_func && created)
      update_func(p_state);
  }

  void update_start(void)
  {
    update_state++;
  }

  void update_stop(void)
  {
    update_state--;
    
    assert(update_state >= 0);
    
    if(update_state == 0)
      update();
  }

  void create(void)
  {
    if(!created) {      
      created = create_func ? create_func(p_state) : TRUE;
      
      if(created) {        
        update();        
      }
    }
  }
  
  void destroy(void)
  {
    if(created) {
      created = destroy_func ? !destroy_func(p_state) : FALSE;
    }
  }

public:  
  
  gui_element(GUI_STATE *p_state = NULL)
  {
    gui_state_set(p_state);
    
    created = FALSE;
    update_state = 0;

    create_func = NULL;
    destroy_func = NULL;
    update_func = NULL;
  }
  
} GUI_ELEMENT;


typedef enum {
  
  EDITOR_PART_TEXTURE_LIST  = 0,
  EDITOR_PART_MATERIAL_LIST,
  EDITOR_PART_MESH_LIST,
  EDITOR_PART_GENERATOR_MESH_LIST,
  EDITOR_PART_GENERATOR_MESH_MODIFICATOR_LIST,
  EDITOR_PART_LAST,
  
} EDITOR_PART;


typedef enum {
  
  SCENE_PANEL_GEOMETRY  = 0,
  SCENE_PANEL_MATERIALS = 1,
  SCENE_PANEL_NONE,
  
} SCENE_PANEL;

typedef struct gui_config {
  
  bool gui_enabled;
  char script_file[MAX_FILENAME];
  
  gui_config();
  
} GUI_CONFIG;

/*
 * Holds state of the GUI
 */
typedef class gui_state :
  public virtual editor_reference,
  public editor_state_selection
{  
  /*
   * GUI config
   */  
public:

  GUI_CONFIG        config;
  
public:
  
  GUI_CONFIG *      config_get(void)
  {
    return(&config);
  }
  
  /*
   * GUI toolbox window state
   */  
private:

  GUI_ELEMENT       toolbox[TOOLBOX_NUM];
  TOOLBOX_ITEM      toolbox_active;

public:

  void              toolbox_user_data_set(TOOLBOX_ITEM tool, void *p_data);
  void *            toolbox_user_data_get(TOOLBOX_ITEM tool);

public:

  TOOLBOX_ITEM      toolbox_active_get(void);
  bool              toolbox_active_set(TOOLBOX_ITEM active);

  /*
   * Scene panels in toolbox
   */    
private:
  
  GUI_ELEMENT       scene_geometry_list;
  GUI_ELEMENT       scene_material_list;

  // Panels in scene toolbox
  SCENE_PANEL       toolbox_scene_panel_active;

private:

  GUI_ELEMENT     * toolbox_scene_panel_get(SCENE_PANEL scene_element);

public:
  
  void              toolbox_scene_panel_set(SCENE_PANEL scene_element = SCENE_PANEL_NONE);

  /*
   * Generator panel in toolbox
   */
public:
  
  // Elements in modificator toolbox
  GUI_ELEMENT       gen_mesh_list;
  GUI_ELEMENT       gen_mesh_modificator_list;


  /*
   * Change notifications (for gui)
   */
private:

  bool              change_part[EDITOR_PART_LAST];

public:

  void              change_start(EDITOR_PART editor_part);
  void              change_stop(EDITOR_PART editor_part);

  /*
   * GUI toolbox object (geometry/material) window
   */
public:
  
  gui_state(EDITOR *p_editor);
  
} GUI_STATE;

/*
 * Helpers from glade
 */
#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

inline GtkWidget * lookup_widget_directly(GtkWidget *widget, const gchar *widget_name)
{
  GtkWidget * found_widget = (GtkWidget*)g_object_get_data(G_OBJECT (widget), widget_name);
  if (!found_widget)
    g_warning ("Widget not found: %s", widget_name);
  return found_widget;
}

#endif // __GENERATOR_GUI_STATE_H__
