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
 
#ifndef __GENERATOR_TARGET_H__
#define __GENERATOR_TARGET_H__

class generator_mesh;
typedef class generator_mesh GENERATOR_MESH;

/* 
 * Mesh params
 */
#define DEFAULT_TEXTURE_SIZE      512
#define DEFAULT_TEXTURE_SIZE_MAX  2048

#define DEFAULT_MESH_SIZE         20


/*
 * Mesh generator
 */
// Parameters for generating mesh params
typedef struct geometry_description
{ 
  /*
   * Parameters synced with mesh helper
   */  
  // Generated type
  MESH_TYPE         type;
  
  // Mesh start
  interval<VECT>    start;
  
  // Diferences between vertexes
  interval<VECT>    diff;
  
  // Size of whole mesh
  interval<VECT3DI> size;
  
  // Number of bunch slices
  interval<int>     bunch_slice_num;
  
  // Number of segments of each slice
  interval<int>     bunch_slice_segments;

  /*
   * Parameters used only with mesh generator
   */

  /*
   * Slice position
   */
  interval<float>   bunch_slice_x_offset;
  interval<float>   bunch_slice_z_offset;

  /*
   * Slice width
   */
  interval<float>   bunch_slice_width_start;
  interval<float>   bunch_slice_width_end;
   
  /*
   * Slice height
   */
  interval<float>   bunch_slice_height_start;
  interval<float>   bunch_slice_height_end;
     
  /*
   * Slice rotation
   */
  interval<float>   bunch_slice_falling;
  interval<float>   bunch_segment_falling;
  
  /*
   * incemental = TRUE
   *   - each slice is rotated by bunch_slice_rotation_step
   *
   * incemental = FALSE
   *   - each slice is rotated by (bunch_slice_rotation_range / slice_num) + bunch_slice_rotation_step
   */    
  bool              bunch_slice_rotation_incemental;
  interval<float>   bunch_slice_rotation_range;
  interval<float>   bunch_slice_rotation_step;  

public:

  geometry_description(void)
  {
    type = MESH_LAND;
    
    start = VECT(0,0,0);
    diff = VECT(1,1,1);
    size = VECT3DI(DEFAULT_MESH_SIZE,DEFAULT_MESH_SIZE,DEFAULT_MESH_SIZE);
    
    bunch_slice_num = 6;
    bunch_slice_segments = 1;
    
    bunch_slice_x_offset = 0;
    bunch_slice_z_offset = 0;

    bunch_slice_width_start = 1;
    bunch_slice_width_end = 1;

    bunch_slice_height_start = 1;
    bunch_slice_height_end = 1;
    
    bunch_slice_falling = 0;
    bunch_segment_falling = 0;
    
    bunch_slice_rotation_incemental = 0;
    bunch_slice_rotation_range = PI;
    bunch_slice_rotation_step = 0;
  }

} GEOMETRY_DESCRIPTION;

// Description of generated material
typedef struct material_description
{
  bool              transparent;
  bool              double_side;
  
public:
  
  material_description(void)
  {
    transparent = FALSE;
    double_side = FALSE;
  }
  
} MATERIAL_DESCRIPTION;

// Description of generated texture
typedef struct texture_description
{
  // Size of generated texture
  VECT2DI           texture_size;
  VECT2DI           texture_size_max;
  tpos              texture_height;
  RGBAF             texture_background_color;
  bool              texture_alpha;
  
public:
  
  texture_description(void)
  {
    texture_size.set(DEFAULT_TEXTURE_SIZE);
    texture_size_max.set(DEFAULT_TEXTURE_SIZE_MAX);
    texture_height = DEFAULT_TEXTURE_SIZE;
    texture_background_color.set();
    texture_alpha = FALSE;
  }
  
} TEXTURE_DESCRIPTION;

/*
 * Target of end modificator
 */
