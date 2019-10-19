/*
   stage00.c

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include <stdlib.h>
#include "bush.h"
#include "constants.h"
#include "game.h"
#include "gameobject.h"
#include "goose.h"
#include "graphic.h"
#include "main.h"
#include "modeltype.h"
#include "testingCube.h"
#include "university_bldg.h"
#include "university_floor.h"
#include "university_map.h"
#include "vec3d.h"

static float modelRot; /* The rotational angle of the model */

static Vec3d viewPos;
static Vec3d viewRot;

static u16 perspNorm;
static u32 near_plane; /* Near Plane */
static u32 far_plane;  /* Far Plane */

static float vel;
static int modeSetting;
static GameObject* sortedObjects[MAX_WORLD_OBJECTS];

void drawStuff(Dynamic* dynamicp);

#define OBJ_START_VAL 1000

/* The initialization of stage 0 */
void initStage00() {
  Game* game;
  GameObject* obj;
  GameObject* loadObj;
  int i;

  modelRot = 0.0;
  vel = 1.0;
  modeSetting = 1;
  near_plane = 10;
  far_plane = 10000;
  Vec3d_init(&viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&viewRot, 0.0F, 0.0F, 0.0F);

  Game_init();

  game = Game_get();
  obj = game->worldObjects;
  loadObj = university_map_data;

  assert(UNIVERSITY_MAP_COUNT <= MAX_WORLD_OBJECTS);
  for (i = 0; i < MAX_WORLD_OBJECTS; i++) {
    if (i < UNIVERSITY_MAP_COUNT) {
      memcpy(obj, loadObj, sizeof(GameObject));
      loadObj++;
    } else {
      obj->position.x = RAND(OBJ_START_VAL) - OBJ_START_VAL / 2;
      obj->position.y = RAND(OBJ_START_VAL) - OBJ_START_VAL / 2;
      obj->position.z = -50.0F + RAND(100);
      if (i % 2 == 0) {
        obj->modelType = BushModel;
      } else {
        obj->modelType = GooseModel;
      }
      obj->modelType = NoneModel;
    }

    obj++;
  }
}

/* Make the display list and activate the task */
void makeDL00() {
  Dynamic* dynamicp;
  char conbuf[20];

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  /* projection, viewing, modeling matrix set */
  guPerspective(&dynamicp->projection, &perspNorm, 30,
                (f32)SCREEN_WD / (f32)SCREEN_HT, near_plane, far_plane, 1.0);

  guPosition(&dynamicp->camera,
             viewRot.x,  // roll
             viewRot.y,  // pitch
             viewRot.z,  // yaw
             1.0F,       // scale
             viewPos.x, viewPos.y, viewPos.z);

  drawStuff(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and
     switch display buffers */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
                 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);

  // debug text overlay
  if (contPattern & 0x1) {
    /* Change character representation positions */
    nuDebConTextPos(0, 12, 23);
    sprintf(conbuf, "viewPos.x=%5.1f", viewPos.x);
    nuDebConCPuts(0, conbuf);

    nuDebConTextPos(0, 12, 24);
    sprintf(conbuf, "viewPos.y=%5.1f", viewPos.y);
    nuDebConCPuts(0, conbuf);

    nuDebConTextPos(0, 12, 25);
    sprintf(conbuf, "viewPos.z=%5.1f", viewPos.z);
    nuDebConCPuts(0, conbuf);

    nuDebConTextPos(0, 12, 26);
    sprintf(conbuf, "modelRot=%5.1f", modelRot);
    nuDebConCPuts(0, conbuf);

    nuDebConTextPos(0, 12, 27);
    sprintf(conbuf, "viewPos=%p", viewPos);
    nuDebConCPuts(0, conbuf);
  } else {
    nuDebConTextPos(0, 9, 24);
    nuDebConCPuts(0, "Controller1 not connect");
  }

  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}

void moveWorldObjects() {
  Game* game;
  GameObject* obj;
  int i;

  game = Game_get();
  obj = game->worldObjects;
  for (i = 0; i < MAX_WORLD_OBJECTS; i++) {
    continue;  // no
    obj->position.x += 0.01F;
    obj->position.y += 0.01F;
    obj->position.z -= 0.1F;

    obj++;
  }
}

