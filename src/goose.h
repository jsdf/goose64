#include "mesh.h"

/*
Name: goose
Texture preview:
	%%%%%%%%%%%@@@@@@@@@@@@@@@@@@@@@
	%%%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@
	%%%%%@@@@@@@@@@@@@@@@@@@@@@@@@
	%%@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@@@@@@@@@@@@@@@@@@@@@@@@%###%%%%
	@@@@@@@@@@@@@@@@@@@@@@@%*====++*
	@@@@@@@@@@@@@@@@@@@@@@@#========
	@@@@@@@@@@@@@@@@@@@@@@@=========
	%%%%%%%@@@@@@@@@@@@@@@@*========
	%%%%%%%%%%@@@@@@@@@@@@@#========
	%%%%%%%%%%%%%%%@@@@@@@@@#*##**##
	%%%%%%%%%%%%%%%%@@@@@@@@@@@@@@@@
	%%%%%%%%%%%%%%%%%%@%@@@@@@@@@@@@
	%%%%%%%%%%%%%%%%%%%%@@@@@@@@@@@@
	%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@@
	%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@@
	%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@
	%%%%%%%%%%%%%%%%%%%%%%%%%@@@@@@@
*/

unsigned short Text_goose_goosetex_diff[] __attribute__((aligned (8))) = {
	0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0001,0x0001,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0x0001,0x0001,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfffd,0xff33,0xfeaf,0xfef1,0xff77,0xffb9,0xff79,0xfffd,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffbd,0xfd5b,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc91,0xfc91,0xfd9f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfead,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,
	0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfd9f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,
	0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfef3,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,0xfc4f,
	0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xfef3,0xfde3,0xfead,0xfeef,0xfda1,0xfde3,0xfe6b,0xff33,
	0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xfffb,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xfffb,0xfffb,0xfffb,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbd,0xffbd,0xfffd,0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbd,0xffbd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
	0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbb,0xffbd,0xfffd,0xfffd,0xfffd,0xfffd,0xfffd,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,
};

