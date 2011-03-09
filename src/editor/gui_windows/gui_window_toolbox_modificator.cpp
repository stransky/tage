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
 * Modificator sub-list
 */
typedef enum
{
  MODIFICATOR_TYPE_NAME_COLUMN  = 0,
  MODIFICATOR_NAME_COLUMN       = 1,
  MODIFICATOR_POINTER_COLUMN    = 2,
  MODIFICATOR_TYPE_ENUM_COLUMN  = 3,
  MODIFICATOR_N_COLUMNS
  
} MODIFICATOR_COLUMNS;

static gboolean gui_modificator_list_selection_func(GtkTreeSelection *selection,
                                               GtkTreeModel *model,
                                               GtkTreePath *path,
                                               gboolean path_currently_selected,
                                               gpointer data)
{
  if(path_currently_selected)
    return(TRUE);
  
  MODIFICATOR *p_item = NULL;
  
  GtkTreeIter iter;
  gtk_tree_model_get_iter(model,&iter,path);
  gtk_tree_model_get(model,&iter,MODIFICATOR_POINTER_COLUMN,&p_item,-1);
  
  if(p_item)
    pprintf("Vyber - %s",p_item->name_get());
  
  return(TRUE);
}

bool gui_generator_modificator_list_create(GUI_STATE *p_state)
{  
  GtkWidget *p_modificator_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_MODIFICATOR);
  assert(p_modificator_toolbox);
  GtkWidget *p_tree = lookup_widget_directly(p_modificator_toolbox,"editor_main_treeview_object_selection");
  assert(p_tree);
  
  gtk_tree_view_set_reorderable(GTK_TREE_VIEW(p_tree), FALSE);
  
  // Remove old comumns
  GtkTreeViewColumn *tmp = NULL;
  while((tmp = gtk_tree_view_get_column(GTK_TREE_VIEW(p_tree),0))) {
    gtk_tree_view_remove_column(GTK_TREE_VIEW(p_tree),tmp);
  }
  
  // Add new ones
  GtkCellRenderer * renderer = gtk_cell_renderer_text_new();
  
  GtkTreeViewColumn * treeViewColumn = NULL;
  
  treeViewColumn = gtk_tree_view_column_new_with_attributes("Type",renderer,"text",MODIFICATOR_TYPE_NAME_COLUMN,NULL);
  gtk_tree_view_column_set_reorderable(treeViewColumn,TRUE);
  gtk_tree_view_column_set_sort_column_id(treeViewColumn,MODIFICATOR_TYPE_NAME_COLUMN);
  gtk_tree_view_append_column(GTK_TREE_VIEW(p_tree),treeViewColumn);
  
  treeViewColumn = gtk_tree_view_column_new_with_attributes("Name",renderer,"text",MODIFICATOR_NAME_COLUMN,NULL);
  gtk_tree_view_column_set_reorderable(treeViewColumn,TRUE);
  gtk_tree_view_column_set_sort_column_id(treeViewColumn,MODIFICATOR_NAME_COLUMN);
  gtk_tree_view_append_column(GTK_TREE_VIEW(p_tree),treeViewColumn);
  
  GtkTreeSelection * treeSelection;
  treeSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(p_tree));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(treeSelection),GTK_SELECTION_MULTIPLE);
  gtk_tree_selection_set_select_function(treeSelection,gui_modificator_list_selection_func,NULL,NULL);
    
  return(TRUE);
}

