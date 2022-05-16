#include "mesh.h"

/*
Name: bush
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

unsigned short Text_bush_bush_diff[] __attribute__((aligned (16))) = {
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x3b51,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
	0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,0x4b95,
};

Vtx_tn Vtx_bush_mesh01_0[32] __attribute__((aligned (16))) = {
	{   13,    13,    46, 0,    689,   1899,    58,    45,   103, 0},
	{   38,    19,    23, 0,    689,    993,    84,    53,    78, 0},
	{    7,    43,    29, 0,   1545,   1648,    55,    80,    81, 0},
	{  -35,    18,    27, 0,    693,     86,   -83,    47,    83, 0},
	{  -13,    22,    41, 0,    693,    992,   -60,    54,    98, 0},
	{  -26,    43,    15, 0,   1592,    281,   -69,    84,    65, 0},
	{  -35,    18,   -28, 0,    689,     81,  -112,    37,   -49, 0},
	{  -46,    17,     9, 0,    677,    994,  -120,    42,     5, 0},
	{  -26,    42,   -22, 0,   1576,    284,   -98,    75,   -32, 0},
	{   13,    18,   -46, 0,    689,   1892,     2,    54,  -115, 0},
	{  -13,    22,   -42, 0,    689,    986,   -20,    57,  -112, 0},
	{   10,    43,   -29, 0,   1545,   1642,     2,    87,   -93, 0},
	{   48,    18,    -7, 0,    701,     82,   107,    60,   -31, 0},
	{   32,    22,   -33, 0,    701,    988,    82,    66,   -70, 0},
	{   28,    44,     1, 0,   1600,    277,    82,    93,   -25, 0},
	{   10,    43,   -29, 0,   1557,   1644,    38,   109,   -52, 0},
	{    0,    53,     0, 0,   1953,    988,    28,   123,   -12, 0},
	{  -26,    42,   -22, 0,   1588,    276,   -36,   108,   -56, 0},
	{    0,    53,     0, 0,   1940,    986,   -13,   124,   -24, 0},
	{  -26,    43,    15, 0,   1533,   1650,   -58,   110,    25, 0},
	{    0,    53,     0, 0,   1928,    994,   -36,   121,    -4, 0},
	{  -35,    18,    27, 0,    677,   1900,   -95,    45,    71, 0},
	{    7,    43,    29, 0,   1549,   1647,    -9,   109,    64, 0},
	{    0,    53,     0, 0,   1945,    992,   -15,   123,    23, 0},
	{   28,    44,     1, 0,   1588,    282,    52,   114,    15, 0},
	{    0,    53,     0, 0,   1940,    993,    25,   123,    16, 0},
	{   41,    -3,   -18, 0,     75,    535,   103,   -42,   -61, 0},
	{   25,    -5,   -39, 0,     75,   1441,    89,   -37,   -83, 0},
	{    0,    -2,   -48, 0,     63,   1439,   -16,   -15,  -126, 0},
	{  -26,    -2,   -39, 0,     63,    533,   -55,   -24,  -113, 0},
	{  -42,    -2,   -15, 0,     51,    541,  -121,   -19,   -36, 0},
	{  -42,    -2,    14, 0,     51,   1447,  -116,   -48,    24, 0},
};Vtx_tn Vtx_bush_mesh01_1[28] __attribute__((aligned (16))) = {
	{  -26,    -4,    38, 0,     67,    539,   -75,   -11,   102, 0},
	{  -13,    22,    41, 0,    693,    992,   -60,    54,    98, 0},
	{  -35,    18,    27, 0,    693,     86,   -83,    47,    83, 0},
	{    0,     2,    47, 0,     67,   1445,   -28,   -17,   122, 0},
	{   13,    13,    46, 0,    689,   1899,    58,    45,   103, 0},
	{   25,    -3,    38, 0,     63,   1446,    81,   -19,    95, 0},
	{   38,    19,    23, 0,    689,    993,    84,    53,    78, 0},
	{   41,    -2,    14, 0,     63,    540,   111,   -42,    43, 0},
	{   48,    18,    -7, 0,    701,     82,   107,    60,   -31, 0},
	{  -42,    -2,    14, 0,     51,   1447,  -116,   -48,    24, 0},
	{  -26,    -4,    38, 0,     55,   1443,   -93,   -29,    81, 0},
	{  -35,    18,    27, 0,    677,   1900,   -95,    45,    71, 0},
	{   41,    -3,   -18, 0,     75,    535,   103,   -42,   -61, 0},
	{   31,   -13,    12, 0,     63,    540,    83,   -89,    36, 0},
	{    0,   -13,    36, 0,     67,   1445,   -11,   -73,   103, 0},
	{  -32,   -13,    12, 0,     51,   1447,   -85,   -87,    39, 0},
	{  -20,   -13,    29, 0,     55,   1443,   -66,   -93,    57, 0},
	{  -32,   -13,   -10, 0,     51,    541,   -98,   -81,   -12, 0},
	{  -26,    -2,   -39, 0,     63,    533,   -55,   -24,  -113, 0},
	{    0,   -13,   -35, 0,     63,   1439,   -24,   -89,   -89, 0},
	{  -20,   -13,   -28, 0,     63,    533,   -38,   -94,   -78, 0},
	{   19,   -13,   -28, 0,     75,   1441,    58,  -105,   -44, 0},
	{   25,    -5,   -39, 0,     75,   1441,    89,   -37,   -83, 0},
	{   19,   -13,    29, 0,     63,   1446,    39,   -91,    79, 0},
	{    0,    -2,   -48, 0,     63,   1439,   -16,   -15,  -126, 0},
	{   31,   -13,   -13, 0,     75,    535,    86,   -93,   -12, 0},
	{  -42,    -2,   -15, 0,     51,    541,  -121,   -19,   -36, 0},
	{  -20,   -13,    29, 0,     67,    539,   -39,   -95,    76, 0},
};

#ifdef __N64__
Gfx Vtx_bush_mesh01_dl[] __attribute__((aligned (16))) = {
	gsSPVertex(&Vtx_bush_mesh01_0[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,14,15,16,0),
	gsSP2Triangles(14,13,15,0,13,9,15,0),
	gsSP2Triangles(11,17,18,0,11,10,17,0),
	gsSP2Triangles(10,6,17,0,8,19,20,0),
	gsSP2Triangles(8,7,19,0,7,21,19,0),
	gsSP2Triangles(5,22,23,0,5,4,22,0),
	gsSP2Triangles(4,0,22,0,2,24,25,0),
	gsSP2Triangles(2,1,24,0,1,12,24,0),
	gsSP2Triangles(26,13,12,0,26,27,13,0),
	gsSP2Triangles(27,9,13,0,28,10,9,0),
	gsSP2Triangles(28,29,10,0,29,6,10,0),
	gsSP2Triangles(30,7,6,0,30,31,7,0),
	gsSP2Triangles(31,21,7,0,27,28,9,0),
	gsSP1Triangle(29,30,6,0),
	gsSPVertex(&Vtx_bush_mesh01_1[0], 28, 0),
	gsSP2Triangles(0,1,2,0,0,3,1,0),
	gsSP2Triangles(3,4,1,0,5,6,4,0),
	gsSP2Triangles(5,7,6,0,7,8,6,0),
	gsSP2Triangles(9,10,11,0,3,5,4,0),
	gsSP2Triangles(7,12,8,0,5,13,7,0),
	gsSP2Triangles(0,14,3,0,10,15,16,0),
	gsSP2Triangles(9,17,15,0,18,19,20,0),
	gsSP2Triangles(12,21,22,0,5,14,23,0),
	gsSP2Triangles(22,19,24,0,7,25,12,0),
	gsSP2Triangles(26,20,17,0,5,23,13,0),
	gsSP2Triangles(0,27,14,0,10,9,15,0),
	gsSP2Triangles(9,26,17,0,18,24,19,0),
	gsSP2Triangles(12,25,21,0,5,3,14,0),
	gsSP2Triangles(22,21,19,0,7,13,25,0),
	gsSP1Triangle(26,18,20,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_bush[] __attribute__((aligned (16))) = {
	  gsDPLoadTextureBlock(Text_bush_bush_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_bush_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_bush[] __attribute__((aligned (16))) = {
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),9,10,11},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),12,13,14},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),14,15,16},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),14,13,15},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),13,9,15},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),11,17,18},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),11,10,17},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),10,6,17},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),8,19,20},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),8,7,19},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),7,21,19},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),5,22,23},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),5,4,22},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),4,0,22},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),2,24,25},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),2,1,24},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),1,12,24},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),26,13,12},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),26,27,13},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),27,9,13},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),28,10,9},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),28,29,10},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),29,6,10},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),30,7,6},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),30,31,7},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),31,21,7},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),27,28,9},
	{(Vtx*)(&Vtx_bush_mesh01_0[0]),29,30,6},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),0,1,2},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),0,3,1},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),3,4,1},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),5,6,4},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),5,7,6},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),7,8,6},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),9,10,11},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),3,5,4},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),7,12,8},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),5,13,7},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),0,14,3},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),10,15,16},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),9,17,15},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),18,19,20},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),12,21,22},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),5,14,23},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),22,19,24},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),7,25,12},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),26,20,17},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),5,23,13},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),0,27,14},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),10,9,15},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),9,26,17},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),18,24,19},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),12,25,21},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),5,3,14},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),22,21,19},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),7,13,25},
	{(Vtx*)(&Vtx_bush_mesh01_1[0]),26,18,20}
};

MeshInfo MeshInfo_bush[] __attribute__((aligned (16))) = {&Tris_bush[0],60,&Text_bush_bush_diff[0]};


#define BUSH_MODEL_MESH_COUNT 1

MeshInfo* bush_meshinfos[1] = {
    MeshInfo_bush,
};
#ifdef __N64__
Gfx* bush_model_meshes[1] __attribute__((aligned (16))) = {
    Wtx_bush,
};
#endif
