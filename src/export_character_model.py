import bpy
import os
import mathutils

"""
Character model export script

- Change export_rel_path to an appropriate name for the exported model
- select all the model parts to be exported (but not the Armature object itself)
- run script



Works best when mesh origins align with bone origins. To fix broken transforms/poorly tracking parts: 
- run "Apply Object Transform"
- run "Set Origin" > "Origin to Geometry"
- to improve further, for each part, select bone in pose mode and choose "Snap Cursor to Selected" then select the child object and "Set Origin" > "Origin to 3d Cursor"
"""

export_rel_path = "../gooserig.obj"  # export goose
# export_rel_path = "../characterrig.obj"  # export character

blend_file_path = bpy.data.filepath
directory = os.path.dirname(blend_file_path)
target_file = os.path.realpath(os.path.join(directory, export_rel_path))

print("target_file", target_file)


assert (
    len(bpy.context.selected_objects) > 0
), "No objects selected, you must select some objects to use this script"

bpy.ops.ed.undo_push()

for obj in bpy.context.selected_objects:
    print("saving", obj.matrix_world)
    obj.matrix_world = mathutils.Matrix()
    obj.animation_data_clear()

bpy.ops.export_scene.obj(
    filepath=target_file,
    check_existing=True,
    axis_forward="Y",
    axis_up="Z",
    filter_glob="*.obj;*.mtl",
    use_selection=True,
    use_animation=False,
    use_mesh_modifiers=True,
    use_edges=True,
    use_smooth_groups=False,
    use_smooth_groups_bitflags=False,
    use_normals=True,
    use_uvs=True,
    use_materials=True,
    use_triangles=True,
    use_nurbs=False,
    use_vertex_groups=False,
    use_blen_objects=True,
    group_by_object=False,
    group_by_material=False,
    keep_vertex_order=False,
    global_scale=1,
    path_mode="STRIP",
)

bpy.ops.ed.undo()
