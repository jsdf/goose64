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
#include "animation.h"
#include "character.h"
#include "constants.h"
#include "game.h"
#include "gameobject.h"
#include "gl/objloader.hpp"
#include "gl/texture.hpp"
#include "input.h"
#include "renderer.h"
#include "university_map.h"
#include "vec3d.h"

#include "goose_anim.h"

#define FREEVIEW_SPEED 0.2f

#define DEBUG_LOG_RENDER 0
#define DEBUG_OBJECTS 0
#define DEBUG_RAYCASTING 0
#define DEBUG_ANIMATION 0
#define DEBUG_ANIMATION_MORE 0
#define DEBUG_PHYSICS 0
#define USE_LIGHTING 1

int glgooseFrame = 0;

// actual vector representing the camera's direction
float cameraLX = 0.0f, cameraLZ = -1.0f;
// XZ position of the camera
Vec3d viewPos = {0.0f, 1.0f, 0.0f};

float cameraAngle = 0.0f;
bool keysDown[127];
Input input;

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

// TODO: allocate this in map header file with correct size
static GameObject* sortedObjects[MAX_WORLD_OBJECTS];

void loadModel(ModelType modelType, char* modelfile, char* texfile) {
  // the map exporter scales the world up by this much, so we scale up the
  // meshes to match
  loadOBJ(modelfile, models[modelType], N64_SCALE_FACTOR);
  models[modelType].texture = loadBMP_custom(texfile);
}

void drawLine(Vec3d* start, Vec3d* end) {
  glBegin(GL_LINES);
  glVertex3f(start->x, start->y, start->z);
  glVertex3f(end->x, end->y, end->z);
  glEnd();
}

void drawString(char* string, int x, int y) {
  int w, h;
  char* c;
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
  glPopAttrib();
}

void drawStringAtPoint(char* string, Vec3d* pos, int centered) {
  GLdouble scr[3];
  GLdouble model[16];
  GLdouble proj[16];
  GLint view[4];

  int stringLength;

  stringLength = strlen(string);

  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);
  glGetIntegerv(GL_VIEWPORT, view);
  gluProject(pos->x, pos->y, pos->z, model, proj, view, &scr[0], &scr[1],
             &scr[2]);

  drawString(string, scr[0] - (centered ? (stringLength * 8 / 2) : 0.0),
             scr[1]);
}

void drawMarker(float r, float g, float b, float radius) {
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glColor3f(r, g, b);  // red
  glutWireSphere(/*radius*/ radius, /*slices*/ 5, /*stacks*/ 5);
  glPopAttrib();
}

void drawPhysBall(float radius) {
  glPushAttrib(GL_LIGHTING_BIT | GL_TEXTURE_BIT | GL_CURRENT_BIT);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 0.0);  // yellow
  glutWireSphere(/*radius*/ radius, /*slices*/ 10, /*stacks*/ 10);
  glPopAttrib();
}

