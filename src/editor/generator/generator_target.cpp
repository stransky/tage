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
 * Modificators targets
 */
const char modificator_target_type[][MAX_NAME] = 
{
  "NONE",
  "TEXTURE",
  "GEOMETRY",
  "GENERATOR_MESH",
  "AUX",
  "MODIFICATOR",
};

MODIFICATOR_TARGET_TYPE modificator_target_translate(const char *p_type)
{
  int i;
  for(i = 0; 
      i < (int)(sizeof(modificator_target_type)/sizeof(modificator_target_type[0])); 
      i++) 
  {
    if(!strncasecmp(p_type,modificator_target_type[i],MAX_NAME))
      return((MODIFICATOR_TARGET_TYPE)i);
  }
  return(MODIFICATOR_TARGET_NONE);
}

const char * modificator_target_translate(MODIFICATOR_TARGET_TYPE type)
{
  return(modificator_target_type[type]);
}

MODIFICATOR_TARGET * modificator_target_create(MODIFICATOR_TARGET_TYPE type,
                                               char *p_name,
                                               MESH_HELPER *p_helper,
                                               GENERATOR_MESH *p_gen)                                               
{
  MODIFICATOR_TARGET *p_tmp = NULL;
  
  switch(type) {
    case MODIFICATOR_TARGET_TEXTURE_TYPE:
      p_tmp = new MODIFICATOR_TARGET_TEXTURE(p_gen);
      break;
    case MODIFICATOR_TARGET_GEOMETRY_TYPE:
      p_tmp = new MODIFICATOR_TARGET_GEOMETRY(p_gen);
      ((MODIFICATOR_TARGET_GEOMETRY*)p_tmp)->create(p_helper);
      break;
    case MODIFICATOR_TARGET_GENERATOR_TYPE:
      break;
    case MODIFICATOR_TARGET_AUX_TYPE:
      p_tmp = new MODIFICATOR_TARGET_AUX(p_gen,MODIFICATOR_TARGET_AUX_TYPE);
      break;
    case MODIFICATOR_TARGET_MODIFICATOR_TYPE:
      {
        if(!p_name) {          
          pperror(TRUE, "MODIFICATOR target type: missing target modificator name!");
        }
        
        GENERATOR *p_generator = p_gen->generator_get();
        MODIFICATOR *p_mod = p_generator->modificator_find(p_name);
        if(!p_mod) {
          pperror(TRUE, "MODIFICATOR target type: Unable to find modificator '%s'!", p_name);
        }
          
        MODIFICATOR_TYPE type = p_mod->modificator_type_get();
        switch(type) {
          case MODIFICATOR_HEIGHTMAP_TYPE:
            p_tmp = new MODIFICATOR_TARGET_HEIGHTMAP(p_gen);
            break;
          case MODIFICATOR_MASK_TYPE:
            p_tmp = new MODIFICATOR_TARGET_MASK(p_gen);
            break;
          default:
            break;
        }
        break;
      }
    default:
      break;
  }
  
  if(p_tmp && p_name)
    p_tmp->name_set(p_name);    
  
  return(p_tmp);
}


modificator_target::modificator_target(GENERATOR_MESH *p_gen, MODIFICATOR_TARGET_TYPE target_type)
  : object_list(NULL),
    generator_reference(p_gen->generator_get()),
    type(target_type), 
    p_mask(NULL),
    scale(0,0),
    scale_active(FALSE)
{
}

modificator_target::~modificator_target(void)
{
}


/*
 * Mesh/material/texture creators
 */
