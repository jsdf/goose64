#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <OpenGL/gl.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

typedef struct ObjMesh {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> uvs;
} ObjMesh;

typedef struct ObjModel {
  std::map<std::string, ObjMesh> meshes;
  GLuint texture;
} ObjModel;

bool loadOBJ(const char* path, ObjModel& result, float meshScale);

#endif
