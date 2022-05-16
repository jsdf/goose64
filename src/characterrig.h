#include "mesh.h"

/*
Name: characterrig
Texture preview:
	********-:::::::::::::::::::::::
	********-:::::::::::::::::::::::
	********-:::::::::::::::::::::::
	********-:::::::::::::::::::::::
	********::::::::::::::::::::::::
	********::::::::::::::::::::::::
	********::::::::::::::::::::::::
	********::::::::::::::::::::::::
	********::::::::::::::::::::::::
	********::::::::::::::::::::::::
	********::::::::::::::::::::::::
	********-:::::::::::::::::::::::
	********-:::::::::::::::::::::::
	@@@@@@@--:::::::::::::::::::::::
	@@@@@@@--:::::::::::::::::::::::
	@@@@@@@--:::::::::::::::::::::::
	@@@@@@@--:::::::::::::::::::::::
	@@@@@@@--:::::::::::::::::::::::
	@@@@@@@@-:::::::::::::::::::::::
	@++@@@@@-:::::::::::::::::::::::
	@++@@@@@-:::::::::::::::::::::::
	@@@@@@@@-:::::::::::::::::::::::
	@@@@@@@@-:::::::::::::::::::::::
	@@..@@@@::::::::::::::::::::::::
	.......@::::::::::::::::::::::::
	........::::::::::::::::::::::::
	........::::::::::::::::::::::::
	........-:::::::::::::::::::::::
	........-:::::::::::::::::::::::
	........-:::::::::::::::::::::::
	........-:::::::::::::::::::::::
	........--::::::::::::::::::::::
*/

unsigned short Text_characterrig_person_diff[] __attribute__((aligned (16))) = {
	0xde2d,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8353,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8353,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b51,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0xd5e9,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xd4e5,0xd4e5,0xffff,0xffff,0xffff,0xffff,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xd4e5,0xd4e5,0xffff,0xffff,0xffff,0xffff,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0xffff,0xffff,0x4189,0x4189,0xffff,0xffff,0xffff,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0xffff,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8351,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
	0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x4189,0x8353,0x8b11,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,0x8b0f,
};

Vtx_tn Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[12] __attribute__((aligned (16))) = {
	{    3,    -3,    -1, 0,     80,   1885,   -23,   -87,    90, 0},
	{   -4,    -5,   -10, 0,    271,   1748,    22,  -106,   -67, 0},
	{   10,    -8,   -10, 0,    203,   1760,   -54,  -116,    -5, 0},
	{   11,     5,   -10, 0,    198,   1705,   -50,    76,   -89, 0},
	{   -7,     1,   -11, 0,    262,   1702,    -5,    81,   -98, 0},
	{   -6,     0,    -1, 0,    417,   1527,  -122,   -37,    -6, 0},
	{   -4,    -3,    -1, 0,    416,   1885,   -61,   -68,    88, 0},
	{   16,    -4,    -5, 0,    169,   1749,    18,   -58,   111, 0},
	{   -1,     4,    -1, 0,     73,   1601,   -67,   100,    40, 0},
	{   19,     3,    -9, 0,    174,   1731,    83,    86,    40, 0},
	{   16,     3,    -5, 0,    162,   1720,    78,    15,    98, 0},
	{   19,    -5,    -9, 0,    178,   1738,     3,  -112,   -61, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0], 12, 0),
	gsSP2Triangles(0,1,2,0,3,1,4,0),
	gsSP2Triangles(5,1,6,0,2,7,0,0),
	gsSP2Triangles(8,4,5,0,7,9,10,0),
	gsSP2Triangles(3,11,2,0,0,10,8,0),
	gsSP2Triangles(3,10,9,0,6,8,5,0),
	gsSP2Triangles(8,3,4,0,0,6,1,0),
	gsSP2Triangles(3,2,1,0,5,4,1,0),
	gsSP2Triangles(2,11,7,0,7,11,9,0),
	gsSP2Triangles(3,9,11,0,0,7,10,0),
	gsSP2Triangles(3,8,10,0,6,0,8,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkfoot_r_gkfoot_rmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkfoot_r_gkfoot_rmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),3,1,4},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),5,1,6},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),2,7,0},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),8,4,5},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),7,9,10},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),3,11,2},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),0,10,8},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),3,10,9},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),6,8,5},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),8,3,4},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),0,6,1},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),3,2,1},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),5,4,1},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),2,11,7},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),7,11,9},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),3,9,11},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),0,7,10},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),3,8,10},
	{(Vtx*)(&Vtx_characterrig_gkfoot_r_gkfoot_rmesh_mesh01_0[0]),6,0,8}
};

MeshInfo MeshInfo_characterrig_gkfoot_r_gkfoot_rmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkfoot_r_gkfoot_rmesh[0],20,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[28] __attribute__((aligned (16))) = {
	{    0,     1,    -4, 0,    990,   1237,   -18,    93,   -85, 0},
	{   -3,   -11,   -22, 0,    990,   1237,  -102,    25,    72, 0},
	{   -4,    -1,    -2, 0,    990,   1237,  -109,    65,   -15, 0},
	{    0,    -9,   -24, 0,    136,    567,   -65,   105,    31, 0},
	{   -4,   -11,   -24, 0,    293,    575,   -85,    18,    92, 0},
	{   -3,   -11,   -22, 0,    403,    407,  -102,    25,    72, 0},
	{    0,     1,    -4, 0,   1585,    373,   -18,    93,   -85, 0},
	{    4,   -10,   -21, 0,   1586,    373,   116,    12,   -49, 0},
	{    0,    -9,   -24, 0,   1586,    373,   -65,   105,    31, 0},
	{   -1,    -4,     1, 0,   1587,    373,    -4,  -122,    37, 0},
	{    5,    -1,    -2, 0,   1586,    373,    81,   -29,    93, 0},
	{   -4,    -1,    -2, 0,   1586,    372,  -109,    65,   -15, 0},
	{    1,   -14,   -22, 0,   1586,    373,    -5,   -66,   108, 0},
	{   -1,    -8,   -26, 0,    192,    604,   -86,    92,   -16, 0},
	{   -3,   -13,   -28, 0,    251,    637,  -126,   -20,     0, 0},
	{    6,   -10,   -23, 0,    203,    701,    86,    91,    20, 0},
	{    1,   -14,   -22, 0,    397,    773,    -5,   -66,   108, 0},
	{    4,   -10,   -21, 0,    144,    773,   116,    12,   -49, 0},
	{    1,   -15,   -23, 0,    302,    694,     2,   -71,   105, 0},
	{   -1,    -9,   -30, 0,    235,    637,    47,    59,  -102, 0},
	{    2,   -16,   -27, 0,    257,    651,   -23,  -103,   -72, 0},
	{    4,    -9,   -28, 0,    241,    656,    69,    91,   -55, 0},
	{    0,     2,     0, 0,   1152,   1001,   -20,   107,    64, 0},
	{    0,     0,     2, 0,   1412,    626,    36,    42,   113, 0},
	{    2,     1,     1, 0,   1421,    612,    20,   113,    53, 0},
	{   -2,     1,     1, 0,   1163,    987,   -98,    11,    80, 0},
	{    0,    -9,   -24, 0,    990,   1237,   -65,   105,    31, 0},
	{   -3,   -11,   -22, 0,   1586,    373,  -102,    25,    72, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0], 28, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,7,10,0),
	gsSP2Triangles(11,12,9,0,13,14,4,0),
	gsSP2Triangles(3,15,13,0,16,15,17,0),
	gsSP2Triangles(16,4,18,0,19,20,14,0),
	gsSP2Triangles(13,21,19,0,18,21,15,0),
	gsSP2Triangles(18,14,20,0,22,23,24,0),
	gsSP2Triangles(24,0,22,0,25,9,23,0),
	gsSP2Triangles(25,0,2,0,24,9,10,0),
	gsSP2Triangles(0,26,1,0,3,13,4,0),
	gsSP2Triangles(6,10,7,0,9,12,7,0),
	gsSP2Triangles(11,27,12,0,13,19,14,0),
	gsSP2Triangles(3,17,15,0,16,18,15,0),
	gsSP2Triangles(16,5,4,0,19,21,20,0),
	gsSP2Triangles(13,15,21,0,18,20,21,0),
	gsSP2Triangles(18,4,14,0,22,25,23,0),
	gsSP2Triangles(24,10,0,0,25,2,9,0),
	gsSP2Triangles(25,22,0,0,24,23,9,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkforearm_r_gkforearm_rmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkforearm_r_gkforearm_rmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),9,7,10},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),11,12,9},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),13,14,4},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),3,15,13},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),16,15,17},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),16,4,18},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),19,20,14},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),13,21,19},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),18,21,15},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),18,14,20},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),22,23,24},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),24,0,22},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),25,9,23},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),25,0,2},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),24,9,10},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),0,26,1},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),3,13,4},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),6,10,7},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),9,12,7},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),11,27,12},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),13,19,14},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),3,17,15},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),16,18,15},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),16,5,4},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),19,21,20},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),13,15,21},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),18,20,21},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),18,4,14},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),22,25,23},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),24,10,0},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),25,2,9},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),25,22,0},
	{(Vtx*)(&Vtx_characterrig_gkforearm_r_gkforearm_rmesh_mesh01_0[0]),24,23,9}
};