bool gui_generator_modificator_list_update(GUI_STATE *p_state)
{
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GUI_WINDOW_RUNNING *p_window = p_interface->gui_main_window_get();
  
  if(p_window == NULL)
    return(FALSE);

  GtkWidget *p_modificator_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_MODIFICATOR);
  assert(p_modificator_toolbox);
  GtkWidget *p_tree = lookup_widget_directly(p_modificator_toolbox,"editor_main_treeview_object_selection");
  assert(p_tree);

  
  static GtkTreeStore *p_store = NULL;
  if(p_store == NULL) {
    p_store = gtk_tree_store_new(MODIFICATOR_N_COLUMNS,
                                  G_TYPE_STRING,
                                  G_TYPE_STRING,
                                  G_TYPE_POINTER,
                                  G_TYPE_INT);
  }
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(p_tree), GTK_TREE_MODEL(p_store));
  
  gtk_tree_store_clear(p_store);
  
  GENERATOR_MESH *p_gen = p_state->selection_generator_mesh_get();
  if(!p_gen)
    return(TRUE);
  /*
  MODIFICATOR *p_mod = &p_gen->mesh;
  MODIFICATOR_ITEM *p_item = p_mod->modificator_get_first();

  GtkTreeIter iterator_modificators;

  // Insert mesh parameters
  gtk_tree_store_append(p_store, &iterator_modificators, NULL);
  gtk_tree_store_set(p_store, &iterator_modificators, 
                              MODIFICATOR_TYPE_NAME_COLUMN, modificator_type_item::type_translate_name(MESH_PARAMETERS),
                              MODIFICATOR_NAME_COLUMN,      "",
                              MODIFICATOR_POINTER_COLUMN,   (void *)NULL,
                              MODIFICATOR_TYPE_ENUM_COLUMN, MESH_PARAMETERS,
                              -1);
  // Insert other parameters
  while(p_item) {
    gtk_tree_store_append(p_store, &iterator_modificators, NULL);
    gtk_tree_store_set(p_store, &iterator_modificators, 
                                MODIFICATOR_TYPE_NAME_COLUMN, p_item->modificator_name_get(),
                                MODIFICATOR_NAME_COLUMN, p_item->name_get(),
                                MODIFICATOR_POINTER_COLUMN, (void *)p_item,
                                MODIFICATOR_TYPE_ENUM_COLUMN, p_item->modificator_type_get(),
                                -1);
    
    if(p_item->modificator_type_get() == TEXTURE_PARAMETERS) {
      GtkTreeIter iterator_texture;
      
      // Insert texture lighting
      gtk_tree_store_append(p_store, &iterator_texture, &iterator_modificators);
      gtk_tree_store_set(p_store, &iterator_texture, 
                                  MODIFICATOR_TYPE_NAME_COLUMN, modificator_type_item::type_translate_name(TEXTURE_LIGHT),
                                  MODIFICATOR_NAME_COLUMN,      "",
                                  MODIFICATOR_POINTER_COLUMN,   (void *)p_item,
                                  MODIFICATOR_TYPE_ENUM_COLUMN, TEXTURE_LIGHT,
                                  -1);    
*/                                  
      /*      
      MESH_MODIFICATOR_TEXTURE *p_text = (MESH_MODIFICATOR_TEXTURE *)p_item;
      TEXTURE_MODIFICATOR_ITEM *p_mod = p_text->texture_modificator_get_first();
      
      while(p_mod) {        
        gtk_tree_store_append(p_store, &iterator_texture, &iterator_modificators);
        gtk_tree_store_set(p_store, &iterator_texture, 
                                    MODIFICATOR_TYPE_NAME_COLUMN, p_mod->modificator_name_get(),
                                    MODIFICATOR_NAME_COLUMN, p_mod->name_get(),
                                    MODIFICATOR_POINTER_COLUMN, (void *)p_mod,
                                    MODIFICATOR_TYPE_ENUM_COLUMN, p_mod->modificator_type_get(),
                                    -1);
        
        p_mod = (TEXTURE_MODIFICATOR_ITEM *)p_mod->object_list_next();
      }
      */
      /*
    }
    p_item = (MESH_MODIFICATOR_ITEM *)p_item->object_list_next();
  }
    */
    
  return(TRUE);
}

/*
 * Generator list
 */
typedef enum
{
  GENERATOR_NAME_COLUMN     = 0,
  GENERATOR_POINTER_COLUMN  = 1,
  GENERATOR_N_COLUMNS
  
} GENERATOR_COLUMNS;

static void gui_modificator_combo_changed(GtkComboBox *p_combo, gpointer user_data)
{  
  GENERATOR_MESH *p_active;
  GtkTreeIter     iter;  
  
  if(gtk_combo_box_get_active_iter(p_combo,&iter)) {
    GtkTreeModel *p_store = gtk_combo_box_get_model(p_combo);
    gtk_tree_model_get(p_store,&iter,GENERATOR_POINTER_COLUMN,&p_active,-1);
    
    GUI_STATE *p_state = (GUI_STATE *)user_data;
    p_state->selection_generator_mesh_set(p_active);
    
    p_state->gen_mesh_modificator_list.update();
  }
}

