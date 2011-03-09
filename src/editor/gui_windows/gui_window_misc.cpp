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
 * Editor gui clue
 */
#include "editor.h"

/*
 * New land generator
 */
GUI_WINDOW_RUNNING * land_new_generator_gui_create(GUI_WINDOW_REGISTERED *p_registered)
{
  GUI_WINDOW_RUNNING *p_window = new GUI_WINDOW_RUNNING(p_registered);

  p_window->gtk_window_set(create_editor_land_new_generator());
  
  GtkWidget *window = p_window->gtk_window_get();
  
  // Set up default values
  GtkWidget *p_tmp;
  
  p_tmp = lookup_widget(window, "entry1");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"0");
  p_tmp = lookup_widget(window, "entry2");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"0");
  p_tmp = lookup_widget(window, "entry3");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"0");
  
  p_tmp = lookup_widget(window, "entry4");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"1");
  p_tmp = lookup_widget(window, "entry5");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"1");
  p_tmp = lookup_widget(window, "entry6");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"1");
  
  p_tmp = lookup_widget(window, "entry7");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"50");
  p_tmp = lookup_widget(window, "entry8");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"50");
  p_tmp = lookup_widget(window, "entry9");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"50");
  
  gtk_widget_show(window);
  
  return(p_window);
}

void land_new_generator_gui_lauch_button(GtkWidget *p_widget)
{/*
  GtkWidget *main_window = lookup_widget(GTK_WIDGET(p_widget), "editor_land_new_generator");
  
  VECT    start;
  VECT    diff;
  VECT3DI size;
  
  start.x = gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry1")));
  start.y = gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry2")));
  start.z = gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry3")));
  
  diff.x = gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry4")));
  diff.y = gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry5")));
  diff.z = gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry6")));
  
  size.x = gtk_entry_get_int(GTK_ENTRY(lookup_widget(main_window, "entry7")));
  size.y = gtk_entry_get_int(GTK_ENTRY(lookup_widget(main_window, "entry8")));
  size.z = gtk_entry_get_int(GTK_ENTRY(lookup_widget(main_window, "entry9")));  
  
  SCENE *p_scene = scene_get();
  
  MESH_PARAMS par(p_scene);
  
  par.mesh_start_set(start);
  par.mesh_diff_set(diff);
  par.mesh_size_set(size);
  
  //GAME_MESH *p_mesh = par.mesh_create();
  
  //p_scene->selection_clear();
  
  //p_mesh->select();
  
  gtk_widget_destroy(main_window);
  */
}

bool land_new_generator_gui_destroy(GUI_WINDOW_REGISTERED *p_registered)
{  
  return(FALSE);
}

/*
 * Height map modificator
 */
void land_height_map_modificator_gui_apply(GtkWidget *p_widget)
{/*
  GtkWidget *main_window = lookup_widget(GTK_WIDGET(p_widget), "editor_land_height_map_modificator");
  
  SCENE *p_scene = scene_get();
  assert(p_scene != NULL);
  
  GAME_MESH *p_mesh = (GAME_MESH *)((p_scene->selection_get())->object_get());
  if(!p_mesh) {
    return;
  }  
  
  MESH_MODIFICATOR mod(p_scene);
  MESH_MODIFICATOR_HEIGHT_MAP *p_map = (MESH_MODIFICATOR_HEIGHT_MAP *)mod.height_map_new();
  
  char *p_file = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(lookup_widget(main_window, "filechooserbutton1")));
  if(p_file) {
    p_map->bitmap_set(NULL, p_file);
  }  
  
  p_map->color_multiplier_set(gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry15"))));
  p_map->color_shift_set(gtk_entry_get_float(GTK_ENTRY(lookup_widget(main_window, "entry16"))));
  
  if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(lookup_widget(main_window, "checkbutton1")))) {
    GdkColor color_low;
    GdkColor color_top;
    
    gtk_color_button_get_color(GTK_COLOR_BUTTON(lookup_widget(main_window, "colorbutton1")),&color_low);
    gtk_color_button_get_color(GTK_COLOR_BUTTON(lookup_widget(main_window, "colorbutton2")),&color_top);
    
    RGBB low(color_low.red,color_low.green,color_low.blue);
    RGBB top(color_top.red,color_top.green,color_top.blue);
    
    p_map->bitmap_colormask_set(low,top);
  }
  
  mod.apply(p_mesh);
  
  //TERRAIN_MAP *p_terr = mod.terrainmap_new();
  */
  /*
    size.z = gtk_entry_get_int(GTK_ENTRY(lookup_widget(main_window, "entry9")));
    
    LANDSCAPE_PARAMS par;
    par.start_set(&start);
    par.diff_set(&diff);
    par.size_set(&size);
  */
}