Vtx_tn Vtx_goose_mesh01_0[32] __attribute__((aligned (8))) = {
	{  -15,     2,     0, 0,   1070,    994,    37,  -122,     0, 0},
	{  -38,     4,     8, 0,   1342,    883,   -25,   -69,   104, 0},
	{  -58,    -2,     0, 0,   1572,   1004,  -102,   -77,     0, 0},
	{  -20,    27,    29, 0,    584,    358,   -98,    74,    33, 0},
	{    8,    43,    39, 0,   1045,    191,    72,    39,    96, 0},
	{    2,    47,    12, 0,   1027,    570,   -57,   101,    50, 0},
	{   72,    73,     2, 0,    193,   1517,    47,    69,    95, 0},
	{   64,    74,     4, 0,    287,   1481,    38,   -55,   107, 0},
	{   73,    70,     3, 0,    198,   1556,    -1,   -80,    99, 0},
	{   64,    74,     0, 0,    287,   1481,    24,  -125,     0, 0},
	{   73,    70,     0, 0,   1005,   1265,   -27,  -125,     0, 0},
	{   64,    74,     4, 0,   1419,   1965,    38,   -55,   107, 0},
	{   59,    68,     0, 0,   1332,   1891,    83,   -96,     0, 0},
	{  -55,     3,     7, 0,   1505,    912,   -83,     3,    96, 0},
	{   -4,    14,    14, 0,    943,    813,    56,   -83,    77, 0},
	{  -22,     6,     9, 0,   1081,    866,    11,   -90,    89, 0},
	{  -27,    32,     0, 0,    634,    771,   -85,    94,     0, 0},
	{  -30,    16,    14, 0,    464,    503,   -68,     5,   107, 0},
	{   11,    17,     0, 0,    810,   1635,   108,   -67,     0, 0},
	{   18,    50,     0, 0,   1216,   1721,    82,   -97,     0, 0},
	{   11,    49,    10, 0,   1173,   1827,    74,     5,   102, 0},
	{   -4,    14,    14, 0,    458,   1306,    56,   -83,    77, 0},
	{   11,    49,    10, 0,     31,   1364,    74,     5,   102, 0},
	{   11,    46,    24, 0,     54,   1171,   104,    58,    40, 0},
	{   11,    58,     8, 0,   1196,    634,   -39,    76,    93, 0},
	{   18,    66,     0, 0,   1335,    745,   -41,   120,     0, 0},
	{    7,    59,     0, 0,   1172,    755,   -77,   101,     0, 0},
	{   -3,    50,     0, 0,   1008,    758,   -80,    98,     0, 0},
	{   18,    50,     0, 0,    811,   1842,    82,   -97,     0, 0},
	{   55,    65,     0, 0,   1273,   1887,    68,  -107,     0, 0},
	{   55,    67,     6, 0,   1319,   1974,    50,   -75,    89, 0},
	{  -49,    11,     0, 0,    252,    701,   -84,    95,     0, 0},
};Vtx_tn Vtx_goose_mesh01_1[32] __attribute__((aligned (8))) = {
	{   11,    58,     8, 0,    977,   1506,   -39,    76,    93, 0},
	{   11,    49,    10, 0,   1008,   1617,    74,     5,   102, 0},
	{   55,    67,     6, 0,    377,   1527,    50,   -75,    89, 0},
	{   11,    58,     8, 0,   1196,    634,   -39,    76,    93, 0},
	{   54,    71,     6, 0,   1728,    570,   -14,    37,   120, 0},
	{   49,    73,     0, 0,   1703,    679,   -53,   115,     0, 0},
	{   73,    70,     3, 0,   1595,   1251,    -1,   -80,    99, 0},
	{   85,    69,     0, 0,   1949,   1406,   124,   -26,     0, 0},
	{   84,    69,     0, 0,   1931,   1406,    57,   113,     0, 0},
	{  -16,    23,    26, 0,    498,   1155,    78,   -98,    17, 0},
	{   -9,    28,    47, 0,    352,    857,    24,    15,   123, 0},
	{  -16,     6,    49, 0,    646,    833,     2,   -94,    86, 0},
	{  -49,    11,     0, 0,    252,    701,   -84,    95,     0, 0},
	{  -58,    -2,     0, 0,     81,    753,  -102,   -77,     0, 0},
	{  -55,     3,     7, 0,    137,    670,   -83,     3,    96, 0},
	{  -38,     4,     8, 0,    451,   1802,   -25,   -69,   104, 0},
	{  -30,    16,    14, 0,    345,   1643,   -68,     5,   107, 0},
	{  -49,    11,     0, 0,    588,   1710,   -84,    95,     0, 0},
	{  -20,    27,    29, 0,    584,    358,   -98,    74,    33, 0},
	{   -9,    28,    47, 0,    664,    123,    24,    15,   123, 0},
	{    8,    43,    39, 0,   1045,    191,    72,    39,    96, 0},
	{  -30,    16,    14, 0,    464,    503,   -68,     5,   107, 0},
	{  -16,     6,    49, 0,    495,     33,     2,   -94,    86, 0},
	{   54,    71,     6, 0,    417,   1481,   -14,    37,   120, 0},
	{   64,    74,     4, 0,    287,   1481,    38,   -55,   107, 0},
	{   62,    79,     4, 0,   1866,    602,     5,    82,    96, 0},
	{   59,    81,     0, 0,   1859,    676,   -47,   118,     0, 0},
	{   62,    79,     4, 0,    883,   1328,     5,    82,    96, 0},
	{   72,    73,     2, 0,    763,   1328,    47,    69,    95, 0},
	{   71,    75,     0, 0,    784,   1283,    63,   109,     0, 0},
	{   73,    70,     0, 0,   1682,   1185,   -27,  -125,     0, 0},
	{   63,    81,     0, 0,   1898,    663,    52,   115,     0, 0},
};Vtx_tn Vtx_goose_mesh01_2[32] __attribute__((aligned (8))) = {
	{   84,    69,     0, 0,   1931,   1406,    57,   113,     0, 0},
	{   71,    75,     0, 0,   1499,   1406,    63,   109,     0, 0},
	{   72,    73,     2, 0,   1546,   1313,    47,    69,    95, 0},
	{  -30,    16,    14, 0,   1418,   1519,   -68,     5,   107, 0},
	{   -4,    14,    14, 0,   1179,   1519,    56,   -83,    77, 0},
	{  -16,    23,    26, 0,   1346,   1394,    78,   -98,    17, 0},
	{  -30,    16,    14, 0,    345,   1643,   -68,     5,   107, 0},
	{  -22,     6,     9, 0,    123,   1802,    11,   -90,    89, 0},
	{   -4,    14,    14, 0,     28,   1691,    56,   -83,    77, 0},
	{   -4,    14,    14, 0,    943,    813,    56,   -83,    77, 0},
	{  -15,     2,     0, 0,   1070,    994,    37,  -122,     0, 0},
	{   11,    17,     0, 0,    787,   1004,   108,   -67,     0, 0},
	{   11,    49,    10, 0,   1008,   1617,    74,     5,   102, 0},
	{   11,    58,     8, 0,    977,   1506,   -39,    76,    93, 0},
	{    2,    47,    12, 0,   1120,   1609,   -57,   101,    50, 0},
	{   11,    17,     0, 0,    810,   1635,   108,   -67,     0, 0},
	{   11,    49,    10, 0,   1173,   1827,    74,     5,   102, 0},
	{   -4,    14,    14, 0,    714,   1827,    56,   -83,    77, 0},
	{   -4,    14,    14, 0,    458,   1306,    56,   -83,    77, 0},
	{   11,    46,    24, 0,     54,   1171,   104,    58,    40, 0},
	{  -16,    23,    26, 0,    498,   1155,    78,   -98,    17, 0},
	{   18,    50,     0, 0,    811,   1842,    82,   -97,     0, 0},
	{   55,    67,     6, 0,   1319,   1974,    50,   -75,    89, 0},
	{   11,    49,    10, 0,    722,   1974,    74,     5,   102, 0},
	{   11,    58,     8, 0,   1196,    634,   -39,    76,    93, 0},
	{   49,    73,     0, 0,   1703,    679,   -53,   115,     0, 0},
	{   18,    66,     0, 0,   1335,    745,   -41,   120,     0, 0},
	{   73,    70,     3, 0,   1595,   1251,    -1,   -80,    99, 0},
	{    2,    47,    12, 0,   1027,    570,   -57,   101,    50, 0},
	{   11,    46,    24, 0,   1104,    388,   104,    58,    40, 0},
	{   11,    49,    10, 0,   1130,    579,    74,     5,   102, 0},
	{    8,    43,    39, 0,   1045,    191,    72,    39,    96, 0},
};Vtx_tn Vtx_goose_mesh01_3[32] __attribute__((aligned (8))) = {
	{  -16,    23,    26, 0,    498,   1155,    78,   -98,    17, 0},
	{  -16,     6,    49, 0,    646,    833,     2,   -94,    86, 0},
	{  -30,    16,    14, 0,    642,   1276,   -68,     5,   107, 0},
	{  -38,     4,     8, 0,    451,   1802,   -25,   -69,   104, 0},
	{  -49,    11,     0, 0,    588,   1710,   -84,    95,     0, 0},
	{  -55,     3,     7, 0,    662,   1794,   -83,     3,    96, 0},
	{   54,    71,     6, 0,    417,   1481,   -14,    37,   120, 0},
	{   64,    74,     4, 0,    287,   1481,    38,   -55,   107, 0},
	{   62,    79,     4, 0,    299,   1398,     5,    82,    96, 0},
	{   62,    79,     4, 0,    883,   1328,     5,    82,    96, 0},
	{   71,    75,     0, 0,    784,   1283,    63,   109,     0, 0},
	{   63,    81,     0, 0,    888,   1266,    52,   115,     0, 0},
	{   59,    68,     0, 0,    363,   1539,    83,   -96,     0, 0},
	{   64,    74,     4, 0,   1419,   1965,    38,   -55,   107, 0},
	{   55,    67,     6, 0,   1319,   1974,    50,   -75,    89, 0},
	{    8,    43,    39, 0,     80,    960,    72,    39,    96, 0},
	{   -9,    28,    47, 0,    352,    857,    24,    15,   123, 0},
	{  -38,     4,    -9, 0,   1342,    883,   -25,   -69,  -105, 0},
	{  -15,     2,     0, 0,   1070,    994,    37,  -122,     0, 0},
	{  -58,    -2,     0, 0,   1572,   1004,  -102,   -77,     0, 0},
	{  -20,    27,   -30, 0,    584,    358,   -98,    74,   -34, 0},
	{    2,    47,   -13, 0,   1027,    570,   -57,   101,   -51, 0},
	{    8,    43,   -40, 0,   1045,    191,    72,    39,   -97, 0},
	{   72,    73,    -3, 0,    193,   1517,    47,    69,   -96, 0},
	{   64,    74,    -5, 0,    287,   1481,    38,   -55,  -108, 0},
	{   62,    79,    -5, 0,    299,   1398,     5,    82,   -97, 0},
	{   73,    70,    -4, 0,    198,   1556,    -1,   -80,  -100, 0},
	{   64,    74,     0, 0,    287,   1481,    24,  -125,     0, 0},
	{   64,    74,    -5, 0,   1419,   1965,    38,   -55,  -108, 0},
	{   59,    68,     0, 0,   1332,   1891,    83,   -96,     0, 0},
	{  -55,     3,    -8, 0,   1505,    912,   -83,     3,   -97, 0},
	{  -30,    16,   -15, 0,    464,    503,   -68,     5,  -108, 0},
};Vtx_tn Vtx_goose_mesh01_4[32] __attribute__((aligned (8))) = {
	{   -4,    14,   -15, 0,    943,    813,    56,   -83,   -78, 0},
	{  -15,     2,     0, 0,   1070,    994,    37,  -122,     0, 0},
	{  -22,     6,   -10, 0,   1081,    866,    11,   -90,   -90, 0},
	{  -27,    32,     0, 0,    634,    771,   -85,    94,     0, 0},
	{    2,    47,   -13, 0,   1027,    570,   -57,   101,   -51, 0},
	{  -30,    16,   -15, 0,    464,    503,   -68,     5,  -108, 0},
	{   11,    17,     0, 0,    810,   1635,   108,   -67,     0, 0},
	{   11,    49,   -11, 0,   1173,   1827,    74,     5,  -103, 0},
	{   18,    50,     0, 0,   1216,   1721,    82,   -97,     0, 0},
	{   -4,    14,   -15, 0,    458,   1306,    56,   -83,   -78, 0},
	{   11,    46,   -25, 0,     54,   1171,   104,    58,   -41, 0},
	{   11,    49,   -11, 0,     31,   1364,    74,     5,  -103, 0},
	{   11,    58,    -9, 0,   1196,    634,   -39,    76,   -94, 0},
	{    7,    59,     0, 0,   1172,    755,   -77,   101,     0, 0},
	{   18,    66,     0, 0,   1335,    745,   -41,   120,     0, 0},
	{   -3,    50,     0, 0,   1008,    758,   -80,    98,     0, 0},
	{   18,    50,     0, 0,    811,   1842,    82,   -97,     0, 0},
	{   55,    67,    -7, 0,   1319,   1974,    50,   -75,   -90, 0},
	{   55,    65,     0, 0,   1273,   1887,    68,  -107,     0, 0},
	{   11,    58,    -9, 0,    977,   1506,   -39,    76,   -94, 0},
	{   55,    67,    -7, 0,    377,   1527,    50,   -75,   -90, 0},
	{   11,    49,   -11, 0,   1008,   1617,    74,     5,  -103, 0},
	{   49,    73,     0, 0,   1703,    679,   -53,   115,     0, 0},
	{   54,    71,    -7, 0,   1728,    570,   -14,    37,  -121, 0},
	{   73,    70,    -4, 0,   1595,   1251,    -1,   -80,  -100, 0},
	{   84,    69,     0, 0,   1931,   1406,    57,   113,     0, 0},
	{   85,    69,     0, 0,   1949,   1406,   124,   -26,     0, 0},
	{  -16,    23,   -27, 0,    498,   1155,    78,   -98,   -18, 0},
	{  -16,     6,   -50, 0,    646,    833,     2,   -94,   -87, 0},
	{   -9,    28,   -48, 0,    352,    857,    24,    15,  -124, 0},
	{  -49,    11,     0, 0,    252,    701,   -84,    95,     0, 0},
	{   59,    68,     0, 0,   1332,   1891,    83,   -96,     0, 0},
};Vtx_tn Vtx_goose_mesh01_5[32] __attribute__((aligned (8))) = {
	{  -49,    11,     0, 0,    252,    701,   -84,    95,     0, 0},
	{  -55,     3,    -8, 0,    137,    670,   -83,     3,   -97, 0},
	{  -58,    -2,     0, 0,     81,    753,  -102,   -77,     0, 0},
	{  -38,     4,    -9, 0,    451,   1802,   -25,   -69,  -105, 0},
	{  -49,    11,     0, 0,    588,   1710,   -84,    95,     0, 0},
	{  -30,    16,   -15, 0,    345,   1643,   -68,     5,  -108, 0},
	{  -20,    27,   -30, 0,    584,    358,   -98,    74,   -34, 0},
	{    8,    43,   -40, 0,   1045,    191,    72,    39,   -97, 0},
	{   -9,    28,   -48, 0,    664,    123,    24,    15,  -124, 0},
	{  -16,     6,   -50, 0,    495,     33,     2,   -94,   -87, 0},
	{  -30,    16,   -15, 0,    464,    503,   -68,     5,  -108, 0},
	{   54,    71,    -7, 0,    417,   1481,   -14,    37,  -121, 0},
	{   64,    74,    -5, 0,    287,   1481,    38,   -55,  -108, 0},
	{   55,    67,    -7, 0,    377,   1527,    50,   -75,   -90, 0},
	{   54,    71,    -7, 0,   1728,    570,   -14,    37,  -121, 0},
	{   59,    81,     0, 0,   1859,    676,   -47,   118,     0, 0},
	{   62,    79,    -5, 0,   1866,    602,     5,    82,   -97, 0},
	{   62,    79,    -5, 0,    883,   1328,     5,    82,   -97, 0},
	{   71,    75,     0, 0,    784,   1283,    63,   109,     0, 0},
	{   72,    73,    -3, 0,    763,   1328,    47,    69,   -96, 0},
	{   73,    70,     0, 0,   1682,   1185,   -27,  -125,     0, 0},
	{   73,    70,    -4, 0,   1595,   1251,    -1,   -80,  -100, 0},
	{   85,    69,     0, 0,   1949,   1406,   124,   -26,     0, 0},
	{   84,    69,     0, 0,   1931,   1406,    57,   113,     0, 0},
	{   72,    73,    -3, 0,   1546,   1313,    47,    69,   -96, 0},
	{   71,    75,     0, 0,   1499,   1406,    63,   109,     0, 0},
	{   63,    81,     0, 0,   1898,    663,    52,   115,     0, 0},
	{  -30,    16,   -15, 0,   1418,   1519,   -68,     5,  -108, 0},
	{  -16,    23,   -27, 0,   1346,   1394,    78,   -98,   -18, 0},
	{   -4,    14,   -15, 0,   1179,   1519,    56,   -83,   -78, 0},
	{  -22,     6,   -10, 0,    123,   1802,    11,   -90,   -90, 0},
	{   11,    58,    -9, 0,    977,   1506,   -39,    76,   -94, 0},
};Vtx_tn Vtx_goose_mesh01_6[31] __attribute__((aligned (8))) = {
	{   -4,    14,   -15, 0,    943,    813,    56,   -83,   -78, 0},
	{   11,    17,     0, 0,    787,   1004,   108,   -67,     0, 0},
	{  -15,     2,     0, 0,   1070,    994,    37,  -122,     0, 0},
	{   11,    49,   -11, 0,   1008,   1617,    74,     5,  -103, 0},
	{    2,    47,   -13, 0,   1120,   1609,   -57,   101,   -51, 0},
	{   11,    58,    -9, 0,    977,   1506,   -39,    76,   -94, 0},
	{   11,    17,     0, 0,    810,   1635,   108,   -67,     0, 0},
	{   -4,    14,   -15, 0,    714,   1827,    56,   -83,   -78, 0},
	{   11,    49,   -11, 0,   1173,   1827,    74,     5,  -103, 0},
	{   18,    50,     0, 0,    811,   1842,    82,   -97,     0, 0},
	{   11,    49,   -11, 0,    722,   1974,    74,     5,  -103, 0},
	{   55,    67,    -7, 0,   1319,   1974,    50,   -75,   -90, 0},
	{    2,    47,   -13, 0,   1027,    570,   -57,   101,   -51, 0},
	{   11,    49,   -11, 0,   1130,    579,    74,     5,  -103, 0},
	{   11,    46,   -25, 0,   1104,    388,   104,    58,   -41, 0},
	{    8,    43,   -40, 0,   1045,    191,    72,    39,   -97, 0},
	{  -16,    23,   -27, 0,    498,   1155,    78,   -98,   -18, 0},
	{  -30,    16,   -15, 0,    642,   1276,   -68,     5,  -108, 0},
	{  -16,     6,   -50, 0,    646,    833,     2,   -94,   -87, 0},
	{  -38,     4,    -9, 0,    451,   1802,   -25,   -69,  -105, 0},
	{  -55,     3,    -8, 0,    662,   1794,   -83,     3,   -97, 0},
	{  -49,    11,     0, 0,    588,   1710,   -84,    95,     0, 0},
	{   54,    71,    -7, 0,    417,   1481,   -14,    37,  -121, 0},
	{   62,    79,    -5, 0,    299,   1398,     5,    82,   -97, 0},
	{   64,    74,    -5, 0,    287,   1481,    38,   -55,  -108, 0},
	{   54,    71,    -7, 0,   1728,    570,   -14,    37,  -121, 0},
	{   49,    73,     0, 0,   1703,    679,   -53,   115,     0, 0},
	{   59,    81,     0, 0,   1859,    676,   -47,   118,     0, 0},
	{   62,    79,    -5, 0,    883,   1328,     5,    82,   -97, 0},
	{   63,    81,     0, 0,    888,   1266,    52,   115,     0, 0},
	{   71,    75,     0, 0,    784,   1283,    63,   109,     0, 0},
};Vtx_tn Vtx_goose_mesh01_7[25] __attribute__((aligned (8))) = {
	{   59,    68,     0, 0,    363,   1539,    83,   -96,     0, 0},
	{   55,    67,    -7, 0,   1319,   1974,    50,   -75,   -90, 0},
	{   64,    74,    -5, 0,   1419,   1965,    38,   -55,  -108, 0},
	{    8,    43,   -40, 0,     80,    960,    72,    39,   -97, 0},
	{  -16,    23,   -27, 0,    498,   1155,    78,   -98,   -18, 0},
	{   -9,    28,   -48, 0,    352,    857,    24,    15,  -124, 0},
	{   72,    73,     2, 0,    193,   1517,    47,    69,    95, 0},
	{   62,    79,     4, 0,    299,   1398,     5,    82,    96, 0},
	{   64,    74,     4, 0,    287,   1481,    38,   -55,   107, 0},
	{  -30,    16,    14, 0,    345,   1643,   -68,     5,   107, 0},
	{  -38,     4,     8, 0,    451,   1802,   -25,   -69,   104, 0},
	{  -22,     6,     9, 0,    123,   1802,    11,   -90,    89, 0},
	{  -16,    23,    26, 0,    498,   1155,    78,   -98,    17, 0},
	{   11,    46,    24, 0,     54,   1171,   104,    58,    40, 0},
	{    8,    43,    39, 0,     80,    960,    72,    39,    96, 0},
	{  -38,     4,    -9, 0,   1342,    883,   -25,   -69,  -105, 0},
	{  -22,     6,   -10, 0,   1081,    866,    11,   -90,   -90, 0},
	{  -15,     2,     0, 0,   1070,    994,    37,  -122,     0, 0},
	{   73,    70,    -4, 0,    198,   1556,    -1,   -80,  -100, 0},
	{   73,    70,     0, 0,   1005,   1265,   -27,  -125,     0, 0},
	{   64,    74,     0, 0,    287,   1481,    24,  -125,     0, 0},
	{  -30,    16,   -15, 0,    345,   1643,   -68,     5,  -108, 0},
	{   -4,    14,   -15, 0,     28,   1691,    56,   -83,   -78, 0},
	{  -22,     6,   -10, 0,    123,   1802,    11,   -90,   -90, 0},
	{   11,    46,   -25, 0,     54,   1171,   104,    58,   -41, 0},
};

