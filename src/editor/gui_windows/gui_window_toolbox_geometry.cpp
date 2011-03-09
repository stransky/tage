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

/*
 * geometry sub-list
 */
typedef enum
{
  GEOMETRY_NAME_COLUMN      = 0,
  GEOMETRY_TYPE_COLUMN      = 1,
  GEOMETRY_MATERIAL_COLUMN  = 2,
  GEOMETRY_POINTER_COLUMN   = 3,
  GEOMETRY_N_COLUMNS
  
} GEOMETRY_COLUMNS;

/*
 * Editor main gui lists
 */
gboolean gui_geometry_selection_func(GtkTreeSelection *selection,
                                     GtkTreeModel *model,
                                     GtkTreePath *path,
                                     gboolean path_currently_selected,
                                     gpointer data)
{
  /*
  SCENE_OBJECT *p_obj = NULL;
  
  GtkTreeIter iter;
  gtk_tree_model_get_iter(model,&iter,path);
  gtk_tree_model_get(model,&iter,GEOMETRY_POINTER_COLUMN,&p_obj,-1);

  SCENE *p_scene = scene_get();
  
  if(p_scene == NULL || p_obj == NULL)
    return(FALSE);
  
  if(path_currently_selected) {
    // Already selected - unselect
    pprintf("Unselect - %s",p_obj->name_get());
    p_obj->unselect();
  }
  else {
    // Select
    pprintf("Select - %s",p_obj->name_get());
    p_obj->select();
  }
  */
  return(TRUE);
}

void gui_geometry_row_changed(GtkTreeModel *tree_model,
                              GtkTreePath  *path,
                              GtkTreeIter  *iter,
                              gpointer      user_data)
{
  /*
  pprintf("%s",__FUNCTION__);
    
  SCENE_OBJECT *p_obj = NULL;
  SCENE_OBJECT *p_parent = NULL;
  gtk_tree_model_get(tree_model,iter,GEOMETRY_POINTER_COLUMN,&p_obj,-1);
  if(p_obj) {
    pprintf("Objekt: %s", p_obj->name_get());
  
    GtkTreeIter iter_parent;
    if(gtk_tree_model_iter_parent(GTK_TREE_MODEL(tree_model),&iter_parent,iter)) {
      gtk_tree_model_get(tree_model,&iter_parent,GEOMETRY_POINTER_COLUMN,&p_parent,-1);
    }
    pprintf("Parent: %p : %s", p_parent, p_parent ? p_parent->name_get() : "NULL");
    p_obj->render_hierarchy_parent_set(p_parent);
  }
  */
}

/*
void editor_main_gui_geometry_row_deleted(GtkTreeModel *tree_model,
                                          GtkTreePath *path,
                                          gpointer user_data)
{
  pprintf("%s",__FUNCTION__);  
  GtkTreeIter iter;
  if(gtk_tree_model_get_iter(tree_model,&iter,path)) {
    pprintf("Iter get");
    SCENE_OBJECT *p_obj;
    gtk_tree_model_get(tree_model,&iter,GEOMETRY_POINTER_COLUMN,&p_obj,-1);  
    if(p_obj)
      pprintf("%s",p_obj->name_get());
  }  
}

void editor_main_gui_geometry_row_inserted(GtkTreeModel *tree_model,
                                           GtkTreePath  *path,
                                           GtkTreeIter  *iter,
                                           gpointer      user_data)
{
  pprintf("%s",__FUNCTION__);
    
  SCENE_OBJECT *p_obj = NULL;
  gtk_tree_model_get(tree_model,iter,GEOMETRY_POINTER_COLUMN,&p_obj,-1);
  
  if(p_obj)
    pprintf("%s : %s", p_obj->name_get());
}
*/

