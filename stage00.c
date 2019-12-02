/*
   stage00.c

  the main game file
*/

#include <assert.h>
#include <nusys.h>
#include <stdlib.h>

// game
#include "animation.h"
#include "constants.h"
#include "game.h"
#include "gameobject.h"
#include "graphic.h"
#include "input.h"
#include "main.h"
#include "modeltype.h"
#include "renderer.h"
#include "vec2d.h"
#include "vec3d.h"

// models
#include "book.h"
#include "bush.h"
#include "flagpole.h"
#include "gooserig.h"
#include "person.h"
#include "testingCube.h"
#include "university_bldg.h"
#include "university_floor.h"
// map
#include "university_map.h"
// anim data
#include "goose_anim.h"

#define CONSOLE_VIEW_DEBUG 0
#define CONSOLE_DEBUG 1

typedef enum RenderMode {
  TextureAndLightingRenderMode,
  TextureNoLightingRenderMode,
  NoTextureNoLightingRenderMode,
  LightingNoTextureRenderMode,
  MAX_RENDER_MODE
} RenderMode;

static Vec3d viewPos;
static Vec3d viewRot;
static Input input;

static u16 perspNorm;
static u32 nearPlane; /* Near Plane */
static u32 farPlane;  /* Far Plane */

static float cycleMode;
static RenderMode renderModeSetting;
static GameObject* sortedObjects[MAX_WORLD_OBJECTS];

void drawWorldObjects(Dynamic* dynamicp);

#define OBJ_START_VAL 1000

Lights1 sun_light = gdSPDefLights1(120,
                                   120,
                                   120, /* weak ambient light */
                                   255,
                                   255,
                                   255, /* white light */
                                   80,
                                   80,
                                   0);

Lights0 amb_light = gdSPDefLights0(200, 200, 200 /*  ambient light */);

/* The initialization of stage 0 */
void initStage00() {
  Game* game;
  GameObject* obj;
  GameObject* loadObj;
  int i;

  cycleMode = 1.0;
  renderModeSetting = TextureAndLightingRenderMode;
  nearPlane = 10;
  farPlane = 10000;
  Vec3d_init(&viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&viewRot, 0.0F, 0.0F, 0.0F);
  Input_init(&input);

  Game_init(university_map_data, UNIVERSITY_MAP_COUNT);
  assert(UNIVERSITY_MAP_COUNT <= MAX_WORLD_OBJECTS);

  game = Game_get();

  // init sortedObjects pointer array, used for painter's algorithm rendering
  for (i = 0, obj = game->worldObjects; i < game->worldObjectsCount;
       i++, obj++) {
    sortedObjects[i] = obj;
  }
}

int debugPrintVec3d(int x, int y, char* label, Vec3d* vec) {
  char conbuf[50];
  nuDebConTextPos(0, x, y++);
  sprintf(conbuf, "%s", label);
  nuDebConCPuts(0, conbuf);
  nuDebConTextPos(0, x, y++);
  sprintf(conbuf, "{x=%5.1f", vec->x);
  nuDebConCPuts(0, conbuf);
  nuDebConTextPos(0, x, y++);
  sprintf(conbuf, "y=%5.1f", vec->y);
  nuDebConCPuts(0, conbuf);
  nuDebConTextPos(0, x, y++);
  sprintf(conbuf, "z=%5.1f}", vec->z);
  nuDebConCPuts(0, conbuf);
  return y;
}