void drawModel(GameObject* obj) {
  glColor3f(1.0f, 1.0f, 1.0f);  // whitish
  ObjModel& model = models[obj->modelType];

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, model.texture);

  if (obj->modelType == GooseModel) {
    // case for multi-part objects using rigid body animation
    // TODO: generalize this for other model types using other skeletons with
    // retargetable animations

    AnimationFrame* animFrame;
    GooseAnimType curAnim;
    AnimationRange* curAnimRange;
    Vec3d* boneOrigin;

    assert(obj->animState != NULL);
    curAnim = (GooseAnimType)obj->animState->state;
    curAnimRange = &goose_anim_ranges[curAnim];

#if DEBUG_ANIMATION
    glDisable(GL_DEPTH_TEST);
    drawMarker(1.0f, 0.0f, 0.0f, 5);  // origin marker, red
    glEnable(GL_DEPTH_TEST);
#endif

    int frameNum = AnimationState_getAnimFrame(obj->animState, curAnimRange);
    for (int modelMeshIdx = 0; modelMeshIdx < MAX_GOOSE_MESH_TYPE;
         ++modelMeshIdx) {
      int frameDataOffset = frameNum * MAX_GOOSE_MESH_TYPE + modelMeshIdx;
      animFrame = &goose_anim_data[frameDataOffset];
      boneOrigin = &goose_anim_bone_origins[modelMeshIdx];

      // push relative transformation matrix, render the mesh, then pop the
      // relative transform off the matrix stack again
      glPushMatrix();

      // rotate from z-up (blender) to y-up (opengl) coords
      glRotatef(-90.0f, 1, 0, 0);

      glTranslatef(animFrame->position.x, animFrame->position.y,
                   animFrame->position.z);

      glRotatef(animFrame->rotation.x, 1, 0, 0);
      glRotatef(animFrame->rotation.y, 0, 1, 0);
      glRotatef(animFrame->rotation.z, 0, 0, 1);

      ObjMesh& mesh = model.meshes.at(GooseMeshTypeStrings[animFrame->object]);

      // draw mesh
      glBegin(GL_TRIANGLES);
      for (int ivert = 0; ivert < mesh.vertices.size(); ++ivert) {
        glTexCoord2d(mesh.uvs[ivert].x, mesh.uvs[ivert].y);
        glNormal3f(mesh.normals[ivert].x, mesh.normals[ivert].y,
                   mesh.normals[ivert].z);
        glVertex3f(mesh.vertices[ivert].x, mesh.vertices[ivert].y,
                   mesh.vertices[ivert].z);
      }
      glEnd();

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
      glColor3f(1.0f, 0.0f, 0.0f);    // red
      glutSolidCone(4.2, 30, 4, 20);  // cone with 4 slices = pyramid-like

      glColor3f(1.0f, 1.0f, 1.0f);
      glEnable(GL_TEXTURE_2D);
      glPopMatrix();

      // overlay text
      glPushMatrix();
      Vec3d animFrameGlobalPos;
      Vec3d_identity(&animFrameGlobalPos);
      drawStringAtPoint(GooseMeshTypeStrings[animFrame->object],
                        &animFrameGlobalPos, FALSE);
      glPopMatrix();
#endif

      glPopMatrix();
    }

  } else {
    // case for simple gameobjects with no moving sub-parts

    // render each model mesh. usually there will only be one
    std::map<std::string, ObjMesh>::iterator it = model.meshes.begin();
    while (it != model.meshes.end()) {
      // std::cout << "drawing model: " << ModelTypeStrings[obj->modelType]
      //           << " mesh:" << it->first << std::endl;

      ObjMesh& mesh = it->second;

      // draw mesh
      glBegin(GL_TRIANGLES);
      for (int ivert = 0; ivert < mesh.vertices.size(); ++ivert) {
        glTexCoord2d(mesh.uvs[ivert].x, mesh.uvs[ivert].y);
        glNormal3f(mesh.normals[ivert].x, mesh.normals[ivert].y,
                   mesh.normals[ivert].z);
        glVertex3f(mesh.vertices[ivert].x, mesh.vertices[ivert].y,
                   mesh.vertices[ivert].z);
      }
      glEnd();

      it++;
    }
  }

  glDisable(GL_TEXTURE_2D);
}

void resizeWindow(int w, int h) {
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
  gluPerspective(45.0f, ratio, 0.1f, 3000.0f);
  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
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
      glDisable(GL_DEPTH_TEST);
    }
    drawModel(obj);

#if DEBUG_RAYCASTING
    glTranslatef(centroidOffset.x, centroidOffset.y, centroidOffset.z);

    glColor3f(0.9, 0.3, 0.2);  // white

    glutWireSphere(modelTypesProperties[obj->modelType].radius,
                   /*slices*/ 5, /*stacks*/ 5);
#endif
  }
  glPopMatrix();
}

void renderScene(void) {
  int i;
  Game* game;

  game = Game_get();

  Game_update(&input);

  if (game->freeView) {
    game->viewPos = viewPos;
  }

  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);
  int w = glutGet(GLUT_WINDOW_WIDTH);
  int h = glutGet(GLUT_WINDOW_HEIGHT);
  resizeWindow(w, h);

