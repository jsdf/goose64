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
  std::string name;
} ObjMesh;

typedef struct ObjModel {
  std::vector<ObjMesh> meshList;
  std::map<std::string, int> meshes;
  GLuint texture;
} ObjModel;

bool loadOBJ(const char* path, ObjModel& result, float meshScale);

#endif
