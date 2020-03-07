#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glm/glm.hpp>

// Dear Imgui
#include "imgui/imgui.h"
// imgui.h needs to be before these
#include "imgui/examples/imgui_impl_glut.h"
#include "imgui/examples/imgui_impl_opengl2.h"

#include "animation.h"
#include "character.h"
#include "collision.h"
#include "constants.h"
#include "game.h"
#include "gameobject.h"
#include "gl/objloader.hpp"
#include "gl/texture.hpp"
#include "input.h"
#include "nodegraph/nodegraph.hpp"
#include "pathfinding.h"
#include "player.h"
#include "renderer.h"
#include "university_map.h"
#include "university_map_collision.h"
#include "university_map_graph.h"
#include "vec3d.h"

#include "character_anim.h"
#include "goose_anim.h"

#define FREEVIEW_SPEED 0.2f

#define DEBUG_LOG_RENDER 0
#define DEBUG_TEXT_BASIC 0
#define DEBUG_OBJECTS 0
#define DEBUG_RAYCASTING 0
#define DEBUG_MODELS 0
#define DEBUG_ANIMATION 0
#define DEBUG_ANIMATION_MORE 0
#define DEBUG_ATTACHMENT 0
#define DEBUG_PHYSICS 1
#define DEBUG_COLLISION_MESH 0
#define DEBUG_COLLISION_MESH_MORE 0
#define DEBUG_COLLISION_SPATIAL_HASH 0
#define DEBUG_COLLISION_MESH_AABB 0
#define DEBUG_PATHFINDING_GRAPH 0
#define DEBUG_PATHFINDING 0
#define DEBUG_PROFILING 1
#define USE_LIGHTING 1
#define USE_ANIM_FRAME_LERP 1
#define ENABLE_NODEGRAPH_EDITOR 1

int glgooseFrame = 0;
int updateSkipRate = 1;

// actual vector representing the freeview camera's direction
float cameraLX = 0.0f, cameraLZ = -1.0f;
// XZ position of the freeview camera
Vec3d viewPos = {0.0f, 50.0f, 0.0f};
// freeview camera angle
float cameraAngle = 180.0f;

bool keysDown[127];
Input input;
GameObject* selectedObject = NULL;

PhysWorldData physWorldData = {
    university_map_collision_collision_mesh, UNIVERSITY_MAP_COLLISION_LENGTH,
    &university_map_collision_collision_mesh_hash,
    /*gravity*/ -9.8 * N64_SCALE_FACTOR, /*viscosity*/ 0.05};

// crap for gluProject/gluUnProject
GLdouble lastModelView[16];
GLdouble lastProjection[16];
GLint lastViewport[4];

// profiling
float profAvgCharacters = 0;
float profAvgPhysics = 0;
float profAvgDraw = 0;
float profAvgPath = 0;

ObjModel models[MAX_MODEL_TYPE];

char* GooseMeshTypeStrings[] = {
    "goosebody_goosebodymesh",      //
    "goosehead_gooseheadmesh",      //
    "gooseleg_l_gooseleg_lmesh",    //
    "goosefoot_l_goosefoot_lmesh",  //
    "gooseleg_r_gooseleg_rmesh",    //
    "goosefoot_r_goosefoot_rmesh",  //
    "gooseneck_gooseneckmesh",      //
    "MAX_GOOSE_MESH_TYPE",          //
};

char* CharacterMeshTypeStrings[] = {
    "gkbicep.l_gkbicep_lrmesh",      // characterbicep_l_characterbicep_lmesh
    "gkbicep.r_gkbicep_rmesh",       // characterbicep_r_characterbicep_rmesh
    "gkfoot.l_gkfoot_lrmesh",        // characterfoot_l_characterfoot_lmesh
    "gkfoot.r_gkfoot_rmesh",         // characterfoot_r_characterfoot_rmesh
    "gkforearm.l_gkforearm_lrmesh",  // characterforearm_l_characterforearm_lmesh
    "gkforearm.r_gkforearm_rmesh",  // characterforearm_r_characterforearm_rmesh
    "gkhead_gkheadmesh",            // characterhead_characterheadmesh
    "gkshin.l_gkshin_lmesh",        // charactershin_l_charactershin_lmesh
    "gkshin.r_gkshin_rmesh",        // charactershin_r_charactershin_rmesh
    "gktorso_gktorsomesh",          // charactertorso_charactertorsomesh
    "gkthigh.l_gkthigh_lmesh",      // characterthigh_l_characterthigh_lmesh
    "gkthigh.r_gkthigh_rmesh",      // characterthigh_r_characterthigh_rmesh
};

// TODO: allocate this in map header file with correct size
static GameObject* sortedObjects[MAX_WORLD_OBJECTS];

int debugPathfindingFrom = 3;
int debugPathfindingTo = 8;
int pathfindingGraphSize = UNIVERSITY_MAP_GRAPH_SIZE;
Graph* pathfindingGraph = &university_map_graph;
PathfindingState* pathfindingState = &university_map_graph_pathfinding_state;
NodeState* pathfindingNodeStates = university_map_graph_pathfinding_node_states;
int* pathfindingResult = university_map_graph_pathfinding_result;

static NodeGraph nodeGraph = NodeGraph();
static int selectedNode = -1;

void loadModel(ModelType modelType, char* modelfile, char* texfile) {
  // the map exporter scales the world up by this much, so we scale up the
  // meshes to match
  loadOBJ(modelfile, models[modelType], N64_SCALE_FACTOR);
  models[modelType].texture = loadBMP_custom(texfile);
}

void screenCoordsToWorld(Vec3d* screenPos, Vec3d* result) {
  GLdouble res[3];

  gluUnProject(/*winX*/ screenPos->x,
               /*winY*/ screenPos->y,
               /*winZ*/ screenPos->z, lastModelView, lastProjection,
               lastViewport,
               /*objX*/ &res[0],
               /*objY*/ &res[1],
               /*objZ*/ &res[2]);

  Vec3d_init(result, res[0], res[1], res[2]);
}

void worldCoordsToScreen(Vec3d* pos, Vec3d* result) {
  GLdouble scr[3];

  gluProject(pos->x, pos->y, pos->z, lastModelView, lastProjection,
             lastViewport, &scr[0], &scr[1], &scr[2]);

  result->x = scr[0];
  result->y = scr[1];
  result->z = scr[2];
}