bool gui_generator_mesh_list_update(GUI_STATE *p_state)
{  
  GtkWidget *p_modificator_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_MODIFICATOR);
  GtkWidget *p_combo = lookup_widget_directly(p_modificator_toolbox,"combobox3");
  
  EDITOR    *p_editor = p_state->editor_get();  
  GENERATOR *p_generator = p_editor->generator_get();
  
  static GtkListStore *p_store = NULL;
  if(p_store == NULL) {
    p_store = gtk_list_store_new(GENERATOR_N_COLUMNS,
                                 G_TYPE_STRING,
                                 G_TYPE_POINTER);
  }
  gtk_combo_box_set_model(GTK_COMBO_BOX(p_combo), GTK_TREE_MODEL(p_store));
  
  gtk_list_store_clear(p_store);
  
  GENERATOR_MESH *p_gen = p_generator->mesh_generator_get_first();
  GtkTreeIter iter;
  while(p_gen) {
    gtk_list_store_append(p_store, &iter);
    gtk_list_store_set(p_store, &iter, GENERATOR_NAME_COLUMN, p_gen->name_get(),
                                       GENERATOR_POINTER_COLUMN, (void *)p_gen,
                                       -1);
    
    if(p_state->is_selected_generator_mesh(p_gen)) {
      gtk_combo_box_set_active_iter(GTK_COMBO_BOX(p_combo), &iter);
    }
    
    p_gen = (GENERATOR_MESH *)p_gen->object_list_next();
  }
  
  if(!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(p_combo), &iter)) {
    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(p_store), &iter);
    gtk_combo_box_set_active_iter(GTK_COMBO_BOX(p_combo), &iter);
  }
  
  return(TRUE);
}

static void gui_modificator_load_new(GtkButton *button, gpointer user_data)
{
  /*
  GUI_STATE *p_state = (GUI_STATE *)user_data;  
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  EDITOR *p_editor = p_state->editor_get();
  GENERATOR *p_generator = p_editor->generator_get();
  
  char *p_file = editor_dialog_file_load(GTK_WINDOW(p_interface->gtk_main_window_get()));
  if(p_file) {
    p_generator->mesh_generator_load(p_file);
  } 
  */  
}

static void gui_modificator_reload(GtkButton *button, gpointer user_data)
{
  /*
  GUI_STATE *p_state = (GUI_STATE *)user_data;
  GENERATOR_MESH *p_gen = p_state->selection_generator_mesh_get();
  if(p_gen) {
    p_gen->load();
  }
  */
}

static void gui_modificator_generate(GtkButton *button, gpointer user_data)
{
  GUI_STATE *p_state = (GUI_STATE *)user_data;  
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  SCENE *p_scene = p_interface->scene_get();
  
  GENERATOR_MESH *p_gmesh = p_state->selection_generator_mesh_get();
  if(p_gmesh) {
    GENERATOR *p_gen = p_gmesh->generator_get();    
    p_gen->run(p_scene);
    
    GUI_WINDOW_RUNNING_AGE *p_window = p_interface->gui_main_window_get();
    p_window->window_age_draw_request_set();
  }
}

/*
 * Create a widget with list of all modificators
 */
