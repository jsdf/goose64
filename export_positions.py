import bpy

out = open("worldobjects.h", 'w')

out.write( """
#ifndef WORLDOBJECTS_H
#define WORLDOBJECTS_H 1
#include "gameobject.h"
void WorldObjects_load(GameObject* obj, int i) {
  switch (i) {
""")
for obj in list(bpy.data.collections['worldobjects'].all_objects):
  pos = obj.location
  rot = obj.rotation_euler
  out.write( "case %f:\n" % (i))
  out.write( "Vec3d_set(&obj->position, %f, %f, %f);\n" % (pos.x, pos.y, pos.z))
  out.write( "obj->rotationZ = %f;\n" % (rot.z))
  out.write( "obj->modelType = %sModel;\n" % (obj.name))
  out.write( "return;\n" % (i))
out.close()
out.write( """
  }
}
#endif /* WORLDOBJECTS_H */
""")

  