#include <stdio.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "objloader.hpp"

bool loadOBJ(const char* path, ObjModel& result, float meshScale) {
  printf("Loading OBJ file %s...\n", path);

  std::vector<unsigned int> faceVertexIndices, faceUVIndices, faceNormalIndices;
  std::vector<glm::vec3> fileVertices;
  std::vector<glm::vec2> fileUVs;
  std::vector<glm::vec3> fileNormals;

  // we keep track of a current mesh and write any faces encountered to it until
  // we hit another "o" line (object definition) in the OBJ file
  std::string currentMeshName = "__default";
  ObjMesh* currentMesh = &result.meshes[currentMeshName];

  FILE* file = fopen(path, "r");
  if (file == NULL) {
    printf("Impossible to open the file !\n");
    return false;
  }

  while (1) {
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break;

    if (strcmp(lineHeader, "o") == 0) {
      char objName[256];
      fscanf(file, "%s\n", objName);
      currentMeshName.assign(objName);
      currentMesh = &result.meshes[currentMeshName];
    } else if (strcmp(lineHeader, "v") == 0) {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

      vertex.x *= meshScale;
      vertex.y *= meshScale;
      vertex.z *= meshScale;
      fileVertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vt") == 0) {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      fileUVs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      fileNormals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                           &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                           &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9) {
        printf(
            "File can't be read by our simple parser :-( Try exporting with "
            "other options\n");
        return false;
      }

      // subtract 1 from indices because OBJ format indices are 1-indexed
      // but the std::vectors we're storing them in are 0-indexed
      currentMesh->vertices.push_back(fileVertices[vertexIndex[0] - 1]);
      currentMesh->vertices.push_back(fileVertices[vertexIndex[1] - 1]);
      currentMesh->vertices.push_back(fileVertices[vertexIndex[2] - 1]);
      currentMesh->uvs.push_back(fileUVs[uvIndex[0] - 1]);
      currentMesh->uvs.push_back(fileUVs[uvIndex[1] - 1]);
      currentMesh->uvs.push_back(fileUVs[uvIndex[2] - 1]);
      currentMesh->normals.push_back(fileNormals[normalIndex[0] - 1]);
      currentMesh->normals.push_back(fileNormals[normalIndex[1] - 1]);
      currentMesh->normals.push_back(fileNormals[normalIndex[2] - 1]);
    } else {
      // Probably a comment, eat up the rest of the line
      char stupidBuffer[1000];
      fgets(stupidBuffer, 1000, file);
    }
  }

  // clean up default mesh if no vertices were written to it
  if (result.meshes["__default"].vertices.size() == 0) {
    result.meshes.erase("__default");
  }

  return true;
}
