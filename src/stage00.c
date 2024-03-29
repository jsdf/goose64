/*
   stage00.c

  the main game file
*/

#include <assert.h>
#include <nusys.h>
// #include <stdlib.h>

#ifdef SOUND
#include <nual_n.h>
#endif

#include <malloc.h>
#include <math.h>

// game
#include "animation.h"
#include "constants.h"
#include "frustum.h"
#include "game.h"
#include "gameobject.h"
#include "graphic.h"
#include "input.h"
#include "main.h"
#include "modeltype.h"
#include "option.h"
#include "pathfinding.h"
#include "physics.h"
#include "renderer.h"
#include "sound.h"
#include "sprite.h"
#include "trace.h"
#include "vec2d.h"
#include "vec3d.h"

#include "models.h"
#include "segments.h"

// map
#include "garden_map.h"
#include "garden_map_collision.h"
#include "garden_map_graph.h"

#ifdef ED64
#include "ed64io.h"
#endif

#define LOG_TRACES 0
#define LOG_PROFILING 1

#define CONTROLLER_DEAD_ZONE 0.1

typedef enum ConsoleMode
{
  CONSOLE_HIDDEN,
  CONSOLE_SHOW_CAMERA,
  CONSOLE_SHOW_CPU_PROFILING,
  CONSOLE_SHOW_CULLING,
  CONSOLE_SHOW_PROFILING,
  CONSOLE_SHOW_RCP_TASKS,
  CONSOLE_SHOW_SOUND,
  CONSOLE_SHOW_TRACING,
  CONSOLE_SHOW_ED64LOG_INTERNALS,
  MAX_CONSOLE_MODE
} ConsoleMode;

#define consoleShown() consoleMode == CONSOLE_HIDDEN
static int nuPerfBarShown = FALSE;
static ConsoleMode consoleMode = CONSOLE_SHOW_PROFILING;

static Vec3d viewPos;
static Vec3d viewRot;
static Input input;

static u16 perspNorm;
static u32 nearPlane; /* Near Plane */
static u32 farPlane;  /* Far Plane */

#if HIGH_RESOLUTION && HIGH_RESOLUTION_HALF_Y
static float aspect = (f32)SCREEN_WD / (f32)(SCREEN_HT * 2);
#else
static float aspect = (f32)SCREEN_WD / (f32)SCREEN_HT;
#endif
static float fovy = DEFAULT_FOVY;
static Vec3d upVector = {0.0f, 1.0f, 0.0f};

/* frame counter */
float frameCounterLastTime;
int frameCounterCurFrames;
int frameCounterLastFrames;

/* profiling */
int totalUpdates;
float profAvgCharacters;
float profAvgPhysics;
float profAvgDraw;
float profAvgPath;
float lastFrameTime;
float profilingAverages[MAX_TRACE_EVENT_TYPE];

static int usbEnabled;
static int usbResult;
#ifdef ED64
static UsbLoggerState usbLoggerState;
#endif

static int logTraceStartOffset = 0;
static int loggingTrace = FALSE;

PhysWorldData physWorldData;

void soundCheck(void);
void Rom2Ram(void *, void *, s32);

#define OBJ_START_VAL 1000

static float sndPitch = 10.5; // i don't fucking know :((
static int sndNumber = 0;
static int honkSoundRange = Honk5Sound - Honk1Sound;
static int seqPlaying = FALSE;

#define NUM_DEBUG_RENDER_MODES 3
static int debugRenderMode = 0;
static RenderMode debugRenderModes[] = {
    ToonFlatShadingRenderMode, WireframeRenderMode, NormalColorRenderMode};