static GtkWidget * gui_modificator_create(GUI_STATE *p_state)
{  
  GtkWidget *vbox9;
  GtkWidget *label32;
  GtkWidget *vbox4;
  GtkWidget *combobox3;
  GtkWidget *hseparator2;
  GtkWidget *scrolledwindow1;
  GtkWidget *editor_main_treeview_object_selection;
  
  vbox9 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox9);
  
  label32 = gtk_label_new ("Mesh modificator:");
  gtk_widget_show (label32);
  gtk_box_pack_start (GTK_BOX(vbox9), label32, FALSE, FALSE, 0);
  
  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_box_pack_start (GTK_BOX(vbox9), vbox4, FALSE, TRUE, 0);
  
  combobox3 = gtk_combo_box_new_text();
  gtk_widget_show (combobox3);
  gtk_box_pack_start (GTK_BOX (vbox4), combobox3, FALSE, FALSE, 0);
  GTK_WIDGET_SET_FLAGS (combobox3, GTK_CAN_DEFAULT);

  GtkWidget *button = NULL;
  
  button = gtk_button_new_with_label("Load new");
  gtk_widget_show(button);
  gtk_box_pack_start(GTK_BOX (vbox4), button, FALSE, FALSE, 0);
  g_signal_connect((gpointer) button, "clicked",
                   G_CALLBACK (gui_modificator_load_new),
                   p_state);
  
  button = gtk_button_new_with_label("Reload");
  gtk_widget_show(button);
  gtk_box_pack_start(GTK_BOX (vbox4), button, FALSE, FALSE, 0);
  g_signal_connect((gpointer) button, "clicked",
                   G_CALLBACK (gui_modificator_reload),
                   p_state);

  button = gtk_button_new_with_label("Generate");
  gtk_widget_show(button);
  gtk_box_pack_start(GTK_BOX (vbox4), button, FALSE, FALSE, 0);
  g_signal_connect((gpointer) button, "clicked",
                   G_CALLBACK (gui_modificator_generate),
                   p_state);

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_show (hseparator2);
  gtk_box_pack_start (GTK_BOX (vbox4), hseparator2, FALSE, TRUE, 0);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox9), scrolledwindow1, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow1), 2);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

  editor_main_treeview_object_selection = gtk_tree_view_new ();
  gtk_widget_show (editor_main_treeview_object_selection);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), editor_main_treeview_object_selection);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (editor_main_treeview_object_selection), TRUE);

  g_signal_connect ((gpointer) combobox3, "changed",
                    G_CALLBACK (gui_modificator_combo_changed),
                    p_state);
  
  gtk_widget_grab_default (combobox3);
  
  /* Store pointers to all widgets, for use by lookup_widget(). */ 
  GLADE_HOOKUP_OBJECT_NO_REF (vbox9, vbox9, "vbox9");
  GLADE_HOOKUP_OBJECT (vbox9, combobox3, "combobox3");
  GLADE_HOOKUP_OBJECT (vbox9, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (vbox9, editor_main_treeview_object_selection, "editor_main_treeview_object_selection");

  return vbox9;
}

bool gui_modificators_create_func(GUI_STATE *p_state)
{
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GtkWidget *p_window = p_interface->gtk_main_window_get();  
  GtkWidget *p_tool_frame = lookup_widget(GTK_WIDGET(p_window),"tool_frame");
  assert(p_tool_frame != NULL);
  
  GtkWidget *p_modificator_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_MODIFICATOR);
  if(!p_modificator_toolbox) {
    p_modificator_toolbox = gui_modificator_create(p_state);
    p_state->toolbox_user_data_set(TOOLBOX_MODIFICATOR, p_modificator_toolbox);
    gtk_widget_ref(p_modificator_toolbox);    
  }
  
  gtk_container_add(GTK_CONTAINER(p_tool_frame), p_modificator_toolbox);
  gtk_widget_show_all(p_modificator_toolbox);
  gtk_widget_unref(p_modificator_toolbox);
  
  p_state->gen_mesh_list.create();
  p_state->gen_mesh_modificator_list.create();
  
  return(TRUE);
}

bool gui_modificators_destroy_func(GUI_STATE *p_state)
{
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GtkWidget *p_window = p_interface->gtk_main_window_get();
  GtkWidget *p_tool_frame = lookup_widget(GTK_WIDGET(p_window),"tool_frame");
  assert(p_tool_frame != NULL);
  
  GtkWidget *p_modificator_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_MODIFICATOR);
  if(p_modificator_toolbox) {
    gtk_widget_ref(p_modificator_toolbox);
    gtk_widget_hide_all(p_modificator_toolbox);
    gtk_container_remove(GTK_CONTAINER(p_tool_frame), p_modificator_toolbox);  
  }
  
  p_state->gen_mesh_list.destroy();
  p_state->gen_mesh_modificator_list.destroy();
  
  return(TRUE);
}
