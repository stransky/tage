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
 
bool mask_application::load(DATA_FILE_SECTION *p_line)
{
  // Modificator mask
  if(p_line->match_name("mask_op")) {
    operation.op = aritmetic_operation_names_translate(p_line->line_value_string_get());
    p_line->line_mark_loaded();
    return(TRUE);
  }

  // Mask blending
  if(p_line->match_name("mask_blend")) {
    operation.blend = p_line->line_value_int_get();
    p_line->line_mark_loaded();
    return(TRUE);
  }
  
  // Mask spreading
  if(p_line->match_name("mask_dithering")) {
    dithering = p_line->line_value_int_get();
    p_line->line_mark_loaded();
    return(TRUE);
  }
  
  return(FALSE);
}

mask_application::mask_application(void)
{
  operation.op = OPERATION_SET;
  operation.blend = TRUE;
  dithering = FALSE;
}

/*
  Modificator mask
*/
VECT2DI mask_reference::mask_to_bitmap(VECT2DI pos, tpos width, tpos height)
{
  VECT2DI res(0);
  
  VECT2DI bsize(width, height);
  
  if(bsize.x == 0 || bsize.y == 0)
    return(res);
  
  float x_factor = bsize.x / (float)mask_reference_size.dx;
  float y_factor = bsize.y / (float)mask_reference_size.dy;
  
  res.x = x_factor*(pos.x - mask_reference_size.x);
  res.y = y_factor*(pos.y - mask_reference_size.y);
  
  return(res);
}

VECT2DI mask_reference::bitmap_to_mask(VECT2DI pos, tpos width, tpos height)
{
  VECT2DI res(0);
  
  VECT2DI bsize(width, height);
  if(bsize.x == 0 || bsize.y == 0)
    return(res);
  
  float x_factor = mask_reference_size.dx / bsize.x;
  float y_factor = mask_reference_size.dy / bsize.y;
  
  res.x = pos.x + x_factor*pos.x;
  res.y = pos.y + y_factor*pos.y;
  
  return(res);
}

VECT2DI mask_reference::mask_to_bitmap(VECT2DI pos)
{
  if(!p_mask || !p_mask->mask_loaded()) {
    return(VECT2DI(0));
  }
  
  return(mask_to_bitmap(pos, p_mask->width_get(), p_mask->height_get()));
}

VECT2DI mask_reference::bitmap_to_mask(VECT2DI pos)
{
  if(!p_mask || !p_mask->mask_loaded()) {
    return(VECT2DI(0));
  }
  
  return(bitmap_to_mask(pos, p_mask->width_get(), p_mask->height_get()));
}

void  mask_reference::mask_size_set(RECT2DI size)
{
  mask_reference_size = size;
}

RECT2DI mask_reference::mask_size_get(void)
{
  return(mask_reference_size);
}

bool mask_reference::mask_get(VECT2DI pos)
{
  bool ret = TRUE;
  
  if(!p_mask)
    return(ret);
  
  VECT2DI mask_coord = mask_to_bitmap(pos);

  switch(p_mask->mask_type_get()) {
    case MASK_BOOL:
    {
      ret = p_mask->mask_bool_get(mask_coord);
      break;
    }
    case MASK_COLOR:
    {
      RGBB mask_color = p_mask->mask_color_get(mask_coord);
      ret = (mask_color_min <= mask_color && mask_color <= mask_color_max);
      break;
    }
    case MASK_HEIGHT:
    {
      float min = MIN(mask_height_min,mask_height_max);
      float max = MAX(mask_height_min,mask_height_max);
      
      float mask_height = p_mask->mask_height_get(mask_coord);
      ret = (min <= mask_height && mask_height <= max);
      break;
    }
    default:
      ret = FALSE;
      break;
  }
  
  if(mask_reference_inverted) {
    ret = !ret;
  }

  return(ret);
}

bool mask_reference::mask_get(tpos x, tpos y)
{
  return(mask_get(VECT2DI(x,y)));
}

bool mask_reference::mask_get(RECT2DI pos)
{
  return(mask_get(pos.center_get()));
}

float mask_reference::mask_overlapping_color_get(VECT2DI pos)
{  
  assert(p_mask && mask_reference_overlapping);
  assert(mask_color_min == mask_color_max);
  
  VECT2DI mask_coord = mask_to_bitmap(pos);
  
  float w = ((float)width_get())/p_mask->width_get();
  float h = ((float)height_get())/p_mask->height_get();  
  int range = (w > h) ? w*mask_reference_overlapping : h*mask_reference_overlapping;
    
  float result = 0;
  p_mask->mask_color_get(mask_coord, mask_color_min, range, result);
  
  return(result);
}