MeshInfo MeshInfo_characterrig_gkforearm_r_gkforearm_rmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkforearm_r_gkforearm_rmesh[0],36,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[32] __attribute__((aligned (16))) = {
	{   -9,    -7,    -1, 0,    270,    274,   -79,   -98,   -18, 0},
	{  -12,     0,     4, 0,    339,    159,  -127,    -2,     1, 0},
	{  -12,     0,     0, 0,    258,    165,  -122,   -30,   -24, 0},
	{    3,     0,   -10, 0,     96,    244,    54,    -8,  -115, 0},
	{    3,    -7,    -6, 0,    166,    197,    26,  -106,   -66, 0},
	{   -1,    -5,   -11, 0,     86,    177,    22,   -82,   -95, 0},
	{  -10,     0,   -10, 0,    105,    232,  -101,    -9,   -78, 0},
	{   -5,    -5,   -11, 0,     95,    323,   -46,   -99,   -66, 0},
	{    3,    -7,    -6, 0,    183,    425,    26,  -106,   -66, 0},
	{   -5,    -7,    10, 0,    429,    321,   -60,   -64,    92, 0},
	{   -7,     0,    13, 0,    482,    225,  -103,     6,    75, 0},
	{    3,    -7,    -4, 0,    210,    425,    49,  -117,     0, 0},
	{    3,    -8,     4, 0,    354,    425,    22,  -124,    17, 0},
	{    8,     0,    -6, 0,    158,    282,   119,    -8,   -43, 0},
	{    8,     0,    -4, 0,    202,    282,   122,   -26,   -22, 0},
	{    8,    -2,    -3, 0,    220,    253,    93,   -87,     1, 0},
	{    4,     0,    13, 0,    390,    232,    24,   -43,   117, 0},
	{   -5,    -7,    10, 0,    349,    122,   -60,   -64,    92, 0},
	{    1,    -7,     9, 0,    331,    177,    30,   -69,   102, 0},
	{   -9,    -7,     4, 0,    342,    270,   -59,  -112,    17, 0},
	{    8,     0,     4, 0,    295,    282,   124,    23,    12, 0},
	{    3,    -8,     4, 0,    277,    179,    22,  -124,    17, 0},
	{    8,     0,     4, 0,    150,   1232,   124,    23,    12, 0},
	{    8,    -2,    -3, 0,     98,   1203,    93,   -87,     1, 0},
	{   10,     0,    -3, 0,     93,   1232,   124,    -5,    25, 0},
	{    3,    -7,    -4, 0,    183,    194,    49,  -117,     0, 0},
	{    8,     0,    -4, 0,     80,   1232,   122,   -26,   -22, 0},
	{   -9,     6,    -1, 0,    270,    274,   -83,    87,   -40, 0},
	{    3,     6,    -6, 0,    166,    197,    39,    96,   -73, 0},
	{   -1,     4,   -11, 0,     86,    177,    28,    75,   -99, 0},
	{   -5,     4,   -11, 0,     95,    323,   -49,    92,   -73, 0},
	{    8,     1,    -3, 0,    220,    253,   107,    58,   -34, 0},
};Vtx_tn Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[32] __attribute__((aligned (16))) = {
	{   -5,     4,   -11, 0,     95,    323,   -49,    92,   -73, 0},
	{    3,     6,    -6, 0,    183,    425,    39,    96,   -73, 0},
	{   -1,     4,   -11, 0,     96,    369,    28,    75,   -99, 0},
	{  -12,     0,     4, 0,    339,    159,  -127,    -2,     1, 0},
	{   -5,     6,    10, 0,    429,    321,   -67,    96,    48, 0},
	{   -9,     6,     4, 0,    342,    270,   -98,    78,    20, 0},
	{    3,     6,    -4, 0,    210,    425,     4,   123,   -28, 0},
	{   -9,     6,    -1, 0,    270,    274,   -83,    87,   -40, 0},
	{    3,     7,     4, 0,    354,    425,    50,   116,   -10, 0},
	{    4,     0,    13, 0,    390,    232,    24,   -43,   117, 0},
	{   -5,     6,    10, 0,    349,    122,   -67,    96,    48, 0},
	{   -7,     0,    13, 0,    434,    133,  -103,     6,    75, 0},
	{    1,     6,     9, 0,    331,    177,    84,    78,    52, 0},
	{    8,     0,     4, 0,    295,    282,   124,    23,    12, 0},
	{    3,     7,     4, 0,    277,    179,    50,   116,   -10, 0},
	{    8,     0,     4, 0,    150,   1232,   124,    23,    12, 0},
	{   10,     0,    -3, 0,     93,   1232,   124,    -5,    25, 0},
	{    8,     1,    -3, 0,     98,   1203,   107,    58,   -34, 0},
	{    8,     1,    -3, 0,    220,    253,   107,    58,   -34, 0},
	{    3,     6,    -4, 0,    183,    194,     4,   123,   -28, 0},
	{    8,     0,    -4, 0,     80,   1232,   122,   -26,   -22, 0},
	{   -5,    -5,   -11, 0,     95,    323,   -46,   -99,   -66, 0},
	{   -1,    -5,   -11, 0,     96,    369,    22,   -82,   -95, 0},
	{    3,    -7,    -6, 0,    183,    425,    26,  -106,   -66, 0},
	{   -5,    -7,    10, 0,    349,    122,   -60,   -64,    92, 0},
	{    3,    -8,     4, 0,    354,    425,    22,  -124,    17, 0},
	{    1,    -7,     9, 0,    434,    397,    30,   -69,   102, 0},
	{   -5,    -7,    10, 0,    429,    321,   -60,   -64,    92, 0},
	{   -7,     0,    13, 0,    482,    225,  -103,     6,    75, 0},
	{    3,     6,    -6, 0,    166,    197,    39,    96,   -73, 0},
	{    1,     6,     9, 0,    434,    397,    84,    78,    52, 0},
	{    8,     0,     4, 0,    272,    282,   124,    23,    12, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkhead_gkheadmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,0,2,0,7,8,0,0),
	gsSP2Triangles(1,9,10,0,11,12,0,0),
	gsSP2Triangles(13,14,15,0,16,17,18,0),
	gsSP2Triangles(12,9,19,0,20,18,21,0),
	gsSP2Triangles(22,23,24,0,15,21,25,0),
	gsSP2Triangles(23,26,24,0,1,27,2,0),
	gsSP2Triangles(28,3,29,0,6,27,30,0),
	gsSP2Triangles(31,14,13,0,0,19,1,0),
	gsSP2Triangles(3,13,4,0,6,7,0,0),
	gsSP2Triangles(1,19,9,0,19,0,12,0),
	gsSP2Triangles(0,8,11,0,25,4,15,0),
	gsSP2Triangles(4,13,15,0,20,16,18,0),
	gsSP2Triangles(15,20,21,0,28,13,3,0),
	gsSP2Triangles(6,2,27,0,13,28,31,0),
	gsSPVertex(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(4,8,5,0,12,13,14,0),
	gsSP2Triangles(15,16,17,0,14,18,19,0),
	gsSP2Triangles(17,16,20,0,21,22,23,0),
	gsSP2Triangles(9,11,24,0,25,26,27,0),
	gsSP2Triangles(3,5,7,0,0,7,1,0),
	gsSP2Triangles(3,28,4,0,1,7,6,0),
	gsSP2Triangles(7,5,8,0,29,19,18,0),
	gsSP2Triangles(9,12,10,0,4,30,8,0),
	gsSP2Triangles(12,9,13,0,14,31,18,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkhead_gkheadmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkhead_gkheadmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkhead_gkheadmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),6,0,2},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),7,8,0},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),1,9,10},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),11,12,0},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),13,14,15},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),16,17,18},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),12,9,19},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),20,18,21},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),22,23,24},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),15,21,25},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),23,26,24},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),1,27,2},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),28,3,29},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),6,27,30},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),31,14,13},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),0,19,1},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),3,13,4},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),6,7,0},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),1,19,9},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),19,0,12},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),0,8,11},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),25,4,15},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),4,13,15},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),20,16,18},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),15,20,21},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),28,13,3},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),6,2,27},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_0[0]),13,28,31},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),9,10,11},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),4,8,5},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),12,13,14},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),15,16,17},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),14,18,19},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),17,16,20},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),21,22,23},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),9,11,24},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),25,26,27},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),3,5,7},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),0,7,1},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),3,28,4},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),1,7,6},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),7,5,8},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),29,19,18},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),9,12,10},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),4,30,8},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),12,9,13},
	{(Vtx*)(&Vtx_characterrig_gkhead_gkheadmesh_mesh01_1[0]),14,31,18}
};

