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
 * Whole terrain texture
 */
void modificator_target_geometry::heightmap_create(MESH_HELPER *p_helper, tpos dx, tpos dy)
{
  assert(p_helper != NULL);
  assert(heightmap_get() == NULL);
  
  modificator_target_height::heightmap_create(dx,dy,0);
  
  SURFACE_HEIGHT *p_heightmap = heightmap_get();
  
  int x,y;  
  for(x = 0; x < dx; x++) {
    for(y = 0; y < dy; y++) {
                        
      float height = p_helper->mesh_height_get(x, y);
      if(height != HEIGHT_UNSET) {
        p_heightmap->height_set(x,y,height);
      }
    }
  }
  
  p_heightmap->height_range_set(p_helper->mesh_size.y);
}

void modificator_target_geometry::heightmap_destroy(MESH_HELPER *p_helper)
{
  SURFACE_HEIGHT *p_heightmap = heightmap_get();
  if(p_heightmap) {  
    int x,y;
    int dx = p_helper->mesh_size.x;
    int dy = p_helper->mesh_size.z;
    float height_range = p_heightmap->height_range_get();
    
    p_helper->mesh_vertex_lock(LOCK_READ_WRITE);
    
    for(x = 0; x < dx; x++) {
      for(y = 0; y < dy; y++) {
        
        float height = p_heightmap->height_get(x,y);
        if(height != HEIGHT_UNSET) {
          height *= height_range;
          p_helper->mesh_height_set(x, y, height);
        }
      }
    }
    
    p_helper->mesh_vertex_unlock();
    
    modificator_target_height::heightmap_destroy();
  }
}

void modificator_target_geometry::create(MESH_HELPER *p_helper)
{
  heightmap_create(p_helper, p_helper->mesh_size.x, p_helper->mesh_size.z);
}

void modificator_target_geometry::flush(MESH_HELPER *p_helper)
{
  heightmap_destroy(p_helper);
}

modificator_target_geometry::modificator_target_geometry(GENERATOR_MESH *p_gen)
  : modificator_target_height(p_gen, MODIFICATOR_TARGET_GEOMETRY_TYPE)
{  
}
