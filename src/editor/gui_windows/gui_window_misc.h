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

#ifndef __GUI_WINDOW_MISC_H__
#define __GUI_WINDOW_MISC_H__

/*
 * GUI callbacks
 */
GUI_WINDOW_RUNNING * land_new_generator_gui_create(GUI_WINDOW_REGISTERED *p_registered);
void                 land_new_generator_gui_lauch_button(GtkWidget *p_widget);
bool                 land_new_generator_gui_destroy(GUI_WINDOW_REGISTERED *p_registered);

GUI_WINDOW_RUNNING * land_height_map_modificator_gui_create(GUI_WINDOW_REGISTERED *p_registered);
bool                 land_height_map_modificator_gui_destroy(GUI_WINDOW_REGISTERED *p_registered);
void                 land_height_map_modificator_gui_apply(GtkWidget *p_widget);

GUI_WINDOW_RUNNING * land_height_fractal_gui_create(GUI_WINDOW_REGISTERED *p_registered);
bool                 land_height_fractal_gui_destroy(GUI_WINDOW_REGISTERED *p_registered);

/*
 * Load/Save dialogs
 */
char *  editor_dialog_file_load(GtkWindow *p_parent);
bool    editor_dialog_file_load(GtkWindow *p_parent, char *p_file, int max_len);

char *  editor_dialog_file_save(GtkWindow *p_parent);
bool    editor_dialog_file_save(GtkWindow *p_parent, char *p_file, int max_len);

#endif // __GUI_WINDOW_MISC_H__