MeshInfo MeshInfo_characterrig_gkhead_gkheadmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkhead_gkheadmesh[0],52,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[18] __attribute__((aligned (16))) = {
	{   -5,     1,    -1, 0,    990,   1237,  -123,     7,   -34, 0},
	{   -1,     4,   -35, 0,   1485,   1237,    -2,   112,   -59, 0},
	{   -6,     0,   -35, 0,    990,   1237,  -111,    -2,   -63, 0},
	{    3,    -7,    -1, 0,    997,    267,    13,  -121,   -40, 0},
	{    3,     0,   -35, 0,    813,    343,    96,     9,   -83, 0},
	{   10,     2,    -1, 0,   1009,    374,   114,   -46,    31, 0},
	{   -5,     1,    -1, 0,   1485,    742,  -123,     7,   -34, 0},
	{   -1,    -5,   -35, 0,    990,    742,     1,  -105,   -73, 0},
	{    3,    -7,    -1, 0,    990,    742,    13,  -121,   -40, 0},
	{    1,     7,    -2, 0,   1000,    478,   -18,   121,   -34, 0},
	{   -1,     4,   -35, 0,    813,    445,    -2,   112,   -59, 0},
	{    3,    -4,     3, 0,   1019,    456,     3,   -44,   119, 0},
	{    1,     4,     3, 0,   1310,   1065,    -6,    46,   118, 0},
	{   -2,     1,     4, 0,   1079,   1068,   -39,     6,   120, 0},
	{    7,     2,     3, 0,   1077,    523,    51,    18,   114, 0},
	{    1,     7,    -2, 0,   1485,   1237,   -18,   121,   -34, 0},
	{   -1,    -5,   -35, 0,    845,    212,     1,  -105,   -73, 0},
	{   -6,     0,   -35, 0,   1485,    742,  -111,    -2,   -63, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0], 18, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,4,10,0),
	gsSP2Triangles(11,12,13,0,11,0,3,0),
	gsSP2Triangles(12,0,13,0,14,3,5,0),
	gsSP2Triangles(14,15,12,0,0,15,1,0),
	gsSP2Triangles(3,16,4,0,6,17,7,0),
	gsSP2Triangles(9,5,4,0,11,14,12,0),
	gsSP2Triangles(11,13,0,0,12,15,0,0),
	gsSP2Triangles(14,11,3,0,14,5,15,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkshin_r_gkshin_rmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkshin_r_gkshin_rmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),9,4,10},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),11,12,13},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),11,0,3},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),12,0,13},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),14,3,5},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),14,15,12},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),0,15,1},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),3,16,4},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),6,17,7},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),9,5,4},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),11,14,12},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),11,13,0},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),12,15,0},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),14,11,3},
	{(Vtx*)(&Vtx_characterrig_gkshin_r_gkshin_rmesh_mesh01_0[0]),14,5,15}
};

MeshInfo MeshInfo_characterrig_gkshin_r_gkshin_rmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkshin_r_gkshin_rmesh[0],18,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[31] __attribute__((aligned (16))) = {
	{   11,    -2,   -31, 0,   1012,    375,   122,   -31,    16, 0},
	{   -1,   -11,   -19, 0,   1178,    257,     6,  -110,   -65, 0},
	{    4,   -12,   -31, 0,    997,    267,    90,   -84,   -34, 0},
	{   -1,   -11,   -19, 0,    990,   1237,     6,  -110,   -65, 0},
	{   -4,    -3,   -31, 0,   1485,    742,  -105,   -59,   -43, 0},
	{    4,   -12,   -31, 0,    990,    742,    90,   -84,   -34, 0},
	{    1,     2,   -31, 0,   1000,    478,   -16,   125,    -7, 0},
	{    1,     8,    -9, 0,   1222,    527,    27,   102,   -70, 0},
	{   11,    -2,   -31, 0,   1009,    374,   122,   -31,    16, 0},
	{   -9,     0,   -14, 0,    990,   1732,   -89,    34,   -85, 0},
	{    1,     2,   -31, 0,   1485,   1237,   -16,   125,    -7, 0},
	{   -4,    -3,   -31, 0,    990,   1237,  -105,   -59,   -43, 0},
	{    9,    -3,   -13, 0,   1193,    388,   120,     6,    39, 0},
	{    8,     8,    -1, 0,   1351,    538,   109,    32,    56, 0},
	{    7,    -7,     5, 0,   1334,    412,   108,   -66,     0, 0},
	{   -9,     0,   -14, 0,   1485,   1237,   -89,    34,   -85, 0},
	{  -12,    -6,    -2, 0,    990,   1237,  -124,   -30,     3, 0},
	{  -10,     6,    -2, 0,   1485,   1237,   -82,    64,   -73, 0},
	{   -1,   -10,     3, 0,   1324,    342,   -44,  -116,    30, 0},
	{  -12,    -6,    -2, 0,   1315,    221,  -124,   -30,     3, 0},
	{   -1,   -10,     3, 0,   1325,    350,   -44,  -116,    30, 0},
	{    1,     8,    -9, 0,   1485,   1732,    27,   102,   -70, 0},
	{    0,     0,   -36, 0,   1326,   1082,   -11,    40,  -121, 0},
	{    2,    -9,   -35, 0,   1016,    437,     1,   -55,  -115, 0},
	{   -3,    -3,   -35, 0,   1071,   1085,   -63,    -6,  -111, 0},
	{  -12,     0,     7, 0,   1378,    623,   -66,   -54,    94, 0},
	{   -5,    -2,    10, 0,   1384,    699,   -66,   -11,   108, 0},
	{  -12,     6,     8, 0,   1440,    992,  -105,    13,    71, 0},
	{    0,     7,     8, 0,   1370,    626,   -33,    93,    79, 0},
	{   -1,    -1,    11, 0,   1347,    488,    22,   -47,   115, 0},
	{    7,    -3,   -35, 0,   1071,    508,    40,    12,  -120, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0], 31, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,15,16,17,0),
	gsSP2Triangles(1,18,19,0,0,7,12,0),
	gsSP2Triangles(20,12,14,0,21,15,17,0),
	gsSP2Triangles(13,12,7,0,21,17,13,0),
	gsSP2Triangles(22,23,24,0,25,26,27,0),
	gsSP2Triangles(26,28,27,0,29,18,14,0),
	gsSP2Triangles(28,14,13,0,25,17,19,0),
	gsSP2Triangles(26,19,18,0,28,17,27,0),
	gsSP2Triangles(23,11,24,0,22,11,10,0),
	gsSP2Triangles(30,2,23,0,30,10,8,0),
	gsSP2Triangles(0,12,1,0,3,15,4,0),
	gsSP2Triangles(9,21,10,0,15,3,16,0),
	gsSP2Triangles(20,1,12,0,22,30,23,0),
	gsSP2Triangles(26,29,28,0,29,26,18,0),
	gsSP2Triangles(28,29,14,0,25,27,17,0),
	gsSP2Triangles(26,25,19,0,28,13,17,0),
	gsSP2Triangles(23,2,11,0,22,24,11,0),
	gsSP2Triangles(30,8,2,0,30,22,10,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkthigh_r_gkthigh_rmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkthigh_r_gkthigh_rmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),9,10,11},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),12,13,14},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),15,16,17},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),1,18,19},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),0,7,12},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),20,12,14},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),21,15,17},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),13,12,7},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),21,17,13},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),22,23,24},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),25,26,27},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),26,28,27},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),29,18,14},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),28,14,13},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),25,17,19},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),26,19,18},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),28,17,27},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),23,11,24},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),22,11,10},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),30,2,23},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),30,10,8},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),0,12,1},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),3,15,4},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),9,21,10},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),15,3,16},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),20,1,12},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),22,30,23},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),26,29,28},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),29,26,18},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),28,29,14},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),25,27,17},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),26,25,19},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),28,13,17},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),23,2,11},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),22,24,11},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),30,8,2},
	{(Vtx*)(&Vtx_characterrig_gkthigh_r_gkthigh_rmesh_mesh01_0[0]),30,22,10}
};