static void gui_geometry_create(GtkWidget *p_tree)
{
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(p_tree),TRUE);
  
  // Remove all current columns
  GtkTreeViewColumn *tmp = NULL;
  while((tmp = gtk_tree_view_get_column(GTK_TREE_VIEW(p_tree),0))) {
    gtk_tree_view_remove_column(GTK_TREE_VIEW(p_tree),tmp);
  }
  
  // Append new ones
  GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
  
  GtkTreeViewColumn * treeViewColumn = NULL;
  treeViewColumn = gtk_tree_view_column_new_with_attributes("Name",renderer,"text",GEOMETRY_NAME_COLUMN,NULL);
  gtk_tree_view_column_set_reorderable(treeViewColumn,TRUE);
  gtk_tree_view_column_set_sort_column_id(treeViewColumn,GEOMETRY_NAME_COLUMN);
  gtk_tree_view_append_column(GTK_TREE_VIEW(p_tree),treeViewColumn);
  
  treeViewColumn = gtk_tree_view_column_new_with_attributes("Type",renderer,"text",GEOMETRY_TYPE_COLUMN,NULL);
  gtk_tree_view_column_set_reorderable(treeViewColumn,TRUE);
  gtk_tree_view_column_set_sort_column_id(treeViewColumn,GEOMETRY_TYPE_COLUMN);
  gtk_tree_view_append_column(GTK_TREE_VIEW(p_tree),treeViewColumn);
  
  treeViewColumn = gtk_tree_view_column_new_with_attributes("Material",renderer,"text",GEOMETRY_MATERIAL_COLUMN,NULL);
  gtk_tree_view_column_set_reorderable(treeViewColumn,TRUE);
  gtk_tree_view_column_set_sort_column_id(treeViewColumn,GEOMETRY_MATERIAL_COLUMN);
  gtk_tree_view_append_column(GTK_TREE_VIEW(p_tree),treeViewColumn);
  
  GtkTreeSelection * treeSelection;
  treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(p_tree));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(treeSelection),GTK_SELECTION_MULTIPLE);
  
  gtk_tree_selection_set_select_function(treeSelection, 
                                         gui_geometry_selection_func,
                                         NULL,NULL);
}

