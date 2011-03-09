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

void mesh_helper::mesh_vertex_lock(LOCK_TYPE type)
{
  if(p_mesh != NULL && p_mesh->geometry_active()) {
    p_mesh->lock(type);
  }
}

void mesh_helper::mesh_vertex_unlock(LOCK_CHANGE_TYPE changed)
{
  if(p_mesh != NULL && p_mesh->geometry_active()) {
    p_mesh->unlock(changed);
  } 
}

// mesh_helper implementation
VECT * mesh_helper::mesh_vertex_get(tpos x, tpos z, VECT *p_vect)
{
  if(p_mesh != NULL && p_mesh->geometry_active()) {
  
    VECT3DI size = mesh_size;
  
    int dx = size.x;
    int dz = size.z;
    
    assert(dx > 0 && dz > 0);
    
    if(x >= 0 && x <= dx && z >= 0 && z <= dz) {
      MESH_GEOMETRY *p_geom = p_mesh->lock(LOCK_READ);
        
      int index = VERTEX_INDEX_GET(x,z,dx,dz);
      assert(index < p_geom->vertexnum_get());
      
      VECT *p_vects = p_geom->position_get();
      VECT *p_selected_vect = p_vects+index;
      
      *p_vect = *p_selected_vect;
      
      p_mesh->unlock();
      
      return(p_vect);
    }
  }
  
  return(NULL);
}

void mesh_helper::mesh_vertex_set(tpos x, tpos z, VECT *p_vect)
{
  assert(p_mesh != NULL && p_mesh->geometry_active());
  
  VECT3DI size = mesh_size;

  int dx = size.x;
  int dz = size.z;
  
  assert(dx > 0 && dz > 0);
  
  if(x >= 0 && x <= dx && z >= 0 && z <= dz) {
    MESH_GEOMETRY *p_geom = p_mesh->lock(LOCK_READ_WRITE);
      
    int index = VERTEX_INDEX_GET(x,z,dx,dz);
    assert(index < p_geom->vertexnum_get());
    
    VECT *p_vects = p_geom->position_get();
    VECT *p_selected_vect = p_vects+index;
    
    *p_selected_vect = *p_vect;
    
    p_mesh->unlock();
  }
}

float mesh_helper::mesh_height_get(tpos x, tpos z, float default_height)
{
  VECT tmp;
  
  if(mesh_vertex_get(x,z,&tmp)) {
    return(tmp.y);
  }
  else {
    return(default_height);
  }
}

void mesh_helper::mesh_height_set(tpos x, tpos z, float height)
{
  VECT tmp;
  
  if(mesh_vertex_get(x,z,&tmp)) {
    tmp.y = height;    
    mesh_vertex_set(x,z,&tmp);
  }
}

void mesh_helper::init(GEOMETRY_DESCRIPTION *p_geom)
{
  p_mesh = NULL;
  
  mesh_type = p_geom->type;
  mesh_start = p_geom->start;
  mesh_diff = p_geom->diff;
  mesh_size = p_geom->size;
  mesh_bunch_slice_num = p_geom->bunch_slice_num;
  mesh_bunch_slice_segments = p_geom->bunch_slice_segments;
}

MESH_HELPER mesh_helper::derivate(tpos sx, tpos sz, tpos width, tpos height)
{
  MESH_HELPER tmp = *this;
  
  // Mesh location
  mesh_start.x = sx;
  mesh_start.z = sz;
  
  // Size of whole mesh
  mesh_size.x = width;
  mesh_size.z = height;
   
  return(tmp);
}

/*
  // Shit! -> vsechny objekty ktery ho pouzivaji sou v riti!
  p_scene->material_delete(p_material);
      - pocitani referenci?
      - neprime odkazy?
      - management techto objektu ve scene?
      - scena -> pripojeni?
  
  - pripojit konkretni operace pouze na objekty ktere to pouzivaji?
    - mesh  -> smazat_material
            -> smazat_texturu
  
  - mit ty pripojne body
    - kontejnery? (mazat obsah)?
    - seznam aktivnich pointeru?  
*/
