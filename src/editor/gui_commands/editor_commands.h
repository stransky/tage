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
 * Basic controls, key binding and so on
 */

#ifndef __EDITOR_GUI_COMMANDS_H__
#define __EDITOR_GUI_COMMANDS_H__

/*
 * Menu commands
 */
void editor_command_quit(void);
void editor_command_scene_new(void);
void editor_command_scene_load(void);
void editor_command_scene_save(void);
void editor_command_scene_save_as(void);
void editor_command_scene_move(void);
void editor_command_scene_rotate(void);
void editor_command_scene_zoom_in(void);
void editor_command_scene_zoom_out(void);
void editor_command_scene_reset(void);
void editor_command_scene_select_set(void);
void editor_command_scene_select_add(void);
void editor_command_scene_select_clear(void);
void editor_command_rendering_mode(RENDER_MODE mode);

/*
 * Run-time commands called from GL rendering screen
 */
bool editor_command_scene_zoom_in(GUI_WINDOW_RUNNING *p_window, void *p_data);
bool editor_command_scene_zoom_out(GUI_WINDOW_RUNNING *p_window, void *p_data);
bool editor_command_scene_popup_menu(GUI_WINDOW_RUNNING *p_window, void *p_data);

/*
 * Land generators
 */
void editor_command_land_create(void);
void editor_command_land_height_map(void);
void editor_command_land_height_fractal(void);
void editor_command_land_height_filter(void);

#endif // __EDITOR_GUI_COMMANDS_H__
