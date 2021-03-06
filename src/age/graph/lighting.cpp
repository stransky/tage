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

#include "age.h"

phong_lighting_intensity::phong_lighting_intensity(void)
{
  light_angle_min = 0.0f;
  light_angle_scale = 1.0f;
  
  light_bidirectional = FALSE;
  
  light_range_active = FALSE;
  light_range = 0.0f;

  light_fade_active = FALSE;
  light_fade_constant = 0.0f;
  light_fade_linear = 0.0f;
  light_fade_cubic = 0.0f;
}

phong_lighting_color::phong_lighting_color(void)
{
  phong_light_color_ambient.set(0,0,0,0);
  
  phong_light_diffuse = TRUE;
  phong_light_color_diffuse.set(1,1,1,1);
  
  phong_light_specular = FALSE;
  phong_light_color_specular.set(1,1,1,1);
  phong_light_color_specular_shine = 1.0f;
}

phong_lighting::phong_lighting(void)
{
  
}
