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
 * Mask target points writes to mask modificator
 */
bool modificator_target_mask::mask_load(void)
{
  MODIFICATOR_MASK *p_mask = mask_get();
  
  if(p_mask && (p_mask->mask_loaded() || p_mask->mask_load_waiting()))
    return(TRUE);
  
  // Try to find the target mask
  if(!p_mask) {
    GENERATOR *p_gen = generator_get();
    
    char *p_name = modificator_name[0] ? modificator_name : name_get();
    p_mask = (MODIFICATOR_MASK *)p_gen->modificator_find(p_name);
    if(!p_mask) {
      pperror(TRUE, "modificator_target_mask::mask_load(): Unable to find modificator '%s'",p_name);
    }
    
    MODIFICATOR_TYPE type = p_mask->modificator_type_get();
    if(type != MODIFICATOR_MASK_TYPE) {
      pperror(TRUE, "modificator_target_mask::mask_load(): modificator '%s' is not MODIFICATOR_MASK",p_name);
    }
  }
  
  // Target mask is not loaded so try to load it now
  if(!p_mask->mask_loaded()) {    
    if(!p_mask->mask_load_waiting()) {
      // Target mask is not configured - try to just create it
      if(target_scale_active()) {
        // try to load size from target
        VECT2DI size = target_scale_get();
        assert(size.x > 0 && size.y > 0);
        p_mask->mask_load(size.x, size.y);        
      }
    }    
  }
  
  // Set mask back to target
  modificator_set(p_mask);
  
  return(TRUE);
}

void modificator_target_mask::mask_clear(void)
{
  MODIFICATOR_MASK *p_mask = mask_get();
  
  if(p_mask) {
    if(p_mask->mask_loaded()) {
      p_mask->mask_remove();
    }
    modificator_set(NULL);
  }
}

bool modificator_target_mask::pixel_valid(int x, int y)
{    
  mask_load();
    
  target_coordinate_get(x,y);
  
  if(!mask_pixel_valid(x,y))
    return(FALSE);
  
  RECT2DI area = target_area_get();
  
  return((x >= area.x && x < area.dx) &&
         (y >= area.y && y < area.dy));
}

tcolor modificator_target_mask::color_get(int x, int y)
{
  mask_load();
  target_coordinate_get(x,y);  
  return((tcolor)mask_get()->mask_color_get(VECT2DI(x,y)));
}

void modificator_target_mask::color_set(int x, int y, tcolor color)
{
  mask_load();
  target_coordinate_get(x,y);
  if(modificator_target::mask_active()) {
    RGBB target = color_get(x, y);
    RGBB final = modificator_target::mask_get()->apply(VECT2DI(x,y), target, RGBB(color));
    color = final;
  }
  mask_get()->mask_color_set(VECT2DI(x,y), RGBB(color));
}

float modificator_target_mask::height_get(int x, int y, float default_value)
{
  mask_load();
  target_coordinate_get(x,y);
  return(mask_get()->mask_height_get(VECT2DI(x,y)));
}

void modificator_target_mask::height_set(int x, int y, float height)
{
  mask_load();
  target_coordinate_get(x,y);  
  if(modificator_target::mask_get(x,y)) {
    mask_get()->mask_height_set(VECT2DI(x,y), height);
  }
}

VECT3DF modificator_target_mask::normal_get(int x, int y)
{
  mask_load();
  target_coordinate_get(x,y);
  return(mask_get()->mask_normal_get(VECT2DI(x,y)));
}

RECT2DI modificator_target_mask::target_area_get(void)
{
  mask_load();
  
  MODIFICATOR_MASK *p_mask = mask_get();
  RECT2DI tmp(0);
  if(p_mask) {
    tmp = RECT2DI(0,0,p_mask->width_get(),p_mask->height_get());
  } else {
    if(target_scale_active()) {
      // try to load size from target
      VECT2DI size = target_scale_get();
      tmp = RECT2DI(0,0,size.x,size.y);
    }
  }
  return(tmp);
}

modificator_target_mask::modificator_target_mask(GENERATOR_MESH *p_gen)
 : modificator_target_modificator(p_gen, MODIFICATOR_TARGET_MASK_TYPE)
{  
}

