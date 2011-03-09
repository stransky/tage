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
 * Texture coordinates
 */
/*
 * Some helpers
 */

// Interpolates two simple coordinates
// c1 and c2
SIMPLE_COORDINATE interpolate(SIMPLE_COORDINATE *p_s1,
                              SIMPLE_COORDINATE *p_s2,
                              float c1, float c2)
{  
  if(p_s1->polar && p_s2->polar) {
    POLAR2DI p1 = p_s1->polar_get();
    POLAR2DI p2 = p_s2->polar_get();
    return(SIMPLE_COORDINATE(interpolate(p1,p2,c1,c2)));
  }
  else {
    VECT2DI v1 = p_s1->cartesian_get();
    VECT2DI v2 = p_s2->cartesian_get();
    return(SIMPLE_COORDINATE(interpolate(v1,v2,c1,c2)));
  }
}

SIMPLE_COORDINATE operate(ARITMETIC_OPERATOR op,
                          SIMPLE_COORDINATE *p_s1,
                          SIMPLE_COORDINATE *p_s2)
{
  switch(op) {
    case OPERATION_SET:
      return(*p_s2);
    case OPERATION_ADD:
      {
        VECT2DI tmp = p_s1->cartesian_get() + p_s2->cartesian_get();
        return(SIMPLE_COORDINATE(tmp));
      }
    case OPERATION_SUB:
      {
        VECT2DI tmp = p_s1->cartesian_get() - p_s2->cartesian_get();
        return(SIMPLE_COORDINATE(tmp));
      }
    case OPERATION_MODULATE:
    case OPERATION_MODULATE2X:
      {
        VECT2DI tmp = p_s1->cartesian_get() * p_s2->cartesian_get();
        return(SIMPLE_COORDINATE(tmp));
      }
    default:
      return(SIMPLE_COORDINATE(0));
  }
}

void coordinate::set(RECT2DI coord)
{
  start.cartesian_set(VECT2DI(coord));
  size.cartesian_set(VECT2DI(coord.dx, coord.dy));
}

void coordinate::start_set(VECT2DI start)
{
  this->start.cartesian_set(start);
}

void coordinate::start_set(POLAR2DI start)
{
  this->start.polar_set(start);
}

void coordinate::size_set(VECT2DI size)
{
  this->size.cartesian_set(size);
}

void coordinate::size_set(POLAR2DI size)
{
  this->size.polar_set(size);
}

RECT2DI coordinate::get(void)
{
  VECT2DI st = start.cartesian_get();
  VECT2DI sz = size.cartesian_get();
  
  return(RECT2DI(st.x,st.y,sz.x,sz.y));
}

void coordinate::print(void)
{
  VECT2DI st = start.cartesian_get();
  VECT2DI sz = size.cartesian_get();
  pprintf("coordinate: [%d, %d] -> [%d, %d]",st.x,st.y,sz.x,sz.y);
}

modificator_coordinates::modificator_coordinates(void)
{ 
  coord_top.set(RECT2DI(0));
  coord_operator = OPERATION_SET;
  
  modificator_start = COORD_CURRENT;
  modificator_size = COORD_CURRENT;
  
  coord_random = FALSE;
  coord_random_num = 0;
  
  coord_num = 0;
  p_coord = NULL;
  
  coord_current_index = 0;
}

modificator_coordinates::~modificator_coordinates(void)
{
  if(p_coord) {
    delete p_coord;
    p_coord = NULL;
  }
}

COORDINATE modificator_coordinates::coord_operator_apply(ARITMETIC_OPERATOR op,
                                                         COORDINATE *p_top,
                                                         COORDINATE *p_current)
{
  COORDINATE tmp;
  
  tmp.start = ::operate(op, &p_top->start, &p_current->start);
  tmp.size = ::operate(op, &p_top->size, &p_current->size);
  
  return(tmp);
  
}

int modificator_coordinates::coord_local_fixed_num_get(void)
{
  return(coord_num);
}

COORDINATE * modificator_coordinates::coord_local_fixed_get(int index)
{
  if(index >= coord_num)
    coord_num = index+1;
  
  if(index < COORD_FIXED_SIZE) {
    return(coord_fixed+index);
  }
  else {  
    if(p_coord == NULL) {
      p_coord = new array<COORDINATE>;      
    }
    return(p_coord->item_get(index-COORD_FIXED_SIZE));
  }
}

void modificator_coordinates::coord_local_fixed_set(RECT2DI pos, int index)
{
  COORDINATE *p_coord = coord_local_fixed_get(index);
  assert(p_coord);
  p_coord->set(pos);
}

