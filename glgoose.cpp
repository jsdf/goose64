#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <OpenGL/gl.h>
#include <glm/glm.hpp>
#include "constants.h"
#include "game.h"
#include "gameobject.h"
#include "gl/objloader.hpp"
#include "gl/texture.hpp"
#include "university_map.h"
#include "vec3d.h"

#define RAND(x) (rand() % x) /* random number between 0 to x */

// actual vector representing the camera's direction
float cameraLX = 0.0f, cameraLZ = -1.0f;
// XZ position of the camera
Vec3d cameraPos = {0.0f, 1.0f, 0.0f};

float cameraAngle = 0.0f;

typedef struct Model {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  GLuint texture;
} Model;

Model models[MAX_MODEL_TYPE];

static GameObject* sortedObjects[MAX_WORLD_OBJECTS];

void loadModel(ModelType modelType, char* modelfile, char* texfile) {
  std::vector<glm::vec3> normals;  // Won't be used at the moment.
  loadOBJ(modelfile, models[modelType].vertices, models[modelType].uvs,
          normals);
  models[modelType].texture = loadBMP_custom(texfile);
}

char* ModelTypeStrings[] = {
    "NoneModel",    "GooseModel",    "BookModel", "CakeModel",
    "UniBldgModel", "UniFloorModel", "BushModel", "MAX_MODEL_TYPE",
};

void drawModel(ModelType modelType) {
  glColor3f(1.0f, 1.0f, 1.0f);  // whitish
  Model model = models[modelType];

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, model.texture);
  glBegin(GL_TRIANGLES);
  for (int ivert = 0; ivert < model.vertices.size(); ++ivert) {
    glTexCoord2d(model.uvs[ivert].x, model.uvs[ivert].y);
    glVertex3f(model.vertices[ivert].x, model.vertices[ivert].y,
               model.vertices[ivert].z);
  }
  glEnd();
}

