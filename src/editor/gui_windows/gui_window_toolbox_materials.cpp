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
 * Material sub-list
 */
typedef enum
{
  MATERIAL_NAME_COLUMN      = 0,
  MATERIAL_POINTER_COLUMN   = 1,
  MATERIAL_N_COLUMNS
  
} MATERIAL_COLUMNS;

static gboolean gui_material_selection_func(GtkTreeSelection *selection,
                                            GtkTreeModel *model,
                                            GtkTreePath *path,
                                            gboolean path_currently_selected,
                                            gpointer data)
{
  if(path_currently_selected)
    return(TRUE);
  
  MESH_MATERIAL *p_mat = NULL;
  
  GtkTreeIter iter;
  gtk_tree_model_get_iter(model,&iter,path);
  gtk_tree_model_get(model,&iter,MATERIAL_POINTER_COLUMN,&p_mat,-1);
  
  pprintf("Vyber - %s",p_mat->name_get());
  
  return(TRUE);
}

static void gui_materials_create(GtkWidget *p_tree)
{
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(p_tree), FALSE);
  
  // Remove old comumns
  GtkTreeViewColumn *tmp = NULL;
  while((tmp = gtk_tree_view_get_column(GTK_TREE_VIEW(p_tree),0))) {
    gtk_tree_view_remove_column(GTK_TREE_VIEW(p_tree),tmp);
  }
  
  // Add new ones
  GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
  
  GtkTreeViewColumn * treeViewColumn = NULL;
  treeViewColumn = gtk_tree_view_column_new_with_attributes("Name",renderer,"text",MATERIAL_NAME_COLUMN,NULL);    
  gtk_tree_view_column_set_reorderable(treeViewColumn,TRUE);
  gtk_tree_view_column_set_sort_column_id(treeViewColumn,MATERIAL_NAME_COLUMN);
  gtk_tree_view_append_column(GTK_TREE_VIEW(p_tree),treeViewColumn);
  
  GtkTreeSelection * treeSelection;
  treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(p_tree));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(treeSelection),GTK_SELECTION_MULTIPLE);
  gtk_tree_selection_set_select_function(treeSelection,gui_material_selection_func,NULL,NULL);
}

static void gui_materials_update(GUI_STATE *p_state, GtkWidget *p_tree)
{
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GUI_WINDOW_RUNNING *p_window = p_interface->gui_main_window_get();  
  
  if(p_window == NULL)
    return;
  
  static GtkListStore *p_store = NULL;
  if(p_store == NULL) {
    p_store = gtk_list_store_new(MATERIAL_N_COLUMNS,
                                 G_TYPE_STRING,
                                 G_TYPE_POINTER);
  }
  gtk_tree_view_set_model(GTK_TREE_VIEW(p_tree), GTK_TREE_MODEL(p_store));
  
  gtk_list_store_clear(p_store);
  
  SCENE *p_scene = p_interface->scene_get();
  if(p_scene == NULL)
    return;
    
  MESH_MATERIAL *p_mat = p_scene->material_get_first();    
  while(p_mat) {
    GtkTreeIter iter;
    gtk_list_store_append(p_store, &iter);
    gtk_list_store_set(p_store, &iter, MATERIAL_NAME_COLUMN, p_mat->name_get(),
                                       MATERIAL_POINTER_COLUMN, (void *)p_mat,
                                      -1);
    p_mat = p_mat->material_next();
  } 
}

bool gui_scene_material_create_func(GUI_STATE *p_state)
{
  // Get the object tool widget
  GtkWidget *p_object_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_SCENE);
  assert(p_object_toolbox);
  GtkWidget *p_tree = lookup_widget_directly(GTK_WIDGET(p_object_toolbox),"editor_main_treeview_object_selection");
  assert(p_tree);

  gui_materials_create(p_tree);
  gui_materials_update(p_state, p_tree);
  
  return(TRUE);
}

bool gui_scene_material_update_func(GUI_STATE *p_state)
{
  // Get the object tool widget
  GtkWidget *p_object_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_SCENE);
  assert(p_object_toolbox);
  GtkWidget *p_tree = lookup_widget_directly(GTK_WIDGET(p_object_toolbox),"editor_main_treeview_object_selection");
  assert(p_tree);

  gui_materials_update(p_state, p_tree);

  return(TRUE);
}
