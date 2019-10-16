import bpy

out = open("/Users/jfriend/.wine/drive_c/nintendo/n64kit/nusys/sample/nu1goose64/worldobjects.h", 'w')

for obj in list(bpy.data.collections['worldobjects'].all_objects):
  pos = obj.location
  rot = obj.rotation_euler
  out.write( "Vec3d_set(&obj->position, %f, %f, %f);\n" % (pos.x, pos.y, pos.z))
  out.write( "obj->rotationZ = %f;\n" % (rot.z))
  out.write( "obj->modelType = %sModel;\n" % (obj.name))
  out.write('\n')
out.close()

