import bpy
import re
import math

filename = 'university_map'

# we scale the models up by this much to avoid n64 fixed point precision issues
N64_SCALE_FACTOR = 30

include_guard = filename.upper()+'_H'
world_objects = list(bpy.data.collections['worldobjects'].all_objects)

out = """
#ifndef %s
#define %s 1
#include "gameobject.h"

""" % (include_guard,include_guard)

out += """
GameObject %s_data[] = {
""" % ( filename)

for obj in world_objects:
  pos = obj.location
  rot = obj.rotation_euler

  out += "{"
  out += "{%f, %f, %f}, " % (pos.x*N64_SCALE_FACTOR, pos.z*N64_SCALE_FACTOR, pos.y*N64_SCALE_FACTOR)
  out += "%f, " % (math.degrees(rot.z))
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
  