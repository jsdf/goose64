import bpy
import re
import sys
import os
import json


blend_dir = os.path.dirname(bpy.data.filepath)
if blend_dir not in sys.path:
    sys.path.append(blend_dir)
print("adding blend_dir", blend_dir)

import spatial_hash
import importlib

importlib.reload(spatial_hash)


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
            vert = obj.matrix_world @ mesh.vertices[vert_idx].co
            # convert into game coord space
            # we rotate the position from z-up (blender) to y-up (opengl)
            # and scale by n64 scale factor
            tri_verts.append(
                [
                    vert.x * N64_SCALE_FACTOR,
                    vert.z * N64_SCALE_FACTOR,
                    -(vert.y * N64_SCALE_FACTOR),
                ]
            )
        triangles.append(tri_verts)


spatial_hash_data = spatial_hash.create_spatial_hash(triangles, 4 * N64_SCALE_FACTOR)


out = """
#ifndef %s
#define %s 1
#include "constants.h"
#include "collision.h"

""" % (
    include_guard,
    include_guard,
)

# collision mesh triangles
out += """
extern Triangle %s_collision_mesh[];
""" % (
    filename
)

# size of collision mesh data
out += """
#define %s_LENGTH %d
""" % (
    filename.upper(),
    len(triangles),
)


out += """
extern SpatialHash %s_collision_mesh_hash;
""" % (
    filename,
)


out += """
#endif /* %s */
""" % (
    include_guard
)

outfile = open(filename + ".h", "w")
outfile.write(out)
outfile.close()


out_c = """
#include "%s.h"
""" % (
    filename
)


# collision mesh triangles
out_c += """
Triangle %s_collision_mesh[] = {
""" % (
    filename
)

for index, game_tri in enumerate(triangles):
    out_c += "{"
    for game_vert in game_tri:
        out_c += "{%f, %f, %f}," % (game_vert[0], game_vert[1], game_vert[2])
    out_c += "},\n"
out_c += """
};
"""

# spatial hash buckets contents
for bucket_index, bucket in enumerate(spatial_hash_data.buckets):
    if bucket is not None:
        out_c += "int %s_collision_mesh_hash_bucket_%d_data[] = {" % (
            filename,
            bucket_index,
        )
        out_c += ", ".join([str(game_tri_index) for game_tri_index in bucket])
        out_c += "};\n"

# definition of each bucket with length and ptr to contents
for bucket_index, bucket in enumerate(spatial_hash_data.buckets):
    if bucket is not None:
        out_c += (
            "SpatialHashBucket %s_collision_mesh_hash_bucket_%d = {%d, (int *)%s_collision_mesh_hash_bucket_%d_data};\n"
            % (filename, bucket_index, len(bucket), filename, bucket_index)
        )


# spatial hash of pointers to buckets (or NULL ptrs)
out_c += """
SpatialHashBucket* %s_collision_mesh_hash_data[] = {
""" % (
    filename
)
for bucket_index, bucket in enumerate(spatial_hash_data.buckets):
    if bucket is None:
        out_c += "NULL,\n"
    else:
        out_c += "&%s_collision_mesh_hash_bucket_%d,\n" % (filename, bucket_index)

out_c += """
};
"""

out_c += """
SpatialHash %s_collision_mesh_hash = {
  %d, // int numBuckets;
  %f, // float gridCellSize;
  %d, // int cellsInDimension;
  %d, // int cellOffsetInDimension;
  %s_collision_mesh_hash_data, // int* data;
};
""" % (
    filename,
    len(spatial_hash_data.buckets),
    spatial_hash_data.cell_width,
    spatial_hash_data.cells_in_dimension,
    spatial_hash_data.cell_offset_in_dimension,
    filename,
)

out_c_file = open(filename + ".c", "w")
out_c_file.write(out_c)
out_c_file.close()

print("successfully exported", filename)


with open(filename + ".json", "w") as outfile:
    json.dump(triangles, outfile)