std::string formatVec3d(Vec3d* self) {
  char buffer[60];
  sprintf(buffer, "{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
  return buffer;
}

std::string formatEuler(EulerDegrees* self) {
  char buffer[60];
  sprintf(buffer, "{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
  return buffer;
}

void drawGUI() {
  GameObject* obj = selectedObject;
  ImGuiInputTextFlags inputFlags =
      ImGuiInputTextFlags_EnterReturnsTrue;  // only update on blur

  ImGui::Begin("Object Inspector");  // Create a window called "Hello, world!"
                                     // and append into it.

  // Display some text (you can use a format strings too)
  ImGui::Text("Selected object: %d (%s)", obj ? obj->id : -1,
              obj ? ModelTypeStrings[obj->modelType] : "none");

  if (obj) {
    int spatialHashResults[100];
    Vec3d objCenter;
    Game_getObjCenter(obj, &objCenter);

    ImGui::Text("Object");
    ImGui::InputFloat3("Position", (float*)&obj->position, "%.3f", inputFlags);
    ImGui::InputFloat3("Rotation", (float*)&obj->rotation, "%.3f", inputFlags);
    ImGui::InputFloat3(
        "centroidOffset",
        (float*)&modelTypesProperties[obj->modelType].centroidOffset, "%.3f",
        inputFlags);

    ImGui::InputFloat("radius",
                      (float*)&modelTypesProperties[obj->modelType].radius, 0.1,
                      1.0, "%.3f", inputFlags);

    ImGui::Text("Physics");
    if (obj->physBody) {
      ImGui::InputInt("physBody", (int*)&obj->physBody->id, 0, 1,
                      ImGuiInputTextFlags_ReadOnly);
      ImGui::InputFloat3("phys Velocity",
                         (float*)&obj->physBody->nonIntegralVelocity, "%.3f",
                         ImGuiInputTextFlags_ReadOnly);
      ImGui::InputFloat3("phys Acceleration",
                         (float*)&obj->physBody->nonIntegralAcceleration,
                         "%.3f", ImGuiInputTextFlags_ReadOnly);

      ImGui::InputFloat3("phys position", (float*)&obj->physBody->position,
                         "%.3f", ImGuiInputTextFlags_ReadOnly);
      ImGui::InputFloat3("phys prevPosition",
                         (float*)&obj->physBody->prevPosition, "%.3f",
                         ImGuiInputTextFlags_ReadOnly);
      ImGui::InputInt("phys enabled", (int*)&obj->physBody->enabled, 0, 1,
                      ImGuiInputTextFlags_ReadOnly);
      ImGui::InputInt("phys controlled", (int*)&obj->physBody->controlled, 0, 1,
                      ImGuiInputTextFlags_ReadOnly);
    }

    ImGui::Text("Collision");
    ImGui::InputInt("testCollisionResult", (int*)&testCollisionResult, 0, 1,
                    inputFlags);

    std::string collKeys;
    if (testCollisionResults.size()) {
      std::map<int, SphereTriangleCollision>::iterator collIter =
          testCollisionResults.begin();

      while (collIter != testCollisionResults.end()) {
        collKeys += std::to_string(collIter->first) + ",";

        collIter++;
      }
    }
    ImGui::Text("colliding tris=%s", collKeys.c_str());

    int spatialHashResultsCount =
        SpatialHash_getTriangles(&objCenter, Game_getObjRadius(obj),
                                 &university_map_collision_collision_mesh_hash,
                                 spatialHashResults, /*maxResults*/ 100);

    std::string bucketKeys;
    int i;
    for (i = 0; i < spatialHashResultsCount; i++) {
      bucketKeys += std::to_string(spatialHashResults[i]) + ",";
    }
    ImGui::Text("current bucket tris=%s", bucketKeys.c_str());
  }

  ImGui::InputFloat("physWorldData.gravity", (float*)&physWorldData.gravity,
                    1.0, 10.0, "%.3f", inputFlags);

  ImGui::InputInt("updateSkipRate", (int*)&updateSkipRate, 1, 10, inputFlags);
  updateSkipRate = MAX(updateSkipRate, 1);

  ImGui::Text("Frametime %.3f ms (%.1f FPS)",
              1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#if DEBUG_PROFILING
  ImGui::Text("Phys=%.3fms, Char=%.3f ms, Draw=%.3f ms, Path=%.3f ms",
              profAvgPhysics, profAvgCharacters, profAvgDraw, profAvgPath);
#endif

  Vec3d* goosePos = &Game_get()->player.goose->position;
#if DEBUG_PATHFINDING
  ImGui::InputInt("debugPathfindingFrom", (int*)&debugPathfindingFrom, 1, 10,
                  inputFlags);
  debugPathfindingFrom =
      CLAMP(debugPathfindingFrom, 0, pathfindingGraphSize - 1);
  ImGui::InputInt("debugPathfindingTo", (int*)&debugPathfindingTo, 1, 10,
                  inputFlags);
  debugPathfindingTo = CLAMP(debugPathfindingTo, 0, pathfindingGraphSize - 1);

  if (ImGui::Button("print pos")) {
    printf("{%.3f, %.3f, %.3f}\n", goosePos->x, goosePos->y, goosePos->z);
  }

#endif

#if ENABLE_NODEGRAPH_EDITOR
  drawNodeGraphGUI(nodeGraph, goosePos, "university_map_graph",
                   "university_map_graph", selectedNode);
#endif

  ImGui::End();
}

void drawString(const char* string, int x, int y) {
  int w, h;
  const char* c;
  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glOrtho(0, w, 0, h, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glColor3f(1.0f, 1.0f, 1.0f);

  glRasterPos2i(x, y);
  for (c = string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopAttrib();
}

void drawStringAtPoint(const char* string, Vec3d* pos, int centered) {
  Vec3d screen;  // x, y, zdepth
  worldCoordsToScreen(pos, &screen);

  int stringLength;

  stringLength = strlen(string);

  drawString(string, screen.x - (centered ? (stringLength * 8 / 2) : 0.0),
             screen.y);
}

void drawMarker(float r, float g, float b, float radius) {
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT |
               GL_DEPTH_BUFFER_BIT);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glColor3f(r, g, b);  // red
  glutWireSphere(/*radius*/ radius, /*slices*/ 10, /*stacks*/ 10);
  glPopAttrib();
}

void drawLine(Vec3d* start, Vec3d* end) {
  glBegin(GL_LINES);
  glVertex3f(start->x, start->y, start->z);
  glVertex3f(end->x, end->y, end->z);
  glEnd();
}

void drawMotionVectorLine(Vec3d* from, Vec3d* to) {
  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT |
               GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glColor3f(0.0f, 0.0f, 0.0f);  // blue
  drawLine(from, to);
  glPushMatrix();
  glTranslatef(from->x, from->y, from->z);
  drawMarker(0.8f, 0.0f, 0.0f, 1);  // from red
  glPopMatrix();
  glPushMatrix();
  glTranslatef(to->x, to->y, to->z);
  drawMarker(0.0f, 0.8f, 0.0f, 1);  // to green
  glPopMatrix();
  glPopAttrib();
  glPopMatrix();
}

void drawTriNormal(Vec3d* normal, Vec3d* position) {
  Vec3d from, to;

  from = *position;
  to = *normal;
  Vec3d_mulScalar(&to, 20.0);
  Vec3d_add(&to, position);

  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT |
               GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glBegin(GL_LINES);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(from.x, from.y, from.z);
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(to.x, to.y, to.z);
  glEnd();

  glPopAttrib();
  glPopMatrix();
}

void drawRaycastLine(RaycastTraceEvent raycast) {
  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  glPushMatrix();

  // create end point based on origin and direction
  Vec3d rayEnd = raycast.direction;
  Vec3d_mulScalar(&rayEnd, 1000.0);
  Vec3d_add(&rayEnd, &raycast.origin);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  if (raycast.result) {
    glColor3f(1.0f, 1.0f, 1.0f);
  } else {
    glColor3f(1.0f, 0.0f, 0.0f);
  }
  drawLine(&raycast.origin, &rayEnd);
  glPushMatrix();
  glTranslatef(raycast.origin.x, raycast.origin.y, raycast.origin.z);
  drawMarker(0.8f, 0.8f, 0.8f, 1);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(rayEnd.x, rayEnd.y, rayEnd.z);
  drawMarker(0.8f, 0.8f, 0.8f, 1);
  glPopMatrix();
  glPopAttrib();
  glPopMatrix();
}

void drawPhysBall(float radius) {
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 0.0);  // yellow
  glutWireSphere(/*radius*/ radius, /*slices*/ 10, /*stacks*/ 10);
  glPopAttrib();
}

void drawMesh(ObjMesh& mesh, GLuint texture) {
  glColor3f(1.0f, 1.0f, 1.0f);  // whitish
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glBegin(GL_TRIANGLES);
  for (int ivert = 0; ivert < mesh.vertices.size(); ++ivert) {
    glTexCoord2d(mesh.uvs[ivert].x, mesh.uvs[ivert].y);
    glNormal3f(mesh.normals[ivert].x, mesh.normals[ivert].y,
               mesh.normals[ivert].z);
    glVertex3f(mesh.vertices[ivert].x, mesh.vertices[ivert].y,
               mesh.vertices[ivert].z);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

void drawMeshesForModel(ObjModel& model) {
  // render each model mesh. usually there will only be one
  std::map<std::string, ObjMesh>::iterator it = model.meshes.begin();
  while (it != model.meshes.end()) {
    // std::cout << "drawing model: " << ModelTypeStrings[obj->modelType]
    //           << " mesh:" << it->first << std::endl;

    ObjMesh& mesh = it->second;

    // draw mesh
    drawMesh(mesh, model.texture);

    it++;
  }
}

char* getMeshNameForModelMeshPart(ModelType modelType, int meshPart) {
  switch (modelType) {
    case GooseModel:
      return GooseMeshTypeStrings[meshPart];
    default:
      return CharacterMeshTypeStrings[meshPart];
  }
}

int getAnimationNumModelMeshParts(ModelType modelType) {
  switch (modelType) {
    case GooseModel:
      return MAX_GOOSE_MESH_TYPE;
    default:
      return MAX_CHARACTER_MESH_TYPE;
  }
}

AnimationRange* getCurrentAnimationRange(GameObject* obj) {
  if (obj->modelType == GooseModel) {
    return &goose_anim_ranges[(GooseAnimType)obj->animState->state];
  } else {
    return &character_anim_ranges[(CharacterAnimType)obj->animState->state];
  }
}

AnimationFrame* getAnimData(ModelType modelType) {
  switch (modelType) {
    case GooseModel:
      return goose_anim_data;
    default:
      return character_anim_data;
  }
}

void drawModel(GameObject* obj) {
  if (!obj->visible) {
    return;
  }

  ObjModel& model = models[obj->modelType];

  if (obj->modelType == GooseModel ||
      obj->modelType == GroundskeeperCharacterModel) {
    // case for multi-part objects using rigid body animation
    // TODO: generalize this for other model types using other skeletons with
    // retargetable animations

    AnimationRange* curAnimRange;  // range of frames representing anim clip
    AnimationInterpolation animInterp;  // interpolation value for frame
    AnimationFrame animFrame;           // animation frame data for one bone
    int modelMeshParts = getAnimationNumModelMeshParts(obj->modelType);

    assert(obj->animState != NULL);
    curAnimRange = getCurrentAnimationRange(obj);

#if DEBUG_ANIMATION
    glDisable(GL_DEPTH_TEST);
    drawMarker(1.0f, 0.0f, 0.0f, 5);  // origin marker, red
    glEnable(GL_DEPTH_TEST);
#endif

    AnimationInterpolation_calc(&animInterp, obj->animState, curAnimRange);
    for (int modelMeshIdx = 0; modelMeshIdx < modelMeshParts; ++modelMeshIdx) {
#if USE_ANIM_FRAME_LERP
      AnimationFrame_lerp(
          &animInterp,  // result of AnimationInterpolation_calc()
          getAnimData(
              obj->modelType),  // pointer to start of AnimationFrame list
          modelMeshParts,       // num bones in rig used by animData
          modelMeshIdx,         // index of bone in rig to produce transform for
          &animFrame            // the resultant interpolated animation frame
      );
#else
      AnimationFrame_get(
          &animInterp,  // result of AnimationInterpolation_calc()
          getAnimData(
              obj->modelType),  // pointer to start of AnimationFrame list
          modelMeshParts,       // num bones in rig used by animData
          modelMeshIdx,         // index of bone in rig to produce transform for
          &animFrame);
#endif

      // push relative transformation matrix, render the mesh, then pop the
      // relative transform off the matrix stack again
      glPushMatrix();

      // rotate from z-up (blender) to y-up (opengl) coords
      glRotatef(-90.0f, 1, 0, 0);

      glTranslatef(animFrame.position.x, animFrame.position.y,
                   animFrame.position.z);

      glRotatef(animFrame.rotation.x, 1, 0, 0);
      glRotatef(animFrame.rotation.y, 0, 1, 0);
      glRotatef(animFrame.rotation.z, 0, 0, 1);

      char* meshName =
          getMeshNameForModelMeshPart(obj->modelType, animFrame.object);
      try {
        ObjMesh& mesh = model.meshes.at(meshName);
        drawMesh(mesh, model.texture);
      } catch (const std::out_of_range& oor) {
        std::cerr << "missing mesh: " << meshName << " on model "
                  << ModelTypeStrings[obj->modelType] << "\n";
      }

      // attachment drawing stuff
      AnimationBoneAttachment& attachment = obj->animState->attachment;
      if (attachment.modelType != NoneModel &&
          attachment.boneIndex == modelMeshIdx) {
#if DEBUG_ATTACHMENT
        printf(
            "drawing attachment %s on %s (animFrame.object=%d) at boneIdx=%d "
            "mesh=%s\n",
            ModelTypeStrings[attachment.modelType],
            ModelTypeStrings[obj->modelType], animFrame.object,
            attachment.boneIndex,
            getMeshNameForModelMeshPart(obj->modelType, attachment.boneIndex));
#endif
        glPushMatrix();
        glTranslatef(attachment.offset.x, attachment.offset.y,
                     attachment.offset.z);
        glRotatef(attachment.rotation.x, 1, 0, 0);
        glRotatef(attachment.rotation.y, 0, 1, 0);
        glRotatef(attachment.rotation.z, 0, 0, 1);
        ObjModel& attachmentModel = models[attachment.modelType];
        drawMeshesForModel(attachmentModel);
        glPopMatrix();
      }

#if DEBUG_ANIMATION
      glDisable(GL_DEPTH_TEST);
      drawMarker(0.0f, 0.0f, 1.0f, 1);  // bone marker, blue
      glEnable(GL_DEPTH_TEST);
#endif

#if DEBUG_ANIMATION_MORE
      // overlay cones
      glPushMatrix();
      glRotatef(90.0f, 0, 0,
                1);               // cone points towards z by default, flip up
                                  // on the z axis to make cone point up at y
      glRotatef(90.0f, 0, 1, 0);  // undo our weird global rotation
      glDisable(GL_TEXTURE_2D);
      glColor3f(1.0f, 0.0f, 0.0f);  // red
      // glutSolidCone(4.2, 30, 4, 20);  // cone with 4 slices = pyramid-like

      glColor3f(1.0f, 1.0f, 1.0f);
      glEnable(GL_TEXTURE_2D);
      glPopMatrix();

      // overlay text
      glPushMatrix();
      Vec3d animFrameGlobalPos;
      Vec3d_origin(&animFrameGlobalPos);
      drawStringAtPoint(
          getMeshNameForModelMeshPart(obj->modelType, animFrame.object),
          &animFrameGlobalPos, FALSE);
      glPopMatrix();
#endif

      glPopMatrix();
    }

  } else {
    // case for simple gameobjects with no moving sub-parts
    drawMeshesForModel(model);
#if DEBUG_MODELS
    drawMarker(255.0, 0.0, 255.0, 1);  // obj marker, purple
#endif
  }
}

void resizeWindow(int w, int h) {
  ImGui_ImplGLUT_ReshapeFunc(w, h);
  float ratio;
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0)
    h = 1;
  ratio = w * 1.0 / h;

  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);
  // Reset Matrix
  glLoadIdentity();
  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);
  // Set the correct perspective.
  gluPerspective(45.0f, ratio, 10.0f, 3000.0f);
  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}

void enableLighting() {
  GLfloat light_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};   /* default value */
  GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};   /* default value */
  GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  /* default value */
  GLfloat light_position[] = {1.0f, 1.0f, -1.0f, 0.0f}; /* NOT default value */
  GLfloat lightModel_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f}; /* default value */
  GLfloat material_specular[] = {1.0f, 1.0f, 1.0f,
                                 1.0f}; /* NOT default value */
  GLfloat material_emission[] = {0.0f, 0.0f, 0.0f, 1.0f}; /* default value */
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModel_ambient);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
  glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0); /* NOT default value   */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
}