typedef enum {

  // Not set
  MODIFICATOR_TARGET_NONE             = 0,
  
  // Operation destination is texture
  MODIFICATOR_TARGET_TEXTURE_TYPE     = 1,
  
  // Operation destination is geometry
  MODIFICATOR_TARGET_GEOMETRY_TYPE    = 2,
  
  // Operation destination is mesh generator
  MODIFICATOR_TARGET_GENERATOR_TYPE   = 3,
  
  // Operation destination is auxilliary target
  MODIFICATOR_TARGET_AUX_TYPE         = 4,
    
  // Operation destination is a modificator
  MODIFICATOR_TARGET_MODIFICATOR_TYPE = 5,
  
  // Operation destination is a mask modificator
  MODIFICATOR_TARGET_MASK_TYPE        = 6,
  
  // Operation destination is a heightmap modificator 
  MODIFICATOR_TARGET_HEIGHTMAP_TYPE   = 7,
      
  MODIFICATOR_TARGET_NUM

} MODIFICATOR_TARGET_TYPE;

// Target of modificator operation
// Can be mesh/texture or any other modificator
typedef class modificator_target 
  : public object_list,
    public generator_reference
{  

public:
  
  MODIFICATOR_TARGET_TYPE  type;

private:
  
  MASK_REFERENCE          *p_mask;
  
public:

  MASK_REFERENCE * mask_get(void)
  {
    return(p_mask);
  }

  void mask_set(MASK_REFERENCE *p_target_mask)
  {
    p_mask = p_target_mask;
    if(p_mask) {
      p_mask->mask_size_set(target_area_get());
    }
  }

public:

  bool mask_active(void)
  {
    return(p_mask && p_mask->mask_active());
  }

  bool mask_get(tpos x, tpos y)
  {
    if(p_mask && p_mask->mask_active()) {
      if(!p_mask->mask_get(x,y))
        return(FALSE);
    }
    return(TRUE);
  }
  
protected:

  VECT2DI                  scale;
  bool                     scale_active;

public:

  bool target_scale_active(void)
  {
    return(scale_active);
  }
  VECT2DI target_scale_get(void)
  {
    return(scale);
  }

public:
  
  virtual void target_scale_set(VECT2DI scale)
  {
    assert(scale.x > 0 && scale.y > 0);
    scale_active = TRUE;
    this->scale = scale;
  }

  virtual void target_scale_clear(void)
  {
    scale_active = FALSE;
  }
  
protected:
  
  void target_coordinate_get(tpos &x, tpos &y)
  {
    if(scale_active) {
      RECT2DI rect = target_area_get();
      
      float scale_x = (float)rect.dx / (float)scale.x;
      float scale_y = (float)rect.dy / (float)scale.y;
      
      x = x*scale_x;
      y = y*scale_y;
    }
  }
  
  VECT2DI target_coordinate_get(VECT2DI coord)
  {
    VECT2DI tmp = coord;
    target_coordinate_get(tmp.x, tmp.y);
    return(tmp);
  }

protected:
  
  bool mask_pixel_valid(int x, int y)
  {    
    if(p_mask && 
       p_mask->mask_active() &&
       !p_mask->mask_overlapping_active())
    {
      if(!p_mask->mask_get(x,y))
        return(FALSE);
    }
    
    return(TRUE);
  }  

public:

  virtual bool    pixel_valid(int x, int y) = 0;

public:
  
  virtual tcolor  color_get(int x, int y) = 0;
  virtual void    color_set(int x, int y, tcolor color) = 0;

public:

  virtual float   height_get(int x, int y, float default_value = 0) = 0;
  virtual void    height_set(int x, int y, float height) = 0;
  virtual VECT3DF normal_get(int x, int y) = 0;

public:
  
  virtual RECT2DI target_area_get(void) = 0;
  virtual tpos    target_height_get(void) = 0;

public:
  
  virtual void    flush(MESH_HELPER *p_helper) = 0;

public:
  
  modificator_target(GENERATOR_MESH *p_gen, MODIFICATOR_TARGET_TYPE target_type);
  virtual ~modificator_target(void);
    
} MODIFICATOR_TARGET;

