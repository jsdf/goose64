import bpy
import re
import math
import mathutils
from collections import defaultdict


"""
exports a level to a header file ready to be included in the game code
"""

filename = "university_map"

# we scale the models up by this much to avoid n64 fixed point precision issues
N64_SCALE_FACTOR = 30

include_guard = filename.upper() + "_H"
world_objects = list(bpy.data.collections["worldobjects"].all_objects)

out = """
#ifndef %s
#define %s 1
#include "constants.h"
#include "gameobject.h"

""" % (
    include_guard,
    include_guard,
)


class AABB:
    def __init__(self):
        self.min = mathutils.Vector((0.0, 0.0, 0.0))
        self.max = mathutils.Vector((0.0, 0.0, 0.0))

    def expand_by_point(self, point):
        self.min.x = min(self.min.x, point.x)
        self.min.y = min(self.min.y, point.y)
        self.min.z = min(self.min.z, point.z)

        self.max.x = max(self.max.x, point.x)
        self.max.y = max(self.max.y, point.y)
        self.max.z = max(self.max.z, point.z)


def print_pos(pos):
    return "{%f, %f, %f}" % (pos.x, pos.y, pos.z)


def pos_from_blender(pos):
    # rotate the position from z-up (blender) to y-up (opengl)
    return mathutils.Vector(
        (
            pos.x * N64_SCALE_FACTOR,
            pos.z * N64_SCALE_FACTOR,
            -(pos.y * N64_SCALE_FACTOR),
        )
    )


out += """
AABB %s_bounds[] = {
""" % (
    filename
)

for index, obj in enumerate(world_objects):
    mesh = obj.data
    aabb = AABB()
    for vertex in mesh.vertices:
        vert_world = obj.matrix_world @ vertex.co
        vert_local_rotated = vert_world - obj.matrix_world.to_translation()
        aabb.expand_by_point(pos_from_blender(vert_local_rotated))

    out += "{"
    # these have already been converted from z-up (blender) to y-up (opengl)
    out += print_pos(aabb.min) + ", " + print_pos(aabb.max)
    out += "},\n"
out += """
};
"""

# modeltype is anything up to the first '.'
def get_modeltype(name):
    return re.sub(r"[.].*$", "", obj.name)


# you can add a .inst suffix, then any copies will get the same subtype
def get_subtypekey(name):
    return re.sub(r"[.]inst.*$", "", obj.name)


modeltype_next_subtype_id = defaultdict(int)
subtypekeys_subtypeids = {}


def get_subtype(name):
    modeltype = get_modeltype(name)
    subtypekey = get_subtypekey(name)
    # dispense a new subtype id if we don't have an id for this subtypekey
    if subtypekey not in subtypekeys_subtypeids:
        subtypekeys_subtypeids[subtypekey] = modeltype_next_subtype_id[modeltype]
        modeltype_next_subtype_id[modeltype] += 1
    print(name, subtypekey, subtypekeys_subtypeids[subtypekey])

    return subtypekeys_subtypeids[subtypekey]


out += """
GameObject %s_data[] = {
""" % (
    filename
)

for index, obj in enumerate(world_objects):
    pos = obj.location
    rot = obj.rotation_euler

    out += "{"
    out += "%d, // object id (%s)\n" % (index, obj.name)
    # we rotate the position and rotation from z-up (blender) to y-up (opengl)
    out += "%s, // position\n" % (print_pos(pos_from_blender(pos)))
    out += "{%f, %f, %f}, // rotation\n" % (
        math.degrees(rot.x),
        math.degrees(rot.z),
        -math.degrees(rot.y),
    )
    out += "%sModel, // modelType\n" % (get_modeltype(obj.name))
    out += "%d, // subtype\n" % (get_subtype(obj.name))
    out += "},\n"
out += """
};
"""

out += """
#define %s_COUNT %d
""" % (
    filename.upper(),
    len(world_objects),
)

out += """
#endif /* %s */
""" % (
    include_guard
)

outfile = open(filename + ".h", "w")
outfile.write(out)
outfile.close()