void drawGameObject(GameObject* obj) {
  Vec3d pos, centroidOffset;
  pos = obj->position;
  centroidOffset = modelTypesProperties[obj->modelType].centroidOffset;

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(obj->rotation.y, 0, 1, 0);

  if (obj->modelType != NoneModel) {
    if (Renderer_isZBufferedGameObject(obj)) {
      glEnable(GL_DEPTH_TEST);
    } else {
      // glDisable(GL_DEPTH_TEST);
      glEnable(GL_DEPTH_TEST);
    }
    drawModel(obj);
  }
  glPopMatrix();
}

void drawAABB(AABB* aabb) {
  // top
  glBegin(GL_LINE_LOOP);
  glVertex3f(aabb->min.x, aabb->min.y, aabb->min.z);
  glVertex3f(aabb->min.x, aabb->min.y, aabb->max.z);
  glVertex3f(aabb->max.x, aabb->min.y, aabb->max.z);
  glVertex3f(aabb->max.x, aabb->min.y, aabb->min.z);
  glEnd();

  // bottom
  glBegin(GL_LINE_LOOP);
  glVertex3f(aabb->max.x, aabb->max.y, aabb->max.z);
  glVertex3f(aabb->max.x, aabb->max.y, aabb->min.z);
  glVertex3f(aabb->min.x, aabb->max.y, aabb->min.z);
  glVertex3f(aabb->min.x, aabb->max.y, aabb->max.z);
  glEnd();

  // sides
  glBegin(GL_LINES);
  // 1
  glVertex3f(aabb->min.x, aabb->min.y, aabb->min.z);
  glVertex3f(aabb->min.x, aabb->max.y, aabb->min.z);
  // 2
  glVertex3f(aabb->min.x, aabb->min.y, aabb->max.z);
  glVertex3f(aabb->min.x, aabb->max.y, aabb->max.z);
  // 3
  glVertex3f(aabb->min.x, aabb->min.y, aabb->min.z);
  glVertex3f(aabb->min.x, aabb->min.y, aabb->min.z);
  // 4
  glVertex3f(aabb->max.x, aabb->min.y, aabb->min.z);
  glVertex3f(aabb->max.x, aabb->max.y, aabb->min.z);
  glEnd();
}

