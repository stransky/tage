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

#ifndef __GENERATOR_MESH_COORD_H__
#define __GENERATOR_MESH_COORD_H__

/*
 * Modificator modificators
 */
typedef struct simple_coordinate {

  // Type of the coordinate - cartesian or polar
  bool polar;
  
  // Coordinates start  
  VECT2DI   v;
  POLAR2DI  p;

  VECT2DI cartesian_get(void)
  {
    if(polar) {
      VECT2DI tmp(p);
      return(tmp);
    }
    else {
      return(v);
    }
  }
  
  void cartesian_set(VECT2DI vect)
  {
    v = vect;
    polar = FALSE;
  }
  
  POLAR2DI polar_get(void)
  {
    if(polar) {
      return(p);
    }
    else {
      POLAR2DI tmp(v);
      return(tmp);
    }    
  }
  
  void polar_set(POLAR2DI pl)
  {
    p = pl;
    polar = TRUE;
  }
  
  simple_coordinate(void)
    : v(0), p(0)
  {
    polar = FALSE;
  }
  
  simple_coordinate(VECT2DI src)
    : v(src), p(0)
  {
    polar = FALSE;
  }
  
  simple_coordinate(POLAR2DI src)
    : v(0), p(src)
  {
    polar = TRUE;
  }
  
} SIMPLE_COORDINATE;

// Interpolates two simple coordinates
// i1 is amount of s1 from <0,1>
SIMPLE_COORDINATE interpolate(SIMPLE_COORDINATE *p_s1,
                              SIMPLE_COORDINATE *p_s2,
                              float i1);
// Operate operator
SIMPLE_COORDINATE operate(ARITMETIC_OPERATOR op,
                          SIMPLE_COORDINATE *p_s1,
                          SIMPLE_COORDINATE *p_s2);

typedef struct coordinate {
  
  // Coordinates start
  SIMPLE_COORDINATE   start;
  SIMPLE_COORDINATE   size;
  
public:
  
  void    set(RECT2DI coord);

  void    start_set(VECT2DI start);
  void    start_set(POLAR2DI start);

  void    size_set(VECT2DI size);
  void    size_set(POLAR2DI size);

public:
  
  RECT2DI get(void);

public:
  
  void print(void);

} COORDINATE;

/*
 * Coordinates for any color (texture) modificator (size of the pattern)
 */
#define COORD_FIXED_SIZE  2

/*
 * Coord modificators - can be used former ones
 */
typedef enum {

  COORD_CURRENT         = 0,  // Use current coordinate
  COORD_LAST_START      = 1,  // Use last start
  COORD_LAST_SIZE       = 2,  // Use last size
  COORD_LAST_START_SIZE = 3,  // Use last start+size

} COORD_MODIFICATOR;

/*
 * Coordinates
 * Current modificator coordinate is composed from top coordinate
 * from superior modificator and the local one.
 *
 * If local coordinate is not set, the top one is used
 */
typedef class modificator_coordinates {
  
  // "Top" coordinates - set by the 
  // superior modificator  
  COORDINATE          coord_top;
  
public:  
  
  void coord_top_set(RECT2DI pos)
  {
    coord_top.set(pos);
  }
  
 /*
  * Coordinate operation (top (op) current)
  */
  ARITMETIC_OPERATOR  coord_operator;
  
public:

  void coord_operator_set(ARITMETIC_OPERATOR op)
  {
    coord_operator = op;
  }

private:  
  
  COORDINATE coord_operator_apply(ARITMETIC_OPERATOR op, COORDINATE *p_top,
                                  COORDINATE *p_current);
  
 /*
  * Local coordinates:
  * raw (fixed) coordinate interface
  */
private:  
  
  int                 coord_num;
  COORDINATE          coord_fixed[COORD_FIXED_SIZE];
  array<COORDINATE>  *p_coord;

public:
  
  int                 coord_local_fixed_num_get(void);
  COORDINATE *        coord_local_fixed_get(int index);
  void                coord_local_fixed_set(RECT2DI pos, int index = 0);

 /*
  * Enhanced coordinates interface
  */
private:
  
  int                 coord_current_index;
  COORDINATE          coord_last;

public:
  // Random coordinates configuration
  bool                coord_random;
  int                 coord_random_num;

public:
  
  // Coordinates modificators
  COORD_MODIFICATOR   modificator_start;
  COORD_MODIFICATOR   modificator_size;

private:

  int                 coord_local_num_get(void);
  COORDINATE          coord_local_random_get(COORDINATE *p_c1, COORDINATE *p_c2);

private:

  bool                coord_local_modificator_active(void);
  COORDINATE          coord_local_modificator_mix(COORDINATE last, COORDINATE current);

private:
  
  // Interpolates two coordinates to a new one
  COORDINATE          interpolate(COORDINATE *p_c1, COORDINATE *p_c2, 
                                  float c11, float c12, float c21, float c22);
public:

  // Final coordinates (composed from the current and top)
  // are obtained by this interface

  void                coord_final_get_start(void);
  bool                coord_final_get_next(COORDINATE *p_coord);
  bool                coord_final_get_next(RECT2DI *p_rect);

public:

  bool  load(DATA_FILE_SECTION *p_section);

public:

  modificator_coordinates(void);
  ~modificator_coordinates(void);
  
} MODIFICATOR_COORDINATES;

#endif // __GENERATOR_MESH_COORD_H__
