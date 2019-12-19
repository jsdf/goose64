import bpy
import re
import math
import collections

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

out += """
GameObject %s_data[] = {
""" % (
    filename
)

for index, obj in enumerate(world_objects):
    pos = obj.location
    rot = obj.rotation_euler

    out += "{"
    out += "%d, // object id\n" % (index)
    # we rotate the position and rotation from z-up (blender) to y-up (opengl)
    out += "{%f, %f, %f}, // position\n" % (
        pos.x * N64_SCALE_FACTOR,
        pos.z * N64_SCALE_FACTOR,
        -(pos.y * N64_SCALE_FACTOR),
    )
    out += "{%f, %f, %f}, // rotation\n" % (
        math.degrees(rot.x),
        math.degrees(rot.z),
        -math.degrees(rot.y),
    )
    out += "%sModel, // modelType\n" % (re.sub(r"[.].*?$", "", obj.name))
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