typedef class modificator_target_stack {

  MODIFICATOR_TARGET *p_stack[MAX_MODIFICATOR_TARGETS];
  int                 stack_last;
  
public:
  
  void push(MODIFICATOR_TARGET *p_target)
  {
    assert(p_target && stack_last < MAX_MODIFICATOR_TARGETS);
    if(stack_last < MAX_MODIFICATOR_TARGETS-1) {
      p_stack[stack_last++] = p_target;
    }
  }

  MODIFICATOR_TARGET ** get_all(int &num)
  {
    num = stack_last;
    return(p_stack);
  }
  
  MODIFICATOR_TARGET * get_first(void)
  {    
    return(p_stack[0]);
  }
  
public:
  
  modificator_target_stack(void)
  {
    stack_last = 0;
  }
  modificator_target_stack(MODIFICATOR_TARGET *p_target)
  {
    p_stack[0] = p_target;
    stack_last = 1;
  }
  
} MODIFICATOR_TARGET_STACK;

// A cache of all modificator targets
// Owns and manages all targets
typedef class modificator_target_cache 
  : private object_list_head
{  
  GENERATOR_MESH       *p_generator;
  MESH_HELPER           helper;
  
  // Targets will inherit this mask
  MASK_REFERENCE       *p_mask;
  
private:

  // Geometry generator helpers
  void            mesh_geometry_create_landscape(GAME_MESH *p_mesh, MESH_HELPER *p_helper);
  void            mesh_geometry_copy_landscape(MESH_HELPER *p_src, MESH_HELPER *p_dest, tpos sx, tpos sy);

  void            mesh_geometry_create_bunch(GAME_MESH *p_mesh, MESH_HELPER *p_helper, GEOMETRY_DESCRIPTION *p_desc);
  void            mesh_geometry_create(GAME_MESH *p_mesh, MESH_HELPER *p_helper, GEOMETRY_DESCRIPTION *p_desc);
  GAME_MESH     * mesh_geometry_create(SCENE *p_scene, MESH_HELPER *p_helper, GEOMETRY_DESCRIPTION *p_desc);  
  GAME_MESH     * mesh_geometry_copy(SCENE *p_scene, MESH_HELPER *p_src, MESH_HELPER *p_dest, tpos sx, tpos sz);

private:  
  
  // Generate material
  MESH_MATERIAL * material_create(SCENE *p_scene, MATERIAL_DESCRIPTION *p_desc);

private:  

  // Generate texture
  MATERIAL_TEXT * texture_create(SCENE *p_scene, TEXTURE_DESCRIPTION *p_desc, SURFACE_SW *p_surf);
  
private:  
  
  MODIFICATOR_TARGET * target_create(MODIFICATOR_TARGET_TYPE type,
                                     char *p_name);
public:  

  // Return already created target or create a new one
  MODIFICATOR_TARGET * target_get(MODIFICATOR_TARGET_TYPE type,
                                  char *p_name);

  // Look for a target
  MODIFICATOR_TARGET * target_find(MODIFICATOR_TARGET_TYPE type,
                                   char *p_name);

public:
  
  void clear(void)
  {
    object_list_clear();
  }

public:  
  
  void target_mask_set(MASK_REFERENCE *p_target_mask)
  {
    p_mask = p_target_mask;
  }
  
public:  
  
  void          init(void);
  MESH_HELPER * flush(SCENE *p_scene);

public:
  
  modificator_target_cache(GENERATOR_MESH *p_gen);
  
} MODIFICATOR_TARGET_CACHE;

MODIFICATOR_TARGET_TYPE modificator_target_translate(const char *p_type);
const char            * modificator_target_translate(MODIFICATOR_TARGET_TYPE type);

#endif // __GENERATOR_TARGET_H__