void drawString(char* string, int x, int y) {
  int w, h;
  char* c;
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glOrtho(0, w, 0, h, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glColor3f(1, 0, 0);

  glRasterPos2i(x, y);
  for (c = string; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
  }

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
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
  Vec3d pos;
  pos = obj->position;

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(obj->rotationZ, 0, 1, 0);

  if (obj->modelType != NoneModel) {
    drawModel(obj->modelType);
  }
  glPopMatrix();
}

// this is a stupid function
float fwrap(float x, float lower, float upper) {
  if (x > upper) {
    return lower;
  } else if (x < lower) {
    return upper;
  }
  return x;
}

void Vec3d_print(Vec3d* self) {
  printf("{x:%.3f, y:%.3f, z:%.3f}", self->x, self->y, self->z);
}

float gameobjectSortDist(GameObject* obj) {
  // always render this at the back
  if (obj->modelType == UniFloorModel) {
    return 10000.0F;
  } else {
    return Vec3d_distanceTo(&obj->position, &cameraPos);
  }
}

int gameobjectDistComparatorFn(const void* a, const void* b) {
  float distA, distB;
  GameObject *objA, *objB;
  objA = *((GameObject**)a);
  objB = *((GameObject**)b);
  distA = gameobjectSortDist(objA);
  distB = gameobjectSortDist(objB);
  return distA - distB;
}

void updateWorld() {
  int i;
  Game* game;
  GameObject* obj;

  game = Game_get();
  obj = game->worldObjects;
  for (i = 0; i < game->worldObjectsCount; i++) {
    if (obj->modelType == GooseModel) {
      obj->rotationZ = fwrap(obj->rotationZ + 2.0F, 0.0F, 360.0F);
    }

    obj++;
  }
}

void renderScene(void) {
  int i;
  Game* game;

  updateWorld();

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
  gluLookAt(                                                        //
      cameraPos.x, cameraPos.y, cameraPos.z,                        // eye
      cameraPos.x + cameraLX, cameraPos.y, cameraPos.z + cameraLZ,  // center
      0.0f, 1.0f, 0.0f                                              // up
  );

  game = Game_get();

  qsort(sortedObjects, MAX_WORLD_OBJECTS, sizeof(char*),
        gameobjectDistComparatorFn);

  printf("draw start\n");
  // render world objects
  for (i = 0; i < MAX_WORLD_OBJECTS; i++) {
    GameObject* obj = sortedObjects[i];
    if (obj->modelType != NoneModel) {
      printf("draw obj %s dist=%.3f {x:%.3f, y:%.3f, z:%.3f}\n",
             ModelTypeStrings[obj->modelType],
             Vec3d_distanceTo(&(obj->position), &cameraPos), obj->position.x,
             obj->position.y, obj->position.z);
      drawGameObject(sortedObjects[i]);
    }
  }

  char debugtext[80];
  Vec3d_toString(&cameraPos, debugtext);
  drawString(debugtext, 20, 20);

  glutSwapBuffers();
}

void updateCameraAngle(float newAngle) {
  cameraAngle = newAngle;
  cameraLX = sin(cameraAngle);
  cameraLZ = -cos(cameraAngle);
}

void turnLeft() {
  updateCameraAngle(cameraAngle - 0.1f);
}

void turnRight() {
  updateCameraAngle(cameraAngle + 0.1f);
}

void moveForward() {
  cameraPos.x += cameraLX * 1.0f;
  cameraPos.z += cameraLZ * 1.0f;
}

void moveBack() {
  cameraPos.x -= cameraLX * 1.0f;
  cameraPos.z -= cameraLZ * 1.0f;
}

void moveLeft() {
  cameraPos.x -= cameraLZ * -1.0f;
  cameraPos.z -= cameraLX * 1.0f;
}

void moveRight() {
  cameraPos.x += cameraLZ * -1.0f;
  cameraPos.z += cameraLX * 1.0f;
}

void moveUp() {
  cameraPos.y += 1.0f;
}

void moveDown() {
  cameraPos.y -= 1.0f;
}

void processNormalKeys(unsigned char key, int _x, int _y) {
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
  if (key == 27) {  // esc
    exit(0);
  }
}

void processSpecialKeys(int key, int xx, int yy) {
  switch (key) {
    case GLUT_KEY_LEFT:
      turnLeft();
      break;
    case GLUT_KEY_RIGHT:
      turnRight();
      break;
    case GLUT_KEY_UP:
      moveUp();
      break;
    case GLUT_KEY_DOWN:
      moveBack();
      break;
  }
}

int main(int argc, char** argv) {
  int i;

  Game* game;
  GameObject* obj;
  GameObject* loadObj;

  Game_init();

  game = Game_get();
  loadObj = university_map_data;
  for (i = 0, obj = game->worldObjects; i < MAX_WORLD_OBJECTS; i++, obj++) {
    if (i < UNIVERSITY_MAP_COUNT) {
      memcpy(obj, loadObj, sizeof(GameObject));
      // the map exporter scales the world up by this much, so we reverse it
      // in for the opengl version
      obj->position.x = obj->position.x / (N64_SCALE_FACTOR * 1.0F);
      obj->position.y = obj->position.y / (N64_SCALE_FACTOR * 1.0F);
      obj->position.z = obj->position.z / (N64_SCALE_FACTOR * 1.0F);

      printf("loaded obj %s  {x:%.3f, y:%.3f, z:%.3f}\n",
             ModelTypeStrings[obj->modelType], obj->position.x, obj->position.y,
             obj->position.z);
      loadObj++;
    } else {
      // default object
      Vec3d_set(&obj->position, RAND(100 - 100 / 2), 2, RAND(100 - 100 / 2));
      obj->rotationZ = RAND(360);
      obj->modelType = NoneModel;
    }
  }

  // init sortedObjects pointers array
  for (i = 0, obj = game->worldObjects; i < MAX_WORLD_OBJECTS; i++, obj++) {
    sortedObjects[i] = obj;
  }

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("Goose");

  // register callbacks
  glutDisplayFunc(renderScene);
  glutReshapeFunc(resizeWindow);
  glutIdleFunc(renderScene);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);

  // OpenGL init
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // load goose
  loadModel(GooseModel, "goose.obj", "goosetex.bmp");
  loadModel(UniFloorModel, "university_floor.obj", "green.bmp");
  loadModel(UniBldgModel, "university_bldg.obj", "redbldg.bmp");
  loadModel(BushModel, "bush.obj", "bush.bmp");

  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
