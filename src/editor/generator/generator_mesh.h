/*
 *        .þÛÛþ þ    þ þÛÛþ.     þ    þ þÛÛÛþ.  þÛÛÛþ .þÛÛþ. þ    þ
 *       .þ   Û Ûþ.  Û Û   þ.    Û    Û Û    þ  Û.    Û.   Û Ûþ.  Û
 *       Û    Û Û Û  Û Û    Û    Û   þ. Û.   Û  Û     Û    Û Û Û  Û
 *     .þþÛÛÛÛþ Û  Û Û þÛÛÛÛþþ.  þþÛÛ.  þþÛÛþ.  þÛ    Û    Û Û  Û Û
 *    .Û      Û Û  .þÛ Û      Û. Û   Û  Û    Û  Û.    þ.   Û Û  .þÛ
 *    þ.      þ þ    þ þ      .þ þ   .þ þ    .þ þÛÛÛþ .þÛÛþ. þ    þ
 *
 * Berusky (C) AnakreoN
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
 * Generator loader
 */

#ifndef __GENERATOR_LOADER_H__
#define __GENERATOR_LOADER_H__

/*
  Some prototypes
*/
class generator;
typedef class generator GENERATOR;
  
class generator_mesh;
typedef class generator_mesh GENERATOR_MESH;

class texture_light_pattern;
typedef class texture_light_pattern TEXTURE_LIGHT_PATTERN;


/*
 * Generator configuration
 */
typedef struct generator_mesh_modificator_target_config {
  
  MODIFICATOR_TARGET_TYPE   target_type;
  char                      target_name[MAX_NAME];
  int                       repeat;
  MASK_REFERENCE            mask;
  
  generator_mesh_modificator_target_config(void)
  {    
    target_type = MODIFICATOR_TARGET_NONE;
    target_name[0] = '\0';
    repeat = 1;
  }
  
} GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG;


typedef struct generator_mesh_modificator_config {

  char                                      modificator_name[MAX_NAME];
  GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG  target_config[MAX_MODIFICATOR_TARGETS];
  
public:
  
  GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG * target_config_ref(int index)
  {
    assert(index < MAX_MODIFICATOR_TARGETS);
    return(target_config+index);
  }
  
  GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG * target_config_get(int index)
  {
    if(index < MAX_MODIFICATOR_TARGETS && 
       target_config[index].target_type != MODIFICATOR_TARGET_NONE) {
      return(target_config+index);
    }
    return(NULL);
  }
  
  GENERATOR_MESH_MODIFICATOR_TARGET_CONFIG * target_config_free_get(void)
  {
    int i;
    for(i = 0; i < MAX_MODIFICATOR_TARGETS; i++) {
      if(target_config[i].target_type == MODIFICATOR_TARGET_NONE)
        return(target_config+i);
    }
    return(NULL);
  }

  generator_mesh_modificator_config(void)
  {
    modificator_name[0] = '\0';
  }
  
} GENERATOR_MESH_MODIFICATOR_CONFIG;

#define MAX_MODIFICATORS  20

typedef class generator_mesh_config {
  
public:  
    
  GENERATOR_MESH_MODIFICATOR_CONFIG  mconfig[MAX_MODIFICATORS];
  
public:
  
  GENERATOR_MESH_MODIFICATOR_CONFIG * modificator_ref(int index)
  {
    assert(index < MAX_MODIFICATORS);
    return(mconfig+index);
  }

  GENERATOR_MESH_MODIFICATOR_CONFIG * modificator_get(int index)
  {
    return(index < MAX_MODIFICATORS && 
           mconfig[index].modificator_name[0] ? 
           mconfig+index : NULL);
  }

public:

  generator_mesh_config(void)
  {
  }
  
} GENERATOR_MESH_CONFIG;

/*
 * A single mesh generator, can be loaded from disk or created run-time
 * it generates single mesh with texture
 */
typedef class generator_mesh :
  public generator_reference,
  public object_list
{
  /*
   * Complete description of generated mesh
   */
public:
  
  GEOMETRY_DESCRIPTION            geometry_desc;
  MATERIAL_DESCRIPTION            material_desc;
  TEXTURE_DESCRIPTION             texture_desc;
  
public:

  GENERATOR_MESH_CONFIG           config;
  MODIFICATOR_TARGET_CACHE        target_cache;

private:
  
  void run(void);

public:
    
  // Create whole mesh with texture and material and attach it to scene
  MESH_HELPER   * mesh_create(SCENE *p_scene);

  // Remove all generated meshes / textures / material from scene
  bool            mesh_remove(SCENE *p_scene);
    
private:
  
  void load_section_mesh_params(DATA_FILE_SECTION *p_section);
  void load_section_material_params(DATA_FILE_SECTION *p_section);
  void load_section_texture_params(DATA_FILE_SECTION *p_section);  
  void load_section_aux_params(DATA_FILE_SECTION *p_section);
  void load_section_generator_config(DATA_FILE_SECTION *p_section);

public:

  void load_section(DATA_FILE_SECTION *p_section);

public:
  
  void print(void);

private:
  
  void create(void);
  void clear(void);
  void destroy(void);
  
public:

  generator_mesh(GENERATOR *p_generator, const char *p_name = NULL);
  ~generator_mesh(void);

} GENERATOR_MESH;

/*
 * List of mesh generators
 */
typedef class generator_mesh_list :
  public object_list_head
{
  
  GENERATOR       *p_generator;
  GENERATOR_MESH  *p_last_created;
  
public:
  
  GENERATOR_MESH * create(const char *p_name)
  {
    GENERATOR_MESH *p_new = new GENERATOR_MESH(p_generator, p_name);
    object_list_insert(p_new);
    p_last_created = p_new;
    return(p_new);
  }
  
  void destroy(GENERATOR_MESH *p_gen)
  {
    if(p_gen == p_last_created)
      p_last_created = NULL;
    
    object_list_remove(p_gen);
    delete (GENERATOR_MESH *)p_gen;
  }
  
public:
  
  GENERATOR_MESH * get_first(void)
  {
    return((GENERATOR_MESH *)object_list_get_first());
  }
  
  GENERATOR_MESH * get_last_created(void)
  {
    return(p_last_created);
  }
    
public:

  void print(void);

public:
  
  generator_mesh_list(GENERATOR *p_generator, SCENE *p_scene)
    : object_list_head(p_scene)
  {
    this->p_generator = p_generator;
    p_last_created = NULL;
  }
    
  virtual ~generator_mesh_list(void)
  {
    // TODO - remove all generators?
    //assert(0);
  }
  
} GENERATOR_MESH_LIST;

#endif // __GENERATOR_LOADER_H__