MeshInfo MeshInfo_characterrig_gkthigh_r_gkthigh_rmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkthigh_r_gkthigh_rmesh[0],40,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[32] __attribute__((aligned (16))) = {
	{  -11,     0,     3, 0,   1613,    232,  -125,    -3,    22, 0},
	{   -6,   -12,    10, 0,   1624,    348,  -108,   -53,    42, 0},
	{   -8,     0,    16, 0,   1682,    259,  -125,     0,    25, 0},
	{    0,   -17,   -13, 0,   1442,    359,    -5,  -127,     9, 0},
	{   -6,   -13,     6, 0,   1574,    326,   -83,   -92,   -32, 0},
	{  -10,   -15,   -17, 0,   1397,    282,   -63,  -107,    29, 0},
	{  -11,     0,   -21, 0,   1428,    177,  -127,     0,    -2, 0},
	{    9,   -16,   -12, 0,   1446,    417,    96,   -74,    38, 0},
	{   12,     0,     3, 0,   1569,    533,   120,   -24,    30, 0},
	{    6,   -14,     9, 0,   1555,    405,    85,   -93,   -12, 0},
	{   10,     0,    13, 0,   1639,    525,   112,   -33,    50, 0},
	{    0,   -16,    14, 0,   1614,    411,    48,  -115,    23, 0},
	{   -6,     0,    21, 0,     52,    231,  -104,   -35,    65, 0},
	{    0,    -5,    25, 0,     95,    323,   -75,   -35,    96, 0},
	{   -6,     0,    26, 0,    105,    232,  -127,     0,    13, 0},
	{    3,    -6,    21, 0,   1708,    430,    43,  -117,    25, 0},
	{    0,    -6,    21, 0,   1718,    400,   -43,  -102,    63, 0},
	{   -6,     0,    21, 0,   1749,    332,  -104,   -35,    65, 0},
	{    3,    -6,    21, 0,     51,    373,    43,  -117,    25, 0},
	{    0,    -6,    21, 0,     45,    332,   -43,  -102,    63, 0},
	{    0,   -18,   -29, 0,   1324,    342,   -32,  -122,   -20, 0},
	{  -11,   -14,   -34, 0,   1315,    221,     4,  -106,   -71, 0},
	{    7,     0,    21, 0,     57,    246,   104,   -35,    62, 0},
	{    3,    -5,    25, 0,     86,    177,    44,  -110,    45, 0},
	{    3,    -6,    21, 0,     46,    177,    43,  -117,    25, 0},
	{    8,   -15,   -27, 0,   1334,    412,   119,   -42,    -2, 0},
	{   13,     0,   -11, 0,   1457,    538,   126,    -2,    -1, 0},
	{    0,   -15,     6, 0,   1550,    373,   -16,  -126,     6, 0},
	{  -11,   -14,   -34, 0,    990,   1237,     4,  -106,   -71, 0},
	{  -10,   -15,   -17, 0,    990,   1237,   -63,  -107,    29, 0},
	{  -11,     0,   -21, 0,   1485,   1237,  -127,     0,    -2, 0},
	{  -10,    14,   -17, 0,   1397,    282,   -85,    79,    50, 0},
};Vtx_tn Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[32] __attribute__((aligned (16))) = {
	{  -11,     0,     3, 0,   1613,    232,  -125,    -3,    22, 0},
	{   -6,    11,    10, 0,   1624,    348,   -95,    84,    -5, 0},
	{   -6,    12,     6, 0,   1574,    326,  -109,    66,    -2, 0},
	{    0,    16,   -13, 0,   1442,    359,    -6,   126,    10, 0},
	{  -10,    14,   -17, 0,   1397,    282,   -85,    79,    50, 0},
	{    9,    15,   -12, 0,   1446,    417,    96,    73,    38, 0},
	{   12,     0,     3, 0,   1569,    533,   120,   -24,    30, 0},
	{   13,     0,   -11, 0,   1457,    538,   126,    -2,    -1, 0},
	{    6,    13,     9, 0,   1555,    405,   104,    52,    50, 0},
	{   10,     0,    13, 0,   1639,    525,   112,   -33,    50, 0},
	{   -6,     0,    21, 0,     52,    231,  -104,   -35,    65, 0},
	{    0,     4,    25, 0,     95,    323,   -39,   110,    48, 0},
	{    0,     5,    21, 0,     45,    332,   -65,    76,    78, 0},
	{    3,     5,    21, 0,   1708,    430,    97,    74,    32, 0},
	{    0,    15,    14, 0,   1614,    411,    13,    71,   104, 0},
	{   -8,     0,    16, 0,   1682,    259,  -125,     0,    25, 0},
	{    0,     5,    21, 0,   1718,    400,   -65,    76,    78, 0},
	{    3,     5,    21, 0,     51,    373,    97,    74,    32, 0},
	{    0,    17,   -29, 0,   1324,    342,   -42,    84,   -86, 0},
	{  -11,    13,   -34, 0,   1315,    221,   -90,    89,   -11, 0},
	{    3,     4,    25, 0,     86,    177,    88,    45,    78, 0},
	{    7,     0,    21, 0,     57,    246,   104,   -35,    62, 0},
	{    3,     5,    21, 0,     46,    177,    97,    74,    32, 0},
	{    8,    14,   -27, 0,   1334,    412,   119,    41,    -2, 0},
	{    9,     0,   -33, 0,   1351,    538,    84,    47,   -83, 0},
	{    0,    17,   -29, 0,   1325,    350,   -42,    84,   -86, 0},
	{    0,    14,     6, 0,   1550,    373,   -17,   125,     4, 0},
	{  -11,     0,   -21, 0,   1485,   1237,  -127,     0,    -2, 0},
	{  -11,    13,   -34, 0,    990,   1237,   -90,    89,   -11, 0},
	{   -9,     2,   -34, 0,   1485,   1237,  -115,    15,   -54, 0},
	{    3,     4,    25, 0,     96,    369,    88,    45,    78, 0},
	{    3,    -5,    25, 0,     86,    177,    44,  -110,    45, 0},
};Vtx_tn Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[32] __attribute__((aligned (16))) = {
	{   10,     0,    13, 0,   1639,    525,   112,   -33,    50, 0},
	{    7,     0,    21, 0,   1718,    496,   104,   -35,    62, 0},
	{    3,    -6,    21, 0,   1708,    430,    43,  -117,    25, 0},
	{    3,    -6,    21, 0,     51,    373,    43,  -117,    25, 0},
	{    3,    -5,    25, 0,     96,    369,    44,  -110,    45, 0},
	{    0,    -5,    25, 0,     95,    323,   -75,   -35,    96, 0},
	{    7,     0,    21, 0,     57,    246,   104,   -35,    62, 0},
	{    7,     0,    26, 0,     96,    244,    58,   -62,    93, 0},
	{    3,    -5,    25, 0,     86,    177,    44,  -110,    45, 0},
	{    8,   -15,   -27, 0,   1334,    412,   119,   -42,    -2, 0},
	{    9,     0,   -33, 0,   1351,    538,    84,    47,   -83, 0},
	{   13,     0,   -11, 0,   1457,    538,   126,    -2,    -1, 0},
	{    0,   -17,   -13, 0,   1442,    359,    -5,  -127,     9, 0},
	{    0,   -18,   -29, 0,   1325,    350,   -32,  -122,   -20, 0},
	{  -11,     0,   -21, 0,   1485,   1237,  -127,     0,    -2, 0},
	{  -11,     0,   -33, 0,   1485,   1536,   -85,   -55,   -79, 0},
	{   -9,    -3,   -34, 0,   1485,   1237,  -115,   -16,   -54, 0},
	{  -11,   -14,   -34, 0,    990,   1237,     4,  -106,   -71, 0},
	{   -6,     0,    21, 0,     52,    231,  -104,   -35,    65, 0},
	{   -6,     0,    26, 0,    105,    232,  -127,     0,    13, 0},
	{    0,     4,    25, 0,     95,    323,   -39,   110,    48, 0},
	{    3,     5,    21, 0,   1708,    430,    97,    74,    32, 0},
	{   -8,     0,    16, 0,   1682,    259,  -125,     0,    25, 0},
	{   -6,     0,    21, 0,   1749,    332,  -104,   -35,    65, 0},
	{    0,     5,    21, 0,   1718,    400,   -65,    76,    78, 0},
	{    3,     4,    25, 0,     86,    177,    88,    45,    78, 0},
	{   -9,     2,   -34, 0,   1485,   1237,  -115,    15,   -54, 0},
	{  -10,    14,   -17, 0,    990,   1237,   -85,    79,    50, 0},
	{  -11,    13,   -34, 0,    990,   1237,   -90,    89,   -11, 0},
	{    0,   -18,   -29, 0,   1324,    342,   -32,  -122,   -20, 0},
	{  -11,   -14,   -34, 0,   1315,    221,     4,  -106,   -71, 0},
	{    3,     4,    25, 0,     96,    369,    88,    45,    78, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gktorso_gktorsomesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(5,0,6,0,7,8,9,0),
	gsSP2Triangles(9,10,11,0,12,13,14,0),
	gsSP2Triangles(10,15,11,0,2,16,17,0),
	gsSP2Triangles(11,16,1,0,18,13,19,0),
	gsSP2Triangles(20,5,21,0,22,23,24,0),
	gsSP2Triangles(25,26,7,0,3,25,7,0),
	gsSP2Triangles(27,7,9,0,28,29,30,0),
	gsSP2Triangles(0,31,6,0,27,11,1,0),
	gsSP2Triangles(0,4,1,0,3,27,4,0),
	gsSP2Triangles(5,4,0,0,7,26,8,0),
	gsSP2Triangles(9,8,10,0,12,19,13,0),
	gsSP2Triangles(2,1,16,0,11,15,16,0),
	gsSP2Triangles(20,3,5,0,27,3,7,0),
	gsSP2Triangles(1,4,27,0,27,9,11,0),
	gsSPVertex(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0], 32, 0),
	gsSP2Triangles(0,1,2,0,2,3,4,0),
	gsSP2Triangles(5,6,7,0,8,9,6,0),
	gsSP2Triangles(10,11,12,0,13,9,14,0),
	gsSP2Triangles(15,16,1,0,16,14,1,0),
	gsSP2Triangles(11,17,12,0,4,18,19,0),
	gsSP2Triangles(20,21,22,0,23,7,24,0),
	gsSP2Triangles(3,23,25,0,26,5,3,0),
	gsSP2Triangles(27,28,29,0,29,18,24,0),
	gsSP2Triangles(30,11,31,0,14,8,26,0),
	gsSP2Triangles(0,15,1,0,2,26,3,0),
	gsSP2Triangles(0,2,4,0,5,8,6,0),
	gsSP2Triangles(8,14,9,0,16,13,14,0),
	gsSP2Triangles(11,30,17,0,4,3,18,0),
	gsSP2Triangles(23,5,7,0,3,5,23,0),
	gsSP2Triangles(26,8,5,0,18,23,24,0),
	gsSP2Triangles(29,28,18,0,26,2,1,0),
	gsSP1Triangle(1,14,26,0),
	gsSPVertex(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,9,0,14,15,16,0),
	gsSP2Triangles(16,17,14,0,18,19,20,0),
	gsSP2Triangles(21,1,0,0,22,23,24,0),
	gsSP2Triangles(25,7,6,0,26,15,14,0),
	gsSP2Triangles(14,27,28,0,9,29,10,0),
	gsSP2Triangles(29,30,16,0,16,15,26,0),
	gsSP2Triangles(29,16,10,0,10,16,26,0),
	gsSP2Triangles(19,5,20,0,5,8,20,0),
	gsSP1Triangle(8,7,31,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gktorso_gktorsomesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gktorso_gktorsomesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gktorso_gktorsomesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),5,0,6},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),7,8,9},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),9,10,11},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),12,13,14},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),10,15,11},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),2,16,17},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),11,16,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),18,13,19},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),20,5,21},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),22,23,24},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),25,26,7},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),3,25,7},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),27,7,9},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),28,29,30},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),0,31,6},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),27,11,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),0,4,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),3,27,4},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),5,4,0},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),7,26,8},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),9,8,10},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),12,19,13},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),2,1,16},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),11,15,16},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),20,3,5},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),27,3,7},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),1,4,27},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_0[0]),27,9,11},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),2,3,4},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),5,6,7},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),8,9,6},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),10,11,12},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),13,9,14},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),15,16,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),16,14,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),11,17,12},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),4,18,19},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),20,21,22},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),23,7,24},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),3,23,25},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),26,5,3},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),27,28,29},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),29,18,24},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),30,11,31},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),14,8,26},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),0,15,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),2,26,3},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),0,2,4},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),5,8,6},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),8,14,9},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),16,13,14},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),11,30,17},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),4,3,18},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),23,5,7},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),3,5,23},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),26,8,5},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),18,23,24},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),29,28,18},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),26,2,1},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_1[0]),1,14,26},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),9,10,11},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),12,13,9},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),14,15,16},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),16,17,14},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),18,19,20},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),21,1,0},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),22,23,24},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),25,7,6},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),26,15,14},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),14,27,28},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),9,29,10},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),29,30,16},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),16,15,26},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),29,16,10},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),10,16,26},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),19,5,20},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),5,8,20},
	{(Vtx*)(&Vtx_characterrig_gktorso_gktorsomesh_mesh01_2[0]),8,7,31}
};

