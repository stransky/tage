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
 * Basic controls, key binding and so on
 */

#include "age.h"
#include "editor.h"

#define CAMERA_POLAR_NAME "CONTROL_CAMERA"

CAMERA_POLAR * camera_control_get(AGE_MAIN *p_age)
{
  SCENE *p_scene = p_age->scene_get();
    
  CAMERA_POLAR *p_camera = (CAMERA_POLAR *)p_scene->camera_get(CAMERA_POLAR_NAME);
  if(!p_camera) {
    p_camera = p_scene->camera_polar_create(CAMERA_POLAR_NAME);
    p_camera->target_set(0,0,0);
    p_camera->distance_set(20);
    p_camera->elevation_set(DEG2RAD(60));
    p_camera->plane_auto_adjust_set(TRUE);
  }
  
  assert(p_camera);
  return(p_camera);
}

CAMERA_POLAR * camera_control_center_get(AGE_MAIN *p_age)
{
  CAMERA_POLAR *p_camera = camera_control_get(p_age);
  SCENE   *p_scene = p_age->scene_get();
  int     sx,sy,sdx,sdy;
  
  p_camera->view_get(&sx,&sy,&sdx,&sdy);
  float depth = p_scene->scene_box_depth_get();
  VECT v1((float)(sx+sdx/2),(float)(sy+sdy/2),depth);
  
  p_camera->screen_to_world(&v1);
  p_camera->target_set(&v1);
  p_camera->distance_set(depth);
  
  return(p_camera);
}

bool scene_move(AGE_MAIN *p_age)
{    
  tpos v1x,v1y;
  tpos v2x,v2y;
  
  INPUT *p_input = p_age->input_get();
  p_input->mouse_position_last_get(&v1x,&v1y);
  p_input->mouse_position_get(&v2x,&v2y);
    
  float depth = (p_age->scene_get())->scene_box_depth_get();
    
  VECT v1((float)v1x,(float)v1y,depth);
  VECT v2((float)v2x,(float)v2y,depth);  
  
  CAMERA_POLAR *p_camera = camera_control_center_get(p_age);  
  p_camera->screen_to_world(&v1);
  p_camera->screen_to_world(&v2);
    
  p_camera->target_add(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);

  return(TRUE);
}

bool scene_rotate(AGE_MAIN *p_age)
{
  INPUT   *p_input = p_age->input_get();
  tpos    dx,dy;
  
  CAMERA_POLAR *p_camera = camera_control_center_get(p_age);
      
  p_input->mouse_position_diff_get(&dx,&dy);
  p_camera->elevation_add(DEG2RAD(dy*0.5f));
  p_camera->rotation_add(DEG2RAD(-dx*0.5f));
  
  return(TRUE);
}

bool scene_zoom_in(AGE_MAIN *p_age)
{
  SCENE *p_scene = p_age->scene_get();
  
  CAMERA_POLAR *p_camera = camera_control_center_get(p_age);
  
  float dist = -0.05f*(p_scene->scene_box_depth_get());  
  p_camera->distance_add(dist);
  
  return(TRUE);
}

bool scene_zoom_out(AGE_MAIN *p_age)
{
  SCENE *p_scene = p_age->scene_get();
  
  CAMERA_POLAR *p_camera = camera_control_center_get(p_age);
  
  float dist = 0.05f*(p_scene->scene_box_depth_get());
  p_camera->distance_add(dist);
  
  return(TRUE);
}

bool scene_reset(AGE_MAIN *p_age)
{
  CAMERA_POLAR *p_camera = camera_control_get(p_age);
  SCENE *p_scene = p_age->scene_get();
    
  VECT  s_center;
  VECT  s_length;
  
  p_scene->scene_box_center_get(&s_center);  // Return center of the scene
  p_scene->scene_box_length_get(&s_length);  // Return size of the scene
    
  p_camera->target_set(&s_center);
  p_camera->elevation_set(DEG2RAD(60));
  p_camera->rotation_set(DEG2RAD(0));
  p_camera->distance_set(s_length.size()*2);
  
  return(TRUE);
}

bool scene_test(AGE_MAIN *p_age)
{
  //SCENE *p_scene = p_age->scene_get();
  
  // create a new animation
  //p_scene->...

  // create a new animation
  //p_scene->...
  
  return(TRUE);
}