/* Make the display list and activate the task */
void makeDL00() {
  Game* game;
  Dynamic* dynamicp;
  char conbuf[50];
  int consoleOffset;
  consoleOffset = 20;

  game = Game_get();

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  /* projection, viewing, modeling matrix set */
  guPerspective(&dynamicp->projection, &perspNorm, 45,
                (f32)SCREEN_WD / (f32)SCREEN_HT, nearPlane, farPlane, 1.0);
  gSPPerspNormalize(glistp++, perspNorm);

  if (game->freeView) {
    guPosition(&dynamicp->camera,
               viewRot.x,  // roll
               viewRot.y,  // pitch
               viewRot.z,  // yaw
               1.0F,       // scale
               viewPos.x, viewPos.y, viewPos.z);
  } else {
    guLookAt(&dynamicp->camera, game->viewPos.x, game->viewPos.y,
             game->viewPos.z, game->viewTarget.x, game->viewTarget.y,
             game->viewTarget.z, 0.0f, 1.0f, 0.0f);
  }

  drawWorldObjects(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and
     switch display buffers */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
                 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, NU_SC_NOSWAPBUFFER);

// debug text overlay
#if CONSOLE_VIEW_DEBUG
  if (contPattern & 0x1) {
    if (game->freeView) {
      consoleOffset = 23;
      nuDebConTextPos(0, 12, consoleOffset++);
      sprintf(conbuf, "viewPos.x=%5.1f", viewPos.x);
      nuDebConCPuts(0, conbuf);

      nuDebConTextPos(0, 12, consoleOffset++);
      sprintf(conbuf, "viewPos.y=%5.1f", viewPos.y);
      nuDebConCPuts(0, conbuf);

      nuDebConTextPos(0, 12, consoleOffset++);
      sprintf(conbuf, "viewPos.z=%5.1f", viewPos.z);
      nuDebConCPuts(0, conbuf);
    } else {
      consoleOffset = 20;
      consoleOffset =
          debugPrintVec3d(4, consoleOffset, "viewPos", &game->viewPos);
      consoleOffset =
          debugPrintVec3d(4, consoleOffset, "viewTarget", &game->viewTarget);
      nuDebConTextPos(0, 4, consoleOffset++);
      sprintf(conbuf, "retrace=%d", nuScRetraceCounter);
      nuDebConCPuts(0, conbuf);
    }
  } else {
    nuDebConTextPos(0, 9, 24);
    nuDebConCPuts(0, "Controller1 not connect");
  }
#endif

  /* Display characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}

void checkDebugControls(Game* game) {
  /* Change the display position by stick data */
  viewRot.x = contdata->stick_y;  // rot around x
  viewRot.y = contdata->stick_x;  // rot around y

  /* The reverse rotation by the A button */
  if (contdata[0].trigger & A_BUTTON) {
    cycleMode = -cycleMode;
  }
  if (contdata[0].button & B_BUTTON) {
    renderModeSetting++;
    if (renderModeSetting >= MAX_RENDER_MODE) {
      renderModeSetting = 0;
    }
  }
  /* Change the moving speed with up/down buttons of controller */
  if (contdata[0].button & U_JPAD)
    viewPos.z += 10.0;
  if (contdata[0].button & D_JPAD)
    viewPos.z -= 10.0;

  if (viewPos.z < (600.0 - farPlane)) {
    /* It comes back near if it goes too far */
    viewPos.z = 600.0 - nearPlane;
  } else if (viewPos.z > (600.0 - nearPlane)) {
    /* It goes back far if it comes too near */
    viewPos.z = 600.0 - farPlane;
  }

  /* << XY axis shift process >> */
  /* Move left/right with left/right buttons of controller */
  if (contdata[0].button & L_JPAD)
    viewPos.x -= 1.0;
  if (contdata[0].button & R_JPAD)
    viewPos.x += 1.0;
  if (contdata[0].button & U_CBUTTONS)
    viewPos.y -= 30.0;
  if (contdata[0].button & D_CBUTTONS)
    viewPos.y += 30.0;
  if (contdata[0].button & L_CBUTTONS)
    viewPos.x -= 30.0;
  if (contdata[0].button & R_CBUTTONS)
    viewPos.x += 30.0;
}

/* The game progressing process for stage 0 */
void updateGame00(void) {
  Game* game;

  game = Game_get();

  Vec2d_identity(&input.direction);

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata, 0);
  if (contdata[0].button & START_BUTTON) {
    renderModeSetting++;
    if (renderModeSetting >= MAX_RENDER_MODE) {
      renderModeSetting = 0;
    }
  }

  if (game->freeView) {
    checkDebugControls(game);
  } else {
    // normal controls
    if (contdata[0].trigger & A_BUTTON) {
      input.run = TRUE;
    }
    if (contdata[0].trigger & B_BUTTON) {
      input.pickup = TRUE;
    }
    if (contdata[0].button & U_CBUTTONS) {
      farPlane += 100.0;
    }

    if (contdata[0].button & D_CBUTTONS) {
      farPlane -= 100.0;
    }
    input.direction.x = -contdata->stick_x / 61.0F;
    input.direction.y = contdata->stick_y / 63.0F;
    if (Vec2d_length(&input.direction) > 1.0F) {
      Vec2d_normalise(&input.direction);
    }
  }

  Game_update(&input);
}

typedef enum LightingType {
  SunLighting,
  OnlyAmbientLighting,
  MAX_LIGHTING_TYPE
} LightingType;

LightingType getLightingType(GameObject* obj) {
  switch (obj->modelType) {
    case UniFloorModel:
      return OnlyAmbientLighting;
    default:
      return SunLighting;
  }
}

// map the mesh type enum (used by animation frames) to the mesh displaylists
Gfx* gooseMeshList[] = {
    /*goosebody_goosebodymesh*/ Wtx_gooserig_goosebody_goosebodymesh,
    /*goosehead_gooseheadmesh*/ Wtx_gooserig_goosehead_gooseheadmesh,
    /*gooseleg_l_gooseleg_lmesh*/ Wtx_gooserig_gooseleg_l_gooseleg_lmesh,
    /*goosefoot_l_goosefoot_lmesh*/ Wtx_gooserig_goosefoot_l_goosefoot_lmesh,
    /*gooseleg_r_gooseleg_rmesh*/ Wtx_gooserig_gooseleg_r_gooseleg_rmesh,
    /*goosefoot_r_goosefoot_rmesh*/ Wtx_gooserig_goosefoot_r_goosefoot_rmesh,
    /*gooseneck_gooseneckmesh*/ Wtx_gooserig_gooseneck_gooseneckmesh,
};