MeshInfo MeshInfo_characterrig_gktorso_gktorsomesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gktorso_gktorsomesh[0],84,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[12] __attribute__((aligned (16))) = {
	{    3,     3,    -1, 0,     80,   1885,   -22,    89,    87, 0},
	{   -5,     4,   -10, 0,    271,   1748,   -74,   102,     8, 0},
	{   -4,     2,    -1, 0,    416,   1885,   -18,    86,    90, 0},
	{   11,    -6,   -10, 0,    198,   1705,   -57,   -88,   -73, 0},
	{   -7,    -2,   -10, 0,    262,   1702,   -75,    52,   -89, 0},
	{   -6,    -1,    -1, 0,    417,   1527,   -47,    71,    94, 0},
	{   16,     3,    -6, 0,    169,   1749,    40,    94,    74, 0},
	{   10,     7,   -10, 0,    203,   1760,   -50,   116,    -2, 0},
	{   -1,    -5,    -1, 0,     73,   1601,   -70,   -61,    87, 0},
	{   19,    -4,   -10, 0,    174,   1731,   124,    19,   -12, 0},
	{   16,    -4,    -6, 0,    162,   1720,   100,   -70,    35, 0},
	{   19,     4,   -10, 0,    178,   1738,    71,   -21,  -103, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0], 12, 0),
	gsSP2Triangles(0,1,2,0,1,3,4,0),
	gsSP2Triangles(1,5,2,0,6,7,0,0),
	gsSP2Triangles(8,5,4,0,9,6,10,0),
	gsSP2Triangles(11,3,7,0,10,0,8,0),
	gsSP2Triangles(3,10,8,0,8,2,5,0),
	gsSP2Triangles(8,4,3,0,0,7,1,0),
	gsSP2Triangles(1,7,3,0,1,4,5,0),
	gsSP2Triangles(6,11,7,0,9,11,6,0),
	gsSP2Triangles(11,9,3,0,10,6,0,0),
	gsSP2Triangles(3,9,10,0,8,0,2,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkfoot_l_gkfoot_lrmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkfoot_l_gkfoot_lrmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),1,3,4},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),1,5,2},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),6,7,0},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),8,5,4},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),9,6,10},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),11,3,7},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),10,0,8},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),3,10,8},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),8,2,5},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),8,4,3},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),0,7,1},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),1,7,3},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),1,4,5},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),6,11,7},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),9,11,6},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),11,9,3},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),10,6,0},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),3,9,10},
	{(Vtx*)(&Vtx_characterrig_gkfoot_l_gkfoot_lrmesh_mesh01_0[0]),8,0,2}
};

