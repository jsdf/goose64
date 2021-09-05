/*
   graphic.h

   The definition of graphic and around

*/

#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_
#include "constants.h"

#define SCREEN_WD_MAX 640
#define XSCALE_MAX 0x400

#define SCREEN_WD_HI 640
#define SCREEN_HT_HI 480

// 2bpp = 16 bit color
#define SCREEN_BPP 2
/* The screen size  */
#if HIGH_RESOLUTION  // 640x480 and 640x240
#define SCREEN_WD 640

#if HIGH_RESOLUTION_HALF_Y
#define SCREEN_HT 240
#define ANTIALIASING 1
#else
#define SCREEN_HT 480
#define ANTIALIASING 0
#endif

#define FRAME_BUFFERS 3
// hi res z buffer uses 600kb (0x96000), resides in 8th mb of RDRAM
#define ZBUFFER_ADDR 0x80700000
// hi res color buffers use 1800kb (0x1C2000), reside in top of 6th, all of 7th
// mb of RDRAM
#define CFB_ADDR \
  (0x80700000 - SCREEN_WD * SCREEN_HT * SCREEN_BPP * FRAME_BUFFERS)
#define CFB0_ADDR (CFB_ADDR)
#define CFB1_ADDR (CFB0_ADDR + SCREEN_WD * SCREEN_HT * SCREEN_BPP)
#define CFB2_ADDR (CFB1_ADDR + SCREEN_WD * SCREEN_HT * SCREEN_BPP)

#else  // low resolution, 320x240
#define SCREEN_WD 320
#define SCREEN_HT 240
#define ANTIALIASING 1
#define FRAME_BUFFERS 3
// in lowest usable ram, uses 150kb
#define ZBUFFER_ADDR 0x80000400
// in highest part of ram, uses 450kb
#define CFB_ADDR \
  (0x80400000 - SCREEN_WD * SCREEN_HT * SCREEN_BPP * FRAME_BUFFERS)
#define CFB0_ADDR (CFB_ADDR)
#define CFB1_ADDR (CFB0_ADDR + SCREEN_WD * SCREEN_HT * SCREEN_BPP)
#define CFB2_ADDR (CFB1_ADDR + SCREEN_WD * SCREEN_HT * SCREEN_BPP)
#endif

#define RES_SCALE_X(x) ((float)x / (float)SCREEN_WD_HI * (float)SCREEN_WD)
#define RES_SCALE_Y(y) ((float)y / (float)SCREEN_HT_HI * (float)SCREEN_HT)

/* The maximum length of the display list of one task  */
#define GFX_GLIST_LEN 2048

// render mode to write to z buffer but not depth compare when rendering
#define RM_AA_ZUPD_OPA_SURF(clk)                                      \
  AA_EN | Z_UPD | IM_RD | CVG_DST_CLAMP | ZMODE_OPA | ALPHA_CVG_SEL | \
      GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define G_RM_AA_ZUPD_OPA_SURF RM_AA_ZUPD_OPA_SURF(1)
#define G_RM_AA_ZUPD_OPA_SURF2 RM_AA_ZUPD_OPA_SURF(2)

#define RM_ZUPD_OPA_SURF(clk)                        \
  Z_UPD | CVG_DST_FULL | ALPHA_CVG_SEL | ZMODE_OPA | \
      GBL_c##clk(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_A_MEM)

#define G_RM_ZUPD_OPA_SURF RM_ZUPD_OPA_SURF(1)
#define G_RM_ZUPD_OPA_SURF2 RM_ZUPD_OPA_SURF(2)

/*-------------------------- define structure ------------------------------ */
/* The structure of the projection-matrix  */
typedef struct {
  // this stuff gets sent to the RCP so it must be 64bit aligned otherwise
  // everything will fuck up
  Mtx projection;
  Mtx modeling;
  Mtx camera;

  Mtx zUpToYUpCoordinatesRotation;

  Mtx objTransforms[MAX_WORLD_OBJECTS];
} Dynamic;

typedef float ViewportF[4];

/*
rotation/scale is from mtx[0][0] to mtx[2][2]
translation (13-15) is from mtx[3][0] to mtx[3][2]
 */
typedef float MtxF[4][4];

/*-------------------------------- parameter---------------------------------*/
extern Dynamic gfx_dynamic[];
extern Gfx* glistp;
extern Gfx gfx_glist[][GFX_GLIST_LEN];
extern u32 gfx_gtask_no;
/*-------------------------------- function ---------------------------------*/
extern void gfxInit(void);     // on startup/switch video mode
extern void gfxRCPInit(void);  // on graphics task
extern void gfxClearCfb(u16 fillColor);
/*------------------------------- other extern define -----------------------*/
extern Gfx setup_rdpstate[];
extern Gfx setup_rspstate[];

#endif /* _GRAPHIC_H_ */
