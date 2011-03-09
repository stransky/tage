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

#ifndef __EDITOR_REFERENCE_H__
#define __EDITOR_REFERENCE_H__

/*
 * Some class references
 */
class editor;
typedef class editor EDITOR;

class generator;
typedef class generator GENERATOR;

class gui_interface;
typedef class gui_interface GUI_INTERFACE;

class gui_state;
typedef class gui_state GUI_STATE;

/*
 * Editor reference
 */
typedef class editor_reference {
  
  EDITOR        * p_editor;
  
public:

  EDITOR        * editor_get(void);
  GENERATOR     * generator_get(void);
  GUI_INTERFACE * gui_interface_get(void);
  GUI_STATE     * gui_state_get(void);

public:

  editor_reference(EDITOR *p_editor)
  {
    this->p_editor = p_editor;
  }
  
} EDITOR_REFERENCE;

/*
 * Global editor reference - it's used in menu where can't be passed 
 * data pointer
 */
EDITOR *  editor_reference_get(void);
void      editor_reference_set(EDITOR * p_editor);

#endif // __EDITOR_REFERENCE_H__
