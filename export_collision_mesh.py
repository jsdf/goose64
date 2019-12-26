import bpy
import re

"""
exports a level to a collision mesh header file ready to be included in the game code

before exporting:
- ensure the blender file has a collection called "collision"
- you can merge all the objects into a single collision mesh with .join() (maybe not needed)
- be sure to "apply object transform" on the collision mesh so its origin is at the world origin

"""

# we scale the models up by this much to avoid n64 fixed point precision issues
N64_SCALE_FACTOR = 30

filename_base = "university_map"
filename = filename_base + "_collision"

# name without numerical suffix like .001
def get_unsuffixed_name(name):
    return re.sub(r"\.\d\d\d$", "", name)


# is something that moves and thus should not be added to our static collision mesh
def is_dynamic_object(name):
    unsuffixed_name = get_unsuffixed_name(name)
    return unsuffixed_name.endswith("Item") or unsuffixed_name.endswith("Character")


include_guard = filename.upper() + "_H"
# world_objects = list(bpy.data.collections["worldobjects"].all_objects)
# collision_objects = [obj for obj in world_objects if not is_dynamic_object(obj.name)]
collision_objects = list(bpy.data.collections["collision"].all_objects)

triangles = []
for index, obj in enumerate(collision_objects):
    mesh = obj.data

    for poly in mesh.polygons:
        assert (
            len(poly.vertices) == 3
        ), "you need to triangulate the collision mesh before exporting it"
        tri_verts = []
        for vert_idx in poly.vertices:
            vert = mesh.vertices[vert_idx].co
            tri_verts.append(vert)
        triangles.append(tri_verts)


out = """
#ifndef %s
#define %s 1
#include "constants.h"
#include "collision.h"

""" % (
    include_guard,
    include_guard,
)

out += """
Triangle %s_collision_mesh[] = {
""" % (
    filename
)

for index, tri in enumerate(triangles):
    out += "{"
    for vert in tri:
        # we rotate the position from z-up (blender) to y-up (opengl)
        out += "{%f, %f, %f}," % (
            vert.x * N64_SCALE_FACTOR,
            vert.z * N64_SCALE_FACTOR,
            -(vert.y * N64_SCALE_FACTOR),
        )
    out += "},\n"
out += """
};
"""

out += """
#define %s_LENGTH %d
""" % (
    filename.upper(),
    len(triangles),
)

out += """
#endif /* %s */
""" % (
    include_guard
)

outfile = open(filename + ".h", "w")
outfile.write(out)
outfile.close()