#ifdef __N64__
Gfx Vtx_goose_mesh01_dl[] __attribute__((aligned (8))) = {
	gsSPVertex(&Vtx_goose_mesh01_0[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,8,9,10,0),
	gsSP2Triangles(11,12,9,0,1,13,2,0),
	gsSP2Triangles(14,15,0,0,16,17,5,0),
	gsSP2Triangles(18,19,20,0,21,22,23,0),
	gsSP2Triangles(24,25,26,0,27,5,24,0),
	gsSP2Triangles(28,29,30,0,5,17,3,0),
	gsSP2Triangles(17,16,31,0,30,29,12,0),
	gsSP2Triangles(16,5,27,0,27,24,26,0),
	gsSP2Triangles(0,15,1,0,8,11,9,0),
	gsSPVertex(&Vtx_goose_mesh01_1[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,15,16,17,0),
	gsSP2Triangles(18,19,20,0,18,21,22,0),
	gsSP2Triangles(23,2,24,0,4,25,26,0),
	gsSP2Triangles(27,28,29,0,30,7,6,0),
	gsSP2Triangles(31,26,25,0,0,2,23,0),
	gsSP2Triangles(18,22,19,0,4,26,5,0),
	gsSPVertex(&Vtx_goose_mesh01_2[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,15,16,17,0),
	gsSP2Triangles(18,19,20,0,21,22,23,0),
	gsSP2Triangles(24,25,26,0,27,0,2,0),
	gsSP2Triangles(28,29,30,0,29,28,31,0),
	gsSPVertex(&Vtx_goose_mesh01_3[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,0,15,16,0),
	gsSP2Triangles(17,18,19,0,20,21,22,0),
	gsSP2Triangles(23,24,25,0,26,27,28,0),
	gsSP2Triangles(28,27,29,0,17,19,30,0),
	gsSP2Triangles(21,20,31,0,23,26,24,0),
	gsSPVertex(&Vtx_goose_mesh01_4[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,15,12,4,0),
	gsSP2Triangles(16,17,18,0,19,20,21,0),
	gsSP2Triangles(12,22,23,0,24,25,26,0),
	gsSP2Triangles(27,28,29,0,5,30,3,0),
	gsSP2Triangles(17,31,18,0,3,15,4,0),
	gsSP2Triangles(9,27,10,0,15,13,12,0),
	gsSP1Triangle(12,14,22,0),
	gsSPVertex(&Vtx_goose_mesh01_5[0], 32, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,6,9,10,0),
	gsSP2Triangles(11,12,13,0,14,15,16,0),
	gsSP2Triangles(17,18,19,0,20,21,22,0),
	gsSP2Triangles(23,24,25,0,26,16,15,0),
	gsSP2Triangles(27,28,29,0,5,30,3,0),
	gsSP2Triangles(31,11,13,0,21,24,23,0),
	gsSP1Triangle(6,8,9,0),
	gsSPVertex(&Vtx_goose_mesh01_6[0], 31, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,14,15,12,0),
	gsSP2Triangles(16,17,18,0,19,20,21,0),
	gsSP2Triangles(22,23,24,0,25,26,27,0),
	gsSP1Triangle(28,29,30,0),
	gsSPVertex(&Vtx_goose_mesh01_7[0], 25, 0),
	gsSP2Triangles(0,1,2,0,3,4,5,0),
	gsSP2Triangles(6,7,8,0,9,10,11,0),
	gsSP2Triangles(12,13,14,0,15,16,17,0),
	gsSP2Triangles(18,19,20,0,21,22,23,0),
	gsSP1Triangle(3,24,4,0),
	gsSPEndDisplayList(),
};
#endif


#ifdef __N64__
Gfx Wtx_goose[] __attribute__((aligned (8))) = {
	  gsDPLoadTextureBlock(Text_goose_goosetex_diff, G_IM_FMT_RGBA, G_IM_SIZ_16b,32,32,0, 
	  	  G_TX_WRAP|G_TX_NOMIRROR, G_TX_WRAP|G_TX_NOMIRROR,5,5, G_TX_NOLOD, G_TX_NOLOD), 
	  gsSPDisplayList(Vtx_goose_mesh01_dl),
	  gsSPEndDisplayList()
};
#endif

MeshTri Tris_goose[] __attribute__((aligned (8))) = {
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),8,9,10},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),11,12,9},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),1,13,2},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),14,15,0},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),16,17,5},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),18,19,20},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),21,22,23},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),24,25,26},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),27,5,24},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),28,29,30},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),5,17,3},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),17,16,31},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),30,29,12},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),16,5,27},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),27,24,26},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),0,15,1},
	{(Vtx*)(&Vtx_goose_mesh01_0[0]),8,11,9},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),9,10,11},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),12,13,14},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),15,16,17},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),18,19,20},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),18,21,22},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),23,2,24},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),4,25,26},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),27,28,29},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),30,7,6},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),31,26,25},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),0,2,23},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),18,22,19},
	{(Vtx*)(&Vtx_goose_mesh01_1[0]),4,26,5},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),9,10,11},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),12,13,14},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),15,16,17},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),18,19,20},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),21,22,23},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),24,25,26},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),27,0,2},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),28,29,30},
	{(Vtx*)(&Vtx_goose_mesh01_2[0]),29,28,31},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),9,10,11},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),12,13,14},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),0,15,16},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),17,18,19},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),20,21,22},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),23,24,25},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),26,27,28},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),28,27,29},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),17,19,30},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),21,20,31},
	{(Vtx*)(&Vtx_goose_mesh01_3[0]),23,26,24},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),9,10,11},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),12,13,14},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),15,12,4},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),16,17,18},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),19,20,21},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),12,22,23},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),24,25,26},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),27,28,29},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),5,30,3},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),17,31,18},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),3,15,4},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),9,27,10},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),15,13,12},
	{(Vtx*)(&Vtx_goose_mesh01_4[0]),12,14,22},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),6,9,10},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),11,12,13},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),14,15,16},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),17,18,19},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),20,21,22},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),23,24,25},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),26,16,15},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),27,28,29},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),5,30,3},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),31,11,13},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),21,24,23},
	{(Vtx*)(&Vtx_goose_mesh01_5[0]),6,8,9},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),9,10,11},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),12,13,14},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),14,15,12},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),16,17,18},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),19,20,21},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),22,23,24},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),25,26,27},
	{(Vtx*)(&Vtx_goose_mesh01_6[0]),28,29,30},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),0,1,2},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),3,4,5},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),6,7,8},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),9,10,11},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),12,13,14},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),15,16,17},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),18,19,20},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),21,22,23},
	{(Vtx*)(&Vtx_goose_mesh01_7[0]),3,24,4}
};

MeshInfo MeshInfo_goose[] __attribute__((aligned (8))) = {&Tris_goose[0],114,&Text_goose_goosetex_diff[0]};


#define GOOSE_MODEL_MESH_COUNT 1

MeshInfo* goose_meshinfos[1] = {
    MeshInfo_goose,
};
#ifdef __N64__
Gfx* goose_model_meshes[1] __attribute__((aligned (8))) = {
    Wtx_goose,
};
#endif
