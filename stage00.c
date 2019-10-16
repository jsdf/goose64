/*
   stage00.c

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include <stdlib.h>
#include "constants.h"
#include "game.h"
#include "gameobject.h"
#include "goose1baked.h"
#include "graphic.h"
#include "main.h"
#include "modeltype.h"
#include "testingCube.h"
#include "vec3d.h"

static float theta;    /* The rotational angle of the square */
static float triPos_x; /* The display position-X */
static float triPos_y; /* The display position-Y */

static Vec3d viewPos;

static u16 perspNorm;
static u32 near_plane; /* Near Plane */
static u32 far_plane;  /* Far Plane */

void drawStuff(Dynamic* dynamicp);

#define OBJ_START_VAL 1000

/* The initialization of stage 0 */
void initStage00() {
  Game* game;
  GameObject* worldObjectPtr;
  GameObject* obj;
  int i;

  triPos_x = 0.0;
  triPos_y = 0.0;
  theta = 0.0;
  near_plane = 10;
  far_plane = 10000;
  Vec3d_init(&viewPos, 0.0F, 0.0F, -400.0F);

  Game_init();

  game = Game_get();
  worldObjectPtr = game->worldObjects;
  for (i = 0; i < MAX_WORLD_OBJECTS; i++) {
    obj = worldObjectPtr;
    obj->position.x = RAND(OBJ_START_VAL) - OBJ_START_VAL / 2;
    obj->position.y = RAND(OBJ_START_VAL) - OBJ_START_VAL / 2;
    obj->position.z = -50.0F;
    if (i == 0) {
      obj->modelType = NoneModel;
    }
    obj->modelType = GooseModel;

    worldObjectPtr++;
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

  guTranslate(&dynamicp->translate, viewPos.x, viewPos.y, viewPos.z);

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
    sprintf(conbuf, "theta=%5.1f", theta);
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
    obj->position.x += 0.01F;
    obj->position.y += 0.01F;
    obj->position.z -= 0.1F;

    obj++;
  }
}

/* The game progressing process for stage 0 */
void updateGame00(void) {
  static float vel = 1.0;

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata, 0);

  /* Change the display position by stick data */
  viewPos.x = contdata->stick_x;
  triPos_y = contdata->stick_y;

  /* The reverse rotation by the A button */
  if (contdata[0].trigger & A_BUTTON) {
    vel = -vel;
  }

  /* Rotate fast while pushing the B button */
  if (contdata[0].button & B_BUTTON)
    theta += vel * 3.0;
  else
    theta += vel;

  if (theta > 360.0)
    theta -= 360.0;
  else if (theta < 0.0)
    theta += 360.0;

  /* Change the moving speed with up/down buttons of controller */
  if (contdata[0].button & U_JPAD)
    viewPos.z += 10.0;

  if (contdata[0].button & D_JPAD)
    viewPos.z -= 10.0;

  /* It comes back near if it goes too far */
  if (viewPos.z < (600.0 - far_plane))
    viewPos.z = 600.0 - near_plane;

  /* It goes back far if it comes too near */
  else if (viewPos.z > (600.0 - near_plane))
    viewPos.z = 600.0 - far_plane;

  /* << XY axis shift process >> */

  /* Move left/right with left/right buttons of controller */
  if (contdata[0].button & L_JPAD)
    viewPos.x -= 1.0;

  if (contdata[0].button & R_JPAD)
    viewPos.x += 1.0;

  moveWorldObjects();
}

/* Draw a square */
void drawStuff(Dynamic* dynamicp) {
  Game* game;
  GameObject* worldObjectPtr;
  GameObject* obj;
  int i;
  game = Game_get();

  // render world objects
  worldObjectPtr = game->worldObjects;
  for (i = 0; i < MAX_WORLD_OBJECTS; i++) {
    // setup view
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
              G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
              G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    gSPClearGeometryMode(glistp++, 0xFFFFFFFF);
    gDPSetCycleType(glistp++, G_CYC_1CYCLE);
    gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPSetGeometryMode(glistp++, G_ZBUFFER);

    // render textured models
    gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTextureFilter(glistp++, G_TF_BILERP);
    gDPSetTexturePersp(glistp++, G_TP_PERSP);
    gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB);

    obj = worldObjectPtr;
    guPosition(&dynamicp->obj_trans[i],
               0.0F,            // roll
               theta,           // pitch
               obj->rotationZ,  // yaw
               1.0F,            // scale
               obj->position.x, obj->position.y, obj->position.z);
    // guTranslate(&dynamicp->obj_trans[i], obj->position.x, obj->position.y,
    //             obj->position.z);
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->obj_trans[i])),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    if (obj->modelType == GooseModel) {
      gSPDisplayList(glistp++, Wtx_goose1baked);
    } else {
      gSPDisplayList(glistp++, Wtx_testingCube);  // use cube for now
    }
    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);

    gDPPipeSync(glistp++);
    worldObjectPtr++;
  }
}