void drawCollisionMesh() {
  int i;

  Triangle* tri;
  Vec3d triCentroid, triNormal;
  char triIndexText[100];

  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT |
               GL_DEPTH_BUFFER_BIT | GL_POLYGON_BIT);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);

  // draw on top of everything else
  glDisable(GL_DEPTH_TEST);

  glPushMatrix();

  // draw all tris wireframes
  float triColor;
  for (i = 0, tri = university_map_collision_collision_mesh;
       i < UNIVERSITY_MAP_COLLISION_LENGTH; i++, tri++) {
    triColor = (i / (float)UNIVERSITY_MAP_COLLISION_LENGTH);
    glColor3f(0.0f, triColor, 1.0f - triColor);
    glBegin(GL_LINE_LOOP);
    glVertex3f(tri->a.x, tri->a.y, tri->a.z);
    glVertex3f(tri->b.x, tri->b.y, tri->b.z);
    glVertex3f(tri->c.x, tri->c.y, tri->c.z);
    glEnd();
  }

// draw spatial hash matching tris
#if DEBUG_COLLISION_SPATIAL_HASH
  if (selectedObject) {
    int spatialHashResults[100];
    Vec3d objCenter;
    Game_getObjCenter(selectedObject, &objCenter);

    int spatialHashResultsCount =
        SpatialHash_getTriangles(&objCenter, Game_getObjRadius(selectedObject),
                                 &university_map_collision_collision_mesh_hash,
                                 spatialHashResults, /*maxResults*/ 100);

    int i;
    for (i = 0; i < spatialHashResultsCount; i++) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      for (i = 0; i < spatialHashResultsCount; i++) {
        tri = university_map_collision_collision_mesh + spatialHashResults[i];
        triColor =
            (spatialHashResults[i] / (float)UNIVERSITY_MAP_COLLISION_LENGTH);

        glColor4f(0.0f, triColor * 0.8, (1.0f - triColor) * 0.8, 0.3);
        glBegin(GL_TRIANGLES);
        glVertex3f(tri->a.x, tri->a.y, tri->a.z);
        glVertex3f(tri->b.x, tri->b.y, tri->b.z);
        glVertex3f(tri->c.x, tri->c.y, tri->c.z);
        glEnd();
      }
      glDisable(GL_BLEND);
    }
