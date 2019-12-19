import bpy
import os
import mathutils

"""
Character model export script

- Change export_rel_path to an appropriate name for the exported model
- select all the model parts to be exported (but not the Armature object itself)
- run script
"""

# export_rel_path = "../gooserig.obj" # export goose
export_rel_path = "../characterrig.obj"  # export character

blend_file_path = bpy.data.filepath
directory = os.path.dirname(blend_file_path)
target_file = os.path.realpath(os.path.join(directory, export_rel_path))

print("target_file", target_file)

objects = []
for obj in bpy.context.selected_objects:
    print("saving", obj.matrix_world)
    objects.append((obj, obj.matrix_world.copy()))
    obj.matrix_world = mathutils.Matrix()

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

for obj, original_position in objects:
    print("restoring", original_position)
    obj.matrix_world = original_position
