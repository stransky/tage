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

#ifndef __SDL_GUI_INTERFACE_H__
#define __SDL_GUI_INTERFACE_H__

#define GUI_FPS    30

/*
 * GUI class prototypes
 */
class gui_interface;
typedef class gui_interface GUI_INTERFACE;

/*
 * Main GUI class
 */
typedef class gui_interface
{ 
  
  AGE_MAIN * p_age;

public:

  AGE_MAIN * age_get(void)
  {
    return(p_age);
  }

  GRAPH3D * graph_get(void)
  {
    AGE_MAIN *p_age = age_get();
    return(p_age ? p_age->graph_get() : NULL);
  }

  SCENE * scene_get(void)
  {
    AGE_MAIN *p_age = age_get();
    return(p_age != NULL ? p_age->scene_get() : NULL);
  } 
  
public:

  void window_create(char *p_name);
  
public:
  
  void run(void);

public:
  
  void create(void);
  void destroy(void);

public:

  gui_interface(EDITOR *p_editor);
  ~gui_interface(void);

} GUI_INTERFACE;


#endif // __GUI_INTERFACE_H__
