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
 
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

/*
 * Variable object generators
 */
#include "params.h"

#include "generator_reference.h"
 
#include "generator_random.h"
#include "interval.h"
#include "data_file.h"
#include "generator_coordinates.h"
#include "generator_load_helpers.h"
#include "generator_mesh_helper.h"

#include "generator_modificator_mask.h"
#include "generator_target.h"
#include "generator_target_texture.h"
#include "generator_target_geometry.h"
#include "generator_target_modificator.h"
#include "generator_modificator_params.h"
#include "generator_modificator.h"

#include "generator_mesh.h"


/*
 * Generator config
 */
typedef struct generator_config
{
  int   max_texture_size;
  char  output_test_file[MAX_FILENAME];  
  
  void output_file_set(char *p_file)
  {
    strncpy(output_test_file, p_file, MAX_FILENAME);
  }
  
public:
  
  generator_config(void);
  
} GENERATOR_CONFIG;

/*
 * Simple generator launcher
 */
typedef struct generator_launcher
{
  MODIFICATOR_TYPE  type;
  char              name[MAX_NAME];
  
} GENERATOR_LAUNCHER;

/*
 * A complex generator, it's part of whole editor
 */
typedef class generator
  : public editor_reference
{  
  /*
   * Generator configuration
   */  
  GENERATOR_CONFIG      config;
  
public:
  
  GENERATOR_CONFIG * config_get(void)
  {
    return(&config);
  }

private:

  /*
   * Mesh generators
   */
  GENERATOR_MESH_LIST   mesh_generators;
 
public:

  GENERATOR_MESH * mesh_generator_create(const char *p_name = NULL);
  void             mesh_generator_remove(GENERATOR_MESH *p_gen);
  GENERATOR_MESH * mesh_generator_find(const char *p_name);
  GENERATOR_MESH * mesh_generator_get_first(void);
  GENERATOR_MESH * mesh_generator_load(const char *p_file, bool safe = TRUE);

  /*
   * Generator modificators
   */
private:

  MODIFICATOR_LIST  modificators;

private:

public:
  
  MODIFICATOR * modificator_create(MODIFICATOR_TYPE type, const char *p_name = NULL);
  void          modificator_remove(MODIFICATOR *p_mod);
  MODIFICATOR * modificator_get_first(void);
  MODIFICATOR * modificator_find(const char *p_name);

  /*
   * Loading routines
   */
private:
    
  void load_generator_launcher(DATA_FILE_SECTION *p_section);
  void load_section(DATA_FILE_SECTION *p_section);
  
public:
  
  bool load(const char *p_file, bool clear_content = TRUE, bool safe = TRUE);

  /*
   * Generator launcher
   */
private:

  #define MAX_GENERATOR_LAUNCHERS 100

  GENERATOR_LAUNCHER launcher[MAX_GENERATOR_LAUNCHERS];
  int                launcher_last;
  
  // Target cache of the running generator
  MODIFICATOR_TARGET_CACHE *p_current_target_cache;

public:
  
  MODIFICATOR_TARGET_CACHE * current_target_cache_get(void)
  {
    return(p_current_target_cache);
  }

public:
  
  void launcher_add(GENERATOR_LAUNCHER *p_new);
  bool launcher_run(GENERATOR_LAUNCHER *p_launcher, SCENE *p_scene);

  void run(SCENE *p_scene);
  
public:
  
  void clear(void) {};

public:
  
  generator(EDITOR *p_editor);
  ~generator(void);

} GENERATOR;

// Section check
void load_section_check(DATA_FILE_SECTION *p_section);

// Error handler
void generator_loader_error(char *p_file, int line, const char *p_text,...);

#endif // __GENERATOR_H__