#if DEBUG_COLLISION_MESH_AABB
    for (i = 0; i < spatialHashResultsCount; i++) {
      for (i = 0; i < spatialHashResultsCount; i++) {
        tri = university_map_collision_collision_mesh + spatialHashResults[i];
        AABB triangleAABB;
        AABB_fromTriangle(tri, &triangleAABB);
        glColor3f(1.0, 1.0, 0.0);
        drawAABB(&triangleAABB);
      }
    }
#endif
  }
#endif

  // draw collided tris
  if (testCollisionResults.size()) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::map<int, SphereTriangleCollision>::iterator collIter =
        testCollisionResults.begin();

    while (collIter != testCollisionResults.end()) {
      tri = university_map_collision_collision_mesh + collIter->first;

      if (testCollisionResult == collIter->first) {
        glColor4f(1.0f, 0.0f, 0.0f, 0.3);
      } else {
        glColor4f(1.0f, 1.0f, 0.0f, 0.3);
      }
      glBegin(GL_TRIANGLES);
      glVertex3f(tri->a.x, tri->a.y, tri->a.z);
      glVertex3f(tri->b.x, tri->b.y, tri->b.z);
      glVertex3f(tri->c.x, tri->c.y, tri->c.z);
      glEnd();

      collIter++;
    }
    glDisable(GL_BLEND);
  }

// draw tri labels & normals
#if DEBUG_COLLISION_MESH_MORE
  for (i = 0, tri = university_map_collision_collision_mesh;
       i < UNIVERSITY_MAP_COLLISION_LENGTH; i++, tri++) {
    triColor = (i / (float)UNIVERSITY_MAP_COLLISION_LENGTH);
    Triangle_getCentroid(tri, &triCentroid);
    Triangle_getNormal(tri, &triNormal);

    float triCollisionDistance = 0;
    if (testCollisionResults.count(i) > 0) {
      try {
        SphereTriangleCollision& collisionTestResult =
            testCollisionResults.at(i);

        // marker showing closest hit point in triangle
        Vec3d& hitPos = collisionTestResult.posInTriangle;
        glPushMatrix();
        glTranslatef(hitPos.x, hitPos.y, hitPos.z);
        if (testCollisionResult == i) {
          drawMarker(0.5f * 0.8, 0.0f, 0.0f, 1);
        } else {
          drawMarker(0.0f, triColor * 0.8, (1.0f - triColor) * 0.8, 1);
        }
        glPopMatrix();

        // text label
        triCollisionDistance = collisionTestResult.distance;
        sprintf(triIndexText, "%d: %.2f %s", i, triCollisionDistance,
                i == testCollisionResult ? "[closest]" : "");
        drawStringAtPoint(triIndexText, &triCentroid, TRUE);
      } catch (const std::out_of_range& oor) {
        std::cerr << "missing SphereTriangleCollision: " << i << "\n";
      }
    } else {
      // print other tri indexes
      sprintf(triIndexText, "%d ", i);
      drawStringAtPoint(triIndexText, &triCentroid, TRUE);
    }

    drawTriNormal(&triNormal, &triCentroid);
  }
#endif

  glPopMatrix();
  glPopAttrib();
}

