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

#ifndef __GENERATOR_TEXTURE_H__
#define __GENERATOR_TEXTURE_H__

/*
 * Helper class for height-based targets (geometry now)
 */
typedef class modificator_target_height :
  public modificator_target
{  
  // Base height surface
  SURFACE_HEIGHT *p_heightmap;
  tpos            texture_height;
  
  // Base height scaled surface
  SURFACE_HEIGHT *p_heightmap_scale;
  
public:
    
  SURFACE_HEIGHT * heightmap_get(void)
  {
    return(p_heightmap);
  }
  
protected:
  
  virtual void heightmap_create(tpos dx, tpos dy, tpos height);
  virtual void heightmap_destroy(void);

public:

  void target_scale_set(VECT2DI scale)
  {
    assert(!p_heightmap_scale);
    
    modificator_target::target_scale_set(scale);
    p_heightmap_scale = new SURFACE_HEIGHT(scale.x, scale.y);
    p_heightmap->blit(p_heightmap_scale);
  }
  
  void target_scale_clear(void)
  { 
    if(p_heightmap_scale) {
      p_heightmap_scale->blit(p_heightmap);
      delete p_heightmap_scale;
      p_heightmap_scale = NULL;
    }
  }

public:
  
  bool pixel_valid(int x, int y)
  {
    target_coordinate_get(x,y);
    return(mask_pixel_valid(x, y) && 
           p_heightmap->pixel_valid(x,y));
  }

public:

  float height_get(int x, int y, float default_value)
  { 
    if(p_heightmap_scale) {
      return(p_heightmap_scale->height_get(x,y,default_value));
    }
    else {
      target_coordinate_get(x,y);
      return(p_heightmap->height_get(x,y,default_value));
    }
  }
  
  void height_set(int x, int y, float height)
  {
    tpos mx = x,
         my = y;
    
    target_coordinate_get(mx, my);
    if(mask_get(mx, my)) {
      p_heightmap_scale ? p_heightmap_scale->height_set(x,y,height) :
                          p_heightmap->height_set(mx, my, height);
    }
  }
  
  VECT3DF normal_get(int x, int y)
  {
    target_coordinate_get(x,y);
    if(mask_get(x,y)) {
      return(p_heightmap->height_normal_get(x, y));
    }
    return(VECT3DF(0));
  }  

public:
  
  tcolor color_get(int x, int y)
  {      
    float height = height_get(x, y, 0.0f);
    RGBAF color(height);
    return(color);    
  }
  
  void color_set(int x, int y, tcolor color)
  { 
    /*
    target_coordinate_get(x,y);
    if(p_mask && p_mask->mask_active()) {
      RGBB target = color_get(x, y);
      RGBB final = p_mask->apply(VECT2DI(x,y), target, RGBB(color));
      color = final;
    }
    RGBAF tmp(color);
    float height = tmp.intensity();
    height_set(x,y,height);
    */
  }
  
public:

  RECT2DI target_area_get(void)
  {
    RECT2DI tmp(0);    
    if(p_heightmap) {
      tmp = RECT2DI(0,0,p_heightmap->width_get(),p_heightmap->height_get());
    }
    return(tmp);
  }
  
  tpos target_height_get(void)
  {
    return(texture_height);
  }
    
public:
  
  void create(tpos dx, tpos dy, tpos height)
  {
    heightmap_create(dx, dy, height);    
  }

public:
  
  modificator_target_height(GENERATOR_MESH *p_gen, MODIFICATOR_TARGET_TYPE target_type);
  virtual ~modificator_target_height(void);

} MODIFICATOR_TARGET_HEIGHT;


/*
 * Auxilliary target, contains bitmap & height data and can be used
 * as target & source 
 */
