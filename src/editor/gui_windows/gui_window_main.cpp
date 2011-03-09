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


void gui_toolbox_type_changed(GtkComboBox *combobox, gpointer user_data)
{
  GUI_STATE *p_state = (GUI_STATE *)user_data;
  int active = gtk_combo_box_get_active(combobox);
  p_state->toolbox_active_set((TOOLBOX_ITEM)active);
} 

GUI_WINDOW_RUNNING * editor_main_gui_create(GUI_WINDOW_REGISTERED *p_registered)
{
  GUI_WINDOW_RUNNING_AGE *p_window = new GUI_WINDOW_RUNNING_AGE(p_registered);

  p_window->gtk_window_set(create_editor_main());
  GtkWidget *window = p_window->gtk_window_get();
  
  /* Set up main gui window */  
  GUI_INTERFACE *p_interface = p_window->gui_interface_get();
  p_interface->gui_main_window_set(p_window);
  
  /* Insert AGE rendering window */
  GtkWidget *p_area = p_window->window_age_create(AGE_GUI_MINIMAL_WIDTH, 
                                                  AGE_GUI_MINIMAL_HEIGHT);
  GtkWidget *main_frame = lookup_widget(GTK_WIDGET(window),"main_age_frame");
  gtk_container_add(GTK_CONTAINER(main_frame), p_area);
  gtk_widget_show(p_area);
    
  p_window->window_command_register(editor_command_scene_popup_menu, GUI_MOUSE_RIGHT, BUTTON_RELEASED);
  p_window->window_command_register(editor_command_scene_zoom_in, GUI_MOUSE_WHEEL_UP, BUTTON_PRESSED);
  p_window->window_command_register(editor_command_scene_zoom_out, GUI_MOUSE_WHEEL_DOWN, BUTTON_PRESSED);
  
  GtkWidget *combobox_tool_type = lookup_widget(GTK_WIDGET(window),"combobox_tool_type");
  assert(combobox_tool_type != NULL);
  
  gtk_combo_box_insert_text(GTK_COMBO_BOX(combobox_tool_type),TOOLBOX_SCENE,       "Objects");
  gtk_combo_box_insert_text(GTK_COMBO_BOX(combobox_tool_type),TOOLBOX_MODIFICATOR, "Modificators");
  
  GUI_STATE *p_state = p_interface->gui_state_get();
  g_signal_connect((gpointer) combobox_tool_type, "changed",
                   G_CALLBACK (gui_toolbox_type_changed),
                   p_state);
                    
  p_state->toolbox_active_set(TOOLBOX_MODIFICATOR);
  
  gtk_widget_show_all(window);
  return(p_window);
}

bool editor_main_gui_destroy(GUI_WINDOW_REGISTERED *p_registered)
{
  // TODO -> Question? -> really quit?  
  gtk_main_quit();
  return(FALSE);
}
