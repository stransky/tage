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
  GUI window:
  
  create callback
  delete callback  
  
  - internal (private) callbacks -> are called from
  window gui
*/

/*
 * End function - calls end callbacks, ask user and quit if requested
 */
static gboolean gui_interface_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  GUI_WINDOW_REGISTERED *p_window = (GUI_WINDOW_REGISTERED *)data;    
  return(p_window->destroy());
}

static void gui_interface_destroy_event(GtkWidget *widget, gpointer data)
{
}

GUI_WINDOW_RUNNING * gui_window_registered::create(void)
{
  /*
   * Is this window already opened?
   */
  if(is_running())
    return(FALSE);
  
  /*
   * Create a new window
   */
  p_running = func_create(this);
  
  /*
   * Attach delete/destroy signals
   */
  g_signal_connect(G_OBJECT(p_running->gtk_window_get()), "delete_event",
                   G_CALLBACK(gui_interface_delete_event), this);
  g_signal_connect(G_OBJECT(p_running->gtk_window_get()), "destroy",
                   G_CALLBACK(gui_interface_destroy_event), this);
    
  return(p_running);
}

bool gui_window_registered::destroy(void)
{
  /*
   * Is this window opened?
   */
  if(!is_running())
    return(FALSE);

  /*
   * Run destroy function
   */
  if(func_destroy) {
    func_destroy(this);
  }
  
  /*
   * Delete the window
   */
  if(p_running) {
    delete p_running;
    p_running = NULL;
  }
  
  return(TRUE);
}

gui_window_registered::gui_window_registered(GUI_INTERFACE *p_interface_reference)
{    
  p_interface = p_interface_reference;
  func_create = NULL;
  func_destroy = NULL;
  p_running = NULL;
}

gui_window_registered::~gui_window_registered(void)
{
  if(p_running) {
    delete p_running;
  }
}

/*
 * Window running interface
 */
gint gui_interface_timeout_callback(gpointer data)
{
  GUI_WINDOW_CALLBACK *p_callback = (GUI_WINDOW_CALLBACK *)data;
  bool ret = p_callback->process();
  if(ret == FALSE) {
    // Remove this callback?
    // gui_interface_end(data);
  }
  return(TRUE);
}

void gui_window_running::callback_add(GUI_WINDOW_CALBACK_CLASS type,
                                      GUI_FUNC_WINDOW_CALLBACK callback,
                                      int interval, 
                                      void *p_func_data)
{
  GUI_WINDOW_CALLBACK *p_callback = new GUI_WINDOW_CALLBACK(type, callback, interval, this, p_func_data);
  
  GUI_CALLBACK_HEAD *p_head = NULL;
  switch(type) {
    case CALLBACK_TIMER:
      p_head = &callbacks_time;
      p_callback->timer_start();
      break;
    case CALLBACK_APLICATION_END:
      p_head = &callbacks_end;
      break;
  }
  
  if(p_head)
    p_head->list_insert_last(p_callback);
}


/*
 * GUI_WINDOW_MOUSE_COMMAND window interface
 */
void gui_window_mouse_command::window_command_register(GUI_FUNC_WINDOW_CALLBACK command, int button, int state)
{
  assert(button >= 0 && button < GUI_MOUSE_BUTTONS);  
  GUI_FUNC_WINDOW_CALLBACK *p_list = (state) ? pressed : released;  
  p_list[button] = command;
}

void gui_window_mouse_command::window_command_clear(int button, int state)
{
  assert(button >= 0 && button < GUI_MOUSE_BUTTONS);  
  GUI_FUNC_WINDOW_CALLBACK *p_list = (state) ? pressed : released;  
  p_list[button] = NULL;  
}

void gui_window_mouse_command::window_mouse_position_set(tpos mx, tpos my)
{  
  x = mx;
  y = my;
}

void gui_window_mouse_command::window_mouse_position_get(tpos *p_mx, tpos *p_my)
{
  *p_mx = x;
  *p_my = y;
}

