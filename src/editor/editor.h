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
 * Main editor class
 */
 
#ifndef __EDITOR_H__
#define __EDITOR_H__

/*
 * Age headers
 */
#include "age.h"

/*
 * Editor references
 */
#include "editor_reference.h"

/*
 * Mesh/Texture Generator
 */
#include "generator.h"


#ifdef ENABLE_GTK_BACKEND
  #include "interface.h"
  #include "support.h"
  
  #include "gui_interface.h"
  #include "gui_helpers.h"

  /*
   * Some editor commands
   */
  #include "editor_commands.h"
  
  /*
   * GUI windows
   */
  #include "gui_state.h"
  #include "gui_window_main.h"
  #include "gui_window_misc.h"
  #include "gui_window_toolbox_scene.h"
  #include "gui_window_toolbox_geometry.h"
  #include "gui_window_toolbox_materials.h"
  #include "gui_window_toolbox_modificator.h"
#endif

#ifdef ENABLE_SDL_BACKEND
  #include "sdl_gui_interface.h"
  #include "sdl_gui_state.h"
  
  #include "sdl_control.h"
  #include "sdl_main.h"  
#endif

/*
 * Main editor class
 */
#include "editor_main.h"

#endif //  __EDITOR_H__
