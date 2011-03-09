#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdlib.h>

#include "editor.h"

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_quit();
}

void
on_scene_center1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_reset();
}


void
on_scene_move1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_move();
}


void
on_scene_rotate1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_rotate();
}


void
on_scene_zoom_m1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_zoom_in();
}


void
on_scene_zoom_p1_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_zoom_out();
}


void
on_mesh_move1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_mesh_rotate1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_mesh_zoom1_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_preferences1_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_new_land_button1_pressed            (GtkButton       *button,
                                        gpointer         user_data)
{  
  land_new_generator_gui_lauch_button(GTK_WIDGET(button));
}


void
on_height_map_load_button2_pressed     (GtkButton       *button,
                                        gpointer         user_data)
{
}

void
on_height_map_button7_pressed          (GtkButton       *button,
                                        gpointer         user_data)
{
  land_height_map_modificator_gui_apply(GTK_WIDGET(button));
}

void
on_clear1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_new();
}


void
on_load1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_load();
}

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_save();
}

void
on_save_as_1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_save_as();
}

void
on_selection_set_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_select_set();
}

void
on_selection_add_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_select_add();
}

void
on_selection_clear_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_select_clear();
}

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}

void
on_reset1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_reset();
}


void
on_move1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_move();
}


void
on_rotate1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_rotate();
}


void
on_zoom_in1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_zoom_in();
}


void
on_zoom_out1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_scene_zoom_out();
}


void
on_set1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_add1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_clear2_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_rendering_points_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_POINTS);
}

void
on_rendering_wired_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_WIRED);
}

void
on_rendering_solid_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_SOLID);
}

void
on_rendering_textured_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_TEXTURED);
}


void
on_points1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_POINTS);
}


void
on_wired1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_WIRED);
}


void
on_solid1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_SOLID);
}


void
on_textured1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  editor_command_rendering_mode(RENDER_TEXTURED);
}

void
on_view2_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_objects1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{  
  GUI_STATE *p_state = editor_reference_get()->gui_state_get();
  p_state->toolbox_active_set(TOOLBOX_SCENE);
}


void
on_modificators2_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GUI_STATE *p_state = editor_reference_get()->gui_state_get();
  p_state->toolbox_active_set(TOOLBOX_MODIFICATOR);
}