Gfx* getModelDisplayList(GameObject* obj) {
  switch (obj->modelType) {
    case BushModel:
      return Wtx_bush;
    case BookItemModel:
      return Wtx_book;
    case UniBldgModel:
      return Wtx_university_bldg;
    case UniFloorModel:
      return Wtx_university_floor;
    case FlagpoleModel:
      return Wtx_flagpole;
    case GroundskeeperCharacterModel:
      return Wtx_person;
    default:
      return Wtx_testingCube;
  }
}

void drawWorldObjects(Dynamic* dynamicp) {
  Game* game;
  GameObject* obj;
  int i, useZBuffering;
  int modelMeshIdx;
  Gfx* modelDisplayList;
  AnimationFrame animFrame;
  AnimationInterpolation animInterp;
  AnimationRange* curAnimRange;
  GooseAnimType curAnim;

  game = Game_get();

  Renderer_sortWorldObjects(sortedObjects, game->worldObjectsCount);

  // render world objects
  for (i = 0; i < game->worldObjectsCount; i++) {
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
    gDPSetCycleType(glistp++, cycleMode > 0.0F ? G_CYC_1CYCLE : G_CYC_2CYCLE);
    useZBuffering = Renderer_isZBufferedGameObject(obj);
    if (useZBuffering) {
      gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
      gSPSetGeometryMode(glistp++, G_ZBUFFER);
    } else {
      gDPSetRenderMode(glistp++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
      gSPClearGeometryMode(glistp++, G_ZBUFFER);
    }

    // render textured models
    gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
    gDPSetTextureFilter(glistp++, G_TF_BILERP);
    gDPSetTexturePersp(glistp++, G_TP_PERSP);

    if (getLightingType(obj) == OnlyAmbientLighting) {
      gSPSetLights0(glistp++, amb_light);

    } else {
      gSPSetLights1(glistp++, sun_light);
    }

    if (renderModeSetting == TextureNoLightingRenderMode) {
      gSPSetGeometryMode(glistp++, G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK);
      gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB);
    } else if (renderModeSetting == TextureAndLightingRenderMode) {
      gSPSetGeometryMode(glistp++,
                         G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_CULL_BACK);
      gDPSetCombineMode(glistp++, G_CC_MODULATERGB, G_CC_MODULATERGB);
    } else if (renderModeSetting == LightingNoTextureRenderMode) {
      gSPSetGeometryMode(glistp++,
                         G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | G_CULL_BACK);
      gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
    } else {  // NoTextureNoLightingRenderMode
      gDPSetPrimColor(glistp++, 0, 0, /*r*/ 180, /*g*/ 180, /*b*/ 180,
                      /*a*/ 255);
      gSPSetGeometryMode(glistp++, G_CULL_BACK);
      gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    }

    // set the transform in world space for the gameobject to render
    guPosition(&dynamicp->objTransform[i],
               0.0F,             // rot x
               obj->rotation.y,  // rot y
               0.0F,             // rot z
               1.0F,             // scale
               obj->position.x,  // pos x
               obj->position.y,  // pos y
               obj->position.z   // pos z
    );
    gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->objTransform[i])),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    if (obj->modelType == GooseModel) {
      // case for multi-part objects using rigid body animation

      curAnim = (GooseAnimType)obj->animState->state;
      curAnimRange = &goose_anim_ranges[curAnim];
      AnimationInterpolation_calc(&animInterp, obj->animState, curAnimRange);

      for (modelMeshIdx = 0; modelMeshIdx < MAX_GOOSE_MESH_TYPE;
           ++modelMeshIdx) {
        AnimationFrame_lerp(
            &animInterp,          // result of AnimationInterpolation_calc()
            goose_anim_data,      // pointer to start of AnimationFrame list
            MAX_GOOSE_MESH_TYPE,  // num bones in rig used by animData
            modelMeshIdx,  // index of bone in rig to produce transform for
            &animFrame     // the resultant interpolated animation frame
        );

        // push matrix with the blender to n64 coord rotation, then mulitply
        // it by the model's rotation and offset

        // rotate from z-up (blender) to y-up (opengl) coords
        // TODO: move as many of these transformations as possible to
        // be precomputed in animation data
        guRotate(&dynamicp->zUpToYUpCoordinatesRotation, -90.0f, 1, 0, 0);
        gSPMatrix(glistp++,
                  OS_K0_TO_PHYSICAL(&(dynamicp->zUpToYUpCoordinatesRotation)),
                  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

        guPosition(&dynamicp->animMeshTransform[modelMeshIdx],
                   animFrame.rotation.x,  // roll
                   animFrame.rotation.y,  // pitch
                   animFrame.rotation.z,  // yaw
                   1.0F,                  // scale
                   animFrame.position.x,  // pos x
                   animFrame.position.y,  // pos y
                   animFrame.position.z   // pos z
        );
        gSPMatrix(
            glistp++,
            OS_K0_TO_PHYSICAL(&(dynamicp->animMeshTransform[modelMeshIdx])),
            G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

        gSPDisplayList(glistp++, gooseMeshList[animFrame.object]);

        gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
      }
    } else {
      // case for simple gameobjects with no moving sub-parts
      modelDisplayList = getModelDisplayList(obj);

      gSPDisplayList(glistp++, modelDisplayList);
    }

    gDPPipeSync(glistp++);
  }
}