void modificator_target_cache::mesh_geometry_create_landscape(GAME_MESH *p_mesh, MESH_HELPER *p_helper)
{
  assert(p_mesh != NULL);
  
  VECT3DF mesh_start = p_helper->mesh_start;
  VECT3DF mesh_diff = p_helper->mesh_diff;
  VECT3DI mesh_size = p_helper->mesh_size;
  
  int dx = mesh_size.x;
  int dz = mesh_size.z;
  
  if(dx < 2 || dz < 2)
    return;
  
  MESH_GEOMETRY *p_geom = p_mesh->lock(LOCK_READ_WRITE);
  
  p_geom->vertex_new(dx*dz);
  VECT   *p_vects = p_geom->position_get();
  VECTUV *p_maps = p_geom->uv_get();
  
  int x,z;
  
  for(z = 0; z < dz; z++) {
    for(x = 0; x < dx; x++) {
      int vertex_index = VERTEX_INDEX_GET(x,z,dx,dz);
      VECT *p_vect = p_vects+vertex_index;
      p_vect->set(mesh_start.x+mesh_diff.x*x,
                  mesh_start.y,
                  mesh_start.z+mesh_diff.z*z);
      
      // We want to map it from <0,1>
      float tu = (float)x/(float)(dx-1);
      float tv = (float)z/(float)(dz-1);
      VECTUV *p_map = p_maps+vertex_index;
      p_map->set(tu, tv);
    }
  }
  
  // Face array is short of 1
  tpos fdx = dx - 1;
  tpos fdz = dz - 1;
  
  int facenum = fdx*fdz*2;
  p_geom->face_new(facenum*3);

  for(z = 0; z < fdz; z++) {
    for(x = 0; x < fdx; x++) {
      int index = FACE_INDEX_GET(x,z,fdx,fdz);
      
      int i1 = VERTEX_INDEX_GET(x,  z,  dx,dz);
      int i2 = VERTEX_INDEX_GET(x+1,z,  dx,dz);
      int i3 = VERTEX_INDEX_GET(x+1,z+1,dx,dz);
      
      p_geom->face_set(index, i1, i2, i3);
      
      i1 = VERTEX_INDEX_GET(x,  z,  dx,dz);
      i2 = VERTEX_INDEX_GET(x+1,z+1,dx,dz);
      i3 = VERTEX_INDEX_GET(x,  z+1,dx,dz);
      
      p_geom->face_set(index+1, i1, i2, i3);
    }
  }

  p_mesh->unlock();
}

void modificator_target_cache::mesh_geometry_copy_landscape(MESH_HELPER *p_src,
                                                            MESH_HELPER *p_dest,
                                                            tpos sx, tpos sz)
{
  assert(p_src->mesh_get() != NULL);
  assert(p_dest->mesh_get() != NULL);

  GAME_MESH *p_smesh = p_src->mesh_get();
  GAME_MESH *p_dmesh = p_dest->mesh_get();
  
  VECT3DF sdiff = p_src->mesh_diff;
  VECT3DI ssize = p_src->mesh_size;
    
  VECT3DF ddiff = p_dest->mesh_diff;
  VECT3DI dsize = p_dest->mesh_size;
  
  MESH_GEOMETRY *p_sgeom = p_smesh->lock(LOCK_READ);
  MESH_GEOMETRY *p_dgeom = p_dmesh->lock(LOCK_READ_WRITE);
  
  tpos sdx = ssize.x;
  tpos sdz = ssize.z;
  
  tpos ddx = dsize.x;
  tpos ddz = dsize.z;
  
  p_dgeom->vertex_new(ddx*ddz);
  
  VECT *p_svects = p_sgeom->position_get();
  VECT *p_dvects = p_dgeom->position_get();
  
  VECTUV *p_dmaps = p_dgeom->uv_get();
  
  int x,z;
  for(z = 0; z < ddz; z++) {
    for(x = 0; x < ddx; x++) {
      
      assert(x+sx < sdx);
      assert(z+sz < sdz);
      
      int d_vertex_index = VERTEX_INDEX_GET(x,z,ddx,ddz);
      int s_vertex_index = VERTEX_INDEX_GET(x+sx,z+sz,sdx,sdz);
      
      p_dvects[d_vertex_index] = p_svects[s_vertex_index];
      
      float tu = (float)x/(float)(ddx-1);
      float tv = (float)z/(float)(ddz-1);
      p_dmaps[d_vertex_index].set(tu, tv);
    }
  }
  
  tpos fdx = ddx - 1;
  tpos fdz = ddz - 1;
  
  int facenum = fdx*fdz*2;
  p_dgeom->face_new(facenum*3);

  for(z = 0; z < fdz; z++) {
    for(x = 0; x < fdx; x++) {
      int index = FACE_INDEX_GET(x,z,fdx,fdz);
      
      int i1 = VERTEX_INDEX_GET(x,  z,  ddx,ddz);
      int i2 = VERTEX_INDEX_GET(x+1,z,  ddx,ddz);
      int i3 = VERTEX_INDEX_GET(x+1,z+1,ddx,ddz);
      
      p_dgeom->face_set(index, i1, i2, i3);
      
      i1 = VERTEX_INDEX_GET(x,  z,  ddx,ddz);
      i2 = VERTEX_INDEX_GET(x+1,z+1,ddx,ddz);
      i3 = VERTEX_INDEX_GET(x,  z+1,ddx,ddz);
      
      p_dgeom->face_set(index+1, i1, i2, i3);
    }
  }

  p_dmesh->unlock();
  p_smesh->unlock();
}