float mask_reference::mask_overlapping_height_get(VECT2DI pos)
{  
  assert(p_mask && mask_reference_overlapping);  
  
  float result = 0;
  
  result = p_mask->mask_height_get(pos);
  result = (result - mask_height_min) / (mask_height_max - mask_height_min);
  
  if(result < 0)
    result = 0;
  if(result > 1)
    result = 1;
  
  assert(result >= 0 && result <= 1);
  return(result);
}

float mask_reference::mask_overlapping_get(VECT2DI pos)
{
  if(!p_mask)
    return(1);
    
  if(!mask_reference_overlapping) {
    return(mask_get(pos) ? 1.0f : 0.0f);
  }
  
  float result = 0;
  
  if(p_mask->mask_type_get() == MASK_BOOL)
  {
    
  } 
  else if(p_mask->mask_type_get() == MASK_HEIGHT)
  {
    result = mask_overlapping_height_get(pos);
  }
  else if(p_mask->mask_type_get() == MASK_COLOR)
  {
    result = mask_overlapping_color_get(pos);
  }
  
  return(result);
}

float mask_reference::mask_overlapping_get(tpos x, tpos y)
{
  return(mask_overlapping_get(VECT2DI(x,y)));
}

float mask_reference::mask_overlapping_get(RECT2DI pos)
{
  return(mask_overlapping_get(pos.center_get()));
}

void mask_reference::mask_clear(bool value)
{
  if(p_mask) {
    p_mask->mask_clear(value);
  }
}

void mask_reference::mask_shrink(int num)
{
  if(p_mask) {
    p_mask->mask_shrink(num);
  }
}

void mask_reference::mask_create(RECT2DI pos)
{
  if(p_mask_reference_create) {
    p_mask_reference_create->mask_load(pos.dx, pos.dy);    
  }
}  

bool mask_reference::load_line(DATA_FILE_SECTION *p_line, GENERATOR *p_generator)
{
  // Mask application
  if(mask_reference_app.load(p_line))
    return(TRUE);
  
  // Modificator mask
  if(p_line->match_name("mask")) {
    MODIFICATOR *p_mask = p_generator->modificator_find(p_line->line_value_string_get());
    if(p_mask) {
      mask_set((MODIFICATOR_MASK *)p_mask);
    }
    else {
      generator_loader_error(p_line->source_file_get(), 
                             p_line->source_line_get(), 
                             "Can't find GENERATOR_MASK %s", 
                             p_line->line_value_string_get());
    }
    p_line->line_mark_loaded();
    return(TRUE);
  }

  // Modificator mask
  if(p_line->match_name("mask_inverted")) {
    MODIFICATOR *p_mask = p_generator->modificator_find(p_line->line_value_string_get());
    if(p_mask) {
      mask_inverted_set((MODIFICATOR_MASK *)p_mask);
    }
    else {
      generator_loader_error(p_line->source_file_get(), 
                             p_line->source_line_get(),
                             "Can't find GENERATOR_MASK %s",
                             p_line->line_value_string_get());
    }
    p_line->line_mark_loaded();
    return(TRUE);
  }
  
  // Modificator mask init
  if(p_line->match_name("mask_create")) {
    MODIFICATOR *p_mask = p_generator->modificator_find(p_line->line_value_string_get());
    if(p_mask) {      
      mask_create_set((MODIFICATOR_MASK *)p_mask);
    }
    else {
      generator_loader_error(p_line->source_file_get(), 
                             p_line->source_line_get(), 
                             "Can't find GENERATOR_MASK %s",
                             p_line->line_value_string_get());
    }
    p_line->line_mark_loaded();
    return(TRUE);
  }

  // Mask overlapping
  if(p_line->match_name("mask_overlapping")) {
    mask_reference_overlapping = p_line->line_value_int_get();
    p_line->line_mark_loaded();
    return(TRUE);
  }
  
  // Mask color
  RGBB    mask_color(0);
  bool    mask_color_active = FALSE;
    
  do {
    load_rgbb_active(mask_color, "mask_color");
  } while(FALSE);
  
  if(mask_color_active) {
    mask_color_set(mask_color);
    return(TRUE);
  }
    
  // Mask height
  if(p_line->match_name("mask_height")) {
    mask_height_set(p_line->line_value_float_get());
    p_line->line_mark_loaded();
    return(TRUE);
  }
  
  // Mask height min
  if(p_line->match_name("mask_height_min")) {
    mask_height_min = p_line->line_value_float_get();
    p_line->line_mark_loaded();
    return(TRUE);
  }
  // Mask height min
  if(p_line->match_name("mask_height_max")) {
    mask_height_max = p_line->line_value_float_get();
    p_line->line_mark_loaded();
    return(TRUE);
  }
  
  return(FALSE);
}

bool mask_reference::load(DATA_FILE_SECTION *p_section, GENERATOR *p_generator)
{  
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line) {
    return(FALSE);
  }    
  
  do {
    if(p_line->is_line()) {
      load_line(p_line, p_generator);
    }
  } while((p_line = p_line->section_next()));
    
  return(TRUE);
}