void doPathfinding(int printResult) {
  float profStartPath = CUR_TIME_MS();

  Path_initState(
      pathfindingGraph,                                          // graph
      pathfindingState,                                          // state
      Path_getNodeByID(pathfindingGraph, debugPathfindingFrom),  // start
      Path_getNodeByID(pathfindingGraph, debugPathfindingTo),    // end
      pathfindingNodeStates,  // nodeStates array
      pathfindingGraphSize,   // nodeStateSize
      pathfindingResult       // results array

  );

  int result = Path_findAStar(pathfindingGraph, pathfindingState);

  float profTimePath = (CUR_TIME_MS() - profStartPath);
  Game_get()->profTimePath += profTimePath;

  if (printResult) {
    printf("finding path from %d to %d\n", pathfindingState->start->id,
           pathfindingState->end->id);
    printf("pathfinding result %s\n", result ? "found" : "not found");
    printf("pathfinding took %f\n", profTimePath);
    if (result) {
      printf("pathfinding result length %d\n", pathfindingState->resultSize);

      // pathfinding result path
      for (int i = 0; i < pathfindingState->resultSize; i++) {
        printf("%d: %d\n", i, *(pathfindingState->result + i));
      }
    }
  }
}

void drawPathfindingGraph() {
  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  Node* node;
  Node* endNode;
  Graph* graph = pathfindingGraph;
  PathfindingState* state = pathfindingState;
  int i, k;
  int* reachedViaNodeID;

  // draw graph edges
  for (i = 0, node = graph->nodes;  //
       i < graph->size;             //
       i++, node++                  //
  ) {
    EdgeList* edges = Path_getNodeEdgesByID(graph, i);

    // Loop through each edge in turn.
    for (                                           //
        k = 0, reachedViaNodeID = edges->elements;  //
        k < edges->size;                            //
        k++, reachedViaNodeID++                     //
    ) {
      endNode = Path_getNodeByID(graph, *reachedViaNodeID);
      glColor3f(0.5f, 0.5f, 0.5f);  // grey
      drawLine(&node->position, &endNode->position);
    }
  }

  // draw pathfinding result path
  for (i = 0;                      //
       i < state->resultSize - 1;  //
       i++                         //
  ) {
    node = Path_getNodeByID(graph, *(state->result + i));
    endNode = Path_getNodeByID(graph, *(state->result + i + 1));

    glColor3f(1.0f, 0.0f, 0.0f);  // red
    drawLine(&node->position, &endNode->position);
  }

  // draw graph nodes
  for (i = 0, node = graph->nodes;  //
       i < graph->size;             //
       i++, node++                  //
  ) {
    glPushMatrix();
    glTranslatef(node->position.x, node->position.y, node->position.z);
    glColor3f(1.0f, 0.7f, 0.0f);  // orange
    glutSolidCube(10);
    glPopMatrix();
  }

  for (i = 0, node = graph->nodes;  //
       i < graph->size;             //
       i++, node++                  //
  ) {
    drawStringAtPoint(std::to_string(i).c_str(), &node->position, TRUE);
  }

  glPopAttrib();
}

void drawNodeGraph() {
  glPushAttrib(GL_TRANSFORM_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  Node* node;
  Node* endNode;
  int i;

  // draw graph edges
  for (auto edge = nodeGraph.edges.begin(); edge != nodeGraph.edges.end();
       ++edge) {
    node = &nodeGraph.nodes[edge->from];
    endNode = &nodeGraph.nodes[edge->to];

    glColor3f(0.5f, 0.5f, 0.5f);  // grey
    drawLine(&node->position, &endNode->position);
  }

  // draw graph nodes
  for (auto node = nodeGraph.nodes.begin(); node != nodeGraph.nodes.end();
       ++node) {
    glPushMatrix();
    glTranslatef(node->position.x, node->position.y, node->position.z);
    if (node->id == selectedNode) {
      glColor3f(1.0f, 0.0f, 0.0f);  // red
    } else {
      glColor3f(1.0f, 0.7f, 0.0f);  // orange
    }
    glutSolidCube(10);
    glPopMatrix();
  }

  // draw graph node ids
  i = 0;
  for (auto node = nodeGraph.nodes.begin(); node != nodeGraph.nodes.end();
       ++node) {
    drawStringAtPoint(std::to_string(i).c_str(), &node->position, TRUE);
    i++;
  }

  glPopAttrib();
}

void renderScene(void) {
  int i;
  Game* game;

  float profStartDraw = CUR_TIME_MS();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGLUT_NewFrame();

  game = Game_get();

  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);
  int w = glutGet(GLUT_WINDOW_WIDTH);
  int h = glutGet(GLUT_WINDOW_HEIGHT);
  resizeWindow(w, h);

  // Reset transformations
  glLoadIdentity();
  // Set the camera
  if (game->freeView) {
    gluLookAt(                                                  //
        viewPos.x, viewPos.y, viewPos.z,                        // eye
        viewPos.x + cameraLX, viewPos.y, viewPos.z + cameraLZ,  // center
        0.0f, 1.0f, 0.0f                                        // up
    );
  } else {
    gluLookAt(                                                       //
        game->viewPos.x, game->viewPos.y, game->viewPos.z,           // eye
        game->viewTarget.x, game->viewTarget.y, game->viewTarget.z,  // center
        0.0f, 1.0f, 0.0f                                             // up
    );
  }

  // store the viewport for later
  glGetDoublev(GL_MODELVIEW_MATRIX, lastModelView);
  glGetDoublev(GL_PROJECTION_MATRIX, lastProjection);
  glGetIntegerv(GL_VIEWPORT, lastViewport);

  Renderer_sortWorldObjects(sortedObjects, game->worldObjectsCount);

#if USE_LIGHTING
  enableLighting();
#endif

#if DEBUG_LOG_RENDER
  printf("draw start\n");
#endif
  // render world objects
  for (i = 0; i < game->worldObjectsCount; i++) {
    GameObject* obj = sortedObjects[i];
    if (obj->modelType != NoneModel) {
#if DEBUG_LOG_RENDER
      printf("draw obj %d %s dist=%.3f {x:%.3f, y:%.3f, z:%.3f}\n", obj->id,
             ModelTypeStrings[obj->modelType],
             Vec3d_distanceTo(&(obj->position), &viewPos), obj->position.x,
             obj->position.y, obj->position.z);
#endif
      drawGameObject(sortedObjects[i]);
    }
  }

#if DEBUG_COLLISION_MESH || DEBUG_COLLISION_MESH_MORE || \
    DEBUG_COLLISION_SPATIAL_HASH
  drawCollisionMesh();
#endif

#if DEBUG_PHYSICS
  PhysBody* body;
  for (i = 0, body = game->physicsBodies; i < game->physicsBodiesCount;
       i++, body++) {
    glPushMatrix();
    glTranslatef(body->position.x, body->position.y, body->position.z);
    drawPhysBall(body->radius);
    glPopMatrix();
    drawMotionVectorLine(&body->prevPosition, &body->position);
  }
#endif

#if DEBUG_RAYCASTING
  for (i = 0; i < gameRaycastTrace.size(); ++i) {
    drawRaycastLine(gameRaycastTrace[i]);
  }

  gameRaycastTrace.clear();
#endif

#if DEBUG_PATHFINDING_GRAPH
  drawPathfindingGraph();
#endif
#if ENABLE_NODEGRAPH_EDITOR
  drawNodeGraph();
#endif

#if DEBUG_OBJECTS
  char objdebugtext[300];
  for (i = 0; i < game->worldObjectsCount; i++) {
    GameObject* obj = sortedObjects[i];
    if (obj->modelType != NoneModel) {
      strcpy(objdebugtext, "");

      sprintf(objdebugtext, "%d: %s", obj->id,
              ModelTypeStrings[obj->modelType]);

      drawStringAtPoint(objdebugtext, &obj->position, TRUE);

      Vec3d objCenter;
      Game_getObjCenter(obj, &objCenter);

      glPushMatrix();
      glTranslatef(objCenter.x, objCenter.y, objCenter.z);
      drawMarker(0.2, 0.2, 0.2, Game_getObjRadius(obj));
      glPopMatrix();
    }
  }
#endif

  if (selectedObject) {
    Vec3d selObjCenter;
    Game_getObjCenter(selectedObject, &selObjCenter);

    glPushMatrix();
    glTranslatef(selObjCenter.x, selObjCenter.y, selObjCenter.z);
    drawMarker(1.0, 0.5, 0.0, Game_getObjRadius(selectedObject));
    glPopMatrix();
  }

  char pausedtext[80];
  if (game->paused) {
    strcpy(pausedtext, "paused");
    drawString(pausedtext, w / 2 - strlen(pausedtext) / 2, h / 2);
  }

#if DEBUG_TEXT_BASIC
  char debugtext[80];
  Vec3d_toString(&game->player.goose->position, debugtext);
  drawString(debugtext, 20, 20);

  char characterString[120];
  Character* character;
  for (i = 0, character = game->characters; i < game->charactersCount;
       i++, character++) {
    Character_toString(character, characterString);
    drawString(characterString, 20, glutGet(GLUT_WINDOW_HEIGHT) - 40 * (i + 1));
  }
  i++;
  Player_toString(&game->player, characterString);
  drawString(characterString, 20, glutGet(GLUT_WINDOW_HEIGHT) - 40 * (i + 1));
#endif

#if ENABLE_NODEGRAPH_EDITOR
  drawString("NodeGraph Editor Mode", 20, glutGet(GLUT_WINDOW_HEIGHT) - 20);
#endif

  // Imgui Rendering
  drawGUI();
  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  game->profTimeDraw += (CUR_TIME_MS() - profStartDraw);
  glutSwapBuffers();
}

