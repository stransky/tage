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
 * Editor main class implementation - editor/test environment
 */
#include "editor.h"

editor::editor(void)
  : editor_reference(this),
    gen(this),
    gui(this),
    state(this)
{
  // Set editor regerence for menu commands
  editor_reference_set(this);
}

editor::~editor(void)
{
}

void editor::process_help(void)
{  
  pprintf("\
TAGE (C) Martin Stransky <stransky@anakreon.cz> \n\n\
This program is free software; you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation; either version 2 of the License, or\n\
(at your option) any later version.\n\n\
Usage: \n\
  tage [OPTIONS] tage_script\n\n\
Options:\n\
  -t  terminal only\n\
  -g  gui mode\n\
");  
}

bool editor::process_params(int argc, char **argv)
{ 
  bool file_params = FALSE;
  
  if(argc > 1) {  
    int i;
    for(i = 0; i < argc; i++) {
      char *p_param = argv[i];
      
      if(!strcasecmp("-h",p_param) || !strcasecmp("--help",p_param)) {
        process_help();
        return(FALSE);
      } else if(!strcasecmp("-t",p_param)) {
        state.config.gui_enabled = FALSE;
      } else if(!strcasecmp("-g",p_param)) {
        state.config.gui_enabled = TRUE;
      } else if(i+1 == argc) {
        strncpy(state.config.script_file, p_param, MAX_FILENAME);
        file_params = TRUE;
      }
    }
  }
  
  if(!file_params) {
    process_help();
    return(FALSE);
  }
  
  return(TRUE);
}

void editor::configure(void)
{
  AGE_MAIN *p_age;
  
#ifdef ENABLE_GUI
  if(state.config.gui_enabled) {
    GUI_INTERFACE *p_interface = gui_interface_get();
    p_age = p_interface->age_get();
  } else
#endif
    p_age = new AGE_MAIN;  
  
  SCENE *p_scene = p_age->scene_new();
  p_age->scene_active_set(p_scene);
  
  GRAPH3D *p_grf = p_age->graph_get();
  if(p_grf) {
    p_grf->config_draw_grid(FALSE);
    p_grf->config_draw_mouse_cursor(FALSE);
    p_grf->config_draw_boxes(FALSE);
    p_grf->config_draw_console(FALSE);
    p_grf->config_draw_fps(FALSE);
    p_grf->config_draw_normals(FALSE);
    p_grf->config_opengl_lighting(FALSE);
    p_grf->config_draw_all_objects(TRUE);
    p_grf->config_draw_pivots(TRUE);
    p_grf->config_draw_selection(TRUE);
    //p_grf->config_render_mode_set(RENDER_WIRED);
  }
  
  GENERATOR *p_gen = generator_get();
  GENERATOR_CONFIG *p_config = p_gen->config_get();
  
  char tmp[MAX_FILENAME];
  strncpy(tmp, state.config.script_file, MAX_FILENAME);
  p_config->output_file_set(tail_change(tmp, ".bmp"));
  
  p_gen->load(state.config.script_file);
  p_gen->run(p_scene);
  
  p_scene->update();
}

void editor::run(int argc, char **argv)
{
#ifdef ENABLE_GTK_BACKEND
  gtk_init(&argc, &argv);
#endif
  
  if(!process_params(argc, argv)) {
    exit(0);
  }
  
#ifdef ENABLE_GUI
  if(state.config.gui_enabled)
    gui.window_create("editor_main");
#endif
  
  configure();
  
#ifdef ENABLE_GUI
  if(state.config.gui_enabled) {
    editor_command_scene_reset();
    gui.run();
  }
#endif
}

/*
 * Main
 */
int main(int argc, char **argv)
{
  EDITOR *p_editor = new EDITOR;
  
  p_editor->run(argc,argv);
  
  delete p_editor;
}