void gui_window_mouse_command::window_mouse_button_set(int button, int state)
{
  assert(button >= 0 && button < GUI_MOUSE_BUTTONS);
  buttons[button] = state;
  
  GUI_FUNC_WINDOW_CALLBACK *p_list = (state) ? pressed : released;
  
  if(p_list[button]) {
    bool keep_it = p_list[button](this,NULL);
    if(!keep_it)
      p_list[button] = NULL;
  }
}

bool gui_window_mouse_command::window_mouse_button_get(int button)
{
  assert(button >= 0 && button < GUI_MOUSE_BUTTONS);
  return(buttons[button]);
}

gui_window_mouse_command::gui_window_mouse_command(GUI_WINDOW_REGISTERED *p_registered_window)
  : gui_window_running(p_registered_window)
{
  command_state = COMMAND_NONE;
  memset(&pressed,0,sizeof(pressed));
  memset(&released,0,sizeof(released));
}

/*
 * GUI_WINDOW_RUNNING_AGE window interface
 */
bool gui_window_running_age::window_age_render_callback_active(void)
{
  return(p_render_callback != NULL);
}

void gui_window_running_age::window_age_render_callback_set(GUI_FUNC_WINDOW_CALLBACK p_callback)
{
  p_render_callback = p_callback;
}

void gui_window_running_age::window_age_render_callback_clear(void)
{
  p_render_callback = NULL;
}

void gui_window_running_age::window_age_render_callback_run(void)
{
  if(p_render_callback)
    p_render_callback(this,NULL);
}

// Mouse callback -> stores mouse events to window and calls the mouse commands
static gint gui_window_running_age_mouse_callback(GtkWidget *widget, GdkEvent *event, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_window = (GUI_WINDOW_RUNNING_AGE *)p_data;
  
  switch(event->type) {
    case GDK_MOTION_NOTIFY:
    {
      GdkEventMotion *mevent = (GdkEventMotion *)event;
      p_window->window_mouse_position_set((tpos)mevent->x,(tpos)mevent->y);
      break;
    }
    case GDK_BUTTON_PRESS:
    {
      GdkEventButton *bevent = (GdkEventButton *)event;
      p_window->window_mouse_button_set(bevent->button, TRUE);
      break;
    }
    case GDK_BUTTON_RELEASE:
    {
      GdkEventButton *bevent = (GdkEventButton *)event;
      p_window->window_mouse_button_set(bevent->button, FALSE);
      break;
    }
    case GDK_SCROLL:
    {
      GdkEventScroll *sevent = (GdkEventScroll *)event;
      
      int button = 0;
      
      if(sevent->direction == GDK_SCROLL_UP) {
        button = GUI_MOUSE_WHEEL_UP;
      }
      else if(sevent->direction == GDK_SCROLL_DOWN) {
        button = GUI_MOUSE_WHEEL_DOWN;
      }
      else {
        assert(0);
      }
      
      p_window->window_mouse_button_set(button,TRUE);
      p_window->window_mouse_button_set(button,FALSE);
      break;
    }
    default:
      return(FALSE);
  }
  return(TRUE);
}

void gui_window_running_age::window_age_mouse_callback_register(void)
{
  GtkWidget *p_area = window_age_get();
  assert(p_area);
    
  gtk_widget_add_events(GTK_WIDGET(p_area),
                        GDK_BUTTON_PRESS_MASK |
                        GDK_BUTTON_RELEASE_MASK |
                        GDK_POINTER_MOTION_MASK);
                        
  gtk_signal_connect(GTK_OBJECT(p_area), "button-press-event",
                     GTK_SIGNAL_FUNC(gui_window_running_age_mouse_callback), (void *)this);
  gtk_signal_connect(GTK_OBJECT(p_area), "button-release-event",
                     GTK_SIGNAL_FUNC(gui_window_running_age_mouse_callback), (void *)this);
  gtk_signal_connect(GTK_OBJECT(p_area), "scroll-event",
                     GTK_SIGNAL_FUNC(gui_window_running_age_mouse_callback), (void *)this);
  gtk_signal_connect(GTK_OBJECT(p_area), "motion-notify-event",
                     GTK_SIGNAL_FUNC(gui_window_running_age_mouse_callback), (void *)this);
}