MeshInfo MeshInfo_characterrig_gkfoot_l_gkfoot_lrmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkfoot_l_gkfoot_lrmesh[0],20,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[28] __attribute__((aligned (16))) = {
	{   -3,    10,   -22, 0,    990,   1237,  -102,   -34,    68, 0},
	{    0,    -2,    -4, 0,    990,   1237,   -55,  -111,   -30, 0},
	{   -4,     0,    -2, 0,    990,   1237,  -121,    13,   -38, 0},
	{   -4,    10,   -24, 0,    293,    575,   -90,   -78,    46, 0},
	{    0,     8,   -24, 0,    136,    567,   -60,  -103,    46, 0},
	{   -3,    10,   -22, 0,    403,    407,  -102,   -34,    68, 0},
	{    0,    -2,    -4, 0,   1585,    373,   -55,  -111,   -30, 0},
	{    4,     9,   -21, 0,   1586,    373,   117,   -13,   -48, 0},
	{    4,     1,    -4, 0,   1586,    373,    99,   -78,     9, 0},
	{    1,    13,   -22, 0,   1586,    373,    -5,    65,   108, 0},
	{   -1,     3,     1, 0,   1587,    373,    62,    85,    70, 0},
	{   -4,     0,    -2, 0,   1586,    372,  -121,    13,   -38, 0},
	{   -3,    12,   -28, 0,    251,    637,  -101,   -34,   -71, 0},
	{   -1,     7,   -26, 0,    192,    604,   -83,   -93,    26, 0},
	{    6,     9,   -23, 0,    203,    701,    85,   -92,    20, 0},
	{    4,     9,   -21, 0,    144,    773,   117,   -13,   -48, 0},
	{    1,    13,   -22, 0,    397,    773,    -5,    65,   108, 0},
	{   -1,     8,   -29, 0,    235,    637,    46,   -61,  -102, 0},
	{    2,    15,   -26, 0,    257,    651,    97,    59,   -57, 0},
	{    4,     8,   -27, 0,    241,    656,    99,     1,   -79, 0},
	{    1,    14,   -23, 0,    302,    694,    63,    83,    71, 0},
	{    1,    -3,    -1, 0,   1162,    988,    -2,  -106,    70, 0},
	{    0,    -1,     2, 0,   1421,    612,   -30,   -64,   105, 0},
	{   -1,    -2,     1, 0,   1155,    997,   -26,  -116,    45, 0},
	{    2,    -2,     0, 0,   1415,    621,    48,  -109,    43, 0},
	{    0,     8,   -24, 0,    990,   1237,   -60,  -103,    46, 0},
	{    0,     8,   -24, 0,   1586,    373,   -60,  -103,    46, 0},
	{   -3,    10,   -22, 0,   1586,    373,  -102,   -34,    68, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0], 28, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,8,9,10,0),
	gsSP2Triangles(9,11,10,0,12,13,3,0),
	gsSP2Triangles(4,14,15,0,14,16,15,0),
	gsSP2Triangles(16,3,5,0,17,18,19,0),
	gsSP2Triangles(13,19,14,0,19,20,14,0),
	gsSP2Triangles(20,12,3,0,21,22,23,0),
	gsSP2Triangles(1,24,21,0,10,23,22,0),
	gsSP2Triangles(23,1,21,0,24,10,22,0),
	gsSP2Triangles(0,25,1,0,3,13,4,0),
	gsSP2Triangles(6,26,7,0,8,7,9,0),
	gsSP2Triangles(9,27,11,0,12,17,13,0),
	gsSP2Triangles(4,13,14,0,14,20,16,0),
	gsSP2Triangles(16,20,3,0,17,12,18,0),
	gsSP2Triangles(13,17,19,0,19,18,20,0),
	gsSP2Triangles(20,18,12,0,21,24,22,0),
	gsSP2Triangles(1,8,24,0,10,2,23,0),
	gsSP2Triangles(23,2,1,0,24,8,10,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkforearm_l_gkforearm_lrmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkforearm_l_gkforearm_lrmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),8,9,10},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),9,11,10},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),12,13,3},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),4,14,15},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),14,16,15},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),16,3,5},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),17,18,19},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),13,19,14},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),19,20,14},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),20,12,3},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),21,22,23},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),1,24,21},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),10,23,22},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),23,1,21},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),24,10,22},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),0,25,1},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),3,13,4},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),6,26,7},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),8,7,9},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),9,27,11},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),12,17,13},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),4,13,14},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),14,20,16},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),16,20,3},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),17,12,18},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),13,17,19},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),19,18,20},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),20,18,12},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),21,24,22},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),1,8,24},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),10,2,23},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),23,2,1},
	{(Vtx*)(&Vtx_characterrig_gkforearm_l_gkforearm_lrmesh_mesh01_0[0]),24,8,10}
};

MeshInfo MeshInfo_characterrig_gkforearm_l_gkforearm_lrmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkforearm_l_gkforearm_lrmesh[0],36,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[18] __attribute__((aligned (16))) = {
	{   -4,    -2,    -1, 0,    990,   1237,  -127,    -9,    -7, 0},
	{    0,    -5,   -35, 0,   1485,   1237,   -13,  -111,   -61, 0},
	{    1,    -8,    -2, 0,   1485,   1237,   -61,  -111,    16, 0},
	{    4,    -1,   -35, 0,    813,    343,    97,   -12,   -81, 0},
	{    4,     6,    -1, 0,    997,    267,    56,   111,    24, 0},
	{   11,    -3,    -1, 0,   1009,    374,   119,   -34,   -29, 0},
	{    0,     3,   -35, 0,    990,    742,    17,    98,   -79, 0},
	{   -4,    -2,    -1, 0,   1485,    742,  -127,    -9,    -7, 0},
	{    4,     6,    -1, 0,    990,    742,    56,   111,    24, 0},
	{    1,    -8,    -2, 0,   1000,    478,   -61,  -111,    16, 0},
	{    2,    -5,     3, 0,   1318,   1073,   -14,   -52,   115, 0},
	{    4,     4,     4, 0,   1018,    447,    10,    42,   119, 0},
	{   -1,    -2,     4, 0,   1075,   1076,   -41,    -7,   120, 0},
	{    8,    -2,     4, 0,   1074,    516,    50,   -14,   115, 0},
	{   -5,    -1,   -35, 0,    990,   1237,  -112,     0,   -62, 0},
	{    0,     3,   -35, 0,    845,    212,    17,    98,   -79, 0},
	{   -5,    -1,   -35, 0,   1485,    742,  -112,     0,   -62, 0},
	{    0,    -5,   -35, 0,    813,    445,   -13,  -111,   -61, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0], 18, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,3,5,0),
	gsSP2Triangles(10,11,12,0,11,0,12,0),
	gsSP2Triangles(0,10,12,0,13,4,11,0),
	gsSP2Triangles(2,13,10,0,0,14,1,0),
	gsSP2Triangles(3,15,4,0,6,16,7,0),
	gsSP2Triangles(9,17,3,0,10,13,11,0),
	gsSP2Triangles(11,4,0,0,0,2,10,0),
	gsSP2Triangles(13,5,4,0,2,5,13,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkshin_l_gkshin_lmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkshin_l_gkshin_lmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),9,3,5},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),10,11,12},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),11,0,12},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),0,10,12},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),13,4,11},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),2,13,10},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),0,14,1},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),3,15,4},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),6,16,7},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),9,17,3},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),10,13,11},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),11,4,0},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),0,2,10},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),13,5,4},
	{(Vtx*)(&Vtx_characterrig_gkshin_l_gkshin_lmesh_mesh01_0[0]),2,5,13}
};