/*
 * Heightmap target writes to heightmap modificator
 */
bool modificator_target_heightmap::heightmap_load(void)
{
  MODIFICATOR_HEIGHT_MAP *p_map = heightmap_get();
  
  if(p_map && (p_map->heightmap_loaded() || p_map->heightmap_load_waiting()))
    return(TRUE);
  
  // Try to find the target mask
  if(!p_map) {
    GENERATOR *p_gen = generator_get();
    
    char *p_name = modificator_name[0] ? modificator_name : name_get();
    p_map = (MODIFICATOR_HEIGHT_MAP *)p_gen->modificator_find(p_name);
    if(!p_map) {
      pperror(TRUE, "modificator_target_heightmap::heightmap_load(): Unable to find modificator '%s'",p_name);
    }
    
    MODIFICATOR_TYPE type = p_map->modificator_type_get();
    if(type != MODIFICATOR_HEIGHTMAP_TYPE) {
      pperror(TRUE, "modificator_target_heightmap::heightmap_load(): modificator '%s' is not MODIFICATOR_HEIGHT_MAP",p_name);
    }
  }
  
  // Target mask is not loaded so try to load it now
  if(!p_map->heightmap_loaded()) {
    if(!p_map->heightmap_load_waiting()) {
      // Target mask is not configured - try to just create it
      if(target_scale_active()) {
        // try to load size from target
        VECT2DI size = target_scale_get();
        assert(size.x > 0 && size.y > 0);
        p_map->heightmap_load(size.x, size.y);
      }
    }
  }
  
  // Set mask back to target
  modificator_set(p_map);
  
  return(TRUE);
}

bool modificator_target_heightmap::pixel_valid(int x, int y)
{
  if(!heightmap_load())
    return(FALSE);
  
  target_coordinate_get(x,y);
  
  if(!mask_pixel_valid(x,y))
    return(FALSE);
  
  RECT2DI area = target_area_get();
  return((x >= area.x && x < area.dx) &&
         (y >= area.y && y < area.dy));
}

tcolor modificator_target_heightmap::color_get(int x, int y)
{
  heightmap_load();
  target_coordinate_get(x,y);
  float height = heightmap_get()->height_get(x,y);
  return(float_rgb(height, height, height));
}

void modificator_target_heightmap::color_set(int x, int y, tcolor color)
{
  heightmap_load();
  target_coordinate_get(x,y);
  if(mask_active()) {
    RGBB target = color_get(x, y);
    RGBB final = mask_get()->apply(VECT2DI(x,y), target, RGBB(color));
    color = final;
  }
  heightmap_get()->height_set(x,y,rgb_intensity(color));
}

float modificator_target_heightmap::height_get(int x, int y, float default_value)
{
  heightmap_load();
  target_coordinate_get(x,y);
  return(heightmap_get()->height_get(x,y));
}

void modificator_target_heightmap::height_set(int x, int y, float height)
{
  heightmap_load();
  target_coordinate_get(x,y);
  if(mask_get(x,y)) {
    heightmap_get()->height_set(x,y,height);
  }
}

VECT3DF modificator_target_heightmap::normal_get(int x, int y)
{
  heightmap_load();
  target_coordinate_get(x,y);
  if(mask_get(x,y)) {    
    return(heightmap_get()->heightmap_get()->height_normal_get(x,y));
  }
  return(VECT3DF(0));
}

RECT2DI modificator_target_heightmap::target_area_get(void)
{
  RECT2DI tmp(0);
  
  heightmap_load();
  
  MODIFICATOR_HEIGHT_MAP *p_map = heightmap_get();
  SURFACE_HEIGHT *p_surface = p_map->heightmap_get();
  
  if(p_surface) {
    tmp = RECT2DI(0,0,p_surface->width_get(),p_surface->height_get());
  }
  return(tmp);
}

modificator_target_heightmap::modificator_target_heightmap(GENERATOR_MESH *p_gen)
 : modificator_target_modificator(p_gen, MODIFICATOR_TARGET_HEIGHTMAP_TYPE)
{  
}
