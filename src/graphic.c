/*
   graphic.c

   The general graphic routine (the initialization and clear the frame
   buffer) and the definition of the external variable

*/

#include <nusys.h>
// keep nusys at the top

#include <malloc.h>

#include "constants.h"
#include "ed64io_usb.h"
#include "game.h"
#include "gameobject.h"
#include "graphic.h"
#include "models.h"
#include "renderer.h"
#include "trace.h"

// anim data
#include "character_anim.h"
#include "goose_anim.h"

#define DRAW_SPRITES 1
#define DEBUG_VI_MODE 1

Gfx gfx_glist[2][GFX_GLIST_LEN];
Dynamic gfx_dynamic[2];
Gfx* glistp;
u32 gfx_gtask_no = 0;
OSViMode viMode;

RenderMode N64Renderer_renderMode = ToonFlatShadingRenderMode;
Frustum N64Renderer_frustum;
int N64Renderer_objectsCulled = 0;
int N64Renderer_twoCycleMode = FALSE;

static Lights1 sun_light = gdSPDefLights1(120,
                                          120,
                                          120, /* weak ambient light */
                                          255,
                                          255,
                                          255, /* white light */
                                          80,
                                          80,
                                          0);

static Lights0 amb_light = gdSPDefLights0(200, 200, 200 /*  ambient light */);

/*----------------------------------------------------------------------------
  gfxRCPIinit

  The initialization of RSP/RDP
----------------------------------------------------------------------------*/
void gfxRCPInit(void) {
  /* Setting the RSP segment register  */
  gSPSegment(glistp++, 0, 0x0); /* For the CPU virtual address  */

  /* Setting RSP  */
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rspstate));

  /* Setting RDP  */
  gSPDisplayList(glistp++, OS_K0_TO_PHYSICAL(setup_rdpstate));
}

/*----------------------------------------------------------------------------
  gfxClearCfb

  Address setting and clearing the frame buffer/Z-buffer

  Using nuGfxZBuffer (the address of the Z-buffer) and  nuGfxCfb_ptr (the
  address of the frame buffer) which are global variables of NuSYSTEM.
----------------------------------------------------------------------------*/
void gfxClearCfb(u16 fillColor) {
  /* Clear the Z-buffer  */
  gDPSetDepthImage(glistp++, OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetCycleType(glistp++, G_CYC_FILL);
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
                   OS_K0_TO_PHYSICAL(nuGfxZBuffer));
  gDPSetFillColor(glistp++,
                  (GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0)));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);

  /* Clear the frame buffer  */
  gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD,
                   osVirtualToPhysical(nuGfxCfb_ptr));
  gDPSetFillColor(glistp++, (fillColor << 16 | fillColor));
  gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, SCREEN_HT - 1);
  gDPPipeSync(glistp++);
}

// The graphic microcode to register
// This should match the order in the spec file
static NUUcode nugfx_ucode[] = {
    {(u64*)gspF3DEX2_fifoTextStart, (u64*)gspF3DEX2_fifoDataStart},
    {(u64*)gspF3DEX2_NoN_fifoTextStart, (u64*)gspF3DEX2_NoN_fifoDataStart},
    {(u64*)gspF3DEX2_Rej_fifoTextStart, (u64*)gspF3DEX2_Rej_fifoDataStart},
    {(u64*)gspF3DLX2_Rej_fifoTextStart, (u64*)gspF3DLX2_Rej_fifoDataStart},
    {(u64*)gspL3DEX2_fifoTextStart, (u64*)gspL3DEX2_fifoDataStart},
    {(u64*)gspS2DEX2_fifoTextStart, (u64*)gspS2DEX2_fifoDataStart},
};

#if FRAME_BUFFERS == 2
static u16* FrameBuf[FRAME_BUFFERS] = {(u16*)CFB0_ADDR, (u16*)CFB1_ADDR};
#else
static u16* FrameBuf[FRAME_BUFFERS] = {(u16*)CFB0_ADDR, (u16*)CFB1_ADDR,
                                       (u16*)CFB2_ADDR};
#endif

