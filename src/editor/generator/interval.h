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
 * Interval types
 */

#ifndef __INTERVAL_H__
#define __INTERVAL_H__

/* 
 * Basic interpolations
 */
inline int interpolate(int min, int max, float t)
{
  return(roundf(interpolate((float)min,(float)max,t)));
}

/* 
 * Interval template
 */
template<class T> struct interval {
  
  // Values for generator
  T min;
  T max;
  
  // Recently generated value
  T current;
  
  // Is the interval static or dynamic? (generated)
  bool generated;
  
public:  

  bool is_generated(void)
  {
    return(generated);
  }
   
  T generate(void)
  {
    float random = random_generator::generator_rand();
    return(::interpolate(min,max,random));
  }
 
public:
  
  void set(T src = 0)
  {
    current = max = min = src;
    generated = FALSE;
  }
  
  void set(T src_min, T src_max)
  {
    max = src_max;
    min = src_min;
    generated = TRUE;
  }
  
  void operator=(T src)
  {
    set(src);
  }
  
  operator T(void)
  {
    if(generated) {
      current = generate();
    }
    return(current);
  }
  
public:

  interval(void)
  {
    generated = FALSE;
  }
};

#endif // __INTERVAL_H__
