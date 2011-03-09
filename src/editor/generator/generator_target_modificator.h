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

#ifndef __GENERATOR_TARGET_MODIFICATOR_H__
#define __GENERATOR_TARGET_MODIFICATOR_H__

class modificator;
typedef class modificator MODIFICATOR;

class modificator_height_map;
typedef class modificator_height_map MODIFICATOR_HEIGHT_MAP;

/*
 * Mask target points writes to mask modificator
 */
typedef class modificator_target_modificator :
  public modificator_target
{     
  
protected:
  
  // Name of mask
  char               modificator_name[MAX_NAME];

  // Connected mask modificator
  MODIFICATOR       *p_modificator;

protected:

  MODIFICATOR * modificator_get(void)
  {
    return(p_modificator);
  }
  
  void modificator_set(MODIFICATOR *p_mod)
  {
    p_modificator = p_mod;
  }

public:
  
  modificator_target_modificator(GENERATOR_MESH *p_gen, 
                                 MODIFICATOR_TARGET_TYPE target_type)
  : modificator_target(p_gen, target_type)
  {    
    modificator_name[0] = '\0';
    p_modificator = NULL;
  }

} MODIFICATOR_TARGET_MODIFICATOR;

/*
 * Mask target points writes to mask modificator
 */
typedef class modificator_target_mask:
  public modificator_target_modificator
{       

private:
  
  MODIFICATOR_MASK * mask_get(void)
  {
    return((MODIFICATOR_MASK *)modificator_get());
  }
  
  bool    mask_load(void);
  void    mask_clear(void);

public:

  bool    pixel_valid(int x, int y);

public:
  
  tcolor  color_get(int x, int y);
  void    color_set(int x, int y, tcolor color);

public:

  float   height_get(int x, int y, float default_value = 0);
  void    height_set(int x, int y, float height);
  VECT3DF normal_get(int x, int y);

public:
  
  RECT2DI target_area_get(void);
  tpos    target_height_get(void) { return(0); }

public:
    
  void flush(MESH_HELPER *p_helper) {}
  
public:
  
  modificator_target_mask(GENERATOR_MESH *p_gen);

} MODIFICATOR_TARGET_MASK;

/*
 * Mask target points writes to heightmap modificator
 */
typedef class modificator_target_heightmap:
  public modificator_target_modificator
{       
  
  MODIFICATOR_HEIGHT_MAP * heightmap_get(void)
  {
    return((MODIFICATOR_HEIGHT_MAP *)modificator_get());
  }
  
  bool    heightmap_load(void);
  void    heightmap_clear(void);
   
public:

  bool    pixel_valid(int x, int y);

public:
  
  tcolor  color_get(int x, int y);
  void    color_set(int x, int y, tcolor color);

public:

  float   height_get(int x, int y, float default_value = 0);
  void    height_set(int x, int y, float height);
  VECT3DF normal_get(int x, int y);

public:
  
  RECT2DI target_area_get(void);
  tpos    target_height_get(void) { return(0); }

public:
    
  void flush(MESH_HELPER *p_helper) {}
  
public:
  
  modificator_target_heightmap(GENERATOR_MESH *p_gen);

} MODIFICATOR_TARGET_HEIGHTMAP;

#endif // __GENERATOR_TARGET_MODIFICATOR_H__
