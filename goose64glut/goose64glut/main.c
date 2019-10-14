
/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

#include <stdlib.h>
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#else
#define random rand
#define srandom srand
#endif
#include <math.h>
#include <GLUT/glut.h>

#include "gameobject.h"
#include "game.h"

#define GL_SILENCE_DEPRECATION 1
/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632
#endif

GLboolean moving = GL_FALSE;

#define MAX_PLANES 15

struct {
  float speed;          /* zero speed means not flying */
  GLfloat red, green, blue;
  float theta;
  float x, y, z, angle;
} planes[MAX_PLANES];

#define v3f glVertex3f  /* v3f was the short IRIS GL name for
                           glVertex3f */
// angle of rotation for the camera direction
float angle=0.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,y=0.0f,z=5.0f;
void drawSnowMan() {

    glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
    glTranslatef(0.0f ,0.75f, 0.0f);
    glutSolidSphere(0.75f,20,20);

// Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f,20,20);

// Draw Eyes
    glPushMatrix();
        glColor3f(0.0f,0.0f,0.0f);
        glTranslatef(0.05f, 0.10f, 0.18f);
        glutSolidSphere(0.05f,10,10);
        glTranslatef(-0.1f, 0.0f, 0.0f);
        glutSolidSphere(0.05f,10,10);
    glPopMatrix();

// Draw Nose
    glColor3f(1.0f, 0.5f , 0.5f);
    glutSolidCone(0.08f,0.5f,10,2);
}

void drawString(char *string)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();             
    glLoadIdentity();   
    int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );
    glOrtho( 0, w, 0, h, -1, 1 );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
 
    glColor3f(1, 0, 0);

    glRasterPos2i(20, 20);
    void *font = GLUT_BITMAP_HELVETICA_18; 
    for (char* c=string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(font, *c); 
    }   

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  
}


void changeSize(int w, int h)
{

// Prevent a divide by zero, when window is too short
// (you cant make a window of zero width).
if (h == 0)
h = 1;
float ratio = w * 1.0 / h;

// Use the Projection Matrix
glMatrixMode(GL_PROJECTION);

// Reset Matrix
glLoadIdentity();

// Set the viewport to be the entire window
glViewport(0, 0, w, h);

// Set the correct perspective.
gluPerspective(45.0f, ratio, 0.1f, 100.0f);

// Get Back to the Modelview
glMatrixMode(GL_MODELVIEW);
}




void renderScene(void) {

    // Clear Color and Depth Buffers
    
    printf("y=%F\n", y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    int w = glutGet( GLUT_WINDOW_WIDTH );
    int h = glutGet( GLUT_WINDOW_HEIGHT );
    changeSize(w, h);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(    x, y+1.0F , z,
            x+lx, y+1.0F,  z+lz,
            0.0f, 1.0f,  0.0f);

        // Draw ground
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f,  100.0f);
        glVertex3f( 100.0f, 0.0f, -100.0f);
    glEnd();
    
    Game* game = Game_get();
    GameObject* worldObjectPtr = game->worldObjects;
    for (int i = 0; i < game->worldObjectsCount; i++) {
        GameObject* obj = worldObjectPtr;
        glPushMatrix();
        glTranslatef(obj->position.x, obj->position.y, obj->position.z);
        drawSnowMan();
        glPopMatrix();
        worldObjectPtr++;
    }

    char debugtext[80];
    sprintf(debugtext, "x=%f y=%f z=%f ", x, y, z);
    drawString(debugtext);

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
    
    printf("keycode %d\n", key);
    
    float fraction = 1.1f;

    switch (key) {
        case 97 : //a
            angle -= fraction * 0.1f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case 100 ://d
            angle += fraction * 0.1f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case 119 :// w
            x += lx * fraction;
            z += lz * fraction;
            break;
        case 115 :// s
            x -= lx * fraction;
            z -= lz * fraction;
            break;
        case 101 :// e
            y +=  fraction;
            break;
        case 99 :// c
            y -= fraction;
            break;
            
    }
    

if (key == 27)
exit(0);
}



void processSpecialKeys(int key, int xx, int yy) {

    float fraction = 1.1f;

    switch (key) {
        case GLUT_KEY_LEFT :
            angle -= fraction * 0.1f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT :
            angle += fraction * 0.1f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_UP :
            x += lx * fraction;
            z += lz * fraction;
            break;
        case GLUT_KEY_DOWN :
            x -= lx * fraction;
            z -= lz * fraction;
            break;
    }
}


float randfloat(float max) {
    return (float)rand()/(float)(RAND_MAX/max);
}

int main(int argc, char *argv[]) {
    float spread = 100;
    Game_init();
    Game* game = Game_get();
    GameObject* worldObjectPtr = game->worldObjects;
    for (int i = 0; i < game->worldObjectsCount; i++) {
        GameObject* obj = worldObjectPtr;
        obj->position.x = randfloat(spread - spread/2) * 1.0F;
        obj->position.y = 0.0F;
        obj->position.z = randfloat(spread - spread/2) * 1.0F;

        worldObjectPtr++;
    }
    
    
    
    
    
    // init GLUT and create window

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(300,300);
    glutInitWindowSize(1024,768);
    glutCreateWindow("goose64glut");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

  return 0;             /* ANSI C requires main to return int. */
}