/*
  Age window render callback 
    - calls all registered render callbacks 
    - render the age window scene
*/
static bool gui_window_running_age_window_render_callback(GUI_WINDOW_RUNNING *p_window, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_window_age = (GUI_WINDOW_RUNNING_AGE *)p_window;
  
  if(p_window_age->window_age_draw_request_get() || 
     p_window_age->window_age_render_callback_active()) {
       
    p_window_age->window_age_draw();    
  }
  
  return(TRUE);
}

void gui_window_running_age::window_age_window_render_callback_register(void)
{
  callback_add(CALLBACK_TIMER,gui_window_running_age_window_render_callback,1000/GUI_FPS);
}

static void gui_age_window_realize(GtkWidget *widget, void *p_data)
{
  GUI_WINDOW_RUNNING_AGE *p_window = (GUI_WINDOW_RUNNING_AGE *)p_data;
  
  bool render = gtk_gl_rendering(widget);
  assert(render);
    
  GRAPH3D *p_graph = p_window->graph_get();
  assert(p_graph);
  
  bool GL_interface = p_graph->create_GL();
  assert(GL_interface);
}

static gint gui_age_window_expose(GtkWidget *widget, GdkEventExpose *event, void *p_data)
{
  /* Draw only last expose. */
  if (event->count > 0)
    return(TRUE);

  GUI_WINDOW_RUNNING_AGE *p_window = (GUI_WINDOW_RUNNING_AGE *)p_data;  
  p_window->window_age_draw_request_set();
  return(TRUE);
}

static gboolean gui_age_window_configure(GtkWidget *widget, GdkEventConfigure *event, void *p_data)
{    
  GUI_WINDOW_RUNNING_AGE *p_window = (GUI_WINDOW_RUNNING_AGE *)p_data;
  p_window->window_age_resize(widget->allocation.width, widget->allocation.height);
  return(TRUE);
}

void gui_window_running_age::window_age_age_callback_register(void)
{
  GtkWidget *p_area = window_age_get();
  assert(p_area);  
  
  /* Events for widget must be set before X Window is created */
  gtk_widget_add_events(GTK_WIDGET(p_area), GDK_EXPOSURE_MASK);
  
  /* Connect signal handlers */
  /* Redraw image when exposed. */
  gtk_signal_connect(GTK_OBJECT(p_area), "expose_event",
                     GTK_SIGNAL_FUNC(gui_age_window_expose), (void *)this);
  /* When window is resized viewport needs to be resized also. */
  gtk_signal_connect(GTK_OBJECT(p_area), "configure_event",
                     GTK_SIGNAL_FUNC(gui_age_window_configure), (void *)this);
  /* Do initialization when widget has been realized. */
  gtk_signal_connect(GTK_OBJECT(p_area), "realize",
                     GTK_SIGNAL_FUNC(gui_age_window_realize), (void *)this);
}

// Register all age window callbacks (internals)
void gui_window_running_age::window_age_callback_register(void)
{
  window_age_mouse_callback_register();
  window_age_window_render_callback_register();
  window_age_age_callback_register();
}

void gui_window_running_age::window_age_draw_request_set(void)
{
  window_age_draw_request = TRUE;
}

bool gui_window_running_age::window_age_draw_request_get(void)
{
  return(window_age_draw_request);
}

void gui_window_running_age::window_age_draw_request_clear(void)
{
  window_age_draw_request = FALSE;
}

GtkWidget * gui_window_running_age::window_age_create(tpos width, tpos height)
{
  assert(p_age == NULL);
  
  p_age = new AGE_MAIN;
  
  p_age->graph_gtk_set(width, height);
  
  if(!p_age->graph_screen_create())
    return(NULL);
  
  window_age_callback_register();
  
  return(window_age_get());
}