/* The game progressing process for stage 0 */
void updateGame00(void) {
  /* Data reading of controller 1 */
  nuContDataGetEx(contdata, 0);

  /* Change the display position by stick data */
  viewRot.x = contdata->stick_y;  // rot around x
  viewRot.y = contdata->stick_x;  // rot around y

  /* The reverse rotation by the A button */
  if (contdata[0].trigger & A_BUTTON) {
    vel = -vel;
  }

  /* Rotate fast while pushing the B button */
  // if (contdata[0].button & B_BUTTON) {
  //   modelRot += vel * 3.0;
  // } else {
  //   modelRot += vel;
  // }

  if (contdata[0].button & B_BUTTON) {
    modeSetting = -modeSetting;
  }

  if (modelRot > 360.0) {
    modelRot -= 360.0;
  } else if (modelRot < 0.0) {
    modelRot += 360.0;
  }

  /* Change the moving speed with up/down buttons of controller */
  if (contdata[0].button & U_JPAD) {
    viewPos.z += 10.0;
  }

  if (contdata[0].button & D_JPAD) {
    viewPos.z -= 10.0;
  }

  /* It comes back near if it goes too far */
  if (viewPos.z < (600.0 - far_plane)) {
    viewPos.z = 600.0 - near_plane;
  }

  /* It goes back far if it comes too near */
  else if (viewPos.z > (600.0 - near_plane)) {
    viewPos.z = 600.0 - far_plane;
  }

  /* << XY axis shift process >> */

  /* Move left/right with left/right buttons of controller */
  if (contdata[0].button & L_JPAD)
    viewPos.x -= 1.0;

  if (contdata[0].button & R_JPAD)
    viewPos.x += 1.0;

  if (contdata[0].button & U_CBUTTONS)
    viewPos.y -= 30.0;

  if (contdata[0].button & D_CBUTTONS) {
    viewPos.y += 30.0;
  }

  if (contdata[0].button & L_CBUTTONS) {
    viewPos.x -= 30.0;
  }

  if (contdata[0].button & R_CBUTTONS) {
    viewPos.x += 30.0;
  }

  moveWorldObjects();
}

int gameobjectDistComparatorFn(GameObject* a, GameObject* b) {
  float distA, distB;
  distA = Vec3d_distanceTo(&(a->position), &viewPos);
  distB = Vec3d_distanceTo(&(b->position), &viewPos);
  return distA - distB;
}

/* Draw a square */
void drawStuff(Dynamic* dynamicp) {
  Game* game;
  GameObject* obj;
  int i;

  game = Game_get();

  for (i = 0, obj = game->worldObjects; i < MAX_WORLD_OBJECTS; i++, obj++) {
    sortedObjects[i] = obj;
  }

  // qsort(sortedObjects, MAX_WORLD_OBJECTS, sizeof(GameObject*),
  //       gameobjectDistComparatorFn);

  // render world objects
  for (i = 0; i < MAX_WORLD_OBJECTS; i++) {
    obj = sortedObjects[i];
    if (obj->modelType == NoneModel) {
      continue;
    }
    // setup view
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
              G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->camera)),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    gSPClearGeometryMode(glistp++, 0xFFFFFFFF);
    gDPSetCycleType(glistp++, vel > 0.0F ? G_CYC_1CYCLE : G_CYC_2CYCLE);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPSetGeometryMode(glistp++, G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH |
                                     G_LIGHTING | G_CULL_BACK);

    // render textured models
    gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTextureFilter(glistp++, G_TF_BILERP);
    gDPSetTexturePersp(glistp++, G_TP_PERSP);

    if (modeSetting > 0) {
      gDPSetCombineMode(glistp++, G_CC_MODULATERGB, G_CC_MODULATERGB);
    } else {
      gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB);
    }
    guPosition(&dynamicp->objTransform[i],
               0.0F,            // roll
               obj->rotationZ,  // pitch. but actually rot around z ???
               0.0F,            // yaw
               1.0F,            // scale
               obj->position.x, obj->position.y, obj->position.z);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->objTransform[i])),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    switch (obj->modelType) {
      case GooseModel:
        gSPDisplayList(glistp++, Wtx_goose);
        break;
      case BushModel:
        gSPDisplayList(glistp++, Wtx_bush);
        break;
      case UniBldgModel:
        gSPDisplayList(glistp++, Wtx_university_bldg);
        break;
      case UniFloorModel:
        gSPDisplayList(glistp++, Wtx_university_floor);
        break;
      default:
        gSPDisplayList(glistp++, Wtx_testingCube);
        break;
    }

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);

    gDPPipeSync(glistp++);
  }
}
