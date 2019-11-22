import bpy
import re
import math
from collections import defaultdict


"""
exports armature bone positions for every frame of an animation, and ranges of
the animation demarcated using timeline markers

assumptions: 
rigid body parts are made up of an object containing a mesh which is named 
with the convention ${object_name}mesh,
eg. an object called 'gooseneck' has a mesh called 'gooseneckmesh' and when
exported as .obj the resultant object section is called 'gooseneck_gooseneckmesh'

the body parts should be parented to an armature with bones named the same as 
the members of the bone_types list below

the 

"""

# we scale the models up by this much to avoid n64 fixed point precision issues
N64_SCALE_FACTOR = 30

modelname = "goose" # should be one of goose, character
filename = modelname+"_anim"

# this needs to match the members and ordering of the anim type enum in
#  ${modelname}animtypes.h
anim_types = ["idle", "walk"]
# this needs to match the members and ordering of the mesh type enum in 
# ${modelname}animtypes.h
bone_types = [
  "body",
  "head",
  "leg_l",
  "foot_l",
  "leg_r",
  "foot_r",
  "neck",
]

include_guard = filename.upper()+'_H'

out = """
#ifndef %s
#define %s 1
#include "animationframe.h"
#include "%sanimtypes.h"

""" % (include_guard,include_guard,modelname)


out += """
AnimationFrame %s_data[] = {
""" % (filename)

scene = bpy.context.scene
frame_current = scene.frame_current

bones_child_objects = {}
for armature_child in scene.objects['Armature'].children:
    bones_child_objects[armature_child.parent_bone] = armature_child

for frame in range(scene.frame_start, scene.frame_end + 1):
    scene.frame_set(frame)
    for bone_name in bone_types:
        obj = bones_child_objects[bone_name]
        mat = obj.matrix_world 
        pos = mat.translation.xzy
        rot = mat.to_euler() 
        print(frame, bone_name, pos, rot)

        out += "{"
        out += "%d, " % (frame-scene.frame_start)

        out += "%s_%smesh, " % (modelname+bone_name, modelname+bone_name)

        out += "{%f, %f, %f}, " % (pos.x*N64_SCALE_FACTOR, (pos.y*N64_SCALE_FACTOR), pos.z*N64_SCALE_FACTOR) # y axis is inverted
        out += "{%f, %f, %f}, " % (math.degrees(rot.x), math.degrees(rot.y), math.degrees(rot.z))
        out += "},\n" 

scene.frame_set(frame_current)

out += """
};
""" 

out += """
#define %s_FRAME_COUNT %d
""" % (filename.upper(),  scene.frame_end - scene.frame_start + 1)

anim_ranges = defaultdict(dict)
for marker_name, marker_data in scene.timeline_markers.items():
    matches = re.match(r"^(.*)_(start|end)$", marker_name)
    print("marker_name",marker_name,"frame", marker_data.frame)
    if matches:
        anim_name = matches[1]
        anim_ranges[anim_name][matches[2]] = marker_data.frame

out += """
AnimationRange %s_ranges[] = {
""" % (filename)
for anim_name in anim_types:
    if anim_ranges[anim_name].get("start") is None:
        raise NameError("missing start for "+anim_name)
    if anim_ranges[anim_name].get("end") is None:
        raise NameError("missing end for "+anim_name)
    out += "{%d, %d}, // %s\n" % (anim_ranges[anim_name].get("start",0), anim_ranges[anim_name].get("end",0), modelname+"_"+anim_name+"_anim")
out += """
};
""" 

out += """
#endif /* %s */
""" % (include_guard)

outfile = open(filename+'.h', 'w')
outfile.write(out)
outfile.close()

print("finished successfully")