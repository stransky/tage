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

/*
 * Temporary static function
 */
static GUI_WINDOW_RUNNING_AGE * gui_window_get(void)
{
  EDITOR *p_editor = editor_reference_get();
  if(p_editor) {
    GUI_INTERFACE *p_state_gui = p_editor->gui_interface_get();
    if(p_state_gui) {
      return(p_state_gui->gui_main_window_get());
    }
  }
  return(NULL);
}

/* 
 * Camera commands
 */
#define CAMERA_POLAR_NAME "CONTROL_CAMERA"

static CAMERA_POLAR * camera_control_get(AGE_MAIN *p_age)
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
  
  p_scene->camera_active_set(p_camera);
  
  assert(p_camera);
  return(p_camera);
}

static CAMERA_POLAR * camera_control_center_get(AGE_MAIN *p_age)
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

void editor_command_scene_reset(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(p_window)
    scene_reset(p_window->age_get());  
  p_window->window_age_draw_request_set();
}

static bool scene_select_run(AGE_MAIN *p_age)
{  
  GRAPH3D *p_graph = p_age->graph_get();
  
  p_graph->selection_rectangle_set();
  
  return(TRUE);
}

typedef enum {
  
  SELECTION_SET = 0,
  SELECTION_ADD  
  
} SELECTION_MODE;

static SELECTION_MODE current_selection_mode = SELECTION_SET;

static bool scene_select_stop(AGE_MAIN *p_age)
{  
  GRAPH3D *p_graph = p_age->graph_get();
  p_graph->selection_rectangle_set();
    
  RECT2DI tmp;
  p_graph->selection_rectangle_get(&tmp);
  
  SCENE *p_scene = p_age->scene_get();
  
  switch(current_selection_mode) {
    case SELECTION_SET:
      p_scene->selection_clear();
      p_scene->selection_add(&tmp);
      break;
    case SELECTION_ADD:
      p_scene->selection_add(&tmp);
      break;
  }
  current_selection_mode = SELECTION_SET;
  
  p_graph->selection_rectangle_clear();
  (p_age->input_get())->shadow_events_clear();
  
  p_graph->mouse_cursor_set();
  
  return(TRUE);
}

static bool scene_select_start(AGE_MAIN *p_age)
{  
  GRAPH3D *p_graph = p_age->graph_get();
  
  p_graph->selection_rectangle_clear();
  p_graph->selection_rectangle_set_start();
  
  static INPUT_EVENT events[] = 
  {
    INPUT_EVENT(MOUSE_STATE(ACTIVATE_MOUSE_MOVE_BUTTON,MOUSE_BUTTON_LEFT),
                (INPUT_EVENT_FUNC)scene_select_run),
    INPUT_EVENT(MOUSE_STATE(ACTIVATE_BUTTON_RELEASED,MOUSE_BUTTON_LEFT),
                (INPUT_EVENT_FUNC)scene_select_stop)
  };

  p_graph->mouse_cursor_set(MOUSE_CURSOR_RECTANGLE_SELECTING);
  
  (p_age->input_get())->shadow_events_set(events, sizeof(events)/sizeof(events[0]));
  
  return(TRUE);
}

static bool scene_select(AGE_MAIN *p_age)
{   
  static INPUT_EVENT events[] = 
  {
    INPUT_EVENT(MOUSE_STATE(ACTIVATE_BUTTON_PRESSED,MOUSE_BUTTON_LEFT), (INPUT_EVENT_FUNC)scene_select_start)
  };
  
  (p_age->input_get())->shadow_events_set(events, sizeof(events)/sizeof(events[0]));
  (p_age->graph_get())->mouse_cursor_set(MOUSE_CURSOR_RECTANGLE_SELECT);
  
  return(TRUE);
}

static bool scene_select_set(AGE_MAIN *p_age)
{
  current_selection_mode = SELECTION_SET;
  return(scene_select(p_age));
}

static bool scene_select_add(AGE_MAIN *p_age)
{
  current_selection_mode = SELECTION_ADD;
  return(scene_select(p_age));
}

