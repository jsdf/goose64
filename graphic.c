/*
   graphic.c

   The general graphic routine (the initialization and clear the frame
   buffer) and the definition of the external variable

*/

#include <nusys.h>
// keep nusys at the top
#include "constants.h"
#include "ed64io_usb.h"
#include "graphic.h"

#define DEBUG_VI_MODE 0

Gfx gfx_glist[2][GFX_GLIST_LEN];
Dynamic gfx_dynamic[2];
Gfx* glistp;
u32 gfx_gtask_no = 0;
OSViMode viMode;

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

  viMode = osViModeTable[HIGH_RESOLUTION ? (ANTIALIASING ? OS_VI_NTSC_HAF1
                                                         : OS_VI_NTSC_HPF1)
                                         : OS_VI_NTSC_LAN1];
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
      "type=%u\ncomRegs={\nctrl=%u\nwidth=%u\nburst=%u\nvSync=%u\nhSync=%u\n"
      "leap=%u\nhStart=%u\nxScale=%u\nvCurrent=%u\n}\nfldRegs[0]={\norigin=%u\n"
      "yScale=%u\nvStart=%u\nvBurst=%u\nvIntr=%u\n}\nfldRegs[1]={\norigin=%u\n"
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
  // when osViSetMode was called these flags were reset to their default values
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