/*
  // We generate random coordinates from coordinates with index 0
  if(coord_random && coord_num < 1) {
    COORDINATE tmp;
    return(tmp);
  }
  return(interpolate(coord_fixed_get(0),coord_fixed_get(1), c11, c12, c21, c22));
*/
COORDINATE modificator_coordinates::coord_local_random_get(COORDINATE *p_c1,
                                                           COORDINATE *p_c2)
{
  if(!p_c1 || !p_c2) {
    COORDINATE tmp;
    return(tmp);
  }
  
  float c11 = random_generator::generator_rand();
  float c12 = random_generator::generator_rand();
  float c21 = random_generator::generator_rand();
  float c22 = random_generator::generator_rand();
    
  return(interpolate(p_c1, p_c2, c11, c12, c21, c22));
}

int modificator_coordinates::coord_local_num_get(void)
{
  return(coord_random ? coord_random_num : coord_num);
}

bool modificator_coordinates::coord_local_modificator_active(void)
{
  return(modificator_start != COORD_CURRENT && 
         modificator_size != COORD_CURRENT);
}

COORDINATE modificator_coordinates::coord_local_modificator_mix(COORDINATE last,
                                                                COORDINATE current)
{
  if(!coord_local_modificator_active()) {
    return(current);
  }  
  
  RECT2DI current_rect = current.get();
  RECT2DI last_rect = last.get();
  
  COORDINATE tmp;
  switch(modificator_start) {
    case COORD_CURRENT:
      tmp.start_set(current_rect.start_get());
      break;
    case COORD_LAST_START:
      tmp.start_set(last_rect.start_get());
      break;
    case COORD_LAST_SIZE:
      tmp.start_set(last_rect.size_get());
      break;
    case COORD_LAST_START_SIZE:
      tmp.start_set(last_rect.start_get() + last_rect.size_get());
      break;
  }
    
  switch(modificator_size) {
    case COORD_CURRENT:
      tmp.size_set(current_rect.start_get());
      break;
    case COORD_LAST_START:
      tmp.size_set(last_rect.start_get());
      break;
    case COORD_LAST_SIZE:
      tmp.size_set(last_rect.size_get());
      break;
    case COORD_LAST_START_SIZE:
      tmp.size_set(last_rect.start_get() + last_rect.size_get());
      break;
  }
  
  return(tmp);
}

void modificator_coordinates::coord_final_get_start(void)
{
  coord_current_index = 0;
}

bool modificator_coordinates::coord_final_get_next(COORDINATE *p_coord)
{  
  int coord_num = coord_local_num_get();
  
  // No local coordinates - return top one
  if(!coord_num) {
    if(coord_current_index > 0) {
      return(FALSE);
    }
    
    *p_coord = coord_top;
  } 
  else {
    if(coord_current_index+1 > coord_num) {
      return(FALSE);
    }
    
    COORDINATE coord_current;
    
    if(coord_random) {
      coord_current = coord_local_random_get(coord_local_fixed_get(0),
                                             coord_local_fixed_get(1));
    }
    else {
      coord_current = *(coord_local_fixed_get(coord_current_index));
    }
    
    coord_last = coord_current = coord_local_modificator_mix(coord_last, coord_current);
    *p_coord = coord_operator_apply(coord_operator, &coord_top, &coord_current);
  }
  
  coord_current_index++;
  return(TRUE);
}

bool modificator_coordinates::coord_final_get_next(RECT2DI *p_rect)
{  
  COORDINATE tmp;
  if(coord_final_get_next(&tmp)) {
    *p_rect = tmp.get();
    return(TRUE);
  }
  return(FALSE);
}

// Interpolates two coordinates to a new one
COORDINATE modificator_coordinates::interpolate(COORDINATE *p_c1, COORDINATE *p_c2, 
                                                float c11, float c12,
                                                float c21, float c22)
{ 
  COORDINATE tmp;
  
  tmp.start = ::interpolate(&p_c1->start, &p_c2->start, c11, c12);
  tmp.size = ::interpolate(&p_c1->size, &p_c2->size, c21, c22);
  
  return(tmp);
}

bool modificator_coordinates::load(DATA_FILE_SECTION *p_section)
{
  DATA_FILE_SECTION *p_line = p_section->section_child_get();
  if(!p_line) {    
    return(FALSE);
  }
  
  do {
    if(p_line->is_line()) {
      load_aritmetic_operation(coord_operator, "coordinates_operation");
      load_int(coord_random, "coordinates_random");
      load_int(coord_random_num, "coordinates_random_num");
      load_coord_modificator(modificator_start, "modificator_start");
      load_coord_modificator(modificator_size, "modificator_size");
    }
  } while((p_line = p_line->section_next()));
  
  return(TRUE);
}