#if USE_LIGHTING

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
#endif

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

  Renderer_sortWorldObjects(sortedObjects, game->worldObjectsCount);

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

#if DEBUG_PHYSICS
  PhysBody* body;
  for (i = 0, body = game->physicsBodies; i < game->physicsBodiesCount;
       i++, body++) {
    glPushMatrix();
    glTranslatef(body->position.x, body->position.y, body->position.z);
    drawPhysBall(body->radius);
    glPopMatrix();
  }
#endif

#if DEBUG_RAYCASTING
  for (i = 0; i < gameRaycastTrace.size(); ++i) {
    RaycastTraceEvent raycast = gameRaycastTrace[i];
    Vec3d rayStart = raycast.origin;
    Vec3d rayEnd = raycast.direction;

    // create end point based on origin and direction
    Vec3d_multiplyScalar(&rayEnd, 10000.0);
    Vec3d_add(&rayEnd, &rayStart);

    if (raycast.result) {
      glColor3f(1.0f, 1.0f, 1.0f);
    } else {
      glColor3f(1.0f, 0.0f, 0.0f);
    }

    glDisable(GL_TEXTURE_2D);
    drawLine(&rayStart, &rayEnd);
  }

  gameRaycastTrace.clear();
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
    }
  }
#endif

  char debugtext[80];
  Vec3d_toString(&game->player.goose->position, debugtext);
  drawString(debugtext, 20, 20);

  char pausedtext[80];
  if (game->paused) {
    strcpy(pausedtext, "paused");
    drawString(pausedtext, w / 2 - strlen(pausedtext) / 2, h / 2);
  }

  char characterString[120];
  Character* character;
  for (i = 0, character = game->characters; i < game->charactersCount;
       i++, character++) {
    Character_toString(character, characterString);
    drawString(characterString, 20, glutGet(GLUT_WINDOW_HEIGHT) - 40 * (i + 1));
  }

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

void updateInputs() {
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
      } else {
        switch (key) {
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

      if (key == 99 && game->tick % 30 == 0) {  // c
        game->freeView = !game->freeView;
      }
    }
  }
}

void processNormalKeysUp(unsigned char key, int _x, int _y) {
  keysDown[key] = false;
}

void processNormalKeysDown(unsigned char key, int _x, int _y) {
  keysDown[key] = true;
  printf("%d\n", key);

  if (key == 27) {  // esc
    exit(0);
  }
}

void updateAndRender() {
  glgooseFrame++;
  updateInputs();
  renderScene();
}

int main(int argc, char** argv) {
  int i;

  Game* game;
  GameObject* obj;

  Game_init(university_map_data, UNIVERSITY_MAP_COUNT);
  game = Game_get();

  Input_init(&input);
  game->viewPos = viewPos;

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
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(1440, 1080);
  glutCreateWindow("Goose");

  // register callbacks
  glutDisplayFunc(renderScene);
  glutReshapeFunc(resizeWindow);
  glutIdleFunc(updateAndRender);
  glutKeyboardFunc(processNormalKeysDown);
  glutKeyboardUpFunc(processNormalKeysUp);

  // OpenGL init
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // load goose
  loadModel(GooseModel, "gooserig.obj", "goosetex.bmp");
  // loadModel(GooseModel, "gooseanim.obj", "goosetex.bmp");
  // loadModel(GooseModel, "goose.obj", "goosetex.bmp");
  loadModel(UniFloorModel, "university_floor.obj", "green.bmp");
  loadModel(UniBldgModel, "university_bldg.obj", "redbldg.bmp");
  loadModel(BushModel, "bush.obj", "bush.bmp");
  loadModel(FlagpoleModel, "flagpole.obj", "flagpole.bmp");
  loadModel(GroundskeeperCharacterModel, "person.obj", "person.bmp");
  loadModel(BookItemModel, "book.obj", "book.bmp");
  loadModel(HomeworkItemModel, "testingCube.obj", "testCubeTex.bmp");

  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
