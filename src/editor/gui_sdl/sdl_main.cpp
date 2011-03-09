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
 * Main.c - editor/test environment
 */

#include "age.h"
#include "editor.h"

bool callback(AGE_MAIN *p_age)
{
  GRAPH3D *p_grf = p_age->graph_get(); 

  GAME_EVENT *p_event;
  
  while((p_event = p_age->event_get())) {    
    switch(p_event->type_get()) {
      case UI_QUIT:
        return(FALSE);
      default:
        break;
    }
  }
 
  // Draw whole scene
  p_grf->draw_start();
  p_grf->draw_end();
  
  return(TRUE);
}

void transf_2d_3d_z(float p_x,   float p_y,   float p_z,
                    float *p_tx, float *p_ty, float *p_tz,
                    GLMATRIX *p_cam, GLMATRIX *p_proj,
                    dword x_c, dword y_c, dword xs, dword ys,
                    float z_near, float z_far)
{
 GLMATRIX  m,mi;
 float  x,y,z,a,b,c;
 float  x3,y3,z3,w3;
     
 x_c >>= 1;
 y_c >>= 1;
 p_x -= xs;
 p_y -= ys;
 x = p_x/x_c - 1;
 y = 1 - p_y/y_c;
 c = z_far-z_near;
 a = (z_near+z_far)/c;
 b =-(2*z_near*z_far)/c;
 z = a+b/p_z;
   
 glu_invert_matrix(matrix_mult(p_cam,p_proj,&mi),&m);
   
 x3 = m._11*x + m._21*y + m._31*z + m._41;
 y3 = m._12*x + m._22*y + m._32*z + m._42;
 z3 = m._13*x + m._23*y + m._33*z + m._43;
 w3 = m._14*x + m._24*y + m._34*z + m._44;
    
 *p_tx = x3/w3;
 *p_ty = y3/w3;
 *p_tz = z3/w3;
}

void test(AGE_MAIN *p_age)
{
  CAMERA_POLAR *p_camera = camera_control_get(p_age);
  int           sx,sy,sdx,sdy;
  
  GLMATRIX m,m2;
  
  p_camera->target_set(0,0,0);
  p_camera->distance_set(0);  
  p_camera->elevation_set(0);  
  p_camera->update();
      
  p_camera->view_get(&sx,&sy,&sdx,&sdy);

  VECT v2;
  
  v2.set((float)(sx+sdx/2),(float)(sy+sdy/2),1);
  p_camera->screen_to_world(&v2);
  p_camera->world_to_screen(&v2);
  v2.set((float)(sx+sdx/2),(float)(sy+sdy/2),10);
  p_camera->screen_to_world(&v2);
  p_camera->world_to_screen(&v2);
  v2.set((float)(sx+sdx/2),(float)(sy+sdy/2),99);
  p_camera->screen_to_world(&v2);
  p_camera->world_to_screen(&v2);

  transf_2d_3d_z((float)(sx+sdx/2),(float)(sy+sdy/2),1,
                  &v2.x,&v2.y,&v2.z,
                  p_camera->camera_get(), p_camera->projection_get(),
                  sdx,sdy,sx,sy,
                  1.0f,100.0f);
  transf_2d_3d_z((float)(sx+sdx/2),(float)(sy+sdy/2),2,
                  &v2.x,&v2.y,&v2.z,
                  p_camera->camera_get(), p_camera->projection_get(),
                  sdx,sdy,sx,sy,
                  1.0f,100.0f);
  transf_2d_3d_z((float)(sx+sdx/2),(float)(sy+sdy/2),10,
                  &v2.x,&v2.y,&v2.z,
                  p_camera->camera_get(), p_camera->projection_get(),
                  sdx,sdy,sx,sy,
                  1.0f,100.0f);
  transf_2d_3d_z((float)(sx+sdx/2),(float)(sy+sdy/2),99,
                  &v2.x,&v2.y,&v2.z,
                  p_camera->camera_get(), p_camera->projection_get(),
                  sdx,sdy,sx,sy,
                  1.0f,100.0f);

  v2.set((float)0,(float)0,1);
  p_camera->world_to_screen(&v2);
  v2.set((float)0,(float)0,2);
  p_camera->world_to_screen(&v2);
  v2.set((float)0,(float)0,100);
  p_camera->world_to_screen(&v2);
  
/*
  m.projection(DEG2RAD(DEFAULT_PROJECTION_ANGLE), 1024.0f/768.0f, 1.0f, 100.0f);
  glu_invert_matrix(&m,&m2);
  //p_camera->projection_set(&m);

  float w;

  w = 1; v2.set(0,0,-1);
  matrix_transformate(&v2.x,&v2.y,&v2.z,&w,&m2);
  v2.x /= w;
  v2.y /= w;
  v2.z /= w;
  
  w = 1; v2.set(0,0,0);
  matrix_transformate(&v2.x,&v2.y,&v2.z,&w,&m2);
  v2.x /= w;
  v2.y /= w;
  v2.z /= w;
  
  w = 1; v2.set(0,0,1);
  matrix_transformate(&v2.x,&v2.y,&v2.z,&w,&m2);
  v2.x /= w;
  v2.y /= w;
  v2.z /= w;
*/
}