/*
 * mesh_start - center of the bunch
 * mesh_diff - single bunch slice size
 * mesh_size - number of the bunch slices
 */
/*
  - segments
  - random angles
  - half slices
  - slices in space, not around the axis
*/
void modificator_target_cache::mesh_geometry_create_bunch(GAME_MESH *p_mesh, MESH_HELPER *p_helper, GEOMETRY_DESCRIPTION *p_desc)
{
  assert(p_mesh != NULL);
    
  // Number of mesh slices
  int slices = p_helper->mesh_bunch_slice_num;
  assert(slices > 0);
  
  // Number of segments in one slice
  int segments = p_desc->bunch_slice_segments;
  assert(segments > 0);
  
  // One slice is quad
  #define SEGMENT_VERTEX_NUM  2
  MESH_GEOMETRY *p_geom = p_mesh->lock(LOCK_READ_WRITE);
  p_geom->vertex_new(slices*(segments+1)*SEGMENT_VERTEX_NUM);
  VECT   *p_vects = p_geom->position_get();
  VECTUV *p_maps = p_geom->uv_get();
  
  int i,s;
  for(i = 0; i < slices; i++) {
    
    int   vertex_start_index = i*(segments+1)*SEGMENT_VERTEX_NUM;
    
    // Slice size modificator
    float width_start = p_desc->bunch_slice_width_start;
    float width_end = p_desc->bunch_slice_width_end;
    
    float height_start = p_desc->bunch_slice_height_start;
    float height_end = p_desc->bunch_slice_height_end;
    
    // Generate slice size
    VECT3DF mesh_diff = p_desc->diff;
    float   x_step = mesh_diff.x/2;
    float   y_step = mesh_diff.y;

    // Generate mesh start
    VECT3DF mesh_start = p_desc->start;
    
    // Create a base for slice
    VECT *p_vect = p_vects+vertex_start_index;
    p_vect->set(mesh_start.x-x_step*width_start,
                mesh_start.y,
                mesh_start.z);    
    p_vect++;
        
    p_vect->set(mesh_start.x+x_step*width_start,
                mesh_start.y,
                mesh_start.z);
    p_vect++;
    
    
    // Fill segments
    for(s = 1; s <= segments; s++) {    
      
      float mult_pos = ((float)s)/segments;
      float x_mult = interpolate(width_start, width_end, mult_pos);
      float y_mult = interpolate(height_start, height_end, mult_pos);
      
      p_vect->set(mesh_start.x-(x_step*x_mult),
                  p_vect[-2].y+(y_step*y_mult),
                  mesh_start.z);
      p_vect++;
      
      p_vect->set(mesh_start.x+(x_step*x_mult),
                  p_vect[-2].y+(y_step*y_mult),
                  mesh_start.z);
      p_vect++;
    }
    
    // Slice rotation    
    float slice_angle = p_desc->bunch_slice_falling;
    float segment_angle = 0;
    
    float slice_rotation = 0;
    if(p_desc->bunch_slice_rotation_incemental) {
      slice_rotation = p_desc->bunch_slice_rotation_step;
    }
    else {
      slice_rotation = (i*(p_desc->bunch_slice_rotation_range / slices))
                        + p_desc->bunch_slice_rotation_step;
    }
    
    // Slice position
    float slice_x_offset = p_desc->bunch_slice_x_offset;
    float slice_z_offset = p_desc->bunch_slice_z_offset;
    
    // Modificate it
    p_vect = p_vects+vertex_start_index;
    for(s = 0; s < segments+1; s++)
    {      
      // Translate the slice
      p_vect->x += slice_x_offset;
      p_vect->z += slice_z_offset;
      
      // Rotate slice
      p_vect->rotate_x(segment_angle);
      p_vect->rotate_x(slice_angle);      
      p_vect->rotate_y(slice_rotation);
            
      p_vect++;
      
      // Translate the slice
      p_vect->x += slice_x_offset;
      p_vect->z += slice_z_offset;
      
      // Rotate slice
      p_vect->rotate_x(segment_angle);
      p_vect->rotate_x(slice_angle);      
      p_vect->rotate_y(slice_rotation);
      
      p_vect++;
      
      segment_angle += p_desc->bunch_segment_falling;
    }
    
    // Generate texture coordinates
    VECTUV *p_map = p_maps+vertex_start_index;    
    
    float map_step = 1.0f / segments;    
    for(s = 0; s < segments+1; s++) {
      p_map->set(0, map_step*(s+1));
      p_map++;
      
      p_map->set(1, map_step*(s+1));
      p_map++;
    }
  }
  
  int slice_facenum = (segments+1)*2;
  int bunch_facenum = slices*slice_facenum;
  p_geom->face_new(bunch_facenum*3);
  
  int face_index = 0;
  for(i = 0; i < slices; i++) {
    int vertex_index = i*(segments+1)*SEGMENT_VERTEX_NUM;
    
    for(s = 0; s < segments; s++) {
      p_geom->face_set(face_index,
                       vertex_index,
                       vertex_index+1,
                       vertex_index+2);      
      p_geom->face_set(face_index+1,
                       vertex_index+2,
                       vertex_index+1,
                       vertex_index+3);
      face_index += 2;
      vertex_index += 2;
    }
  }

  p_mesh->unlock();
}

