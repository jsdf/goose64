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
#include "input.h"
#include "renderer.h"
#include "university_map.h"
#include "vec3d.h"

#define RAND(x) (rand() % x) /* random number between 0 to x */
#define DEBUG_LOG_RENDER 0
#define FREEVIEW_SPEED 0.2f

// actual vector representing the camera's direction
float cameraLX = 0.0f, cameraLZ = -1.0f;
// XZ position of the camera
Vec3d viewPos = {0.0f, 1.0f, 0.0f};

float cameraAngle = 0.0f;
bool keysDown[127];
Input input;

typedef struct Model {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  GLuint texture;
} Model;

Model models[MAX_MODEL_TYPE];

// TODO: allocate this in map header file with correct size
static GameObject* sortedObjects[MAX_WORLD_OBJECTS];

void loadModel(ModelType modelType, char* modelfile, char* texfile) {
  std::vector<glm::vec3> normals;  // Won't be used at the moment.

  // the map exporter scales the world up by this much, so we scale up the
  // meshes to match
  loadOBJ(modelfile, models[modelType].vertices, models[modelType].uvs, normals,
          N64_SCALE_FACTOR);
  models[modelType].texture = loadBMP_custom(texfile);
}

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
    if (Renderer_isZBufferedGameObject(obj)) {
      glEnable(GL_DEPTH_TEST);
    } else {
      glDisable(GL_DEPTH_TEST);
    }
    drawModel(obj->modelType);
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

  char debugtext[80];
  Vec3d_toString(&game->player.goose->position, debugtext);
  drawString(debugtext, 20, 20);

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
        }
      }

      if (key == 99 && game->tick % 30 == 0) {  // c
        game->freeView = !game->freeView;
      }
    }
  }
}

void processNormalKeysUp(unsigned char key, int _x, int _y) {
  keysDown[key] = false;
  printf("normals keys up %d\n", key);
}

void processNormalKeysDown(unsigned char key, int _x, int _y) {
  keysDown[key] = true;

  if (key == 27) {  // esc
    exit(0);
  }
}

void updateAndRender() {
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
  loadModel(GooseModel, "goose.obj", "goosetex.bmp");
  loadModel(UniFloorModel, "university_floor.obj", "green.bmp");
  loadModel(UniBldgModel, "university_bldg.obj", "redbldg.bmp");
  loadModel(BushModel, "bush.obj", "bush.bmp");
  loadModel(FlagpoleModel, "flagpole.obj", "flagpole.bmp");
  loadModel(GroundskeeperCharacterModel, "person.obj", "person.bmp");
  loadModel(BookItemModel, "testingCube.obj", "testCubeTex.bmp");
  loadModel(HomeworkItemModel, "testingCube.obj", "testCubeTex.bmp");

  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
