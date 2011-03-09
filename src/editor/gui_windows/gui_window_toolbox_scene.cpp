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
 * GUI modificators for main gui pannels
 */
void gui_toolbox_scene_changed(GtkComboBox *combobox, gpointer user_data)
{  
  int active = gtk_combo_box_get_active(combobox);
  
  GUI_STATE *p_state = (GUI_STATE *)user_data;
  p_state->toolbox_scene_panel_set((SCENE_PANEL)active);
}

/*
 * List of all objects in scene
 */
GtkWidget * gui_scene_create(GUI_STATE *p_state)
{  
  GtkWidget *vbox9;
  GtkWidget *label32;
  GtkWidget *vbox4;
  GtkWidget *combobox3;
  GtkWidget *hseparator3;
  GtkWidget *checkbutton_mesh;
  GtkWidget *checkbutton_helpers;
  GtkWidget *checkbutton_lights;
  GtkWidget *hseparator2;
  GtkWidget *scrolledwindow1;
  GtkWidget *editor_main_treeview_object_selection;

  vbox9 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox9);  

  label32 = gtk_label_new ("Objects in list:");
  gtk_widget_show (label32);
  gtk_box_pack_start (GTK_BOX (vbox9), label32, FALSE, FALSE, 0);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_box_pack_start (GTK_BOX (vbox9), vbox4, FALSE, TRUE, 0);

  combobox3 = gtk_combo_box_new_text ();
  gtk_widget_show (combobox3);
  gtk_box_pack_start (GTK_BOX (vbox4), combobox3, FALSE, FALSE, 0);
  GTK_WIDGET_SET_FLAGS (combobox3, GTK_CAN_DEFAULT);
  
  gtk_combo_box_insert_text(GTK_COMBO_BOX(combobox3),SCENE_PANEL_GEOMETRY, "Geometry");
  gtk_combo_box_insert_text(GTK_COMBO_BOX(combobox3),SCENE_PANEL_MATERIALS,"Materials");

  hseparator3 = gtk_hseparator_new ();
  gtk_widget_show (hseparator3);
  gtk_box_pack_start (GTK_BOX (vbox4), hseparator3, FALSE, FALSE, 2);

  checkbutton_mesh = gtk_check_button_new_with_mnemonic ("Meshes");
  gtk_widget_show (checkbutton_mesh);
  gtk_box_pack_start (GTK_BOX (vbox4), checkbutton_mesh, FALSE, FALSE, 0);

  checkbutton_helpers = gtk_check_button_new_with_mnemonic ("Geometry helpers");
  gtk_widget_show (checkbutton_helpers);
  gtk_box_pack_start (GTK_BOX (vbox4), checkbutton_helpers, FALSE, FALSE, 0);

  checkbutton_lights = gtk_check_button_new_with_mnemonic ("Lights");
  gtk_widget_show (checkbutton_lights);
  gtk_box_pack_start (GTK_BOX (vbox4), checkbutton_lights, FALSE, FALSE, 0);

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

  g_signal_connect((gpointer)combobox3, "changed",
                   G_CALLBACK(gui_toolbox_scene_changed),
                   p_state);

  /* Store pointers to all widgets, for use by lookup_widget(). */ 
  GLADE_HOOKUP_OBJECT_NO_REF (vbox9, vbox9, "vbox9");
  GLADE_HOOKUP_OBJECT (vbox9, label32, "label32");
  GLADE_HOOKUP_OBJECT (vbox9, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (vbox9, combobox3, "combobox3");
  GLADE_HOOKUP_OBJECT (vbox9, hseparator3, "hseparator3");
  GLADE_HOOKUP_OBJECT (vbox9, checkbutton_mesh, "checkbutton_mesh");
  GLADE_HOOKUP_OBJECT (vbox9, checkbutton_helpers, "checkbutton_helpers");
  GLADE_HOOKUP_OBJECT (vbox9, checkbutton_lights, "checkbutton_lights");
  GLADE_HOOKUP_OBJECT (vbox9, hseparator2, "hseparator2");
  GLADE_HOOKUP_OBJECT (vbox9, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (vbox9, editor_main_treeview_object_selection, "editor_main_treeview_object_selection");

  gtk_widget_grab_default (combobox3);

  return vbox9;
}

bool gui_scene_create_func(GUI_STATE *p_state)
{
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GtkWidget *p_window = p_interface->gtk_main_window_get();  
  GtkWidget *p_tool_frame = lookup_widget(GTK_WIDGET(p_window),"tool_frame");
  assert(p_tool_frame != NULL);
  
  GtkWidget *p_object_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_SCENE);
  if(!p_object_toolbox) {
    p_object_toolbox = gui_scene_create(p_state);
    gtk_widget_ref(p_object_toolbox);
    
    p_state->toolbox_user_data_set(TOOLBOX_SCENE, p_object_toolbox);
  }
  
  gtk_container_add(GTK_CONTAINER(p_tool_frame), p_object_toolbox);
  gtk_widget_unref(p_object_toolbox);
  gtk_widget_show_all(p_object_toolbox); 
    
  p_state->toolbox_scene_panel_set();
  
  return(TRUE);
}

bool gui_scene_destroy_func(GUI_STATE *p_state)
{
  GUI_INTERFACE *p_interface = p_state->gui_interface_get();
  GtkWidget *p_window = p_interface->gtk_main_window_get();
  GtkWidget *p_tool_frame = lookup_widget(GTK_WIDGET(p_window),"tool_frame");
  assert(p_tool_frame != NULL);
  
  GtkWidget *p_object_toolbox = (GtkWidget *)p_state->toolbox_user_data_get(TOOLBOX_SCENE);
  if(p_object_toolbox) {
    gtk_widget_ref(p_object_toolbox);
    gtk_widget_hide_all(p_object_toolbox);
    gtk_container_remove(GTK_CONTAINER(p_tool_frame), p_object_toolbox);
  }
  
  return(TRUE);
}
