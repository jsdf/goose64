#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <glm/glm.hpp>
#include <vector>

bool loadOBJ(const char* path,
             std::vector<glm::vec3>& out_vertices,
             std::vector<glm::vec2>& out_uvs,
             std::vector<glm::vec3>& out_normals,
             float meshScale);

#endif
