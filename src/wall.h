#include "mesh.h"

/*
Name: wall
Texture preview:
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
	::::::::::::::::::::::::::::::::
*/

unsigned short Text_wall_wall_diff[] __attribute__((aligned (16))) = {
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,
	0x824f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x824f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x824f,0x824f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
	0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x720f,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,0x8a51,
};

Vtx_tn Vtx_wall_mesh01_0[28] __attribute__((aligned (16))) = {
	{ -237,    18,    -8, 0,   1374,   1830,     0,   127,     0, 0},
	{  236,    18,     7, 0,   1597,   1353,     0,   127,     0, 0},
	{  236,    18,    -8, 0,   1374,   1353,     0,   127,     0, 0},
	{  236,     9,   -16, 0,   1232,   1777,   127,     0,     0, 0},
	{  236,   -55,    15, 0,   1727,   1341,   127,     0,     0, 0},
	{  236,   -55,   -16, 0,   1232,   1341,   127,     0,     0, 0},
	{ -237,     9,    15, 0,    441,   1805,  -127,     0,     0, 0},
	{ -237,   -55,   -16, 0,    936,   1368,  -127,     0,     0, 0},
	{ -237,   -55,    15, 0,    441,   1368,  -127,     0,     0, 0},
	{ -237,     9,   -16, 0,    495,    773,     0,     0,  -127, 0},
	{  236,   -55,   -16, 0,    990,    337,     0,     0,  -127, 0},
	{ -237,   -55,   -16, 0,    495,    337,     0,     0,  -127, 0},
	{ -237,    18,     7, 0,    793,   1600,  -127,     1,     0, 0},
	{ -237,     9,   -16, 0,    434,   1753,  -127,     1,     0, 0},
	{ -237,     9,    15, 0,    929,   1753,  -127,     1,     0, 0},
	{  236,     9,   -16, 0,   1238,   1344,     0,    89,   -90, 0},
	{ -237,     9,   -16, 0,   1238,   1839,     0,    89,   -90, 0},
	{ -237,    18,     7, 0,   1597,   1830,     0,    89,    89, 0},
	{  236,     9,    15, 0,   1733,   1344,     0,    89,    89, 0},
	{  236,     9,    15, 0,    441,   1796,     0,     0,   127, 0},
	{ -237,   -55,    15, 0,    936,   1359,     0,     0,   127, 0},
	{  236,   -55,    15, 0,    441,   1359,     0,     0,   127, 0},
	{  236,     9,    15, 0,   1727,   1777,   127,     0,     0, 0},
	{ -237,     9,   -16, 0,    936,   1805,  -127,     0,     0, 0},
	{  236,     9,   -16, 0,    990,    773,     0,     0,  -127, 0},
	{ -237,    18,    -8, 0,    570,   1600,  -127,     1,     0, 0},
	{ -237,     9,    15, 0,   1733,   1839,     0,    89,    89, 0},
	{ -237,     9,    15, 0,    936,   1796,     0,     0,   127, 0},
};

#ifdef __N64__
Gfx Vtx_wall_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_wall_mesh01_0[0], 28, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,0,15,16,0),
	gsSP2Triangles(1,15,2,0,17,18,1,0),
	gsSP2Triangles(19,20,21,0,0,17,1,0),
	gsSP2Triangles(3,22,4,0,6,23,7,0),
	gsSP2Triangles(9,24,10,0,12,25,13,0),
	gsSP2Triangles(0,2,15,0,1,18,15,0),
	gsSP2Triangles(17,26,18,0,19,27,20,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_wall[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_wall_wall_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_wall_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_wall[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),9,10,11},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),12,13,14},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),0,15,16},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),1,15,2},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),17,18,1},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),19,20,21},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),0,17,1},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),3,22,4},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),6,23,7},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),9,24,10},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),12,25,13},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),0,2,15},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),1,18,15},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),17,26,18},
	{(Vtx*)(&Vtx_wall_mesh01_0[0]),19,27,20}
};

MeshInfo MeshInfo_wall[] __attribute__((aligned (16))) = {&Tris_wall[0],18,&Text_wall_wall_diff[0]};


#define WALL_MODEL_MESH_COUNT 1

MeshInfo* wall_meshinfos[1] = {
    MeshInfo_wall,
};
#ifdef __N64__
Gfx* wall_model_meshes[1] __attribute__((aligned (16))) = {
    Wtx_wall,
};
#endif