/* The initialization of stage 0 */
void initStage00()
{
  Game *game;
  int i;

  // load in the models segment into higher memory
  Rom2Ram(_modelsSegmentRomStart, _modelsSegmentStart,
          _modelsSegmentRomEnd - _modelsSegmentRomStart);
  // load in the sprites segment into higher memory
  Rom2Ram(_spritesSegmentRomStart, _spritesSegmentStart,
          _spritesSegmentRomEnd - _spritesSegmentRomStart);
  // load in the collision segment into higher memory
  Rom2Ram(_collisionSegmentRomStart, _collisionSegmentStart,
          _collisionSegmentRomEnd - _collisionSegmentRomStart);
#ifdef SOUND
  /* Read and register the sample bank. */
  nuAuPtrBankInit(PBANK_END - PBANK_START);
  nuAuPtrBankSet((u8 *)PBANK_START, PBANK_END - PBANK_START,
                 (u8 *)WBANK_START);

  /* Read and register the sound effects. */
  nuAuSndPlayerDataSet((u8 *)SFX_START, SFX_END - SFX_START);

  nuAuSeqPlayerDataSet(0, (u8 *)SONG_START, SONG_END - SONG_START);

  debugPrintfSync("audio heap used=%d, free=%d\n", nuAuHeapGetUsed(),
                  nuAuHeapGetFree());
#endif

  physWorldData = (PhysWorldData){garden_map_collision_collision_mesh,
                                  GARDEN_MAP_COLLISION_LENGTH,
                                  &garden_map_collision_collision_mesh_hash,
                                  /*gravity*/ -9.8 * N64_SCALE_FACTOR,
                                  /*viscosity*/ 0.05,
                                  /*waterHeight*/ WATER_HEIGHT};

  usbEnabled = TRUE;
  usbResult = 0;

  frameCounterLastTime = 0;
  frameCounterCurFrames = 0;
  totalUpdates = 0;
  profAvgCharacters = 0;
  profAvgPhysics = 0;
  profAvgDraw = 0;
  profAvgPath = 0;

  loggingTrace = FALSE;

  nearPlane = DEFAULT_NEARPLANE;
  farPlane = DEFAULT_FARPLANE;
  Vec3d_init(&viewPos, 0.0F, 0.0F, -400.0F);
  Vec3d_init(&viewRot, 0.0F, 0.0F, 0.0F);
  Input_init(&input);

  Game_init(garden_map_data, GARDEN_MAP_COUNT, &physWorldData);
  Option_initAll();

  invariant(GARDEN_MAP_COUNT <= MAX_WORLD_OBJECTS);

  game = Game_get();

  game->pathfindingGraph = &garden_map_graph;
  game->pathfindingState = &garden_map_graph_pathfinding_state;
  game->worldObjectsBounds = garden_map_bounds;

  lastFrameTime = CUR_TIME_MS();

  for (i = 0; i < MAX_TRACE_EVENT_TYPE; ++i)
  {
    profilingAverages[i] = 0;
    profilingAccumulated[i] = 0;
    profilingCounts[i] = 0;
  }

#ifdef ED64
  // start a thread to watch the totalUpdates value
  // if it doesn't change each second, we've crashed
  ed64StartWatchdogThread(&totalUpdates, 1000);

  debugPrintfSync("getModelDisplayList at %p\n", getModelDisplayList);
#endif

  debugPrintf("good morning\n");
}

void debugPrintVec3d(int x, int y, char *label, Vec3d *vec)
{
  char conbuf[100];
  nuDebConTextPos(0, x, y);
  sprintf(conbuf, "%s {%5.0f,%5.0f,%5.0f}", label, vec->x, vec->y, vec->z);
  nuDebConCPuts(0, conbuf);
}

void debugPrintFloat(int x, int y, char *format, float value)
{
  char conbuf[100];
  nuDebConTextPos(0, x, y);
  sprintf(conbuf, format, value);
  nuDebConCPuts(0, conbuf);
}

