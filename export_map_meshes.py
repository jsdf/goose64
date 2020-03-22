import bpy

import mathutils

from bpy.props import StringProperty
from bpy_extras.io_utils import ExportHelper
from bpy.types import Operator

"""
Ground model export script

- select all the ground parts
- run script
"""

# from https://sinestesia.co/blog/tutorials/using-blenders-filebrowser-with-python/
class OpenFilebrowser(Operator, ExportHelper):
    bl_idname = "export_ground.open_filebrowser"
    bl_label = "Open the file browser (yay)"

    filename_ext = ".obj"
    filter_glob: StringProperty(default="*.obj", options={"HIDDEN"})

    def execute(self, context):
        """Do something with the selected file(s)."""
        do_export(self.filepath)
        return {"FINISHED"}


def register():
    bpy.utils.register_class(OpenFilebrowser)


def unregister():
    bpy.utils.unregister_class(OpenFilebrowser)


register()
bpy.ops.export_ground.open_filebrowser("INVOKE_DEFAULT")


def do_export(target_file):

    print("target_file", target_file)

    assert (
        len(bpy.context.selected_objects) > 0
    ), "No objects selected, you must select some objects to use this script"

    objects = []
    for obj in bpy.context.selected_objects:
        print("saving", obj.matrix_world)
        objects.append((obj, obj.matrix_world.copy()))
        obj.matrix_world = mathutils.Matrix()

    bpy.ops.export_scene.obj(
        filepath=target_file,
        check_existing=True,
        axis_forward="-Z",
        axis_up="Y",
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
        group_by_object=True,
        group_by_material=False,
        keep_vertex_order=False,
        global_scale=1,
        path_mode="STRIP",
    )

    for obj, original_position in objects:
        print("restoring", original_position)
        obj.matrix_world = original_position