void modificator_target_cache::mesh_geometry_create(GAME_MESH *p_mesh,
                                                    MESH_HELPER *p_helper,
                                                    GEOMETRY_DESCRIPTION *p_desc)
{
  switch(p_helper->mesh_type_get()) {
    case MESH_LAND:
      mesh_geometry_create_landscape(p_mesh,p_helper);
      break;
    case MESH_BUNCH:
      mesh_geometry_create_bunch(p_mesh,p_helper,p_desc);
      break;
    default:
      return;
  }
}

GAME_MESH * modificator_target_cache::mesh_geometry_create(SCENE *p_scene, MESH_HELPER *p_helper, GEOMETRY_DESCRIPTION *p_desc)
{
  GAME_MESH *p_mesh = NULL;
    
  p_mesh = p_scene->mesh_create("GENERATED_MESH");
  assert(p_mesh);
  
  p_mesh->mark_set(MARK_CREATED_BY_GENERATOR);
  
  mesh_geometry_create(p_mesh,p_helper,p_desc);
  
  p_helper->mesh_set(p_mesh);
  
  return(p_mesh);
}

GAME_MESH * modificator_target_cache::mesh_geometry_copy(SCENE *p_scene,
                                                         MESH_HELPER *p_src,
                                                         MESH_HELPER *p_dest,
                                                         tpos sx, tpos sz)
{  
  GAME_MESH *p_mesh = NULL;
   
  // Only land-copy is supported now
  if(p_src->mesh_type_get() != MESH_LAND) {    
    return(NULL);
  }
  
  p_mesh = p_scene->mesh_create("GENERATED_MESH");
  assert(p_mesh);  
  p_mesh->mark_set(MARK_CREATED_BY_GENERATOR);
  p_dest->mesh_set(p_mesh);
    
  mesh_geometry_copy_landscape(p_src, p_dest, sx, sz);
  
  return(p_mesh);
}