#ifdef NU_DEBUG
void traceRCP()
{
  int i;
  float longestRDPTaskTime = 0;
  float longestRSPTaskTime = 0;
  // s64 retraceTime;

  // retraceTime = nuDebTaskPerfPtr->retraceTime;
  // debugPrintf("rt=%f ", retraceTime / (1000000.0));
  for (i = 0; i < nuDebTaskPerfPtr->gfxTaskCnt; i++)
  {
    // debugPrintf(
    //     "[t%d: st=%f rsp=%f,rdp=%f] ", i,
    //     (nuDebTaskPerfPtr->gfxTaskTime[i].rspStart ) / 1000.0,
    //     (nuDebTaskPerfPtr->gfxTaskTime[i].rspEnd -
    //      nuDebTaskPerfPtr->gfxTaskTime[i].rspStart) /
    //         1000.0,
    //     (nuDebTaskPerfPtr->gfxTaskTime[i].rdpEnd -
    //      nuDebTaskPerfPtr->gfxTaskTime[i].rspStart) /
    //         1000.0);

    // pulled out to a variable so the modern compiler doesn't
    // generate broken code
    NUDebTaskTime gfxTaskTime = nuDebTaskPerfPtr->gfxTaskTime[i];

    Trace_addEvent(RSPTaskTraceEvent, gfxTaskTime.rspStart / 1000.0f,
                   gfxTaskTime.rspEnd / 1000.0f);

    Trace_addEvent(RDPTaskTraceEvent, gfxTaskTime.rspStart / 1000.0f,
                   gfxTaskTime.rdpEnd / 1000.0f);

    // longestRDPTaskTime = MAX(longestRDPTaskTime, ((gfxTaskTime.rspEnd) /
    // 1000.0f -
    //                                         (gfxTaskTime.rdpStart /
    //                                         1000.0f)));
    longestRDPTaskTime =
        MAX(longestRDPTaskTime,
            ((gfxTaskTime.rdpEnd) / 1000.0f - (gfxTaskTime.rspEnd / 1000.0f)));
    longestRSPTaskTime = MAX(
        longestRSPTaskTime,
        ((gfxTaskTime.rspEnd) / 1000.0f - (gfxTaskTime.rspStart / 1000.0f)));
  }
  // debugPrintf("\n");
  profilingAccumulated[RDPTaskTraceEvent] += longestRDPTaskTime;
  profilingCounts[RDPTaskTraceEvent]++;
  profilingAccumulated[RSPTaskTraceEvent] += longestRSPTaskTime;
  profilingCounts[RSPTaskTraceEvent]++;
}
#endif

