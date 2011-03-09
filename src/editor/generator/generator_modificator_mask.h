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

#ifndef __GENERATOR_MODIFICATOR_MASK_H__
#define __GENERATOR_MODIFICATOR_MASK_H__

/*
 * Mask modificator definition
 */
class modificator_mask;
typedef class modificator_mask MODIFICATOR_MASK;

/*
 * Mask application
 *
 * Describes application of mask
 */
typedef class mask_application
{
public:  
  
  // Basic operations between source & target
  BLEND_OPERATION     operation;
  
  // Is the color distribution based on probability?
  bool                dithering;
  
public:  

  // target is current texture/bitmap color
  // source is a new color comming from modificator
  // factor is <0,1> ratio of source
  RGBB apply(RGBB target, RGBB source, float factor)
  {
    assert(factor >= 0 && factor <= 1);
    
    if(dithering && factor < 1) {
      float probability = random_generator::generator_rand();
      if(probability > factor) {
        return(target);
      }      
    }
    
    if(operation.blend) {
      operation.op = OPERATION_ADD;
      operation.blend_factor = factor;
    }
    
    return(blend(operation, (RGBF)target, (RGBF)source));
  }

public:

  bool load(DATA_FILE_SECTION *p_line);
  
public:
  
  mask_application(void);
  
} MASK_APPLICATION;

/*
 * Mask reference
 */
typedef class mask_reference
  : public mask_interface
{ 
  // Mask reference
  MODIFICATOR_MASK   *p_mask;
  RECT2DI             mask_reference_size;
  int                 mask_reference_overlapping;
  MASK_APPLICATION    mask_reference_app;
  bool                mask_reference_inverted;
  
  // Create remote mask (in initialization)
  MODIFICATOR_MASK   *p_mask_reference_create;
  
public:

  bool mask_active(void)
  {
    return(p_mask);
  }
  bool mask_create_active(void)
  {
    return(p_mask_reference_create);
  }
  bool mask_overlapping_active(void)
  {
    return(p_mask && mask_reference_overlapping);
  }
  void mask_set(MODIFICATOR_MASK *p_mask)
  {
    this->p_mask = p_mask;
    mask_reference_inverted = FALSE;
  }
  void mask_inverted_set(MODIFICATOR_MASK *p_mask)
  {
    this->p_mask = p_mask;
    mask_reference_inverted = TRUE;
  }
  
  // Create remote mask
  void mask_create_set(MODIFICATOR_MASK *p_mask)
  {
    this->p_mask_reference_create = p_mask;
  }  
  
  void mask_clear(bool value);
  void mask_shrink(int num);

public:

  VECT2DI mask_to_bitmap(VECT2DI pos, tpos widht, tpos height);
  VECT2DI bitmap_to_mask(VECT2DI pos, tpos widht, tpos height);

private:

  VECT2DI mask_to_bitmap(VECT2DI pos);
  VECT2DI bitmap_to_mask(VECT2DI pos);

public:

  void    mask_size_set(RECT2DI size);
  RECT2DI mask_size_get(void);
  
public:
  
  RGBB    mask_color_min;
  RGBB    mask_color_max;

public:

  void mask_color_set(RGBB mask)
  {
    mask_color_min = mask;
    mask_color_max = mask;
  }
  void mask_color_set(RGBB min, RGBB max)
  {
    mask_color_min = min;
    mask_color_max = max;
  }
  
public:
  
  float   mask_height_min;
  float   mask_height_max;

public:
  
  void mask_height_set(float mask)
  {
    mask_height_min = mask;
    mask_height_max = mask;
  }
  void mask_height_set(float min, float max)
  {
    mask_height_min = min;
    mask_height_max = max;
  }
  
public:
  
  tpos width_get(void)
  {
    return(mask_reference_size.dx);
  }
  
  tpos height_get(void)
  {
    return(mask_reference_size.dy);
  }
  
  void mask_set(int x, int y, bool mask)
  {
    // not implemented yet
    assert(0);
  }
    
public:
  
  // Get the mask for given point without overlapping
  bool mask_get(tpos x, tpos y);
  bool mask_get(VECT2DI pos);
  bool mask_get(RECT2DI pos);
  
public:

  // Get the mask for given point with overlapping
  // 1 = maximal mask intensity
  // 0 = minimal mask intensity
  float mask_overlapping_get(tpos x, tpos y);
  float mask_overlapping_get(VECT2DI pos);
  float mask_overlapping_get(RECT2DI pos);

private:
  
  float mask_overlapping_color_get(VECT2DI pos);
  float mask_overlapping_height_get(VECT2DI pos);

private:
  
  RGBB apply_normal(VECT2DI pos, RGBB target, RGBB source)
  {
    float factor = (mask_active() && !mask_get(pos)) ? 0 : 1;
    return(mask_reference_app.apply(target, source, factor));
  }
  
  RGBB apply_overlapped(VECT2DI pos, RGBB target, RGBB source)
  {
    float factor = 1;
    if(mask_active()) {
      factor = mask_overlapping_get(pos);
    }
    return(mask_reference_app.apply(target, source, factor));
  }
  
public:
  
  RGBB apply(VECT2DI pos, RGBB target, RGBB source)
  {
    return mask_reference_overlapping ? apply_overlapped(pos, target, source) : 
                                        apply_normal(pos, target, source);
  }

public:

  bool load_line(DATA_FILE_SECTION *p_line, GENERATOR *p_generator);
  bool load(DATA_FILE_SECTION *p_section, GENERATOR *p_generator);

public:

  void mask_create(RECT2DI pos);

public:

  mask_reference(void)
  {
    p_mask = NULL;
    p_mask_reference_create = NULL;
    mask_reference_size.set(0);
    mask_reference_overlapping = 0;
    mask_reference_inverted = FALSE;
    mask_height_min = 0;
    mask_height_max = 1;
  }
  
} MASK_REFERENCE;

#endif // __GENERATOR_MODIFICATOR_MASK_H__