MESH_MATERIAL * modificator_target_cache::material_create(SCENE *p_scene, MATERIAL_DESCRIPTION *p_desc)
{     
  MESH_MATERIAL *p_mat = p_scene->material_create("GENERATED_MATERIAL");
  p_mat->mark_set(MARK_CREATED_BY_GENERATOR);
  
  if(p_desc->transparent) {
    p_mat->transparent_set(TRUE);
  }
  if(p_desc->double_side) {
    p_mat->double_side_set(TRUE);
  }
  
  return(p_mat);
}

MATERIAL_TEXT * modificator_target_cache::texture_create(SCENE *p_scene,
                                                         TEXTURE_DESCRIPTION *p_desc,
                                                         SURFACE_SW *p_surf)
{  
  MATERIAL_TEXT *p_text = p_scene->texture_create("GENERATED_TEXTURE");
  p_text->mark_set(MARK_CREATED_BY_GENERATOR);
  p_text->bitmap_create(p_desc->texture_size.x,
                        p_desc->texture_size.y);
  
  if(p_desc->texture_alpha)
    p_text->gl_text_format_set(FORMAT_RGBA4);
      
  p_text->load(p_surf);  
  
  return(p_text);
}

MODIFICATOR_TARGET * modificator_target_cache::target_create(MODIFICATOR_TARGET_TYPE type,
                                                             char *p_name)
{
  MODIFICATOR_TARGET *p_target = modificator_target_create(type, p_name, &helper, p_generator);
  object_list_insert(p_target);
  return(p_target);
}

MODIFICATOR_TARGET * modificator_target_cache::target_find(MODIFICATOR_TARGET_TYPE type,
                                                           char *p_name)
{  
  MODIFICATOR_TARGET *p_target = p_name ?
                                (MODIFICATOR_TARGET *)object_list_find(p_name) :
                                (MODIFICATOR_TARGET *)object_list_get_first();
  
  if(type == MODIFICATOR_TARGET_NONE) {
    assert(p_name);
  } 
  else {  
    while(p_target) {
      if(p_target->type == type)
        return(p_target);
    
      p_target = p_name ?
                (MODIFICATOR_TARGET *)p_target->object_list_next_find(p_name) :
                (MODIFICATOR_TARGET *)p_target->object_list_next();
    }
  }
  return(p_target);
}

MODIFICATOR_TARGET * modificator_target_cache::target_get(MODIFICATOR_TARGET_TYPE type, 
                                                          char *p_name)
{  
  MODIFICATOR_TARGET *p_target = target_find(type, p_name);
  if(p_target)
    return(p_target);
  
  // not found - create a new one
  return(target_create(type,p_name));
}

void modificator_target_cache::init(void)
{
  helper.init(&p_generator->geometry_desc);
}

