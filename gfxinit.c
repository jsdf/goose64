/*
   gfxinit.c

   The display list for the initialization of graphic

   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <nusys.h>
#include "graphic.h"

/*
  The viewport structure
  The conversion from (-1,-1,-1)-(1,1,1).  The decimal part of 2-bit.
 */
static Vp vp = {
    SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* The scale factor  */
    SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* Move  */
};

Lights1 default_sun_light = gdSPDefLights1(120,
                                           120,
                                           120, /* weak ambient light */
                                           255,
                                           255,
                                           255, /* white light */
                                           80,
                                           80,
                                           0);

/*
  The initialization of RDP
*/
Gfx setup_rdpstate[] = {
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WD, SCREEN_HT),
    gsDPSetColorDither(G_CD_BAYER),
    gsSPEndDisplayList(),
};

/*
  The initialization of RSP
*/
Gfx setup_rspstate[] = {
    gsSPViewport(&vp),
    gsSPClearGeometryMode(0xFFFFFFFF),
    gsSPSetGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK),
    gsSPSetLights1(default_sun_light),
    gsSPTexture(0, 0, 0, 0, G_OFF),
    gsSPEndDisplayList(),
};