GUI_WINDOW_RUNNING * land_height_map_modificator_gui_create(GUI_WINDOW_REGISTERED *p_registered)
{
  GUI_WINDOW_RUNNING *p_window = new GUI_WINDOW_RUNNING(p_registered);
  
  p_window->gtk_window_set(create_editor_land_height_map_modificator());
  GtkWidget *window = p_window->gtk_window_get();

  // Set up default values
  GtkWidget *p_tmp;
  
  p_tmp = lookup_widget(window, "combobox2");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Set");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Add");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Substract");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Multiply");  
  gtk_combo_box_set_active(GTK_COMBO_BOX(p_tmp),0);
  
  p_tmp = lookup_widget(window, "entry15");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"1");  
  p_tmp = lookup_widget(window, "entry16");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"0");
   
  gtk_widget_show(window);
  return(p_window);
}

bool land_height_map_modificator_gui_destroy(GUI_WINDOW_REGISTERED *p_registered)
{  
  return(FALSE);
}

GUI_WINDOW_RUNNING * land_height_fractal_gui_create(GUI_WINDOW_REGISTERED *p_registered)
{
  GUI_WINDOW_RUNNING *p_window = new GUI_WINDOW_RUNNING(p_registered);
  
  p_window->gtk_window_set(create_editor_land_height_fractal());
  GtkWidget *window = p_window->gtk_window_get();  
/*
  // Set up default values
  GtkWidget *p_tmp;
  
  p_tmp = lookup_widget(window, "combobox2");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Set");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Add");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Substract");
  gtk_combo_box_append_text(GTK_COMBO_BOX(p_tmp),"Multiply");  
  gtk_combo_box_set_active(GTK_COMBO_BOX(p_tmp),0);
  
  p_tmp = lookup_widget(window, "entry15");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"1");  
  p_tmp = lookup_widget(window, "entry16");
  gtk_entry_set_text(GTK_ENTRY(p_tmp),"0");
*/
  gtk_widget_show(window);
  return(p_window);
}

bool land_height_fractal_gui_destroy(GUI_WINDOW_REGISTERED *p_registered)
{  
  return(FALSE);
}

char * editor_dialog_file_load(GtkWindow *p_parent)
{  
  char      *filename = NULL;
  GtkWidget *dialog;
  
  dialog = gtk_file_chooser_dialog_new("Open File",
                                       GTK_WINDOW(p_parent),
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                       NULL);
  
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }
  
  gtk_widget_destroy (dialog);
  
  return(filename);
}

bool editor_dialog_file_load(GtkWindow *p_parent, char *p_file, int max_len)
{  
  char *p_return = editor_dialog_file_load(p_parent);
  if(p_return) {
    strncpy(p_file,p_return,max_len);
    g_free(p_return);    
  }  
  return(p_return != NULL);
}

char * editor_dialog_file_save(GtkWindow *p_parent)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new("Save File",
                                        GTK_WINDOW(p_parent),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                        NULL);
  
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
  
  /*
  if(user_edited_a_new_document) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), default_folder_for_saving);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "Untitled document");
  }
  else {
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(dialog), filename_for_existing_document);
  }
  */
  
  char *filename = NULL;
  
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }  
  gtk_widget_destroy(dialog);
  
  return(filename);
}

bool editor_dialog_file_save(GtkWindow *p_parent, char *p_file, int max_len)
{
  char *p_return = editor_dialog_file_save(p_parent);
  
  if(p_return) {
    strncpy(p_file,p_return,max_len);
    g_free(p_return);    
  }
  
  return(p_return != NULL);
}