static bool scene_select_clear(AGE_MAIN *p_age)
{ 
  SCENE *p_scene = p_age->scene_get();
  
  p_scene->selection_clear();
  current_selection_mode = SELECTION_SET;
  
  return(TRUE);
}

void editor_command_scene_select_set(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(p_window)
    scene_select_set(p_window->age_get());
}

void editor_command_scene_select_add(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(p_window)
    scene_select_add(p_window->age_get());
}

void editor_command_scene_select_clear(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(p_window)
    scene_select_clear(p_window->age_get());
}

static bool scene_new(AGE_MAIN *p_age)
{
  (p_age->scene_active_get())->clear();
//  editor_main_gui_update();
  return(TRUE);
}

void editor_command_scene_new(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(p_window)
    scene_new(p_window->age_get());
}

void editor_command_scene_load(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(!p_window)
    return;
  
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new("Open File",
                                       GTK_WINDOW(p_window->window_age_get()),
                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                       NULL);
  
  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
      char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      (p_window->scene_get())->load(NULL,filename);
      g_free(filename);
  }  
  
  gtk_widget_destroy (dialog);
  //editor_main_gui_update();
}

//gtk_file_chooser_set_extra_widget - extra widget
  /*
  if(user_edited_a_new_document) {
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), default_folder_for_saving);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "Untitled document");
  }
  else {
    gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(dialog), filename_for_existing_document);
  }
  */
void editor_command_scene_save(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(!p_window)
    return;
  
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new("Save Scene As...",
                                        GTK_WINDOW(p_window->window_age_get()),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                        NULL);
  
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
  
  GtkFileFilter *filter_txt = gtk_file_filter_new ();
  gtk_file_filter_set_name(filter_txt, "3D ASCII export");
  gtk_file_filter_add_pattern(filter_txt,"*.txt");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_txt);
  
  GtkFileFilter *filter_3ds = gtk_file_filter_new ();
  gtk_file_filter_set_name(filter_3ds, "3DS Studio");
  gtk_file_filter_add_pattern(filter_3ds,"*.3ds");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_3ds);
  
  GtkFileFilter *filter_b2m = gtk_file_filter_new ();
  gtk_file_filter_set_name(filter_b2m, "Berusky 2 files");
  gtk_file_filter_add_pattern(filter_b2m,"*.b2m");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter_b2m);
  
  gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "scene.txt");
  gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter_txt);
  
  if(gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    GtkFileFilter *filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(dialog));    
    
    SCENE_TYPE type = SCENE_B2M;
    if(filter == filter_txt) {
      type = SCENE_TXT;
    } else if(filter == filter_3ds) {
      type = SCENE_3DS;
    }

    char dir[MAX_FILENAME];
    char file[MAX_FILENAME];
    
    return_dir(filename,dir,MAX_FILENAME);
    return_file(filename,file,MAX_FILENAME);
    
    (p_window->scene_get())->save(dir,file,type);
    g_free(filename);
  }
  
  gtk_widget_destroy(dialog);
}

void editor_command_scene_save_as(void)
{
  
}

void editor_command_scene_redraw(void)
{
  
}

void editor_command_quit(void)
{
  
}

void editor_command_rendering_mode(RENDER_MODE mode)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  if(!p_window)
    return;
  
  GRAPH3D *p_graph = p_window->graph_get();
  if(p_graph)
    p_graph->config_render_mode_set(mode);
}


/*
 * Land generators
 */
void editor_command_land_create(void)
{/*
  GUI_INTERFACE *p_gui = gui_interface_get();
  if(p_gui)
    p_gui->window_create("editor_land_generator");
  */
}

void editor_command_land_height_map(void)
{
  /*
  GUI_INTERFACE *p_gui = gui_interface_get();
  if(p_gui)
    p_gui->window_create("editor_land_heightmap_modificator");
  */
}

void editor_command_land_height_fractal(void)
{
  
}

void editor_command_land_height_filter(void)
{
  
}