GtkWidget * gui_window_running_age::window_age_get(void)
{
  return(p_age ? p_age->graph_gtk_window_get() : NULL);
}

void gui_window_running_age::window_age_resize(tpos width, tpos height)
{
  assert(p_age);    
  p_age->graph_screen_resize(width, height);
  window_age_draw_request_set();
}

void gui_window_running_age::window_age_destroy(void)
{
  
}

bool gui_window_running_age::window_age_draw(void)
{
  window_age_render_callback_run();
  
  if(!p_age || !gtk_gl_rendering(window_age_get()))
    return(FALSE);
  
  if(!p_age->draw()) {
    return(FALSE);
  }
  
  window_age_draw_request_clear();
  
  return(TRUE);
}

/*
 * AGE input interface
 */
gui_window_running_age::gui_window_running_age(GUI_WINDOW_REGISTERED *p_registered_window)
  : gui_window_mouse_command(p_registered_window)
{
  p_age = NULL;
  window_age_draw_request = TRUE;
  p_render_callback = NULL;
}

gui_window_running_age::~gui_window_running_age(void)
{
  
}

/*
 * GUI interface main class
 */
/*
EDITOR * gui_window_running::editor_get(void)
{
  GUI_INTERFACE *p_int = interface_get();
  return(p_int ? p_int->editor_get() : NULL);
}

EDITOR_STATE_GUI * gui_window_running::state_gui_get(void)
{
  EDITOR *p_editor = editor_get();
  if(p_editor)
    return(p_editor->state_gui_get());
  
  return(NULL);
}
*/
GUI_WINDOW_REGISTERED * gui_interface::window_new(const char *p_name)
{
  GUI_WINDOW_REGISTERED *p_new_window = new GUI_WINDOW_REGISTERED(this);
  p_new_window->name_set(p_name);
  windows_registered.object_list_insert_last(p_new_window);
  return(p_new_window);
}

GUI_WINDOW_REGISTERED * gui_interface::window_get(const char *p_name)
{
  return((GUI_WINDOW_REGISTERED *)windows_registered.object_list_find(p_name));
}

GUI_WINDOW_RUNNING * gui_interface::window_create(const char *p_name)
{
  GUI_WINDOW_REGISTERED *p_registered_window;

  /*
   * Try to find this window
   */
  p_registered_window = (GUI_WINDOW_REGISTERED *)windows_registered.object_list_find(p_name);
  if(p_registered_window == NULL)
    return(FALSE);
  
  /*
   * Create a new window
   */
  return(p_registered_window->create());
}

bool gui_interface::window_destroy(const char *p_name)
{
  GUI_WINDOW_REGISTERED *p_registered_window;
    
  /*
   * Try to find this window
   */
  p_registered_window = (GUI_WINDOW_REGISTERED *)windows_registered.object_list_find(p_name);  
  if(p_registered_window == NULL)
    return(FALSE);
  
  /*
   * Destroy this window
   */
  return(p_registered_window->destroy());
}

void gui_interface::create(void)
{
  GUI_WINDOW_REGISTERED *p_window;
  
  p_window = window_new("editor_main");
  p_window->func_set(editor_main_gui_create,
                     editor_main_gui_destroy);
  
  p_window = window_new("editor_new_land_generator");
  p_window->func_set(land_new_generator_gui_create,
                     land_new_generator_gui_destroy);
  
  p_window = window_new("editor_land_heightmap_modificator");
  p_window->func_set(land_height_map_modificator_gui_create,
                     land_height_map_modificator_gui_destroy);
  
  p_window = window_new("editor_land_height_fractal");
  p_window->func_set(land_height_fractal_gui_create,
                     land_height_fractal_gui_destroy);
}

void gui_interface::destroy(void)
{
  
}

void gui_interface::run(void)
{
  gtk_main();
}

gui_interface::gui_interface(EDITOR *p_editor)
  : editor_reference(p_editor)
{
  create();
}

gui_interface::~gui_interface(void)
{
  destroy();
}
