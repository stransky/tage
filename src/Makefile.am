## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

INCLUDES =\
	 -Iage/graph -Iage/include -Iage/utils -Iage/support -Ieditor -Ieditor/generator -Ieditor/gui_glade -Ieditor/gui_commands -Ieditor/gui_interface -Ieditor/gui_windows -Ieditor/gui_sdl

AM_CXXFLAGS =\
	 -Wall\
	 -g

AM_CXXFLAGS += $(TAGE_GTK_CFLAGS)
AM_CXXFLAGS += $(TAGE_GLAREA_CFLAGS)

bin_PROGRAMS = tage

tage_SOURCES = \
	age/graph/age.cpp\
	age/graph/age.h\
	age/graph/alpha.cpp\
	age/graph/alpha.h\
	age/graph/bitmap.cpp\
	age/graph/bitmap.h\
	age/graph/camera.h\
	age/graph/color.h\
	age/graph/glwrapper.cpp\
	age/graph/glwrapper.h\
	age/graph/gpipe.cpp\
	age/graph/gpipe.h\
	age/graph/graph3d.cpp\
	age/graph/graph3d.h\
	age/graph/material.cpp\
	age/graph/material.h\
	age/graph/matrix.cpp\
	age/graph/matrix.h\
	age/graph/mesh.cpp\
	age/graph/mesh.h\
	age/graph/obb.cpp\
	age/graph/obb.h\
	age/graph/quat.h\
	age/graph/scene.cpp\
	age/graph/scene.h\
	age/graph/texture_func.cpp\
	age/graph/texture_func.h\
	age/graph/vector.h\
	age/include/defs.h\
	age/include/floats.h\
	age/include/opengl.h\
	age/include/typ.h\
	age/support/timer.cpp\
	age/support/timer.h\
	age/utils/ini.cpp\
	age/utils/ini.h\
	age/utils/stack.h\
	age/utils/types.h\
	age/utils/utils.cpp\
	age/utils/utils.h\
	age/graph/vertex_arrays.cpp\
	age/graph/vertex_arrays.h\
	age/graph/gext.cpp\
	age/graph/gext.h\
	age/graph/surface.cpp\
	age/graph/surface.h\
	age/include/typ_graph.h\
	age/support/events.cpp\
	age/support/events.h\
	age/support/events_types.h\
	age/support/input.cpp\
	age/support/input.h\
	age/graph/animation.cpp\
	age/graph/animation.h\
	age/utils/llist.h\
	age/utils/mtree.cpp\
	age/utils/mtree.h\
	age/graph/object_hierarchy.cpp\
	age/graph/object_hierarchy.h\
	age/graph/object_list.cpp\
	age/graph/object_list.h\
	age/graph/object_render_list.cpp\
	age/graph/object_render_list.h\
	age/graph/scene_reference.h\
	age/graph/camera.cpp\
	age/graph/camera_interface.cpp\
	age/graph/camera_interface.h\
	age/graph/world_interface.cpp\
	age/graph/world_interface.h\
	age/utils/flag.h\
	age/graph/matrix_notified.cpp\
	age/graph/matrix_notified.h\
	age/graph/matrix_animated.cpp\
	age/graph/matrix_animated.h\
	age/include/defs_config.h\
	age/graph/object_transformation.cpp\
	age/graph/object_transformation.h\
	age/graph/object_mark.h\
	age/graph/texture_unit.cpp\
	age/graph/texture_unit.h\
	age/graph/material_texture.cpp\
	age/graph/material_texture.h\
	age/support/age_main.cpp\
	age/support/age_main.h\
	age/graph/triangle.cpp\
	age/graph/triangle.h\
	age/graph/draw.cpp\
	age/graph/draw.h\
	age/graph/aabb.cpp\
	age/graph/aabb.h\
	age/graph/object_world.cpp\
	age/graph/object_world.h\
	age/graph/object_box.cpp\
	age/graph/object_box.h\
	age/graph/box.cpp\
	age/graph/box.h\
	age/utils/lock.h\
	age/graph/scene_import_3ds.cpp\
	age/graph/scene_import_3ds.h\
	age/utils/change.h\
	age/graph/object_base.cpp\
	age/graph/object_base.h\
	age/graph/object_geometry.cpp\
	age/graph/object_geometry.h\
	age/graph/animation_track.cpp\
	age/graph/animation_track.h\
	age/graph/animation_track_bezier.cpp\
	age/graph/animation_track_bezier.h\
	age/graph/animation_track_linear.cpp\
	age/graph/animation_track_linear.h\
	age/graph/animation_time.cpp\
	age/graph/animation_time.h\
	age/utils/llist_named.h\
	age/utils/change.cpp\
	age/graph/print.cpp\
	age/graph/print.h\
	age/graph/normals.cpp\
	age/graph/normals.h\
	age/graph/scene_import_b2m.cpp\
	age/graph/scene_import_b2m.h\
	age/graph/color_helpers.h\
	editor/generator/generator.cpp\
	editor/generator/generator.h\
	age/graph/selection.cpp\
	age/graph/selection.h\
	age/graph/intersect.cpp\
	age/graph/intersect.h\
	age/graph/vector.cpp\
	age/graph/vector_operators.h\
	age/graph/color.cpp\
	age/graph/color_operators.h\
	age/graph/lighting.cpp\
	age/graph/lighting.h\
	age/utils/array.h\
	age/utils/data_file.cpp\
	age/utils/data_file.h\
	editor/generator/generator_load_helpers.cpp\
	editor/generator/generator_load_helpers.h\
	editor/generator/generator_random.cpp\
	editor/generator/generator_random.h\
	editor/generator/generator_mesh.cpp\
	editor/generator/generator_mesh.h\
	editor/gui_commands/editor_commands.cpp\
	editor/gui_commands/editor_commands.h\
	editor/gui_glade/callbacks.cpp\
	editor/gui_glade/callbacks.h\
	editor/gui_glade/interface.cpp\
	editor/gui_glade/interface.h\
	editor/gui_glade/support.cpp\
	editor/gui_glade/support.h\
	editor/gui_interface/gui_interface.cpp\
	editor/gui_interface/gui_interface.h\
	editor/gui_windows/gui_helpers.cpp\
	editor/gui_windows/gui_helpers.h\
	editor/gui_windows/gui_state.cpp\
	editor/gui_windows/gui_state.h\
	editor/gui_windows/gui_window_main.cpp\
	editor/gui_windows/gui_window_main.h\
	editor/gui_windows/gui_window_misc.cpp\
	editor/gui_windows/gui_window_misc.h\
	editor/gui_windows/gui_window_toolbox_geometry.cpp\
	editor/gui_windows/gui_window_toolbox_geometry.h\
	editor/gui_windows/gui_window_toolbox_materials.cpp\
	editor/gui_windows/gui_window_toolbox_materials.h\
	editor/gui_windows/gui_window_toolbox_modificator.cpp\
	editor/gui_windows/gui_window_toolbox_modificator.h\
	editor/editor_main.cpp\
	editor/editor_main.h\
	editor/editor.h\
	editor/editor_reference.h\
	editor/editor_reference.cpp\
	editor/gui_windows/gui_window_toolbox_scene.cpp\
	editor/gui_windows/gui_window_toolbox_scene.h\
	age/graph/object_name.h\
	age/graph/object_select.h\
	age/utils/mtree_named.h\
	age/graph/object_scene.cpp\
	age/graph/object_scene.h\
	age/include/compatibility.h\
	age/graph/polar.cpp\
	age/graph/polar.h\
	editor/generator/generator_mesh_helper.cpp\
	editor/generator/generator_mesh_helper.h\
	editor/generator/interval.cpp\
	editor/generator/interval.h\
	editor/generator/generator_coordinates.cpp\
	editor/generator/generator_coordinates.h\
	editor/generator/generator_target.cpp\
	editor/generator/generator_target.h\
	editor/generator/generator_target_geometry.cpp\
	editor/generator/generator_target_geometry.h\
	editor/generator/generator_target_texture.cpp\
	editor/generator/generator_target_texture.h\
	editor/generator/generator_modificator.cpp\
	editor/generator/generator_modificator.h\
	editor/generator/generator_reference.h\
	age/graph/scene_import_txt.cpp\
	age/graph/scene_import_txt.h\
	age/graph/surface_sdl.cpp\
	age/graph/surface_sdl.h\
	age/graph/surface_height.cpp\
	age/graph/surface_height.h\
	age/graph/surface_sw.cpp\
	age/graph/surface_sw.h\
	age/utils/barray.h\
	age/graph/surface_mask.cpp\
	age/graph/surface_mask.h\
	age/graph/ferguson.cpp\
	age/graph/ferguson.h\
	age/graph/color_hsv.cpp\
	age/graph/color_hsv.h\
	editor/generator/generator_modificator_mask.cpp \
	editor/generator/generator_modificator_mask.h \
	editor/generator/generator_target_modificator.cpp \
	editor/generator/generator_target_modificator.h \
	editor/generator/generator_modificator_params.cpp\
	editor/generator/generator_modificator_params.h \
	editor/generator/params.c \
	editor/generator/params.h \
	editor/generator/y.tab.c \
	editor/generator/y.tab.h \
	age/utils/mem_alloc.cpp \
	age/utils/mem_alloc.h \
	age/utils/log.cpp \
	age/utils/log.h

tage_LDFLAGS = $(TAGE_GTK_LIBS)
tage_LDFLAGS += $(TAGE_GLAREA_LIBS)

tage_LDADD = \
	 -lSDL -lSDL_image -lGL -lGLU -lz