// The display list to initialize RDP on video init
static Gfx rdpstateinit_dl[] = {

    /* set all of the attribute registers to zero */
    gsDPSetEnvColor(0, 0, 0, 0),
    gsDPSetPrimColor(0, 0, 0, 0, 0, 0),
    gsDPSetBlendColor(0, 0, 0, 0),
    gsDPSetFogColor(0, 0, 0, 0),
    gsDPSetFillColor(0),
    gsDPSetPrimDepth(0, 0),
    gsDPSetConvert(0, 0, 0, 0, 0, 0),
    gsDPSetKeyR(0, 0, 0),
    gsDPSetKeyGB(0, 0, 0, 0, 0, 0),

    /* set combine mode */
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    // rdpstateinit_dl[10] =
    /* initialize the scissor box */
    gsDPSetScissor(G_SC_NON_INTERLACE,
                   0,
                   0,
                   SCREEN_WD,  // screen size dependent
                   SCREEN_HT),

    /* initialize all the texture tile descriptors to zero */
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0),

    gsDPSetTileSize(0,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(1,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(2,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(3,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(4,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(5,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(6,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPSetTileSize(7,
                    0,
                    0,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC,
                    (1 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};

// customized replacement of nuGfxInit()
// performed on video init
// Initializes the graphic thread and manager, and z buffer and color frame
// buffer
void gfxInit(void) {
  Gfx gfxList[0x100];
  Gfx* gfxList_ptr;
  OSIntMask im;
  u32 xScale;
  xScale = (u32)((SCREEN_WD * XSCALE_MAX) / SCREEN_WD_MAX);

  /* Activate the graphic thread  */
  nuGfxThreadStart();

  viMode = osViModeTable
      [HIGH_RESOLUTION
           ? (options[OptAntialias]
                  ? (options[OptDeflickerInterlace] ? OS_VI_NTSC_HAF1
                                                    : OS_VI_NTSC_HAN1)
                  : (options[OptDeflickerInterlace] ? OS_VI_NTSC_HPF1
                                                    : OS_VI_NTSC_HPN1))
           : (options[OptAntialias] ? OS_VI_NTSC_LAN1 : OS_VI_NTSC_LPN1)];
#if HIGH_RESOLUTION_HALF_Y
  viMode = osViModeTable[OS_VI_NTSC_LAN1];
  /* Change width, xScale, and origin */
  im = osSetIntMask(OS_IM_VI);
  viMode.comRegs.width = SCREEN_WD;
  viMode.comRegs.xScale = xScale;
  viMode.fldRegs[0].origin = SCREEN_WD * 2;
  viMode.fldRegs[1].origin = SCREEN_WD * 2;
  (void)osSetIntMask(im);
#endif

#if DEBUG_VI_MODE
  debugPrintfSync("SCREEN_WD=%d SCREEN_HT=%d", SCREEN_WD, SCREEN_HT);

  debugPrintfSync(
      "type=%u\ncomRegs={\nctrl=%u\nwidth=%u\nburst=%u\nvSync=%u\nhSync=%"
      "u\n"
      "leap=%u\nhStart=%u\nxScale=%u\nvCurrent=%u\n}\nfldRegs[0]={"
      "\norigin=%u\n"
      "yScale=%u\nvStart=%u\nvBurst=%u\nvIntr=%u\n}\nfldRegs[1]={\norigin="
      "%u\n"
      "yScale=%u\nvStart=%u\nvBurst=%u\nvIntr=%u\n}\n",
      viMode.type, viMode.comRegs.ctrl, viMode.comRegs.width,
      viMode.comRegs.burst, viMode.comRegs.vSync, viMode.comRegs.hSync,
      viMode.comRegs.leap, viMode.comRegs.hStart, viMode.comRegs.xScale,
      viMode.comRegs.vCurrent, viMode.fldRegs[0].origin,
      viMode.fldRegs[0].yScale, viMode.fldRegs[0].vStart,
      viMode.fldRegs[0].vBurst, viMode.fldRegs[0].vIntr,
      viMode.fldRegs[1].origin, viMode.fldRegs[1].yScale,
      viMode.fldRegs[1].vStart, viMode.fldRegs[1].vBurst,
      viMode.fldRegs[1].vIntr);
#endif

  /* Set VI */
  osViSetMode(&viMode);
  // when osViSetMode was called these flags were reset to their default
  // values
  osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF |
                         OS_VI_GAMMA_DITHER_OFF | OS_VI_DIVOT_ON);

#if HIGH_RESOLUTION && HIGH_RESOLUTION_HALF_Y
  // osViSetYScale(0.5);
#endif

  /* Since osViBlack becomes FALSE when the VI mode is changed, */
  /* set the screen display to OFF again.                 */
  nuGfxDisplayOff();

  nuGfxSetCfb(FrameBuf, FRAME_BUFFERS); /* Register the frame buffer  */
  nuGfxSetZBuffer((u16*)ZBUFFER_ADDR);

  /* Register the call-back function to switch frame buffers  */
  nuGfxSwapCfbFuncSet(nuGfxSwapCfb);

  /* Set FIFO buffer for FIFO microcode */
  nuGfxSetUcodeFifo(nuRDPOutputBuf, NU_GFX_RDP_OUTPUTBUFF_SIZE);

  /* Register graphics microcode */
  nuGfxSetUcode(nugfx_ucode);

  /* Initialize the graphic task manager  */
  nuGfxTaskMgrInit();

  // rdpstateinit_dl[10] =
  //     /* initialize the scissor box */
  //     gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD,
  //                    SCREEN_HT);

  /* init the RDP, only required on video init  */
  gfxList_ptr = gfxList;
  gSPDisplayList(gfxList_ptr++, OS_K0_TO_PHYSICAL(rdpstateinit_dl));
  gDPFullSync(gfxList_ptr++);
  gSPEndDisplayList(gfxList_ptr++);
  nuGfxTaskStart(gfxList, (s32)(gfxList_ptr - gfxList) * sizeof(Gfx),
                 NU_GFX_UCODE_F3DEX, 0);

  /* Wait for the task ends.  */
  nuGfxTaskAllEndWait();
}

static LightingType getLightingType(GameObject* obj) {
  switch (obj->modelType) {
    case UniFloorModel:
      return OnlyAmbientLighting;
    default:
      return SunLighting;
  }
}

static int getAnimationNumModelMeshParts(ModelType modelType) {
  switch (modelType) {
    case GooseModel:
      return MAX_GOOSE_MESH_TYPE;
    default:
      return MAX_CHARACTER_MESH_TYPE;
  }
}

static int shouldLerpAnimation(ModelType modelType) {
  switch (modelType) {
    case GooseModel:
      return TRUE;
    default:
      return FALSE;
  }
}

static AnimationRange* getCurrentAnimationRange(GameObject* obj) {
  if (obj->modelType == GooseModel) {
    return &goose_anim_ranges[(GooseAnimType)obj->animState->state];
  } else {
    return &character_anim_ranges[(CharacterAnimType)obj->animState->state];
  }
}

static AnimationFrame* getAnimData(ModelType modelType) {
  switch (modelType) {
    case GooseModel:
      return goose_anim_data;
    default:
      return character_anim_data;
  }
}

static void N64Renderer_drawSprite(unsigned short* sprData,
                                   int sprWidth,
                                   int sprHeight,
                                   int atX,
                                   int atY,
                                   int width,
                                   int height,
                                   int centered) {
  int x = atX + (centered ? -(width / 2) : 0);
  int y = atY + (centered ? -(height / 2) : 0);

  gDPLoadTextureBlock(glistp++, sprData, G_IM_FMT_RGBA, G_IM_SIZ_16b, sprWidth,
                      sprHeight, 0, G_TX_WRAP | G_TX_NOMIRROR,
                      G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_NOLOD, G_TX_NOLOD);
  gDPSetTexturePersp(glistp++, G_TP_NONE);
  gDPSetAlphaCompare(glistp++, G_AC_THRESHOLD);
  gDPSetCombineMode(glistp++, G_CC_MODULATERGBA_PRIM, G_CC_MODULATERGBA_PRIM);
  gDPSetRenderMode(glistp++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
  // TODO: clip rectangle and offset+scale tex coords so we can render partly
  // offscreen textures
  gSPScisTextureRectangle(
      glistp++,
      (int)(x) << 2,           // upper left x,
      (int)(y) << 2,           // upper left y
      (int)(x + width) << 2,   // lower right x,
      (int)(y + height) << 2,  // lower right y,
      G_TX_RENDERTILE,         // tile
      // upper left s, t
      // when using gSPTextureRectangleFlip, these args are flipped
      0 << 5, 0 << 5,
      // change in s,t for each change in screen space x,y.
      // smaller values = larger scaling
      // when using gSPTextureRectangleFlip, these args are flipped
      ((float)sprWidth / (float)width) * (1 << 10),
      ((float)sprHeight / (float)height) * (1 << 10));

  gDPPipeSync(glistp++);
}

void mulMtxFVecF(MtxF matrix, float* in /*[4]*/, float* out /*[4]*/) {
  int i;

  for (i = 0; i < 4; i++) {
    out[i] = in[0] * matrix[0][i] +  //  0, 1, 2, 3
             in[1] * matrix[1][i] +  //  4, 5, 6, 7,
             in[2] * matrix[2][i] +  //  8, 9,10, 9,
             in[3] * matrix[3][i];   // 12,13,14,15,
  }
}

// like gluProject()
int screenProject(Vec3d* obj,
                  MtxF modelMatrix,
                  MtxF projMatrix,
                  ViewportF viewport,
                  Vec3d* win) {
  float in[4];
  float out[4];

  in[0] = obj->x;
  in[1] = obj->y;
  in[2] = obj->z;
  in[3] = 1.0;
  mulMtxFVecF(modelMatrix, in, out);
  mulMtxFVecF(projMatrix, out, in);

  if (in[3] == 0.0)
    return FALSE;
  in[0] /= in[3];
  in[1] /= in[3];
  in[2] /= in[3];
  /* Map x, y and z to range 0-1 */
  in[0] = in[0] * 0.5 + 0.5;
  in[1] = in[1] * 0.5 + 0.5;
  in[2] = in[2] * 0.5 + 0.5;

  /* Map x,y to viewport */
  in[0] = in[0] * viewport[2] + viewport[0];
  in[1] = in[1] * viewport[3] + viewport[1];

  win->x = in[0];
  win->y = in[1];
  win->z = in[2];
  return TRUE;
}

static void setupLights() {
  gSPSetLights0(glistp++, amb_light);
}

static void setupLightsPerObject(GameObject* obj) {
  switch (N64Renderer_renderMode) {
    case TextureAndLightingRenderMode:
    case LightingNoTextureRenderMode:
      if (getLightingType(obj) == OnlyAmbientLighting) {
        gSPSetLights0(glistp++, amb_light);

      } else {
        gSPSetLights1(glistp++, sun_light);
      }
      break;
    default:
      break;
  }
}

#define GEOM_MODE_COMMON G_CULL_BACK | G_CLIPPING

// returns geometry mode flags
static u32 setupShading() {
  switch (N64Renderer_renderMode) {
    case ToonFlatShadingRenderMode:
      // if (Renderer_isLitGameObject(obj)) {
      //   gSPSetGeometryMode(glistp++, G_SHADE | G_LIGHTING |
      //   GEOM_MODE_COMMON);
      // } else {
      //   gSPSetGeometryMode(glistp++, G_SHADE | GEOM_MODE_COMMON);
      // }

      gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB);
      return GEOM_MODE_COMMON;
      break;
    case TextureNoLightingRenderMode:
    case WireframeRenderMode:
      gDPSetCombineMode(glistp++, G_CC_DECALRGB, G_CC_DECALRGB);
      return G_SHADE | G_SHADING_SMOOTH | GEOM_MODE_COMMON;
    case TextureAndLightingRenderMode:
      gDPSetCombineMode(glistp++, G_CC_MODULATERGB, G_CC_MODULATERGB);
      return G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | GEOM_MODE_COMMON;
    case LightingNoTextureRenderMode:
      gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
      return G_SHADE | G_SHADING_SMOOTH | G_LIGHTING | GEOM_MODE_COMMON;
    case NormalColorRenderMode:
      gDPSetCombineMode(glistp++, G_CC_SHADE, G_CC_SHADE);
      return G_SHADE | GEOM_MODE_COMMON;
    default:  // NoTextureNoLightingRenderMode
      gDPSetPrimColor(glistp++, 0, 0, /*r*/ 180, /*g*/ 180, /*b*/ 180,
                      /*a*/ 255);
      gDPSetCombineMode(glistp++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
      return GEOM_MODE_COMMON;
  }
}

// returns geometry mode flags
u32 setupRenderModePerObject(GameObject* obj, int isIntersectingObj) {
  int needsZBuffer =
      !Renderer_isBackgroundGameObject(obj) ||
      // ^^^ TEMP: TESTING PERF OF ZBUFFERING ALL FOREGROUNG OBJECTS
      !RENDERER_PAINTERS_ALGORITHM || Renderer_isZBufferedGameObject(obj);
  int needsZWrite = TRUE;
  if (needsZBuffer) {
    // depth test
    if (options[OptAntialias]) {
      gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    } else {
      gDPSetRenderMode(glistp++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
    }
    return G_ZBUFFER;
  } else if (needsZWrite) {
    // depth write
    if (options[OptAntialias]) {
      gDPSetRenderMode(glistp++, G_RM_AA_ZUPD_OPA_SURF, G_RM_AA_ZUPD_OPA_SURF2);
    } else {
      gDPSetRenderMode(glistp++, G_RM_ZUPD_OPA_SURF, G_RM_ZUPD_OPA_SURF2);
    }
    return G_ZBUFFER;
  } else {
    // TODO: probably remove this case which doesn't write depth at all
    // this is only useful if we have objects that are always rendered first
    // and never intersected
    if (options[OptAntialias]) {
      gDPSetRenderMode(glistp++, G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
    } else {
      gDPSetRenderMode(glistp++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    }
    return 0;
  }
}

void drawAnimatedGameObject(GameObject* obj, Dynamic* dynamicp) {
  AnimationFrame animFrame;
  AnimationInterpolation animInterp;
  AnimationRange* curAnimRange;
  AnimationBoneAttachment* attachment;
  float profStartAnim;
  int modelMeshIdx;
  int modelMeshParts;
  Gfx* modelDisplayList;

  profStartAnim = CUR_TIME_MS();

  modelMeshParts = getAnimationNumModelMeshParts(obj->modelType);
  curAnimRange = getCurrentAnimationRange(obj);
  AnimationInterpolation_calc(&animInterp, obj->animState, curAnimRange);

  for (modelMeshIdx = 0; modelMeshIdx < modelMeshParts; ++modelMeshIdx) {
    // profStartAnimLerp = CUR_TIME_MS();
    // lerping takes about 0.2ms per bone
    if (shouldLerpAnimation(obj->modelType)) {
      AnimationFrame_lerp(
          &animInterp,  // result of AnimationInterpolation_calc()
          getAnimData(
              obj->modelType),  // pointer to start of AnimationFrame list
          modelMeshParts,       // num bones in rig used by animData
          modelMeshIdx,         // index of bone in rig to produce transform for
          &animFrame            // the resultant interpolated animation frame
      );
    } else {
      AnimationFrame_get(
          &animInterp,  // result of AnimationInterpolation_calc()
          getAnimData(
              obj->modelType),  // pointer to start of AnimationFrame list
          modelMeshParts,       // num bones in rig used by animData
          modelMeshIdx,         // index of bone in rig to produce transform for
          &animFrame            // the resultant interpolated animation frame
      );
    }
    // Trace_addEvent(AnimLerpTraceEvent, profStartAnimLerp, CUR_TIME_MS());

    // push matrix with the blender to n64 coord rotation, then mulitply
    // it by the model's rotation and offset

    // rotate from z-up (blender) to y-up (opengl) coords
    // TODO: move as many of these transformations as possible to
    // be precomputed in animation data
    guRotate(&dynamicp->zUpToYUpCoordinatesRotation, -90.0f, 1, 0, 0);
    gSPMatrix(glistp++,
              OS_K0_TO_PHYSICAL(&(dynamicp->zUpToYUpCoordinatesRotation)),
              G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);

    guPosition(&obj->animState->animMeshTransform[modelMeshIdx],
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
        OS_K0_TO_PHYSICAL(&(obj->animState->animMeshTransform[modelMeshIdx])),
        G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);

    gSPDisplayList(glistp++, getMeshDisplayListForModelMeshPart(
                                 obj->modelType, animFrame.object));

    attachment = &obj->animState->attachment;
    if (attachment->modelType != NoneModel &&
        attachment->boneIndex == modelMeshIdx) {
      guPosition(&obj->animState->attachmentTransform,
                 attachment->rotation.x,  // roll
                 attachment->rotation.y,  // pitch
                 attachment->rotation.z,  // yaw
                 1.0F,                    // scale
                 attachment->offset.x,    // pos x
                 attachment->offset.y,    // pos y
                 attachment->offset.z     // pos z
      );
      gSPMatrix(glistp++,
                OS_K0_TO_PHYSICAL(&(obj->animState->attachmentTransform)),
                G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);
      modelDisplayList = getModelDisplayList(attachment->modelType, 0);
      gSPDisplayList(glistp++, modelDisplayList);
      gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
    }

    gSPPopMatrix(glistp++, G_MTX_MODELVIEW);
  }
  Trace_addEvent(DrawAnimTraceEvent, profStartAnim, CUR_TIME_MS());
}

void N64Renderer_drawWorldObjects(Dynamic* dynamicp) {
  Game* game;
  GameObject* obj;
  int i;
  Gfx* modelDisplayList;
  RendererSortDistance* visibleObjDistance;
  int* worldObjectsVisibility;
  int* intersectingObjects;
  int visibleObjectsCount;
  int objectsVisitedByRenderPass = 0;
  int visibilityCulled = 0;
  float profStartSort, profStartIter;
  // float profStartAnimLerp;
  float profStartFrustum;
  int renderPass;
  u32 baseGeometryMode;
#if RENDERER_PAINTERS_ALGORITHM
  renderPass = BackgroundRenderPass;
#else
  renderPass = ZBufferedRenderPass;
#endif

  game = Game_get();
  worldObjectsVisibility = (int*)malloc(game->worldObjectsCount * sizeof(int));
  invariant(worldObjectsVisibility);

  profStartFrustum = CUR_TIME_MS();
  visibilityCulled = Renderer_cullVisibility(
      game->worldObjects, game->worldObjectsCount, worldObjectsVisibility,
      &N64Renderer_frustum, game->worldObjectsBounds);
  N64Renderer_objectsCulled = visibilityCulled;

  Trace_addEvent(DrawFrustumCullTraceEvent, profStartFrustum, CUR_TIME_MS());

  // only alloc space for num visible objects
  visibleObjectsCount = game->worldObjectsCount - visibilityCulled;
  visibleObjDistance = (RendererSortDistance*)malloc(
      (visibleObjectsCount) * sizeof(RendererSortDistance));
  invariant(visibleObjDistance);

  profStartSort = CUR_TIME_MS();
  Renderer_sortVisibleObjects(game->worldObjects, game->worldObjectsCount,
                              worldObjectsVisibility, visibleObjectsCount,
                              visibleObjDistance, &game->viewPos,
                              game->worldObjectsBounds);
  Trace_addEvent(DrawSortTraceEvent, profStartSort, CUR_TIME_MS());

  // boolean of whether an object intersects another (for z buffer
  // optimization)
  intersectingObjects = (int*)malloc((visibleObjectsCount) * sizeof(int));
  invariant(intersectingObjects);
  Renderer_calcIntersecting(intersectingObjects, visibleObjectsCount,
                            visibleObjDistance, game->worldObjectsBounds);

  gSPClearGeometryMode(glistp++, 0xFFFFFFFF);
  gDPSetCycleType(glistp++,
                  N64Renderer_twoCycleMode ? G_CYC_2CYCLE : G_CYC_1CYCLE);

  // z-buffered, antialiased triangles by default
  // gDPSetRenderMode(glistp++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
  // gSPSetGeometryMode(glistp++, G_ZBUFFER);

  setupLights();
  baseGeometryMode = setupShading();

  // setup view
  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
            G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
  gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->camera)),
            G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

  profStartIter = CUR_TIME_MS();
  // render world objects
  for (; renderPass < MAX_RENDER_PASS; renderPass++) {
    for (i = 0; i < visibleObjectsCount; i++) {
      u32 objGeometryMode = baseGeometryMode;
      // iterate visible objects far to near
      obj = (visibleObjDistance + i)->obj;

// in painters alg, skip stuff that doesn't apply to this pass
#if RENDERER_PAINTERS_ALGORITHM
      switch (renderPass) {
        case BackgroundRenderPass:
          if (!Renderer_isBackgroundGameObject(obj))
            continue;
          break;
        case ForegroundRenderPass:
          if (Renderer_isBackgroundGameObject(obj) ||
              Renderer_isZBufferedGameObject(obj))
            continue;
          break;
        case ZBufferedRenderPass:
          if (!Renderer_isZBufferedGameObject(obj))
            continue;
          break;
      }
#endif
      objectsVisitedByRenderPass++;

      // render textured models
      gSPTexture(glistp++, 0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON);
      gDPSetTextureFilter(glistp++, G_TF_BILERP);
      gDPSetTexturePersp(glistp++, G_TP_PERSP);

      setupLightsPerObject(obj);

      gSPClearGeometryMode(glistp++, 0xFFFFFFFF);
      invariant(i < visibleObjectsCount);
      invariant(obj != NULL);

      objGeometryMode |= setupRenderModePerObject(obj, intersectingObjects[i]);
      gSPSetGeometryMode(glistp++, objGeometryMode);

      // set the transform in world space for the gameobject to render
      guPosition(&dynamicp->objTransforms[i],
                 0.0F,                                        // rot x
                 obj->rotation.y,                             // rot y
                 0.0F,                                        // rot z
                 modelTypesProperties[obj->modelType].scale,  // scale
                 obj->position.x,                             // pos x
                 obj->position.y,                             // pos y
                 obj->position.z                              // pos z
      );
      gSPMatrix(
          glistp++, OS_K0_TO_PHYSICAL(&(dynamicp->objTransforms[i])),
          G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_PUSH);  // gameobject mtx start

      if (Renderer_isAnimatedGameObject(obj)) {
        // case for multi-part objects using rigid body animation
        drawAnimatedGameObject(obj, dynamicp);
      } else {
        // case for simple gameobjects with no moving sub-parts
        modelDisplayList = getModelDisplayList(obj->modelType, obj->subtype);

        gSPDisplayList(glistp++, modelDisplayList);
      }

      gSPPopMatrix(glistp++, G_MTX_MODELVIEW);  // gameobject mtx end

      gDPPipeSync(glistp++);
    }  // end world objects iteration

  }  // end render passes

  // ensure each obj only visited by one render pass
  invariant(objectsVisitedByRenderPass == visibleObjectsCount);

  // extra sprite pass

  if (DRAW_SPRITES) {
    ViewportF viewport = {0, 0, SCREEN_WD, SCREEN_HT};
    float width = 64;
    float height = 64;
    Vec3d center;
    Vec3d projected;
    MtxF modelViewMtxF;
    MtxF projectionMtxF;

    guMtxL2F(modelViewMtxF, &dynamicp->camera);
    guMtxL2F(projectionMtxF, &dynamicp->projection);
    for (i = 0; i < visibleObjectsCount; i++) {
      obj = visibleObjDistance[i].obj;
      Game_getObjCenter(obj, &center);
      center.y += 75;
      if (!obj->animState) {
        continue;
      }
      screenProject(&center, modelViewMtxF, projectionMtxF, viewport,
                    &projected);

      // render sprites
      N64Renderer_drawSprite(
          getSpriteForSpriteType(HonkSprite,
                                 Sprite_frameCycle(HONK_SPRITE_FRAMES,
                                                   /*sprite frame
                                                   duration*/
                                                   10, game->tick)),

          32, 32,  // texture dimensions
          projected.x,
          SCREEN_HT - projected.y,  // draw at pos, invert y
          RES_SCALE_X(width), RES_SCALE_Y(height), TRUE

      );
    }  // end sprit visible objects iter
  }

  free(intersectingObjects);
  free(visibleObjDistance);
  free(worldObjectsVisibility);

  Trace_addEvent(DrawIterTraceEvent, profStartIter, CUR_TIME_MS());
}