MeshInfo MeshInfo_characterrig_gkshin_l_gkshin_lmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkshin_l_gkshin_lmesh[0],18,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[31] __attribute__((aligned (16))) = {
	{    9,     0,   -31, 0,   1012,    375,   124,   -16,   -20, 0},
	{   -2,    10,   -19, 0,   1178,    257,    37,   120,    16, 0},
	{    8,     2,   -14, 0,   1193,    388,   113,    22,   -52, 0},
	{   -6,     2,   -30, 0,   1485,    742,  -125,     1,    26, 0},
	{   -2,    10,   -19, 0,    990,   1237,    37,   120,    16, 0},
	{    2,    10,   -31, 0,    990,    742,   -31,   102,   -69, 0},
	{   -1,    -3,   -31, 0,   1000,    478,   -28,  -120,   -33, 0},
	{    9,     0,   -31, 0,   1009,    374,   124,   -16,   -20, 0},
	{    0,    -9,    -9, 0,   1222,    527,    -3,  -102,   -77, 0},
	{   -1,    -3,   -31, 0,   1485,   1237,   -28,  -120,   -33, 0},
	{  -10,    -1,   -14, 0,    990,   1732,   -93,   -17,   -86, 0},
	{   -6,     2,   -30, 0,    990,   1237,  -125,     1,    26, 0},
	{    8,     6,     5, 0,   1334,    412,   112,    43,    40, 0},
	{    8,    -9,    -1, 0,   1351,    538,   100,   -68,   -39, 0},
	{  -10,    -1,   -14, 0,   1485,   1237,   -93,   -17,   -86, 0},
	{  -12,     5,    -1, 0,    990,   1237,   -98,    81,   -10, 0},
	{  -12,     5,    -1, 0,   1315,    221,   -98,    81,   -10, 0},
	{   -1,     9,     3, 0,   1324,    342,     5,   126,    11, 0},
	{   -1,     9,     3, 0,   1325,    350,     5,   126,    11, 0},
	{    0,    -9,    -9, 0,   1485,   1732,    -3,  -102,   -77, 0},
	{  -11,    -6,    -1, 0,   1485,   1237,   -88,   -77,   -52, 0},
	{    1,     7,   -36, 0,   1021,    468,     9,   -32,  -123, 0},
	{   -1,    -1,   -36, 0,   1300,   1054,   -35,    34,  -118, 0},
	{   -4,     2,   -36, 0,   1084,   1057,    17,    35,  -121, 0},
	{  -11,     0,     7, 0,   1366,    546,  -100,     1,    78, 0},
	{  -10,    -7,     7, 0,   1448,   1038,   -62,   -32,   106, 0},
	{   -3,     3,    10, 0,   1372,    631,   -63,    32,   105, 0},
	{    2,    -8,     7, 0,   1367,    612,    14,   -59,   111, 0},
	{    2,     1,    11, 0,   1344,    468,    -2,     8,   126, 0},
	{    5,     1,   -36, 0,   1081,    533,    84,     7,   -95, 0},
	{    2,    10,   -31, 0,    997,    267,   -31,   102,   -69, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0], 31, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(2,12,13,0,14,15,4,0),
	gsSP2Triangles(1,16,17,0,0,2,8,0),
	gsSP2Triangles(18,2,1,0,19,20,14,0),
	gsSP2Triangles(13,8,2,0,19,13,20,0),
	gsSP2Triangles(21,22,23,0,24,25,26,0),
	gsSP2Triangles(26,27,28,0,28,17,26,0),
	gsSP2Triangles(27,12,28,0,24,20,25,0),
	gsSP2Triangles(26,16,24,0,20,27,25,0),
	gsSP2Triangles(11,21,23,0,9,23,22,0),
	gsSP2Triangles(21,7,29,0,29,9,22,0),
	gsSP2Triangles(0,30,1,0,3,14,4,0),
	gsSP2Triangles(9,19,10,0,14,20,15,0),
	gsSP2Triangles(18,12,2,0,21,29,22,0),
	gsSP2Triangles(26,25,27,0,28,12,17,0),
	gsSP2Triangles(27,13,12,0,24,16,20,0),
	gsSP2Triangles(26,17,16,0,20,13,27,0),
	gsSP2Triangles(11,30,21,0,9,11,23,0),
	gsSP2Triangles(21,30,7,0,29,7,9,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkthigh_l_gkthigh_lmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkthigh_l_gkthigh_lmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),9,10,11},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),2,12,13},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),14,15,4},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),1,16,17},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),0,2,8},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),18,2,1},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),19,20,14},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),13,8,2},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),19,13,20},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),21,22,23},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),24,25,26},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),26,27,28},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),28,17,26},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),27,12,28},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),24,20,25},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),26,16,24},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),20,27,25},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),11,21,23},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),9,23,22},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),21,7,29},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),29,9,22},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),0,30,1},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),3,14,4},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),9,19,10},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),14,20,15},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),18,12,2},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),21,29,22},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),26,25,27},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),28,12,17},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),27,13,12},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),24,16,20},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),26,17,16},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),20,13,27},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),11,30,21},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),9,11,23},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),21,30,7},
	{(Vtx*)(&Vtx_characterrig_gkthigh_l_gkthigh_lmesh_mesh01_0[0]),29,7,9}
};

MeshInfo MeshInfo_characterrig_gkthigh_l_gkthigh_lmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkthigh_l_gkthigh_lmesh[0],40,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[22] __attribute__((aligned (16))) = {
	{    0,    -4,    -8, 0,   1550,    373,    37,    89,   -82, 0},
	{    4,   -17,   -20, 0,   1586,    373,    84,    60,   -74, 0},
	{    0,   -14,   -20, 0,   1585,    373,   -13,   107,   -67, 0},
	{    6,    -3,    -4, 0,   1555,    405,   115,   -31,   -43, 0},
	{   -1,   -19,   -15, 0,   1587,    373,    44,  -118,    15, 0},
	{   -6,    -2,    -7, 0,    990,   1237,   -72,    56,   -89, 0},
	{   -4,   -17,   -18, 0,    990,   1237,  -127,     8,     1, 0},
	{   -6,    -2,    -4, 0,    990,   1237,  -113,    29,   -52, 0},
	{   -6,    -2,    -4, 0,   1624,    348,  -113,    29,   -52, 0},
	{    0,    -6,     0, 0,   1614,    411,   -61,   -43,   102, 0},
	{   -2,     1,    -4, 0,   1596,    371,   -18,   114,   -52, 0},
	{    3,     0,    -1, 0,   1555,    405,    40,    64,   101, 0},
	{    0,    -1,     1, 0,   1614,    411,     4,   -28,   123, 0},
	{    0,   -17,   -22, 0,   1141,   1018,   -37,   -47,  -113, 0},
	{    0,   -20,   -19, 0,   1441,    583,   -51,  -104,   -55, 0},
	{   -2,   -18,   -21, 0,   1135,   1027,   -46,   -33,  -115, 0},
	{    2,   -18,   -22, 0,   1435,    592,     4,   -26,  -125, 0},
	{    0,   -14,   -20, 0,    990,   1237,   -13,   107,   -67, 0},
	{   -1,     1,    -2, 0,   1596,    371,   -17,   112,    56, 0},
	{   -2,     0,    -1, 0,   1624,    348,   -18,    61,   109, 0},
	{   -6,    -2,    -7, 0,   1574,    326,   -72,    56,   -89, 0},
	{   -4,   -17,   -18, 0,   1586,    372,  -127,     8,     1, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0], 22, 0),
	gsSP2Triangles(0,1,2,0,3,4,1,0),
	gsSP2Triangles(5,6,7,0,8,4,9,0),
	gsSP2Triangles(10,5,8,0,0,10,3,0),
	gsSP2Triangles(9,11,12,0,13,14,15,0),
	gsSP2Triangles(13,1,16,0,15,4,6,0),
	gsSP2Triangles(15,17,13,0,14,1,4,0),
	gsSP2Triangles(12,18,19,0,10,19,18,0),
	gsSP2Triangles(9,19,8,0,10,11,3,0),
	gsSP2Triangles(2,20,0,0,0,3,1,0),
	gsSP2Triangles(3,9,4,0,5,17,6,0),
	gsSP2Triangles(8,21,4,0,10,0,5,0),
	gsSP2Triangles(9,3,11,0,13,16,14,0),
	gsSP2Triangles(13,17,1,0,15,14,4,0),
	gsSP2Triangles(15,6,17,0,14,16,1,0),
	gsSP2Triangles(12,11,18,0,10,8,19,0),
	gsSP2Triangles(9,12,19,0,10,18,11,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkbicep_r_gkbicep_rmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkbicep_r_gkbicep_rmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),3,4,1},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),5,6,7},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),8,4,9},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),10,5,8},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),0,10,3},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),9,11,12},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),13,14,15},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),13,1,16},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),15,4,6},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),15,17,13},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),14,1,4},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),12,18,19},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),10,19,18},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),9,19,8},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),10,11,3},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),2,20,0},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),0,3,1},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),3,9,4},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),5,17,6},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),8,21,4},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),10,0,5},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),9,3,11},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),13,16,14},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),13,17,1},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),15,14,4},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),15,6,17},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),14,16,1},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),12,11,18},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),10,8,19},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),9,12,19},
	{(Vtx*)(&Vtx_characterrig_gkbicep_r_gkbicep_rmesh_mesh01_0[0]),10,18,11}
};

