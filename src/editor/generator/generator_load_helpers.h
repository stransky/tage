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

//
// Load helpers
//

#ifndef __GENERATOR_LOAD_HELPERS_H__
#define __GENERATOR_LOAD_HELPERS_H__

bool load_line_vect(char *line, float &v1, float &v2, float &v3, float &v4);
bool load_line_vect(char *line, float &v1, float &v2, float &v3);
bool load_line_vect(char *line, float &v1, float &v2);

bool load_line_vect(char *line, int &v1, int &v2, int &v3, int &v4);
bool load_line_vect(char *line, int &v1, int &v2, int &v3);
bool load_line_vect(char *line, int &v1, int &v2);

#define load_int(section_name, data_name)                       \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    section_name = (p_line->line_value_int_get());              \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#define load_string(section_name, data_name)                    \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    strcpy(section_name,p_line->line_value_string_get());       \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#define load_interval_int(section_name, data_name)              \
{                                                               \
  load_int(section_name, data_name)                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".min");                                        \
    load_int(section_name.min, name)                            \
    section_name.generated = TRUE;                              \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".max");                                        \
    load_int(section_name.max, name)                            \
    section_name.generated = TRUE;                              \
  }                                                             \
}

#define load_float(section_name, data_name)                     \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    section_name = (p_line->line_value_float_get());            \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#define load_interval_float(section_name, data_name)            \
{                                                               \
  load_float(section_name, data_name)                           \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".min");                                        \
    load_float(section_name.min, name)                          \
    section_name.generated = TRUE;                              \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".max");                                        \
    load_float(section_name.max, name)                          \
    section_name.generated = TRUE;                              \
  }                                                             \
}

#define load_angle(section_name, data_name)                     \
{                                                               \
  {                                                             \
    if(p_line->match_name(data_name)) {                         \
      section_name = (p_line->line_value_angle_get());          \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}

#define load_interval_angle(section_name, data_name)            \
{                                                               \
  load_angle(section_name, data_name)                           \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".min");                                        \
    load_angle(section_name.min, name)                          \
    section_name.generated = TRUE;                              \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".max");                                        \
    load_angle(section_name.max, name)                          \
    section_name.generated = TRUE;                              \
  }                                                             \
}

#define load_vect3df(section_name, data_name)                   \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      section_name.x, section_name.y,           \
                      section_name.z))                          \
    {                                                           \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_float_get();          \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_float_get();          \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".z");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.z = p_line->line_value_float_get();          \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_interval_vect3df(section_name, data_name)          \
{                                                               \
  load_vect3df((section_name).current, data_name)               \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".min");                                        \
    load_vect3df((section_name).min, name)                      \
    section_name.generated = TRUE;                              \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".max");                                        \
    load_vect3df((section_name).max, name)                      \
    section_name.generated = TRUE;                              \
  }                                                             \
}

#define load_vect3df_active(section_name, data_name)            \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                        section_name.x, section_name.y,         \
                        section_name.z))                        \
    {                                                           \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_float_get();          \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_float_get();          \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".z");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.z = p_line->line_value_float_get();          \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_vect2di(section_name, data_name)                   \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                        section_name.x, section_name.y))        \
    {                                                           \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_vect2di_active(section_name, data_name)            \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                        section_name.x, section_name.y))        \
    {                                                           \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_vect3di(section_name, data_name)                   \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                        section_name.x, section_name.y,         \
                        section_name.z))                        \
    {                                                           \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".z");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.z = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_interval_vect3di(section_name, data_name)          \
{                                                               \
  load_vect3di(section_name.current, data_name)                 \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".min");                                        \
    load_vect3di(section_name.min, name)                        \
    section_name.generated = TRUE;                              \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".max");                                        \
    load_vect3di(section_name.max, name)                        \
    section_name.generated = TRUE;                              \
  }                                                             \
}

