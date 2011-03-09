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
 * Helper class for height-based targets (geometry now)
 */
void modificator_target_height::heightmap_create(tpos dx, tpos dy, tpos height)
{
  assert(p_heightmap == NULL);
  p_heightmap = new SURFACE_HEIGHT(dx,dy);
  texture_height = height;
}

void modificator_target_height::heightmap_destroy(void)
{
  if(p_heightmap != NULL) {
    delete p_heightmap;
    p_heightmap = NULL;
  }
}

modificator_target_height::modificator_target_height(GENERATOR_MESH *p_gen, 
                                                     MODIFICATOR_TARGET_TYPE target_type)
  : modificator_target(p_gen, target_type)
{
  p_heightmap = NULL;
  texture_height = 0;
  p_heightmap_scale = NULL;
}

modificator_target_height::~modificator_target_height(void)
{ 
  heightmap_destroy();
}

/*
 * Auxilliary target
 */
modificator_target_aux::modificator_target_aux(GENERATOR_MESH *p_gen, MODIFICATOR_TARGET_TYPE target_type)
  : modificator_target(p_gen, target_type),
    size(0)
{   
}

modificator_target_aux::~modificator_target_aux(void)
{
  clear();
}

/*
 * Texture target
 */
void modificator_target_texture::flush(MESH_HELPER *p_helper)
{
}

modificator_target_texture::modificator_target_texture(GENERATOR_MESH *p_gen)
  : modificator_target_aux(p_gen, MODIFICATOR_TARGET_TEXTURE_TYPE)
{ 
  TEXTURE_DESCRIPTION *p_text = &p_gen->texture_desc;
  init(p_text->texture_size, p_text->texture_height);
  
  SURFACE_SW *p_bitmap = bitmap_get();
  p_bitmap->fill(p_text->texture_background_color);
}

modificator_target_texture::~modificator_target_texture(void)
{
  clear();
}