void updateCameraAngle(float newAngle) {
  cameraAngle = newAngle;
  cameraLX = sin(cameraAngle);
  cameraLZ = -cos(cameraAngle);
}

void turnLeft() {
  updateCameraAngle(cameraAngle - 0.01f);
}

void turnRight() {
  updateCameraAngle(cameraAngle + 0.01f);
}

void moveForward() {
  viewPos.x += cameraLX * FREEVIEW_SPEED * N64_SCALE_FACTOR;
  viewPos.z += cameraLZ * FREEVIEW_SPEED * N64_SCALE_FACTOR;
}

void moveBack() {
  viewPos.x -= cameraLX * FREEVIEW_SPEED * N64_SCALE_FACTOR;
  viewPos.z -= cameraLZ * FREEVIEW_SPEED * N64_SCALE_FACTOR;
}

void moveLeft() {
  viewPos.x -= cameraLZ * -FREEVIEW_SPEED * N64_SCALE_FACTOR;
  viewPos.z -= cameraLX * FREEVIEW_SPEED * N64_SCALE_FACTOR;
}

void moveRight() {
  viewPos.x += cameraLZ * -FREEVIEW_SPEED * N64_SCALE_FACTOR;
  viewPos.z += cameraLX * FREEVIEW_SPEED * N64_SCALE_FACTOR;
}

void moveUp() {
  viewPos.y += FREEVIEW_SPEED * N64_SCALE_FACTOR;
}

void moveDown() {
  viewPos.y -= FREEVIEW_SPEED * N64_SCALE_FACTOR;
}

void updateFreeView() {
  Game* game;
  game = Game_get();

  for (int key = 0; key < 127; ++key) {
    if (keysDown[key]) {
      if (game->freeView) {
        switch (key) {
          case 97:  // a
            moveLeft();
            break;
          case 100:  // d
            moveRight();
            break;
          case 119:  // w
            moveForward();
            break;
          case 115:  // s
            moveBack();
            break;
          case 113:  // q
            turnLeft();
            break;
          case 101:  // e
            turnRight();
            break;
          case 114:  // r
            moveUp();
            break;
          case 102:  // f
            moveDown();
            break;
        }
      }

      if (key == 99 && glgooseFrame % 10 == 0) {  // c
        game->freeView = !game->freeView;
        if (game->freeView) {
          printf("changing to freeview\n");
        } else {
          printf("changing from freeview\n");
        }
      }
    }
  }
}

void updateInputs() {
  Game* game;
  game = Game_get();

  for (int key = 0; key < 127; ++key) {
    if (keysDown[key]) {
      if (!game->freeView) {
        switch (key) {
          case 113:  // q
            input.zoomIn = TRUE;
            break;
          case 101:  // e
            input.zoomOut = TRUE;
            break;
          case 97:  // a
            input.direction.x += 1.0;
            break;
          case 100:  // d
            input.direction.x -= 1.0;
            break;
          case 119:  // w
            input.direction.y += 1.0;
            break;
          case 115:  // s
            input.direction.y -= 1.0;
            break;
          case 32:  // space
            input.pickup = true;
            break;
          case 118:  // v
            input.run = true;
            break;
        }
      }

      if (key == 112 && glgooseFrame % 10 == 0) {  // p
        game->paused = !game->paused;
      }
    }
  }
}