MESH_HELPER * modificator_target_cache::flush(SCENE *p_scene)
{  
  char name_base[MAX_NAME] = "generated_head";
  
  GENERATOR_CONFIG *p_conf = p_generator->generator_get()->config_get();
  char *p_texture_file = p_conf->output_test_file;
  
  // Create plain mesh and apply geometry targets
  GAME_MESH *p_mesh = mesh_geometry_create(p_scene, &helper, &p_generator->geometry_desc);  
  assert(p_mesh);
  p_mesh->name_set(name_base);
  
  // Projit to smyckou a dat flush!! -> + remove?
  MODIFICATOR_TARGET *p_geom = (MODIFICATOR_TARGET *)target_find(MODIFICATOR_TARGET_GEOMETRY_TYPE, NULL);
  if(p_geom) {
    p_geom->flush(&helper);
  }
  
  // Save textures
  MODIFICATOR_TARGET_TEXTURE *p_text = (MODIFICATOR_TARGET_TEXTURE *)target_find(MODIFICATOR_TARGET_TEXTURE_TYPE, NULL);
  if(p_text) {
    p_text->flush(&helper);
    
    SURFACE_SW *p_bitmap = (SURFACE_SW *)p_text->bitmap_get();
    pprintf("Writing MODIFICATOR_TARGET_TEXTURE content to %s",p_texture_file);
    p_bitmap->save(p_texture_file);
    
    tpos width = p_bitmap->width_get();
    tpos height = p_bitmap->height_get();
    
    TEXTURE_DESCRIPTION *p_desc = &p_generator->texture_desc;
    
    int segments_x = 1;
    int segments_x_size = 1;
    if(width > p_desc->texture_size_max.x) {
      segments_x = width / p_desc->texture_size_max.x + 1;
      segments_x_size = width / segments_x;
    }
      
    int segments_y = 1;
    int segments_y_size = 1;
    if(height > p_desc->texture_size_max.y) {
      segments_y = height / p_desc->texture_size_max.y + 1;
      segments_y_size = width / segments_y;
    }
    
    if(segments_x == 1 && segments_y == 1) {
      // 1:1 -> just export it
      // helper&p_mesh
      MATERIAL_TEXT *p_text = texture_create(p_scene, &p_generator->texture_desc,
                                             p_bitmap);
      assert(p_text);
      p_text->name_set(name_base);
      
      MESH_MATERIAL *p_mat = material_create(p_scene, &p_generator->material_desc);
      assert(p_mat);
      p_mat->name_set(name_base);
      
      p_mat->texture_set(0, p_text);
      p_mat->texture_enable(0, 0, 0, 0);
      
      p_mesh->material_set(p_mat);
    }
    else {
      int x,y;
      for(x = 0; x < segments_x; x++) {
        for(y = 0; y < segments_y; y++) {
          char name[MAX_NAME];
          snprintf(name,MAX_NAME,"generated[%d.%d]",x,y);
          
          MESH_HELPER helper_tmp = helper.derivate(x*segments_x_size,
                                                   y*segments_y_size,
                                                   segments_x_size,
                                                   segments_y_size);
          
          GAME_MESH *p_mesh_tmp = mesh_geometry_copy(p_scene,
                                                     &helper,
                                                     &helper_tmp,
                                                     x*segments_x_size,
                                                     y*segments_y_size);
          p_mesh_tmp->name_set(name);
          assert(p_mesh);
          
          SURFACE_SW bitmap_tmp(segments_x_size, segments_y_size);
          p_bitmap->blit(x*segments_x_size,
                         y*segments_y_size,
                         segments_x_size,
                         segments_y_size,
                         &bitmap_tmp);
          
          MATERIAL_TEXT *p_text = texture_create(p_scene, &p_generator->texture_desc,
                                                &bitmap_tmp);
          assert(p_text);
          p_text->name_set(name);
          
          MESH_MATERIAL *p_mat = material_create(p_scene, &p_generator->material_desc);
          assert(p_mat);
          p_mat->name_set(name);
          
          p_mat->texture_set(0, p_text);
          p_mat->texture_enable(0, 0, 0, 0);
          p_mesh_tmp->material_set(p_mat);
          
          p_mesh_tmp->render_hierarchy_insert(p_mesh);
        }
      }
      
      // Clear original geometry
      p_mesh->geometry_delete();
    }    
  }
  
  object_list_clear();
  
  return(new MESH_HELPER(helper));
}

modificator_target_cache::modificator_target_cache(GENERATOR_MESH *p_gen)
 : object_list_head(NULL), p_generator(p_gen), p_mask(NULL)
{
}