// Scene movements
bool scene_move(GUI_WINDOW_RUNNING *p_window, void *p_data)
{  
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  
  AGE_MAIN  *p_age = p_awindow->age_get();
  assert(p_age);
  
  static tpos           v1x,v1y;
  static tpos           v2x,v2y;
    
  if(p_data != NULL) {
    p_awindow->window_mouse_position_get(&v1x,&v1y);
  }
  else {  
    p_awindow->window_mouse_position_get(&v2x,&v2y);
    
    float depth = (p_age->scene_get())->scene_box_depth_get();
      
    VECT v1((float)v1x,(float)v1y,depth);
    VECT v2((float)v2x,(float)v2y,depth);  
    
    CAMERA_POLAR *p_camera = camera_control_center_get(p_age);  
    p_camera->screen_to_world(&v1);
    p_camera->screen_to_world(&v2);
      
    p_camera->target_add(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
    
    v1x = v2x;
    v1y = v2y;
  }
  
  return(TRUE);
}

bool scene_move_stop(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  p_awindow->window_age_render_callback_clear();
  return(FALSE);
}

bool scene_move_start(GUI_WINDOW_RUNNING *p_window, void *p_data)
{  
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  p_awindow->window_command_register(scene_move_stop, GUI_MOUSE_LEFT, BUTTON_RELEASED);
  p_awindow->window_age_render_callback_set(scene_move);
  scene_move(p_window, (void *)TRUE);
  return(FALSE);
}

void editor_command_scene_move(void)
{ 
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  assert(p_window);
  p_window->window_command_register(scene_move_start, GUI_MOUSE_LEFT, BUTTON_PRESSED);
}

bool scene_rotate(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  
  AGE_MAIN  *p_age = p_awindow->age_get();
  assert(p_age);
    
  static tpos v1x,v1y;
  static tpos v2x,v2y;

  if(p_data != NULL) {    
    p_awindow->window_mouse_position_get(&v1x,&v1y);
    camera_control_center_get(p_age);
  }
  else {  
    CAMERA_POLAR *p_camera = camera_control_get(p_age);
    p_awindow->window_mouse_position_get(&v2x,&v2y);
      
    tpos dx = v2x - v1x;
    tpos dy = v2y - v1y;
      
    p_camera->elevation_add(DEG2RAD(dy*0.5f));
    p_camera->rotation_add(DEG2RAD(-dx*0.5f));
    
    v1x = v2x;
    v1y = v2y;      
  }    
  return(TRUE);
}

bool scene_rotate_stop(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  p_awindow->window_age_render_callback_clear();
  return(FALSE);
}

bool scene_rotate_start(GUI_WINDOW_RUNNING *p_window, void *p_data)
{  
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  p_awindow->window_command_register(scene_rotate_stop, GUI_MOUSE_LEFT, BUTTON_RELEASED);
  p_awindow->window_age_render_callback_set(scene_rotate);
  scene_rotate(p_window, (void *)TRUE);
  return(FALSE);
}

void editor_command_scene_rotate(void)
{   
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  assert(p_window);
  p_window->window_command_register(scene_rotate_start, GUI_MOUSE_LEFT, BUTTON_PRESSED);
}

/*
 * Run-time commands called from GL rendering screen
 */
void scene_zoom_in(AGE_MAIN *p_age)
{
  SCENE *p_scene = p_age->scene_get();
  
  CAMERA_POLAR *p_camera = camera_control_center_get(p_age);
  
  float dist = -0.05f*(p_scene->scene_box_depth_get());
  p_camera->distance_add(dist);
}

void scene_zoom_out(AGE_MAIN *p_age)
{
  SCENE *p_scene = p_age->scene_get();
  
  CAMERA_POLAR *p_camera = camera_control_center_get(p_age);
  
  float dist = 0.05f*(p_scene->scene_box_depth_get());
  p_camera->distance_add(dist);
}

bool editor_command_scene_zoom_in(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  scene_zoom_in(p_awindow->age_get());
  p_awindow->window_age_draw_request_set();
  return(TRUE);
}

bool editor_command_scene_zoom_out(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_awindow = (GUI_WINDOW_RUNNING_AGE *)p_window;
  scene_zoom_out(p_awindow->age_get());
  p_awindow->window_age_draw_request_set();
  return(TRUE);
}

void editor_command_scene_zoom_in(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  assert(p_window);
  editor_command_scene_zoom_in(p_window,NULL);
}

void editor_command_scene_zoom_out(void)
{
  GUI_WINDOW_RUNNING_AGE *p_window = gui_window_get();
  assert(p_window);
  editor_command_scene_zoom_out(p_window,NULL);
}

bool editor_command_scene_popup_menu(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  static GtkWidget * popup_menu = NULL;
  if(popup_menu == NULL) {
    popup_menu = gtk_menu_new();
    
    GtkAccelGroup *accel_group = gtk_accel_group_new();

    /*
     * View sub-menu
     */    
    GtkWidget * view_item = gtk_menu_item_new_with_label("View");
    gtk_widget_show(view_item);

    GtkWidget * view_item_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_item), view_item_menu);
  
    GtkWidget * scene_center1 = gtk_menu_item_new_with_mnemonic("Reset");
    gtk_widget_show (scene_center1);
    gtk_container_add (GTK_CONTAINER (view_item_menu), scene_center1);
    gtk_widget_add_accelerator (scene_center1, "activate", accel_group,
                                GDK_R, (GdkModifierType) GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)scene_center1,"activate",
                     G_CALLBACK(on_scene_center1_activate),
                     NULL);
  
    GtkWidget * scene_move1 = gtk_menu_item_new_with_mnemonic("Move");
    gtk_widget_show (scene_move1);
    gtk_container_add (GTK_CONTAINER (view_item_menu), scene_move1);
    gtk_widget_add_accelerator (scene_move1, "activate", accel_group,
                                GDK_M, (GdkModifierType) 0,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)scene_move1,"activate",
                     G_CALLBACK(on_scene_move1_activate),
                     NULL);
  
    GtkWidget * scene_rotate1 = gtk_menu_item_new_with_mnemonic("Rotate");
    gtk_widget_show (scene_rotate1);
    gtk_container_add (GTK_CONTAINER (view_item_menu), scene_rotate1);
    gtk_widget_add_accelerator (scene_rotate1, "activate", accel_group,
                                GDK_R, (GdkModifierType) 0,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)scene_rotate1,"activate",
                     G_CALLBACK(on_scene_rotate1_activate),
                     NULL);
  
    GtkWidget * scene_zoom_m1 = gtk_menu_item_new_with_mnemonic("Zoom In");
    gtk_widget_show (scene_zoom_m1);
    gtk_container_add (GTK_CONTAINER (view_item_menu), scene_zoom_m1);
    gtk_widget_add_accelerator (scene_zoom_m1, "activate", accel_group,
                                GDK_plus, (GdkModifierType) 0,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)scene_zoom_m1,"activate",
                     G_CALLBACK(on_scene_zoom_m1_activate),
                     NULL);
  
    GtkWidget * scene_zoom_p1 = gtk_menu_item_new_with_mnemonic("Zoom Out");
    gtk_widget_show (scene_zoom_p1);
    gtk_container_add (GTK_CONTAINER(view_item_menu), scene_zoom_p1);
    gtk_widget_add_accelerator (scene_zoom_p1, "activate", accel_group,
                                GDK_minus, (GdkModifierType) 0,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)scene_zoom_p1,"activate",
                     G_CALLBACK(on_scene_zoom_p1_activate),
                     NULL);
    /*
     * Selection sub-menu
     */    
    GtkWidget * select_item = gtk_menu_item_new_with_label("Selection");
    gtk_widget_show(select_item);

    GtkWidget * select_item_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(select_item), select_item_menu);

    GtkWidget * selection_insert = gtk_menu_item_new_with_mnemonic("Set");
    gtk_widget_show (selection_insert);
    gtk_container_add (GTK_CONTAINER (select_item_menu), selection_insert);
    g_signal_connect((gpointer)selection_insert,"activate",
                     G_CALLBACK(on_selection_set_activate),
                     NULL);
                     
    GtkWidget * selection_add = gtk_menu_item_new_with_mnemonic("Add");
    gtk_widget_show (selection_add);
    gtk_container_add (GTK_CONTAINER (select_item_menu), selection_add);
    gtk_widget_add_accelerator (selection_add, "activate", accel_group,
                                GDK_S, (GdkModifierType) 0,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)selection_add,"activate",
                     G_CALLBACK(on_selection_add_activate),
                     NULL);
  
    GtkWidget * selection_clear = gtk_menu_item_new_with_mnemonic("Clear");
    gtk_widget_show (selection_clear);
    gtk_container_add (GTK_CONTAINER (select_item_menu), selection_clear);
    gtk_widget_add_accelerator (selection_clear, "activate", accel_group,
                                GDK_S, (GdkModifierType) GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE);
    g_signal_connect((gpointer)selection_clear,"activate",
                     G_CALLBACK(on_selection_clear_activate),
                     NULL);
                     
    /*
     * Rendering mode sub-menu
     */    
    GtkWidget * render_item = gtk_menu_item_new_with_label("Rendering Mode");
    gtk_widget_show(render_item);

    GtkWidget * render_item_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(render_item), render_item_menu);

    GtkWidget * rendering_points = gtk_menu_item_new_with_mnemonic("Points");
    gtk_widget_show (rendering_points);
    gtk_container_add (GTK_CONTAINER (render_item_menu), rendering_points);
    g_signal_connect((gpointer)rendering_points,"activate",
                     G_CALLBACK(on_rendering_points_activate),
                     NULL);
                     
    GtkWidget * rendering_wired = gtk_menu_item_new_with_mnemonic("Wired");
    gtk_widget_show (rendering_wired);
    gtk_container_add (GTK_CONTAINER (render_item_menu), rendering_wired);
    /*
    gtk_widget_add_accelerator (rendering_wired, "activate", accel_group,
                                GDK_S, (GdkModifierType) 0,
                                GTK_ACCEL_VISIBLE);                                
    */
    g_signal_connect((gpointer)rendering_wired,"activate",
                     G_CALLBACK(on_rendering_wired_activate),
                     NULL);
  
    GtkWidget * rendering_solid = gtk_menu_item_new_with_mnemonic("Solid");
    gtk_widget_show (rendering_solid);
    gtk_container_add (GTK_CONTAINER (render_item_menu), rendering_solid);
    /*
    gtk_widget_add_accelerator (rendering_solid, "activate", accel_group,
                                GDK_S, (GdkModifierType) GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE);
    */                            
    g_signal_connect((gpointer)rendering_solid,"activate",
                     G_CALLBACK(on_rendering_solid_activate),
                     NULL);
                     
    GtkWidget * rendering_textured = gtk_menu_item_new_with_mnemonic("Textured");
    gtk_widget_show (rendering_textured);
    gtk_container_add (GTK_CONTAINER (render_item_menu), rendering_textured);
    /*
    gtk_widget_add_accelerator (rendering_textured, "activate", accel_group,
                                GDK_S, (GdkModifierType) GDK_CONTROL_MASK,
                                GTK_ACCEL_VISIBLE);
    */                            
    g_signal_connect((gpointer)rendering_textured,"activate",
                     G_CALLBACK(on_rendering_textured_activate),
                     NULL);
                     
    /*
     * Insert all sub-menus
     */
    gtk_container_add(GTK_CONTAINER(popup_menu), select_item);
    gtk_container_add(GTK_CONTAINER(popup_menu), view_item);                     
    gtk_container_add(GTK_CONTAINER(popup_menu), render_item);
  }
  
  gtk_menu_popup(GTK_MENU(popup_menu),NULL,NULL,NULL,NULL,0,gtk_get_current_event_time());
  return(TRUE);
}
