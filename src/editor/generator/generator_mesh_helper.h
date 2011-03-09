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
 * Mesh description
 */
#ifndef __GENERATOR_MESH_HELPER_H__
#define __GENERATOR_MESH_HELPER_H__

#define VERTEX_INDEX_GET(x,z,dx,dz) ((x)+(z)*(dx))
#define FACE_INDEX_GET(x,z,dx,dz)   (((x)+(z)*(dx))*2)

typedef enum {
  
  MESH_LAND       = 0,
  MESH_BUNCH      = 1,
  MESH_GRASS      = 2,
  MESH_BUSH       = 3,
  
} MESH_TYPE;

class geometry_description;
typedef class geometry_description GEOMETRY_DESCRIPTION;

// Geometry helper
typedef class mesh_helper
{
  friend class game_mesh;
  
private:  
  
  // Described mesh
  GAME_MESH * p_mesh;
  
public:
  
  GAME_MESH * mesh_get(void)
  {
    return(p_mesh);
  }
  
  void mesh_set(GAME_MESH *p_mesh)
  {
    assert(mesh_get() == NULL);
    this->p_mesh = p_mesh;
  }
  
public:
  
  // mesh type
  MESH_TYPE         mesh_type;

  // Mesh location
  VECT              mesh_start;
  
  // Diferences between vertexes
  VECT              mesh_diff;
  
  // Size of whole mesh
  VECT3DI           mesh_size;
  
  // Slice num
  int               mesh_bunch_slice_num;

  // Slice segments
  int               mesh_bunch_slice_segments;

public:
  
  MESH_TYPE mesh_type_get(void)
  {
    return(mesh_type);
  }

public:

  void   mesh_vertex_lock(LOCK_TYPE type = LOCK_READ);
  void   mesh_vertex_unlock(LOCK_CHANGE_TYPE changed = LOCK_DEFAULT);

  VECT * mesh_vertex_get(tpos x, tpos z, VECT *p_vect);
  void   mesh_vertex_set(tpos x, tpos z, VECT *p_vect);
    
  VECT * mesh_color_get(tpos x, tpos z, VECT *p_vect);
  void   mesh_color_set(tpos x, tpos z, VECT *p_vect);

  float  mesh_height_get(tpos x, tpos z, float default_height = 0.0f);
  void   mesh_height_set(tpos x, tpos z, float height);

public:
  
  void   init(GEOMETRY_DESCRIPTION *p_geom);

public:
  
  mesh_helper derivate(tpos sx, tpos sy, tpos width, tpos height);

public:
  
  mesh_helper(void)
  {    
    mesh_type = MESH_LAND;
    p_mesh = NULL;    
  }
  
} MESH_HELPER;

#endif // __GENERATOR_MESH_HELPER_H__