typedef class modificator_target_aux :
  public modificator_target
{
  /*
   * Target size
   */
  RECT2DI               size;
  
  /*
   * Write to this surface
   */
  SURFACE_SW            bitmap;
  SURFACE_HEIGHT        heightmap;
  
public:
  
  bool bitmap_loaded(void)
  {    
    return(bitmap.loaded());
  }
  
  bool heightmap_loaded(void)
  {
    return(heightmap.loaded());
  }
   
  SURFACE_SW * bitmap_get(void)
  {
    bitmap_create();
    return(&bitmap);
  }
  
  SURFACE_HEIGHT * heightmap_get(void)
  {
    heightmap_create();
    return(&heightmap);
  }
  
private:
  
  void bitmap_create(void)
  { 
    if(!bitmap.loaded()) {    
      bitmap.create(size.dx, size.dy);
    }
  }
  
  void bitmap_destroy(void)
  {  
    bitmap.clear();
  }
  
  void heightmap_create(void)
  {
    if(!heightmap.loaded()) {    
      heightmap.create(size.dx, size.dy);
    }
  }
  
  void heightmap_destroy(void)
  {
    heightmap.clear();
  }
  
public:
  
  void clear(void)
  {
    heightmap_destroy();
    bitmap_destroy();
  }

public:
  
  bool pixel_valid(int x, int y)
  {
    target_coordinate_get(x,y);
    return(mask_pixel_valid(x,y) &&
           (x >= size.x && x < size.dx) &&
           (y >= size.y && y < size.dy));
  }
  
public:
  
  tcolor color_get(int x, int y)
  {
    target_coordinate_get(x,y);
    bitmap_create();
    return(bitmap.pixel_get(x,y));
  }
  
  void color_set(int x, int y, tcolor color)
  {
    target_coordinate_get(x,y);
    bitmap_create();
    if(mask_active()) {
      RGBB target = color_get(x, y);
      RGBB final = mask_get()->apply(VECT2DI(x,y), target, RGBB(color));
      color = final;
    }
    bitmap.pixel_set(x,y,color);
  }

public:

  float height_get(int x, int y, float default_value)
  {
    target_coordinate_get(x,y);
    heightmap_create();
    return(heightmap.height_get(x,y,default_value));
  }
  
  void height_set(int x, int y, float height)
  {
    target_coordinate_get(x,y);
    if(mask_get(x,y)) {
      heightmap_create();
      heightmap.height_set(x,y,height);
    }
  }
  
  VECT3DF normal_get(int x, int y)
  {
    target_coordinate_get(x,y);
    if(mask_get(x,y)) {
      heightmap_create();
      return(heightmap.height_normal_get(x, y));
    }
    return(VECT3DF(0));
  }  

public:

  RECT2DI target_area_get(void)
  {
    return(size);
  }

  /*
   * "Virtual" maximal height of this texture
   */
private:

  tpos texture_height;

public:
  
  tpos target_height_get(void)
  {
    return(texture_height);
  }
  
  void target_height_set(tpos height)
  {    
    texture_height = height;
  }
  
public:

  void size_set(VECT2DI bitmap_size)
  {
    assert(!bitmap.loaded() && !heightmap.loaded());    
    size = RECT2DI(VECT2DI(0),bitmap_size);
    MASK_REFERENCE *p_mask = mask_get();
    if(p_mask) {
      p_mask->mask_size_set(size);
    }
  }
  
  void init(VECT2DI size, tpos height)
  {
    size_set(size);
    target_height_set(height);    
  }
  
public:
  
  void flush(MESH_HELPER *p_helper) {};

public:
  
  modificator_target_aux(GENERATOR_MESH *p_gen, MODIFICATOR_TARGET_TYPE target_type);
  virtual ~modificator_target_aux(void);

} MODIFICATOR_TARGET_AUX;

/*
 * Terrain textures
 */
typedef enum {
  
  TEXTURE_SET = 0,
  TEXTURE_ADD,
  TEXTURE_SUB,
  TEXTURE_MULT
  
} TEXTURE_OPERATOR;

typedef class modificator_target_texture :
  public modificator_target_aux
{   
  
  /*
   *  Texture operation - What is this filter used with the current texture
   */
private:
  
  TEXTURE_OPERATOR height_operator;

public:
  
  void  texture_operator_set(TEXTURE_OPERATOR op);

public:
    
  void flush(MESH_HELPER *p_helper);

public:
  
  modificator_target_texture(GENERATOR_MESH *p_gen);
  virtual ~modificator_target_texture(void);

} MODIFICATOR_TARGET_TEXTURE;

#endif // __GENERATOR_TEXTURE_H__
