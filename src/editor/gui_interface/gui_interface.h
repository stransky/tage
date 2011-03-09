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

#ifndef __GUI_INTERFACE_H__
#define __GUI_INTERFACE_H__

#include <gtk/gtk.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GUI_FPS    30

 
/*
 * GUI class prototypes
 */
class gui_callback_head;
typedef class gui_callback_head GUI_CALLBACK_HEAD;

class gui_window_registered;
typedef class gui_window_registered GUI_WINDOW_REGISTERED;

class gui_window_running;
typedef class gui_window_running GUI_WINDOW_RUNNING;

class gui_interface;
typedef class gui_interface GUI_INTERFACE;

/*
 * GUI function prototypes
 */
typedef GUI_WINDOW_RUNNING *  (* GUI_FUNC_WINDOW_CREATE)(GUI_WINDOW_REGISTERED *p_registered);
typedef bool                  (* GUI_FUNC_WINDOW_DESTROY)(GUI_WINDOW_REGISTERED *p_registered);
typedef bool                  (* GUI_FUNC_WINDOW_CALLBACK)(GUI_WINDOW_RUNNING *p_window, void *p_data);

/*
 * Registered GUI window
 */
typedef class gui_window :
  public object_list
{ 
  
public:
  
  gui_window(void)
  : object_list(NULL)
  {
  }

} GUI_WINDOW;

typedef class gui_window_registered :
  public gui_window
{ 

  /*
   * Interface reference
   */  
private:
  
  GUI_INTERFACE * p_interface;

public:
  
  GUI_INTERFACE * gui_interface_get(void)
  {
    return(p_interface);
  }
  
  /*
   * Is this window active (running)?
   */  
private:
  
  GUI_WINDOW_RUNNING *p_running;
  
public:

  bool is_running(void)
  {
    return(p_running != NULL);
  }
  
  GUI_WINDOW_RUNNING * running_get(void)
  {
    return(p_running);
  }
  
  /*
   * Create/destroy func
   */  
private:
  
  GUI_FUNC_WINDOW_CREATE  func_create;
  GUI_FUNC_WINDOW_DESTROY func_destroy;
  
public:
  
  void func_set(GUI_FUNC_WINDOW_CREATE create, GUI_FUNC_WINDOW_DESTROY destroy)
  {
    func_create = create;
    func_destroy = destroy;
  }

public:

  GUI_WINDOW_RUNNING * create(void);
  bool destroy(void);  

public:
  
  gui_window_registered(GUI_INTERFACE *p_interface_reference);  
  ~gui_window_registered(void);
 
} GUI_WINDOW_REGISTERED;

/*
 * Running GUI window
 */

typedef enum {
  
  CALLBACK_TIMER,
  CALLBACK_APLICATION_END
  
} GUI_WINDOW_CALBACK_CLASS;

gint gui_interface_timeout_callback(gpointer data);

typedef class gui_window_callback
  : public llist_item
{ 
  GUI_WINDOW_CALBACK_CLASS     timer_type;
  GUI_FUNC_WINDOW_CALLBACK     timer_func;
  guint32                      timer_interval;  
  
  GUI_WINDOW_RUNNING          *p_window;
  void                        *p_data;

private:
  
  guint32                      gtk_timer_handle;

public:

  void timer_start(void)
  {
    if(!gtk_timer_handle) {
      gtk_timer_handle = g_timeout_add(timer_interval,gui_interface_timeout_callback,this);
    }
  }
  
  void timer_stop(void)
  {
    if(gtk_timer_handle) {
      g_source_remove(gtk_timer_handle);
      gtk_timer_handle = 0;
    }
  }

public:
  
  bool process(void)
  {
    return(timer_func(p_window,p_data));
  }
    
public:

  gui_window_callback(GUI_WINDOW_CALBACK_CLASS type,
                      GUI_FUNC_WINDOW_CALLBACK callback,
                      guint32 interval,
                      GUI_WINDOW_RUNNING *p_window,
                      void *p_func_data = NULL)
  {
    timer_type = type;
    timer_func = callback;
    timer_interval = interval;
        
    this->p_window = p_window;
    p_data = p_func_data;
    
    gtk_timer_handle = 0;
  }
  
} GUI_WINDOW_CALLBACK;