/* Make the display list and activate the task */
void makeDL00()
{
  Game *game;
  Dynamic *dynamicp;
  int consoleOffset;
  float curTime;
  int i;
  float profStartDraw, profEndDraw, profStartDebugDraw;
#if DEBUG
  char conbuf[100];
#endif

  game = Game_get();
  consoleOffset = 20;

  curTime = CUR_TIME_MS();
  frameCounterCurFrames++;

  profStartDraw = curTime;

  Trace_addEvent(FrameTraceEvent, curTime, curTime + 16);

  if (curTime - frameCounterLastTime >= 1000.0)
  {
    frameCounterLastFrames = frameCounterCurFrames;
    frameCounterCurFrames = 0;
    frameCounterLastTime += 1000.0;
  }

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* Initialize RCP */
  gfxRCPInit();

/* Clear the frame and Z-buffer */
#if RENDERER_FAKE_GROUND
  gfxClearCfb(GPACK_RGBA5551(111, 160, 120, 1));
#else
  gfxClearCfb(GPACK_RGBA5551(0, 0, 0, 1));
#endif

  Frustum_setCamInternals(&N64Renderer_frustum, fovy, aspect, nearPlane,
                          farPlane);

  /* projection, viewing, modeling matrix set */
  guPerspective(&dynamicp->projection, &perspNorm, fovy, aspect, nearPlane,
                farPlane, 1.0);
  gSPPerspNormalize(glistp++, perspNorm);

  // gSPClipRatio(glistp++, FRUSTRATIO_4);

  Frustum_setCamDef(&N64Renderer_frustum, &game->viewPos, &game->viewTarget,
                    &upVector);

  if (game->freeView)
  {
    guPosition(&dynamicp->camera,
               viewRot.x, // roll
               viewRot.y, // pitch
               viewRot.z, // yaw
               1.0F,      // scale
               viewPos.x, viewPos.y, viewPos.z);
  }
  else
  {
    guLookAt(&dynamicp->camera, game->viewPos.x, game->viewPos.y,
             game->viewPos.z, game->viewTarget.x, game->viewTarget.y,
             game->viewTarget.z, upVector.x, upVector.y, upVector.z);
  }

  N64Renderer_drawWorldObjects(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  invariant((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and (maybe)
     switch display buffers */
  nuGfxTaskStart(
      &gfx_glist[gfx_gtask_no][0],
      (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof(Gfx),
      N64Renderer_renderMode == WireframeRenderMode ? NU_GFX_UCODE_L3DEX2
                                                    : NU_GFX_UCODE_F3DEX,
      // swap buffer if we don't also have another task to show debug stuff
      consoleShown() || nuPerfBarShown ? NU_SC_NOSWAPBUFFER : NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;

  profEndDraw = CUR_TIME_MS();
  Trace_addEvent(DrawTraceEvent, profStartDraw, profEndDraw);
  game->profTimeDraw += profEndDraw - profStartDraw;

  profStartDebugDraw = CUR_TIME_MS();

  if (nuPerfBarShown)
    nuDebTaskPerfBar1(1, 200, NU_SC_SWAPBUFFER);

// debug text overlay
#if DEBUG
  if (consoleShown())
  {
    invariant(!(consoleShown() && nuPerfBarShown));
    nuDebConClear(0);
    consoleOffset = 21;

    if (!(contPattern & 0x1))
    {
      nuDebConTextPos(0, 9, 24);
      nuDebConCPuts(0, "Controller1 not connected");
    }
    else if (game->freeView)
    {
      debugPrintVec3d(4, consoleOffset++, "viewPos", &viewPos);
    }
    else
    {
      switch (consoleMode)
      {
      case CONSOLE_SHOW_CPU_PROFILING:
        debugPrintFloat(4, consoleOffset++, "char=%3.2fms",
                        profAvgCharacters);
        debugPrintFloat(4, consoleOffset++, "phys=%3.2fms", profAvgPhysics);
        debugPrintFloat(4, consoleOffset++, "draw=%3.2fms", profAvgDraw);
        debugPrintFloat(0, consoleOffset++, "path=%3.2fms", profAvgPath);
        break;
      case CONSOLE_SHOW_PROFILING:
        nuDebConTextPos(0, 4, consoleOffset++);
        sprintf(conbuf, "cpu=%3.2f rsp=%3.2f rdp=%3.2f",
                profilingAverages[MainCPUTraceEvent],
                profilingAverages[RSPTaskTraceEvent],
                profilingAverages[RDPTaskTraceEvent]);
        nuDebConCPuts(0, conbuf);
        nuDebConTextPos(0, 4, consoleOffset++);
        sprintf(conbuf, "OptMeshSubdiv=%d", options[OptMeshSubdiv]);
        nuDebConCPuts(0, conbuf);
        break;
      case CONSOLE_SHOW_TRACING:
        nuDebConTextPos(0, 4, consoleOffset++);
        sprintf(conbuf, "trace rec=%d,log=%d,evs=%d,lgd=%d",
                Trace_isTracing(), loggingTrace, Trace_getEventsCount(),
                logTraceStartOffset);
        nuDebConCPuts(0, conbuf);
        break;
      case CONSOLE_SHOW_RCP_TASKS:
#ifdef NU_DEBUG
      {
        int tskIdx;
        for (tskIdx = 0; tskIdx < nuDebTaskPerfPtr->gfxTaskCnt; tskIdx++)
        {
          nuDebConTextPos(0, 4, consoleOffset++);
          sprintf(conbuf, "[t%d:  rsp=%.2f,rdp=%.2f] ", tskIdx,
                  (nuDebTaskPerfPtr->gfxTaskTime[tskIdx].rspEnd -
                   nuDebTaskPerfPtr->gfxTaskTime[tskIdx].rspStart) /
                      1000.0,
                  (nuDebTaskPerfPtr->gfxTaskTime[tskIdx].rdpEnd -
                   nuDebTaskPerfPtr->gfxTaskTime[tskIdx].rspStart) /
                      1000.0);
          nuDebConCPuts(0, conbuf);
        }
      }
#endif
      break;
      case CONSOLE_SHOW_CULLING:
        nuDebConTextPos(0, 4, consoleOffset++);
        sprintf(conbuf, "culled=%d", N64Renderer_objectsCulled);
        nuDebConCPuts(0, conbuf);
        break;
      case CONSOLE_SHOW_CAMERA:
        consoleOffset = 16; // needs extra space
        debugPrintVec3d(4, consoleOffset++, "viewPos", &game->viewPos);
        debugPrintVec3d(4, consoleOffset++, "viewTarget", &game->viewTarget);
        debugPrintFloat(4, consoleOffset++, "viewZoom=%1.1f", game->viewZoom);
        debugPrintFloat(4, consoleOffset++, "fovy=%3.1f", fovy);
        debugPrintFloat(4, consoleOffset++, "nearPlane=%.2f", nearPlane);
        debugPrintFloat(4, consoleOffset++, "farPlane=%.2f", farPlane);
        break;

      case CONSOLE_SHOW_ED64LOG_INTERNALS:
#ifdef ED64
        usbLoggerGetState(&usbLoggerState);
        nuDebConTextPos(0, 4, consoleOffset++);
        sprintf(conbuf, "usb=%d,res=%d,st=%d,id=%d,mqsz=%d", usbEnabled,
                usbResult, usbLoggerState.fifoWriteState,
                usbLoggerState.msgID, usbLoggerState.msgQSize);
        nuDebConCPuts(0, conbuf);
        nuDebConTextPos(0, 4, consoleOffset++);
        sprintf(conbuf, "off=%4d,flu=%d,ovf=%d,don=%d,err=%d",
                usbLoggerState.usbLoggerOffset,
                usbLoggerState.usbLoggerFlushing,
                usbLoggerState.usbLoggerOverflow, usbLoggerState.countDone,
                usbLoggerState.writeError);
        nuDebConCPuts(0, conbuf);
#endif
        break;

      case CONSOLE_SHOW_SOUND:
        debugPrintFloat(4, consoleOffset++, "snd=%.0f", sndNumber);
        debugPrintFloat(4, consoleOffset++, "pitch=%f", sndPitch);
        break;
      } // end switch
    }   // end if (freeView)
    debugPrintFloat(4, consoleOffset++, "frame=%3.2fms",
                    1000.0 / frameCounterLastFrames);

    nuDebConTextPos(0, 4, consoleOffset++);
    sprintf(conbuf, "retrace=%lu", nuScRetraceCounter);
    nuDebConCPuts(0, conbuf);
    debugPrintVec3d(4, consoleOffset++, "pos", &game->player.goose->position);
    /* Display characters on the frame buffer */
    nuDebConDisp(NU_SC_SWAPBUFFER);
  }

#endif // end of #if DEBUG

  Trace_addEvent(DebugDrawTraceEvent, profStartDebugDraw, CUR_TIME_MS());
}

void checkDebugControls(Game *game)
{
  /* Change the display position by stick data */
  viewRot.x = contdata->stick_y; // rot around x
  viewRot.y = contdata->stick_x; // rot around y

  /* Change the moving speed with up/down buttons of controller */
  if (contdata[0].button & U_JPAD)
    viewPos.z += 10.0;
  if (contdata[0].button & D_JPAD)
    viewPos.z -= 10.0;

  if (viewPos.z < (600.0 - farPlane))
  {
    /* It comes back near if it goes too far */
    viewPos.z = 600.0 - nearPlane;
  }
  else if (viewPos.z > (600.0 - nearPlane))
  {
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

void logTraceChunk()
{
  int i;
  int printedFirstItem;
#if ED64

  printedFirstItem = FALSE;
  if (usbLoggerBufferRemaining() < 120)
  {
    return;
  }
  debugPrintf("TRACE=[");
  for (i = logTraceStartOffset; i < Trace_getEventsCount(); i++)
  {
    // check we have room for more data
    if (usbLoggerBufferRemaining() < 40)
    {
      break;
    }
    debugPrintf("%s[%.2f,%.2f,%d]", printedFirstItem ? "," : "",
                traceEvents[i].start, traceEvents[i].end, traceEvents[i].type);
    printedFirstItem = TRUE;
    logTraceStartOffset = i;
  }
  debugPrintf("]\n");

  if (logTraceStartOffset == Trace_getEventsCount() - 1)
  {
    // finished
    loggingTrace = FALSE;
    logTraceStartOffset = 0;
    Trace_clear();
  }
#endif
}

void startRecordingTrace()
{
  Trace_clear();
  Trace_start();
}

void finishRecordingTrace()
{
  Trace_stop();
  loggingTrace = TRUE;
}

/* The game progressing process for stage 0 */
void updateGame00(void)
{
  int i;
  Game *game;

  totalUpdates++;
#ifdef NU_DEBUG
  traceRCP(); // record rcp perf from prev frame
#endif

  game = Game_get();

  Vec2d_origin(&input.direction);

  /* Data reading of controller 1 */
  nuContDataGetEx(contdata, 0);
  if (contdata[0].trigger & START_BUTTON)
  {
    debugRenderMode++;
    if (debugRenderMode >= NUM_DEBUG_RENDER_MODES)
    {
      debugRenderMode = 0;
    }
    N64Renderer_renderMode = debugRenderModes[debugRenderMode];
  }

#if SOUND_TEST
  soundCheck();
#endif
  if (contdata[0].trigger & L_TRIG)
  {
    game->freeView = !game->freeView;
  }
  if (game->freeView)
  {
    checkDebugControls(game);
  }
  else
  {
    // normal controls
    if (contdata[0].button & A_BUTTON)
    {
      input.run = TRUE;
    }
    if (contdata[0].button & B_BUTTON)
    {
      input.pickup = TRUE;
    }
    if (contdata[0].button & Z_TRIG)
    {
      input.zoomIn = TRUE;
    }
    if (contdata[0].button & R_TRIG)
    {
      input.zoomOut = TRUE;
    }

    if (contdata[0].trigger & L_JPAD)
    {
      // letting an enum go negative is undefined behavior, so wrap it now
      if (consoleMode == 0)
      {
        consoleMode = MAX_CONSOLE_MODE - 1;
      }
      else
      {
        consoleMode -= 1;
      }
    }
    if (contdata[0].trigger & R_JPAD)
    {
      if (consoleMode == MAX_CONSOLE_MODE - 1)
      {
        consoleMode = 0;
      }
      else
      {
        consoleMode += 1;
      }
    }

    if (contdata[0].trigger & D_CBUTTONS)
    {
      options[OptMeshSubdiv] = !options[OptMeshSubdiv];
    }

#if SOUND_TEST
    if (contdata[0].trigger & L_CBUTTONS)
    {
      // TODO: trigger this sound from inside the player state machine
      sndNumber = RAND(honkSoundRange) + Honk1Sound;
      // this cuts out any previous honk
      // TODO: prevent overlapping honks?
      if (sndHandle != 0)
      {
        nuAuSndPlayerSndStop(sndHandle, 0);
      }
      // sndPitch = 10.5;  // hand tuned... the sample tuning is fucked
      sndPitch = 0;
      sndHandle = nuAuSndPlayerPlay(sndNumber);
      nuAuSndPlayerSetSndPitch(sndHandle, sndPitch);
    }

    if (contdata[0].trigger & R_CBUTTONS)
    {
      if (seqPlaying)
      {
        debugPrintf("stop playing seq\n");
        nuAuSeqPlayerStop(/*frames until stop*/ 0);
        seqPlaying = FALSE;
        seqHandle = 0;
      }
      else
      {
        debugPrintf("start playing seq\n");
        seqHandle = nuAuSeqPlayerPlay(/*seq player num*/ NU_AU_SEQ_PLAYER0);
        nuAuSeqPlayerSetMasterVol(/*max*/ 0x7fff);
        MusHandleSetVolume(seqHandle, /* 200% */ 0x100);
        seqPlaying = TRUE;
      }
    }
#endif

    input.direction.x = -contdata->stick_x / 61.0F;
    input.direction.y = contdata->stick_y / 63.0F;
    if (fabsf(input.direction.x) < CONTROLLER_DEAD_ZONE)
      input.direction.x = 0;
    if (fabsf(input.direction.y) < CONTROLLER_DEAD_ZONE)
      input.direction.y = 0;
    if (Vec2d_length(&input.direction) > 1.0F)
    {
      Vec2d_normalise(&input.direction);
    }
  }

  Game_update(&input);

#if LOG_PROFILING
  if (nuScRetraceCounter % 60 == 0)
  {
    debugPrintf(
        "frm=%3.2f cpu=%3.2f rsp=%3.2f rdp=%3.2f tsks=%d drpd=%d\n",
        1000.0 / frameCounterLastFrames, profilingAverages[MainCPUTraceEvent],
        profilingAverages[RSPTaskTraceEvent],
        profilingAverages[RDPTaskTraceEvent], nuDebTaskPerfPtr->gfxTaskCnt,
        profilingCounts[SkippedGfxTaskTraceEvent]);
  }
#endif

#if LOG_TRACES
  if (contdata[0].trigger & U_CBUTTONS)
  {
    if (!loggingTrace)
    {
      if (!Trace_isTracing())
      {
        startRecordingTrace();
      }
      else
      {
        finishRecordingTrace();
      }
    }
  }
  if (Trace_getEventsCount() == TRACE_EVENT_BUFFER_SIZE)
  {
    finishRecordingTrace();
  }
#endif
  if (usbEnabled)
  {
#if LOG_TRACES
    if (loggingTrace)
    {
      logTraceChunk();
    }
#endif
  }

  // if (totalUpdates % 60 == 0) {
  //   debugPrintfSync("retrace=%d\n", nuScRetraceCounter);
  // }

  if (usbEnabled)
  {
#if LOG_TRACES
    if (loggingTrace)
    {
      logTraceChunk();
    }
#endif
#ifdef ED64
    usbResult = ed64AsyncLoggerFlush();
#endif
  }

  if (totalUpdates % 60 == 0)
  {
    // calc averages for last 60 updates
    profAvgCharacters = game->profTimeCharacters / 60;
    game->profTimeCharacters = 0.0f;
    profAvgPhysics = game->profTimePhysics / 60;
    game->profTimePhysics = 0.0f;
    profAvgDraw = game->profTimeDraw / 60;
    game->profTimeDraw = 0.0f;
    profAvgPath = game->profTimePath / 60;
    game->profTimePath = 0.0f;

    for (i = 0; i < MAX_TRACE_EVENT_TYPE; ++i)
    {
      profilingAverages[i] =
          profilingCounts[i] == 0
              ? 0
              : profilingAccumulated[i] / (float)profilingCounts[i];
      profilingAccumulated[i] = 0;
      profilingCounts[i] = 0;
    }
  }
}

/*-----------------------------------------------------------------------------
  The call-back function

  pendingGfx which is passed from Nusystem as the argument of the call-back
  function is the total of RCP tasks that are currently processing and
  waiting for the process.
-----------------------------------------------------------------------------*/
void stage00(int pendingGfx)
{
  float skippedGfxTime, profStartUpdate, profStartFrame, profEndFrame;
  // an extra task is used to draw debug stuff
  int gfxTasksPerMakeDL = consoleShown() || nuPerfBarShown ? 2 : 1;
  profStartFrame = CUR_TIME_MS();
  /* Provide the display process if n or less RCP tasks are processing or
        waiting for the process.  */
  if (nuScRetraceCounter % FRAME_SKIP == 0)
  {
    if (pendingGfx < gfxTasksPerMakeDL * 2)
    {
      makeDL00();
      Trace_addEvent(MainMakeDisplayListTraceEvent, profStartFrame,
                     CUR_TIME_MS());
    }
    else
    {
      skippedGfxTime = CUR_TIME_MS();
      Trace_addEvent(SkippedGfxTaskTraceEvent, skippedGfxTime,
                     skippedGfxTime + 16.0f);
      profilingCounts[SkippedGfxTaskTraceEvent]++;
      // debugPrintfSync("dropped frame %d\n", nuScRetraceCounter / FRAME_SKIP);
    }
  }

  profStartUpdate = CUR_TIME_MS();
  /* The process of game progress  */
  updateGame00();
  profEndFrame = CUR_TIME_MS();
  Trace_addEvent(MainCPUTraceEvent, profStartFrame, profEndFrame);
  Trace_addEvent(MainUpdateTraceEvent, profStartUpdate, profEndFrame);
  profilingAccumulated[MainCPUTraceEvent] += profEndFrame - profStartFrame;
  profilingCounts[MainCPUTraceEvent]++;
}

#ifdef SOUND
/* Provide playback and control of audio by the button of the controller */
void soundCheck(void)
{
  /* The order in which audio is played can be determined using the right and
   * left sides of the cross key. */
  if ((contdata[0].trigger & L_JPAD) || (contdata[0].trigger & R_JPAD) ||
      contdata[0].trigger & U_JPAD || contdata[0].trigger & D_JPAD)
  {
    if (sndNumber)
      nuAuSndPlayerSndStop(sndHandle, 0);

    if (contdata[0].trigger & L_JPAD)
    {
      sndNumber--;
      if (sndNumber < 1)
        sndNumber = MAX_SOUND_TYPE - 1;
    }
    else if (contdata[0].trigger & R_JPAD)
    {
      sndNumber++;
      if (sndNumber > MAX_SOUND_TYPE - 1)
        sndNumber = 1;
    }
    else if (contdata[0].trigger & U_JPAD)
    {
      sndPitch += 0.5;
      if (sndPitch > 12.0)
        sndPitch = -12.0;
    }
    else if (contdata[0].trigger & D_JPAD)
    {
      sndPitch -= 0.5;
      if (sndPitch < -12.0)
        sndPitch = 12.0;
    }

    sndHandle = nuAuSndPlayerPlay(sndNumber - 1);
    nuAuSndPlayerSetSndPitch(sndHandle, sndPitch);
  }
}
#endif // SOUND
/*----------------------------------------------------------------------*/
/*  Rom2Ram - Reads data from ROM into RAM      */
/*  IN: from_addr The source address of the data in ROM   */
/*    to_addr   The destination address of the data in RAM */
/*    seq_size          Size of the data to read      */
/*  RET:  Nothing             */
/*----------------------------------------------------------------------*/
void Rom2Ram(void *from_addr, void *to_addr, s32 seq_size)
{
  /* Cannot transfer if size is an odd number, so make it an even number. */
  if (seq_size & 0x00000001)
    seq_size++;

  nuPiReadRom((u32)from_addr, to_addr, seq_size);
}