AGE_MAIN * initialize(void)
{
  AGE_MAIN *p_age = new AGE_MAIN;
  
  GRAPH3D *p_grf = p_age->graph_sdl_set(1024, 768);
  
  SCENE *p_scene = p_age->scene_new();
  p_scene->render_state_set(TRUE);

  //log_open("/home/komat/b1log.txt");
  
  //GAME_MESH *p_mesh = p_scene->mesh_new();
  //MESH_MATERIAL *p_mat = p_scene->material_new();
  
  // TODO -> material_load() ->
  
  // TODO
  // Loading mesh / mesh list
  // Set material list for meshes during loading
  
  //p_scene->load(NULL, "/home/komat/Projekty/Graph/test.b2m");
  //p_scene->load(NULL, "/home/komat/Projekty/Graph/Scenes/level1.bpr");
  //p_scene->load(NULL, "/home/komat/Projekty/test.3ds",SCENE_3DS);
  //p_scene->load(NULL, "/home/komat/Projekty/komat.3ds",SCENE_3DS);
  //p_scene->load(NULL, "/home/komat/Projekty/redshit.3ds",SCENE_3DS);
  //p_scene->load(NULL, "/home/komat/Projekty/test.b2m");
  //p_scene->load(NULL, "/home/komat/Projekty/auto.b2m");
  //p_scene->save(NULL, "/home/komat/Projekty/auto-1.b2m");
  //p_scene->load(NULL, "/home/komat/Projekty/auto-1.b2m");
  //p_scene->load(NULL, "/home/komat/tmp310/apollo13.3ds",SCENE_3DS);
  //p_scene->texture_dir_add("/home/komat/Projekty/Graph/Textures");
  //p_scene->texture_reload();
    
  CAMERA_POLAR *p_camera = (CAMERA_POLAR *)camera_control_get(p_age);
  p_scene->camera_active_set(p_camera);
  
  /*
  GAME_MESH *p_mesh = p_scene->mesh_get("bedna8.b2m");
  p_mesh->translate_set(0.0f,0.0f,5.0f);
  */
 
  p_age->callback_set(callback);

  INPUT *p_input = p_age->input_get();
  
  static INPUT_EVENT events[] = 
  {
    INPUT_EVENT(KEY_STATE(K_M), (INPUT_EVENT_FUNC)scene_move),
    INPUT_EVENT(KEY_STATE(K_R), (INPUT_EVENT_FUNC)scene_rotate),
    INPUT_EVENT(KEY_STATE(K_R), (INPUT_EVENT_FUNC)scene_reset),
    INPUT_EVENT(KEY_STATE(K_A), (INPUT_EVENT_FUNC)scene_test)
  };
  
  p_input->events_add(events, sizeof(events)/sizeof(events[0]));
  
  p_grf->config_draw_grid(FALSE);
  p_grf->config_draw_mouse_cursor(TRUE);
  //p_grf->config_draw_boxes(TRUE);
  p_grf->config_draw_console(TRUE);
  p_grf->config_draw_wired(TRUE);
  //p_grf->config_normals_draw(TRUE);
  //p_grf->config_opengl_lighting(TRUE);
  p_grf->config_draw_all_objects(TRUE);
  p_grf->config_draw_pivots(TRUE);
  
  p_grf->console_font_color_set(RGBB(0,0xff,0));
  p_grf->console_origin_set(0,10);
  p_grf->console_print("Zdar ty jelito 1!");
  p_grf->console_print("Zdar ty jelito 2!");
  p_grf->console_print("Zdar ty jelito 3!");
    
  
  return(p_age);
}

int sdl_main(int argc, char **argv)
{
  AGE_MAIN *p_age = initialize();
  p_age->run();
  delete p_age;
}