void quit(int exitCode) {
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGLUT_Shutdown();
  ImGui::DestroyContext();
  exit(exitCode);
}

void processNormalKeysUp(unsigned char key, int _x, int _y) {
  ImGui_ImplGLUT_KeyboardUpFunc(key, _x, _y);
  if (ImGui::GetIO().WantCaptureKeyboard) {
    return;
  }

  keysDown[key] = false;
}

void processNormalKeysDown(unsigned char key, int _x, int _y) {
  ImGui_ImplGLUT_KeyboardFunc(key, _x, _y);
  if (ImGui::GetIO().WantCaptureKeyboard) {
    return;
  }

  keysDown[key] = true;

  if (key == 27) {  // esc
    quit(0);
  }
}

void selectObjectAtScreenPos(int x, int y) {
  Vec3d mouseScreenPos0, mouseScreenPos1;
  Vec3d raySource, rayTarget, rayDirection;
  int invY = lastViewport[3] - y;

  // near end, ray origin
  Vec3d_init(&mouseScreenPos0, x, invY, 0.0f);
  screenCoordsToWorld(&mouseScreenPos0, &raySource);

  // far end
  GLfloat depth;
  glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
  Vec3d_init(&mouseScreenPos1, x, invY, depth);
  screenCoordsToWorld(&mouseScreenPos1, &rayTarget);

  // ray direction
  Vec3d_directionTo(&raySource, &rayTarget, &rayDirection);

#if ENABLE_NODEGRAPH_EDITOR
  selectedNode = -1;
  for (auto node = nodeGraph.nodes.begin(); node != nodeGraph.nodes.end();
       ++node) {
    if (Game_rayIntersectsSphere(&raySource, &rayDirection, &node->position,
                                 10)) {
      selectedNode = node->id;
      break;
    }
  }
#else
  selectedObject = Game_getIntersectingObject(&raySource, &rayDirection);
#endif
}

void processMouse(int button, int state, int x, int y) {
  ImGui_ImplGLUT_MouseFunc(button, state, x, y);
  if (ImGui::GetIO().WantCaptureMouse) {
    return;
  }

  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    selectObjectAtScreenPos(x, y);
  }
}

void testCollision() {
  if (selectedObject) {
    Vec3d objCenter;
    Game_getObjCenter(selectedObject, &objCenter);
    float objRadius = Game_getObjRadius(selectedObject);
    SphereTriangleCollision result;
    testCollisionTrace = TRUE;
    Collision_testMeshSphereCollision(
        university_map_collision_collision_mesh,
        UNIVERSITY_MAP_COLLISION_LENGTH, &objCenter, objRadius,
        &university_map_collision_collision_mesh_hash, &result);
    testCollisionTrace = FALSE;
  } else {
    testCollisionResult = -1;
    testCollisionResults.clear();
  }
}

void updateAndRender() {
  Game* game;
  game = Game_get();

  glgooseFrame++;
  updateFreeView();
  if (glgooseFrame % updateSkipRate == 0) {
    updateInputs();

    if (game->tick % 60 == 0) {
      profAvgCharacters = game->profTimeCharacters / 60.0f;
      game->profTimeCharacters = 0.0f;
      profAvgPhysics = game->profTimePhysics / 60.0f;
      game->profTimePhysics = 0.0f;
      profAvgDraw = game->profTimeDraw / 60.0f;
      game->profTimeDraw = 0.0f;
      profAvgPath = game->profTimePath / 60.0f;
      game->profTimePath = 0.0f;
    }

#if DEBUG_COLLISION_MESH
    testCollision();
#endif
#if DEBUG_PATHFINDING
    doPathfinding(FALSE);
#endif

    Game_update(&input);
  }
  if (game->freeView) {
    game->viewPos = viewPos;
  }

  renderScene();
}

int main(int argc, char** argv) {
  int i;

  Game* game;
  GameObject* obj;

  Game_init(university_map_data, UNIVERSITY_MAP_COUNT, &physWorldData);
  game = Game_get();

  Input_init(&input);

  updateCameraAngle(180);

  for (i = 0, obj = game->worldObjects; i < game->worldObjectsCount;
       i++, obj++) {
    printf("loaded obj %d %s  {x:%.3f, y:%.3f, z:%.3f}\n", obj->id,
           ModelTypeStrings[obj->modelType], obj->position.x, obj->position.y,
           obj->position.z);
  }

  // init sortedObjects pointers array
  for (i = 0, obj = game->worldObjects; i < game->worldObjectsCount;
       i++, obj++) {
    sortedObjects[i] = obj;
  }

  assert(UNIVERSITY_MAP_COUNT <= MAX_WORLD_OBJECTS);

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(1440, 1080);
  glutCreateWindow("Goose");

  // register callbacks
  glutDisplayFunc(renderScene);
  glutIdleFunc(updateAndRender);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGLUT_Init();
  ImGui_ImplGLUT_InstallFuncs();
  ImGui_ImplOpenGL2_Init();

  // replace some handlers after ImGui_ImplGLUT_InstallFuncs() sets its own
  // our impls will call the Imgui impls internally
  glutReshapeFunc(resizeWindow);
  glutKeyboardFunc(processNormalKeysDown);
  glutKeyboardUpFunc(processNormalKeysUp);
  glutMouseFunc(processMouse);

  // OpenGL init
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Load models. This has to be after OpenGL init because it creates textures
  loadModel(GooseModel, "gooserig.obj", "goosetex.bmp");
  loadModel(UniFloorModel, "university_floor.obj", "green.bmp");
  loadModel(UniBldgModel, "university_bldg.obj", "redbldg.bmp");
  loadModel(BushModel, "bush.obj", "bush.bmp");
  loadModel(FlagpoleModel, "flagpole.obj", "flagpole.bmp");
  loadModel(GroundskeeperCharacterModel, "characterrig.obj", "person.bmp");
  loadModel(BookItemModel, "book.obj", "book.bmp");
  loadModel(HomeworkItemModel, "testingCube.obj", "testCubeTex.bmp");
  loadModel(WallModel, "wall.obj", "wall.bmp");

  doPathfinding(DEBUG_PATHFINDING);
  nodeGraph.load(pathfindingGraph);

  // enter GLUT event processing cycle
  glutMainLoop();

  quit(1);
}