MeshInfo MeshInfo_characterrig_gkbicep_r_gkbicep_rmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkbicep_r_gkbicep_rmesh[0],32,&Text_characterrig_person_diff[0]};


Vtx_tn Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[22] __attribute__((aligned (16))) = {
	{    3,    15,   -20, 0,   1586,    373,   118,   -44,    12, 0},
	{    5,     3,    -5, 0,   1555,    405,    98,   -20,   -78, 0},
	{   -1,     4,    -8, 0,   1550,    373,    37,   -88,   -85, 0},
	{   -2,    18,   -15, 0,   1587,    373,     4,    64,   109, 0},
	{   -1,     5,     0, 0,   1614,    411,    59,    63,    92, 0},
	{   -5,    17,   -18, 0,    990,   1237,  -126,   -13,    13, 0},
	{   -7,     2,    -8, 0,    990,   1237,   -84,   -86,   -44, 0},
	{   -7,     1,    -4, 0,    990,   1237,  -122,    30,   -24, 0},
	{   -7,     1,    -4, 0,   1624,    348,  -122,    30,   -24, 0},
	{   -2,    -2,    -4, 0,   1596,    371,   -21,  -106,   -68, 0},
	{    2,     0,    -1, 0,   1555,    405,    52,  -111,    31, 0},
	{   -1,    16,   -22, 0,   1105,   1069,   -38,    54,  -109, 0},
	{   -2,    19,   -18, 0,   1459,    557,    57,   106,   -39, 0},
	{    1,    17,   -21, 0,   1469,    542,     7,    45,  -119, 0},
	{   -2,    12,   -20, 0,    990,   1237,   -20,   -33,  -122, 0},
	{   -4,    18,   -19, 0,   1116,   1053,  -120,    35,   -25, 0},
	{   -1,     1,     1, 0,   1614,    411,    -6,   -69,   106, 0},
	{   -1,    -2,    -1, 0,   1596,    371,    62,  -100,    49, 0},
	{   -3,    -1,     0, 0,   1624,    348,  -103,   -56,    49, 0},
	{   -7,     2,    -8, 0,   1574,    326,   -84,   -86,   -44, 0},
	{   -2,    12,   -20, 0,   1585,    373,   -20,   -33,  -122, 0},
	{   -5,    17,   -18, 0,   1586,    372,  -126,   -13,    13, 0},
};

#ifdef __N64__
Gfx Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0], 22, 0),
	gsSP2Triangles(0,1,2,0,1,3,4,0),
	gsSP2Triangles(5,6,7,0,3,8,4,0),
	gsSP2Triangles(9,6,2,0,2,1,9,0),
	gsSP2Triangles(10,9,1,0,11,12,13,0),
	gsSP2Triangles(13,14,11,0,5,12,15,0),
	gsSP2Triangles(11,5,15,0,3,13,12,0),
	gsSP2Triangles(16,17,10,0,4,10,1,0),
	gsSP2Triangles(9,18,8,0,18,4,8,0),
	gsSP2Triangles(2,19,20,0,20,0,2,0),
	gsSP2Triangles(1,0,3,0,5,14,6,0),
	gsSP2Triangles(3,21,8,0,9,8,6,0),
	gsSP2Triangles(10,17,9,0,11,15,12,0),
	gsSP2Triangles(13,0,14,0,5,3,12,0),
	gsSP2Triangles(11,14,5,0,3,0,13,0),
	gsSP2Triangles(16,18,17,0,4,16,10,0),
	gsSP2Triangles(9,17,18,0,18,16,4,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_characterrig_gkbicep_l_gkbicep_lrmesh[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_characterrig_person_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_characterrig_gkbicep_l_gkbicep_lrmesh[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),1,3,4},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),5,6,7},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),3,8,4},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),9,6,2},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),2,1,9},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),10,9,1},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),11,12,13},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),13,14,11},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),5,12,15},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),11,5,15},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),3,13,12},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),16,17,10},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),4,10,1},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),9,18,8},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),18,4,8},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),2,19,20},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),20,0,2},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),1,0,3},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),5,14,6},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),3,21,8},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),9,8,6},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),10,17,9},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),11,15,12},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),13,0,14},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),5,3,12},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),11,14,5},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),3,0,13},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),16,18,17},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),4,16,10},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),9,17,18},
	{(Vtx*)(&Vtx_characterrig_gkbicep_l_gkbicep_lrmesh_mesh01_0[0]),18,16,4}
};

MeshInfo MeshInfo_characterrig_gkbicep_l_gkbicep_lrmesh[] __attribute__((aligned (16))) = {&Tris_characterrig_gkbicep_l_gkbicep_lrmesh[0],32,&Text_characterrig_person_diff[0]};


#define CHARACTERRIG_MODEL_MESH_COUNT 12

MeshInfo* characterrig_meshinfos[12] = {
    MeshInfo_characterrig_gkfoot_r_gkfoot_rmesh,
    MeshInfo_characterrig_gkforearm_r_gkforearm_rmesh,
    MeshInfo_characterrig_gkhead_gkheadmesh,
    MeshInfo_characterrig_gkshin_r_gkshin_rmesh,
    MeshInfo_characterrig_gkthigh_r_gkthigh_rmesh,
    MeshInfo_characterrig_gktorso_gktorsomesh,
    MeshInfo_characterrig_gkfoot_l_gkfoot_lrmesh,
    MeshInfo_characterrig_gkforearm_l_gkforearm_lrmesh,
    MeshInfo_characterrig_gkshin_l_gkshin_lmesh,
    MeshInfo_characterrig_gkthigh_l_gkthigh_lmesh,
    MeshInfo_characterrig_gkbicep_r_gkbicep_rmesh,
    MeshInfo_characterrig_gkbicep_l_gkbicep_lrmesh,
};
#ifdef __N64__
Gfx* characterrig_model_meshes[12] __attribute__((aligned (16))) = {
    Wtx_characterrig_gkfoot_r_gkfoot_rmesh,
    Wtx_characterrig_gkforearm_r_gkforearm_rmesh,
    Wtx_characterrig_gkhead_gkheadmesh,
    Wtx_characterrig_gkshin_r_gkshin_rmesh,
    Wtx_characterrig_gkthigh_r_gkthigh_rmesh,
    Wtx_characterrig_gktorso_gktorsomesh,
    Wtx_characterrig_gkfoot_l_gkfoot_lrmesh,
    Wtx_characterrig_gkforearm_l_gkforearm_lrmesh,
    Wtx_characterrig_gkshin_l_gkshin_lmesh,
    Wtx_characterrig_gkthigh_l_gkthigh_lmesh,
    Wtx_characterrig_gkbicep_r_gkbicep_rmesh,
    Wtx_characterrig_gkbicep_l_gkbicep_lrmesh,
};
#endif
