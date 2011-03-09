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

/* Material class
*/

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

class mesh_material;
typedef class mesh_material MESH_MATERIAL;

/*
 * System / Runtime flags
 */

/*
 * Material flags
 */
#define MATERIAL_TRANSPARENT  (1<<0)

#define MATERIAL_ZMASK        (1<<1)
#define MATERIAL_ZTEST        (1<<2)

#define MATERIAL_CULL         (1<<3)

#define MATERIAL_DIFFUSE      (1<<4)
#define MATERIAL_SPECULAR     (1<<5)

/*
 * Texture configuration for each texture layer
 */
typedef class material_text_config {
  
public:  
  
  bool  active;

public:

  int   text_index;
  int   text_operator;
  int   text_coordinates;

public:

  material_text_config(void)
  {
    active = FALSE;
    
    text_index = 0;
    text_operator = 0;
    text_coordinates = 0;    
  }

} MATERIAL_TEXT_CONFIG;

/*
 * Mesh material
 */
typedef class mesh_material:  
  public flag_interface,
  public object_list
{
  
  /* 
   * Material aplha blending
   */
private:
  
  MATERIAL_ALPHA    alpha;

private:

  void  alpha_set(void);

public:
  
  void  alpha_func_set(ALPHA_TYPE function)
  {
    alpha.alpha_func_set(function);
  }  
  ALPHA_TYPE  alpha_func_get(void)
  {
    return(alpha.alpha_func_get());
  }

  /* 
   * Texture set & configuration
   */
private:
  
  MATERIAL_TEXT_COORDINATES     text_coord[MAT_TEXTURES];
  MATERIAL_TEXT_CONFIG          text_config[MAT_TEXTURES];
  MATERIAL_TEXT                *p_text[MAT_TEXTURES];

public:

  MATERIAL_TEXT_COORDINATES * text_coordinates_get(int texture)
  {
    assert(texture >= 0 && texture <= MAT_TEXTURES);
    return(text_coord+texture);
  }
  MATERIAL_TEXT_CONFIG * text_config_get(int texture)
  {
    assert(texture >= 0 && texture <= MAT_TEXTURES);
    return(text_config+texture);
  }

  void texture_clear(int texture_num)
  {
    if(p_text[texture_num]) {
      p_text[texture_num]->reference_dec();
      p_text[texture_num] = NULL;
    }
  }
  
  void texture_set(int texture_num, MATERIAL_TEXT *p_txt)
  {
    texture_clear(texture_num);
    
    if(p_txt) {
      p_text[texture_num] = p_txt;
      p_txt->reference_add();
    }
  }  
  
  MATERIAL_TEXT * texture_get(int texture_num = 0)
  {
    return(p_text[texture_num]);
  }
   
  void texture_enable(int config_num, int text_index, int text_operator, int text_coordinates)
  {
    MATERIAL_TEXT_CONFIG *p_config = text_config+config_num;
    p_config->active = TRUE;
    p_config->text_index = text_index;
    p_config->text_operator = text_operator;
    p_config->text_coordinates = text_coordinates;
  }
  
  void texture_disable(int config_num)
  {
    MATERIAL_TEXT_CONFIG *p_config = text_config+config_num;
    p_config->active = FALSE;
  }

private:

  void             texture_set(void);
  int              texture_on(int unit, int config, int reserved = 0);
  static void      texture_off(int unit);
  static void      texture_off(void);

private:

  /* Surface color parameters
  */
  float            shine;

public:
  
  float shine_get(void)
  {
    return(shine);
  }
  void shine_set(float mat_shine)
  {
    shine = mat_shine;
  }

  /* 
   * Material color parameters
   */
private:

  RGBAF            ambient;     // ambient
  RGBAF            diff;        // diffuse color
  RGBF             spec;        // specular color

public:
  
  RGBAF * ambient_get(void)
  {
    return(&ambient);
  }
  void ambient_set(RGBAF *p_ambient)
  {
    ambient = *p_ambient;
  }

  RGBAF * diffuse_get(void)
  {
    return(&diff);
  }
  void diffuse_set(RGBAF *p_diff)
  {
    diff = *p_diff;    
  }
  
  RGBF * specular_get(void)
  {
    return(&spec);
  }
  void specular_set(RGBF *p_spec)
  {
    spec = *p_spec;
  }

public:  
  
  RGBAF            factor;      // factor

public:
  
  /*
   * Material high level configuration
   */
  void transparent_set(bool transparent)
  {
    if(transparent) {
      flag_set(MATERIAL_TRANSPARENT);
      flag_clear(MATERIAL_ZMASK);
      alpha_func_set(ALPHA_TRANSPARENT);
    }
    else {
      flag_clear(MATERIAL_TRANSPARENT);
      flag_set(MATERIAL_ZMASK);
      alpha_func_set(ALPHA_OFF);      
    }
  }
  bool transparent_get(void)
  {
    return(flag_get(MATERIAL_TRANSPARENT));
  }

  void double_side_set(bool double_side)
  {
    flag_set(MATERIAL_CULL, !double_side);
  }
  bool double_side_get(void)
  {
    return(flag_get(MATERIAL_CULL));
  }

public:  
  
  /* Last used material
  */
  static class mesh_material *p_mat_last;
    
  /* Global (top scene) material
  */
  static class mesh_material *p_mat_scene;
  
public:

  void clear(void);

public:

  int         set(bool forced = FALSE);

  /* Predefined materials
  */
public:
  
  static void set_default(bool forced = FALSE);
  static void set_selection(bool forced = FALSE);

public:
  
  void        print(void);

public:
  
  MESH_MATERIAL * material_next(void)
  {
    return((MESH_MATERIAL *)object_list_next());
  }

  void material_remove(void);
  
public:
  
  mesh_material(SCENE *p_scene);
  ~mesh_material(void);

private:

  class mesh_material operator=(class mesh_material &src)
  {
    assert(0);
  }
  
  mesh_material(class mesh_material &src)
    : object_list(NULL)
  {
    assert(0);
  }
  
} MESH_MATERIAL;

// List of all materials used in the scene
typedef class material_list :  
  public object_list_head
{
  
public:
  
  MESH_MATERIAL * create(void)
  {
    MESH_MATERIAL *p_new = new MESH_MATERIAL(scene_get());
    object_list_insert(p_new);
    return(p_new);
  }
  
  void destroy(MESH_MATERIAL *p_mat)
  {
    object_list_remove(p_mat);
    delete (MESH_MATERIAL *)p_mat;
  }  
  
public:
  
  MESH_MATERIAL * get_first(void)
  {
    return((MESH_MATERIAL *)object_list_get_first());
  }  
    
public:

  void print(void);

public:
  
  material_list(class scene *p_scene) 
    : object_list_head(p_scene)
  {
  }
  
  // TODO - remove all materials?
  virtual ~material_list(void) {};
  
} MATERIAL_LIST;

#endif // __MATERIAL_H__