typedef class gui_callback_head
  : public llist_head
{
  
} GUI_CALLBACK_HEAD;

typedef class gui_window_running :
  public gui_window
{

  /*
   * Registered window
   */  
private:  
  
  GUI_WINDOW_REGISTERED *p_registered;
  
public:
  
  GUI_WINDOW_REGISTERED * registered_get(void)
  {
    return(p_registered);
  }
  
  GUI_INTERFACE * gui_interface_get(void)
  {
    return(p_registered ? p_registered->gui_interface_get() : NULL);
  }
  
  /*
   * Running gtk window
   */  
private:
  
  GtkWidget * gtk_window;

public:

  GtkWidget * gtk_window_get(void)
  {
    return(gtk_window);
  }
  
  void gtk_window_set(GtkWidget * window)
  {
    gtk_window = window;
  }
  
  void gtk_window_destroy(void)
  {
    if(gtk_window) {
      gtk_widget_destroy(gtk_window);
      gtk_window = NULL;
    }
  }
  
  /*
   * General callbacks
   */
public:  

  GUI_CALLBACK_HEAD callbacks_time;
  GUI_CALLBACK_HEAD callbacks_end;
    
public:
  
  void callback_add(GUI_WINDOW_CALBACK_CLASS type, 
                    GUI_FUNC_WINDOW_CALLBACK callback, 
                    int interval, 
                    void *p_func_data = NULL);

public:
  
  gui_window_running(GUI_WINDOW_REGISTERED *p_registered_window)
  {
    p_registered = p_registered_window;
    gtk_window = NULL;
  }
  
  ~gui_window_running(void)
  {    
    gtk_window_destroy();
  }
 
} GUI_WINDOW_RUNNING;

typedef class gui_window_head
  : public object_list_head
{

public:
  
  gui_window_head(void)
  : object_list_head(NULL)
  {
  }
  
} GUI_WINDOW_HEAD;

/*
 * Current state of mouse for this window
 */

#define GUI_MOUSE_BUTTONS     6

typedef struct gui_window_mouse_state {

  tpos x,y;
  int  buttons[GUI_MOUSE_BUTTONS];

public:
  
  gui_window_mouse_state(void)
  {
    x = y = 0;
    memset(buttons,0,sizeof(buttons));
  }
  
} GUI_WINDOW_MOUSE_STATE;


/*
 * Window mouse commands
 */
#define GUI_MOUSE_LEFT        1
#define GUI_MOUSE_MIDDLE      2
#define GUI_MOUSE_RIGHT       3
#define GUI_MOUSE_WHEEL_UP    4
#define GUI_MOUSE_WHEEL_DOWN  5

typedef enum {
  
  COMMAND_NONE = 0,
  COMMAND_WAITING_FOR_START,
  COMMAND_RUNNING
  
} GUI_WINDOW_COMMAND_STATE;

typedef enum {
  
  BUTTON_RELEASED = 0,
  BUTTON_PRESSED,
  
} GUI_WINDOW_COMMAND_BUTTON_STATE;

typedef class gui_window_mouse_command :
  public  gui_window_running,
  private gui_window_mouse_state
{
  
private:

  GUI_WINDOW_COMMAND_STATE    command_state;
  GUI_FUNC_WINDOW_CALLBACK    pressed[GUI_MOUSE_BUTTONS];
  GUI_FUNC_WINDOW_CALLBACK    released[GUI_MOUSE_BUTTONS];

public:
  
  void        window_command_register(GUI_FUNC_WINDOW_CALLBACK command, int button, int state);
  void        window_command_clear(int button, int state);

public:
  
  void        window_mouse_position_set(tpos mx, tpos my);
  void        window_mouse_position_get(tpos *p_mx, tpos *p_my);
  void        window_mouse_button_set(int button, int state);
  bool        window_mouse_button_get(int button);
  
public:
  
  gui_window_mouse_command(GUI_WINDOW_REGISTERED *p_registered_window);

} GUI_WINDOW_MOUSE_COMMAND;

