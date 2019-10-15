#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "game.h"
#include "gameobject.h"
#include "vec3d.h"

#define RAND(x) (rand() % x) /* random number between 0 to x */

// actual vector representing the camera's direction
float cameraLX = 0.0f, cameraLZ = -1.0f;
// XZ position of the camera
Vec3d cameraPos = {0.0f, 1.0f, 0.0f};

#define NUM_WORLD_OBJECTS 36
static GameObject worldObjects[NUM_WORLD_OBJECTS];

float cameraAngle = 0.0f;

// int dummy = 5;

void drawSnowMan() {
  glColor3f(1.0f, 1.0f, 1.0f);

  // Draw Body
  glTranslatef(0.0f, 0.75f, 0.0f);
  glutSolidSphere(0.75f, 20, 20);

  // Draw Head
  glTranslatef(0.0f, 1.0f, 0.0f);
  glutSolidSphere(0.25f, 20, 20);

  // Draw Eyes
  glPushMatrix();
  glColor3f(0.0f, 0.0f, 0.0f);
  glTranslatef(0.05f, 0.10f, 0.18f);
  glutSolidSphere(0.05f, 10, 10);
  glTranslatef(-0.1f, 0.0f, 0.0f);
  glutSolidSphere(0.05f, 10, 10);
  glPopMatrix();

  // Draw Nose
  glColor3f(1.0f, 0.5f, 0.5f);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
  glutSolidCone(0.08f, 0.5f, 10, 2);
}

void drawString(char* string) {
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

  glRasterPos2i(20, 20);
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
  drawSnowMan();
  glPopMatrix();
}

void renderScene(void) {
  int i;
  Game* game;
  GameObject* worldObjectPtr;
  // Clear Color and Depth Buffers

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glLoadIdentity();
  // Set the camera
  gluLookAt(                                                        //
      cameraPos.x, cameraPos.y, cameraPos.z,                        // eye
      cameraPos.x + cameraLX, cameraPos.y, cameraPos.z + cameraLZ,  // center
      0.0f, 1.0f, 0.0f                                              // up
  );

  // Draw ground
  glColor3f(0.9f, 0.9f, 0.9f);
  glBegin(GL_QUADS);
  glVertex3f(-100.0f, 0.0f, -100.0f);
  glVertex3f(-100.0f, 0.0f, 100.0f);
  glVertex3f(100.0f, 0.0f, 100.0f);
  glVertex3f(100.0f, 0.0f, -100.0f);
  glEnd();

  // Draw 36 SnowMen
  // for (i = 0; i < NUM_WORLD_OBJECTS; ++i) {
  //   drawGameObject(&worldObjects[i]);
  // }

  game = Game_get();
  worldObjectPtr = game->worldObjects;
  for (i = 0; i < game->worldObjectsCount; i++) {
    drawGameObject(worldObjectPtr);
    worldObjectPtr++;
  }

  // char debugtext[80];
  // sprintf(debugtext, "x=%f y=%f z=%f ", cameraPos.x, cameraPos.y,
  // cameraPos.z); drawString(debugtext);

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
    case 102:  // f
      moveUp();
      break;
    case 99:  // c
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

  Game_init();

  game = Game_get();
  obj = game->worldObjects;
  for (i = 0; i < game->worldObjectsCount; i++) {
    Vec3d_set(&obj->position, RAND(100 - 100 / 2), 0.0F, RAND(100 - 100 / 2));
    obj->rotationZ = RAND(360);

    obj++;
  }

  // create snowmans

  // objIdx = 0;
  // for (i = -3; i < 3; i++) {
  //   for (j = -3; j < 3; j++) {
  //     GameObject_init(obj, NULL);
  //     Vec3d_set(&obj->position, i * 10.0, 0, j * 10.0);
  //     obj++;
  //   }
  // }

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(300, 100);
  glutInitWindowSize(800, 600);
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

  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