#define load_vect3di_active(section_name, data_name)            \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                        section_name.x, section_name.y,         \
                        section_name.z))                        \
    {                                                           \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".z");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.z = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rect2di(section_name, data_name)                   \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      section_name.x, section_name.y,           \
                      section_name.dx, section_name.dy))        \
    {                                                           \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_int_get();            \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".dx");                                         \
    if(p_line->match_name(name)) {                              \
      section_name.dx = p_line->line_value_int_get();           \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".dy");                                         \
    if(p_line->match_name(name)) {                              \
      section_name.dy = p_line->line_value_int_get();           \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rect2di_active(section_name, data_name)            \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      section_name.x, section_name.y,           \
                      section_name.dx, section_name.dy))        \
    {                                                           \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".x");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.x = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".y");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.y = p_line->line_value_int_get();            \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".dx");                                         \
    if(p_line->match_name(name)) {                              \
      section_name.dx = p_line->line_value_int_get();           \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".dy");                                         \
    if(p_line->match_name(name)) {                              \
      section_name.dy = p_line->line_value_int_get();           \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_polar2di_active(section_name, data_name)           \
{                                                               \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.rotation = p_line->line_value_angle_get();   \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = data_name;                       \
    strcat(name,".l");                                          \
    if(p_line->match_name(name)) {                              \
      section_name.length = p_line->line_value_int_get();       \
      section_name##_active = TRUE;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rgbf(color_section_name, color_data_name)          \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue;                                         \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue))                          \
    {                                                           \
      color_section_name.r = COLOR_TO_FLOAT(red);               \
      color_section_name.g = COLOR_TO_FLOAT(green);             \
      color_section_name.b = COLOR_TO_FLOAT(blue);              \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get()); \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = COLOR_TO_FLOAT(p_line->line_value_int_get()); \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = COLOR_TO_FLOAT(p_line->line_value_int_get()); \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = COLOR_TO_FLOAT(p_line->line_value_int_get()); \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rgbf_active(color_section_name, color_data_name)   \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue;                                         \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue))                          \
    {                                                           \
      color_section_name.r = COLOR_TO_FLOAT(red);               \
      color_section_name.g = COLOR_TO_FLOAT(green);             \
      color_section_name.b = COLOR_TO_FLOAT(blue);              \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get());    \
    color_section_name##_active = TRUE;                         \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rgbaf(color_section_name, color_data_name)         \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue,alpha;                                   \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue,alpha))                    \
    {                                                           \
      color_section_name.r = COLOR_TO_FLOAT(red);               \
      color_section_name.g = COLOR_TO_FLOAT(green);             \
      color_section_name.b = COLOR_TO_FLOAT(blue);              \
      color_section_name.a = COLOR_TO_FLOAT(alpha);             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get()); \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = COLOR_TO_FLOAT(p_line->line_value_int_get()); \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".a");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.a = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               

#define load_rgbaf_active(color_section_name, color_data_name)  \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue,alpha;                                   \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue,alpha))                    \
    {                                                           \
      color_section_name.r = COLOR_TO_FLOAT(red);               \
      color_section_name.g = COLOR_TO_FLOAT(green);             \
      color_section_name.b = COLOR_TO_FLOAT(blue);              \
      color_section_name.a = COLOR_TO_FLOAT(alpha);             \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get()); \
    color_section_name##_active = TRUE;                         \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = COLOR_TO_FLOAT(p_line->line_value_int_get());         \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".a");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.a = COLOR_TO_FLOAT(p_line->line_value_int_get());  \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               

#define load_rgbb(color_section_name, color_data_name)          \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue;                                         \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue))                          \
    {                                                           \
      color_section_name.r = red;                               \
      color_section_name.g = green;                             \
      color_section_name.b = blue;                              \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get());    \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rgbb_active(color_section_name, color_data_name)   \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue;                                         \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue))                          \
    {                                                           \
      color_section_name.r = red;                               \
      color_section_name.g = green;                             \
      color_section_name.b = blue;                              \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get()); \
    color_section_name##_active = TRUE;                         \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               \

#define load_rgbab(color_section_name, color_data_name)         \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue,alpha;                                   \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue,alpha))                    \
    {                                                           \
      color_section_name.r = red;                               \
      color_section_name.g = green;                             \
      color_section_name.b = blue;                              \
      color_section_name.a = alpha;                             \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get());    \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".a");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.a = p_line->line_value_int_get();      \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}                                                               

#define load_rgbab_active(color_section_name, color_data_name)  \
{                                                               \
  if(p_line->match_name(color_data_name)) {                     \
    int red,green,blue,alpha;                                   \
    if(load_line_vect(p_line->line_value_string_get(),          \
                      red,green,blue,alpha))                    \
    {                                                           \
      color_section_name.r = red;                               \
      color_section_name.g = green;                             \
      color_section_name.b = blue;                              \
      color_section_name.a = alpha;                             \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  if(p_line->match_name(color_data_name)) {                     \
    color_section_name = (tcolor)rb_switch(p_line->line_value_uint_hexa_get());    \
    color_section_name##_active = TRUE;                         \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".r");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.r = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".g");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.g = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".b");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.b = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
  {                                                             \
    char name[MAX_TOKEN_LEN] = color_data_name;                 \
    strcat(name,".a");                                          \
    if(p_line->match_name(name)) {                              \
      color_section_name.a = p_line->line_value_int_get();      \
      color_section_name##_active = TRUE;                       \
      p_line->line_mark_loaded();                               \
      continue;                                                 \
    }                                                           \
  }                                                             \
}

#define load_aritmetic_operation(section_name, data_name)       \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    section_name = aritmetic_operation_names_translate(p_line->line_value_string_get()); \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#define load_rand_generator_type(section_name, data_name)       \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    section_name = random_generator_type_translate(p_line->line_value_string_get()); \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#define load_mesh_type(section_name, data_name)                 \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    section_name = mesh_type_translate(p_line->line_value_string_get()); \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#define load_coord_modificator(section_name, data_name)         \
{                                                               \
  if(p_line->match_name(data_name)) {                           \
    section_name = coord_modificator_translate(p_line->line_value_string_get()); \
    p_line->line_mark_loaded();                                 \
    continue;                                                   \
  }                                                             \
}

#endif // __GENERATOR_LOAD_HELPERS_H__