typedef class gui_window_running_age : 
  public gui_window_mouse_command  
{

  AGE_MAIN           *p_age;

public:

  AGE_MAIN * age_get(void)
  {
    return(p_age);
  }

  GRAPH3D * graph_get(void)
  {
    AGE_MAIN *p_age = age_get();
    return(p_age ? p_age->graph_get() : NULL);
  }

  SCENE * scene_get(void)
  {
    AGE_MAIN *p_age = age_get();
    return(p_age != NULL ? p_age->scene_get() : NULL);
  } 
  
  /*
   * AGE window interface
   */
public:

  GtkWidget * window_age_create(tpos width, tpos height);
  GtkWidget * window_age_get(void);
  void        window_age_resize(tpos width, tpos height);
  void        window_age_destroy(void);
  bool        window_age_draw(void);

  /*
   * AGE window draw request
   */
private:

  bool        window_age_draw_request;

public:
  
  void        window_age_draw_request_set(void);
  bool        window_age_draw_request_get(void);

private:
  
  void        window_age_draw_request_clear(void);

  /*
   * AGE window render callback - is called before
   * gui renders the age window so you can modify the age scene
   * from it
   */
private:
  
  GUI_FUNC_WINDOW_CALLBACK p_render_callback;

public:

  bool        window_age_render_callback_active(void);
  void        window_age_render_callback_set(GUI_FUNC_WINDOW_CALLBACK p_callback);
  void        window_age_render_callback_clear(void);

private:

  void        window_age_render_callback_run(void);

  /*
   * Internal gui callbacks
   * They handle window rendering, mouse input and mouse commands
   */
private:

  void        window_age_mouse_callback_register(void);
  void        window_age_window_render_callback_register(void);
  void        window_age_age_callback_register(void);
  void        window_age_callback_register(void);

public:

  gui_window_running_age(GUI_WINDOW_REGISTERED *p_registered_window);
  ~gui_window_running_age(void);

} GUI_WINDOW_RUNNING_AGE;

/*
 * GUI references
 */
typedef class gui_reference
{
  /*
   * Main GUI window
   */  
  GUI_WINDOW_RUNNING_AGE * p_main_window;
  
public:
  
  void gui_main_window_set(GUI_WINDOW_RUNNING_AGE *p_window)
  {
    p_main_window = p_window;
  }
    
  GUI_WINDOW_RUNNING_AGE * gui_main_window_get(void)
  {
    return(p_main_window);
  }

  GtkWidget * gtk_main_window_get(void)
  {
    if(p_main_window)
      return(p_main_window->gtk_window_get());
    
    return(NULL);
  }
  
  AGE_MAIN * age_get(void)
  {          
    return(p_main_window != NULL ? p_main_window->age_get() : NULL);
  }
  
  GRAPH3D * graph_get(void)
  {
    return(p_main_window != NULL ? p_main_window->graph_get() : NULL);
  }
  
  SCENE * scene_get(void)
  {
    return(p_main_window != NULL ? p_main_window->scene_get() : NULL);
  }
  
public:
  
  gui_reference(void)
  {
    p_main_window = NULL;
  }
  
} GUI_REFERENCE;

/*
 * Main GUI class
 */
typedef class gui_interface :
  public editor_reference,
  public gui_reference
{  
  /*
   * GUI main window
   */
private:
  
  GUI_WINDOW_HEAD windows_registered;

public:

  GUI_WINDOW_REGISTERED * window_new(const char *p_name);
  GUI_WINDOW_REGISTERED * window_get(const char *p_name);

public:
  
  GUI_WINDOW_RUNNING * window_create(const char *p_name);
  bool window_destroy(const char *p_name);

public:
  
  void run(void);

public:
  
  void create(void);
  void destroy(void);

public:

  gui_interface(EDITOR *p_editor);
  ~gui_interface(void);

} GUI_INTERFACE;


#endif // __GUI_INTERFACE_H__