static void gui_geometry_update(GUI_STATE *p_state, GtkWidget *p_tree)
{  
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GUI_WINDOW_RUNNING *p_window = p_interface->gui_main_window_get();  
  
  if(p_window == NULL)
    return;
  
  static GtkTreeStore *p_store = NULL;
  static gulong row_signal_id = 0;
  
  if(p_store == NULL) {
    p_store = gtk_tree_store_new(GEOMETRY_N_COLUMNS,
                                 G_TYPE_STRING,
                                 G_TYPE_STRING, 
                                 G_TYPE_STRING, 
                                 G_TYPE_POINTER);
    row_signal_id = g_signal_connect((gpointer)p_store, "row-changed",
                                 G_CALLBACK(gui_geometry_row_changed),
                                 NULL);
  }
  
  assert(row_signal_id != 0);
  
  g_signal_handler_block((gpointer)p_store,row_signal_id);
  gtk_tree_view_set_model(GTK_TREE_VIEW(p_tree), GTK_TREE_MODEL(p_store));
    
  GtkTreeSelection *treeSelection;
  treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(p_tree));
  
  gtk_tree_store_clear(p_store);
  
  SCENE *p_scene = p_interface->scene_get();
  if(!p_scene)
    return;
  
  OBJECT_HIERARCHY_ITERATOR it = p_scene->render_hierarchy_next_new();
  SCENE_OBJECT *p_obj;
  SCENE_OBJECT *p_obj_last = NULL;
      
  GtkTreeIter iter;
  GtkTreeIter iter_last;    
  while((p_obj = p_scene->render_hierarchy_next(&it))) {
    
    SCENE_OBJECT *p_obj_parent = p_obj->render_hierarchy_parent_get();
    assert(p_obj_parent != NULL);
    
    if(p_obj_last == NULL || p_obj_parent->is_render_hierarchy_root()) {
      // Insert a new TOP-level entry
      gtk_tree_store_insert(p_store, &iter, NULL, 0);
    } else if(p_obj_last->render_hierarchy_parent_get() == p_obj->render_hierarchy_parent_get()) {
      // Insert on the same level
      GtkTreeIter iter_parent;
      gtk_tree_model_iter_parent(GTK_TREE_MODEL(p_store),&iter_parent,&iter_last);
      gtk_tree_store_append(p_store,&iter,&iter_parent);
    } else if(p_obj_last == p_obj->render_hierarchy_parent_get()) {
      // Insert under this level
      gtk_tree_store_append(p_store,&iter,&iter_last);
    } else {
      // Search parent
      SCENE_OBJECT *p_tmp = p_obj_last;
      GtkTreeIter       iter_tmp = iter_last;
      
      while(p_tmp != NULL || p_tmp != p_obj->render_hierarchy_parent_get())
      {
        GtkTreeIter iter_parent;
        gtk_tree_model_iter_parent(GTK_TREE_MODEL(p_store),&iter_parent,&iter_tmp);
        
        p_tmp = (SCENE_OBJECT *)p_tmp->render_hierarchy_parent_get();
        iter_tmp = iter_parent;
      }
      
      if(p_tmp != NULL) {
        gtk_tree_store_append(p_store,&iter,&iter_tmp);
      }
      else {
        gtk_tree_store_insert(p_store, &iter, NULL, 0);
      }
    }
    pprintf(p_obj->name_get());
    
    gtk_tree_store_set(p_store, &iter,GEOMETRY_NAME_COLUMN, p_obj->name_get(),
                                      GEOMETRY_TYPE_COLUMN, "MESH",
                                      GEOMETRY_MATERIAL_COLUMN, "(none)",
                                      GEOMETRY_POINTER_COLUMN, (void *)p_obj,
                                      -1);
    
    if(p_obj->mark_get(MARK_SELECTED)) {
      gtk_tree_selection_select_iter(treeSelection,&iter);
    }

    p_obj_last = p_obj;
    iter_last = iter;
  }
  
  g_signal_handler_unblock((gpointer)p_store,row_signal_id); 
}

//void gui_geometry_update(void)
//{
  /*
  GUI_WINDOW_RUNNING *p_window = main_gui_state.gui_window_get();
  
  if(p_window == NULL)
    return;
  
  GtkWidget *combo = lookup_widget(GTK_WIDGET(p_window->gtk_window_get()),"combobox3");
  gint active_list = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
  
  if(main_gui_state.object_list != active_list)
  {
    main_gui_state.object_list = (OBJECT_LIST)active_list;
    editor_main_gui_switch();
  }      
  
  switch(main_gui_state.object_list) {
    case OBJECT_LIST_GEOMETRY:
      editor_main_gui_update_geometry();
      break;
    case OBJECT_LIST_MATERIALS:
      editor_main_gui_update_materials();
      break;
    default:
      assert(0);
      break;
  }
  */
//}

/*
 * Activate geometry selection in the geometry toolbox
 */
bool gui_scene_geometry_create_func(GUI_STATE *p_state)
{ 
  // Get the object tool widget
  GtkWidget *p_object_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_SCENE);
  assert(p_object_toolbox);
  GtkWidget *p_tree = lookup_widget_directly(GTK_WIDGET(p_object_toolbox),"editor_main_treeview_object_selection");
  assert(p_tree);
  
  gui_geometry_create(p_tree);  
  
  return(TRUE);
}

bool gui_scene_geometry_update_func(GUI_STATE *p_state)
{
  // Get the object tool widget  
  GtkWidget *p_object_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_SCENE);
  assert(p_object_toolbox);
  GtkWidget *p_tree = lookup_widget_directly(GTK_WIDGET(p_object_toolbox),"editor_main_treeview_object_selection");
  assert(p_tree);
    
  gui_geometry_update(p_state, p_tree);
  
  return(TRUE);
}
