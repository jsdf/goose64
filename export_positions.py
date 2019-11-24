import bpy
import re
import math
import collections

"""
exports a level to a header file ready to be included in the game code
"""

filename = 'university_map'

# we scale the models up by this much to avoid n64 fixed point precision issues
N64_SCALE_FACTOR = 30

include_guard = filename.upper()+'_H'
world_objects = list(bpy.data.collections['worldobjects'].all_objects)

 

def get_bounds(obj):

    local_coords = obj.bound_box[:]
 
    coords = [p[:] for p in local_coords]

    rotated = zip(*coords[::-1])

    push_axis = []
    for (axis, _list) in zip('xyz', rotated):
        info = lambda: None
        info.max = max(_list)
        info.min = min(_list)
        info.distance = info.max - info.min
        push_axis.append(info)


    originals = dict(zip(['x', 'y', 'z'], push_axis))

    o_details = collections.namedtuple('object_details', 'x y z')
    return o_details(**originals)

out = """
#ifndef %s
#define %s 1
#include "gameobject.h"

""" % (include_guard,include_guard)

out += """
GameObject %s_data[] = {
""" % ( filename)

for index,obj in enumerate(world_objects):
  pos = obj.location
  rot = obj.rotation_euler

  out += "{"
  out += "%d, " % (index)
  out += "{%f, %f, %f}, " % (pos.x*N64_SCALE_FACTOR, pos.z*N64_SCALE_FACTOR, -(pos.y*N64_SCALE_FACTOR)) # y axis is inverted
  out += "%f, " % (math.degrees(rot.x), math.degrees(rot.z), -math.degrees(rot.y))
  out += "%sModel" % (re.sub(r'[.].*?$', '', obj.name))
  out += "},\n"
out += """
};
"""

out += """
#define %s_COUNT %d
""" % (filename.upper(), len(world_objects))

out += """
#endif /* %s */
""" % (include_guard)

outfile = open(filename+'.h', 'w')
outfile.write(out)
outfile.close()
  