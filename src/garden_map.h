
#ifndef GARDEN_MAP_H
#define GARDEN_MAP_H 1
#include "constants.h"
#include "gameobject.h"


AABB garden_map_bounds[] = {
{{-45.071983, -12.775320, -47.049881}, {48.456688, 53.892517, 47.044338}}, // (Bush.inst) 
{{-45.071983, -12.775320, -47.049881}, {48.456745, 53.892513, 47.044338}}, // (Bush.inst.001) 
{{-45.071983, -12.775320, -47.049881}, {48.456745, 53.892517, 47.044338}}, // (Bush.inst.002) 
{{-16.400528, 0.000000, -44.529362}, {20.007133, 159.193802, 44.529362}}, // (GardenerCharacter) 
{{-236.518585, -54.389256, -15.605621}, {236.913071, 18.310425, 15.605621}}, // (Wall.inst) 
{{-75.067978, -32.971409, -74.530678}, {75.116386, 31.478992, 74.758820}}, // (Planter.inst) 
{{-75.067978, -32.971409, -74.530678}, {75.116386, 31.478992, 74.758820}}, // (Planter.inst.001) 
{{-45.071869, -12.775320, -47.049866}, {48.456688, 53.892517, 47.044342}}, // (Bush.inst.007) 
{{-32.452812, 0.000000, -15.666389}, {28.539619, 69.391663, 15.363922}}, // (Goose) 
{{-6.503963, -2.904237, -12.591991}, {10.518951, 2.923790, 12.591991}}, // (BookItem) 
{{-129.634094, -3.599929, -2559.352051}, {421.491394, 0.000000, 0.000000}}, // (Ground.001) 
{{-680.759399, -0.059928, -527.152649}, {421.491394, 0.000000, 575.098328}}, // (Ground.002) 
{{-2191.250244, -0.059928, -527.152649}, {0.000000, 1.616156, 575.098328}}, // (Ground.003) 
{{-964.678833, -21.323647, -1096.278320}, {0.000000, 0.000000, 0.000000}}, // (Ground.004) 
{{-2193.948730, -36.123112, -1106.562256}, {0.000000, 22.836750, 0.000000}}, // (Ground.005) 
{{-972.713501, -72.125366, -2559.860107}, {0.000000, 0.000000, 0.000000}}, // (Ground.006) 
{{-2196.480225, -72.125366, -1377.208740}, {0.000000, 16.236820, 0.000000}}, // (Ground) 
{{-1233.899170, -65.354500, -2476.443848}, {0.000000, 0.000000, 0.000000}}, // (Water) 
{{-29.933395, -13.390024, -25.482903}, {30.003204, 5.335236, 31.683998}}, // (Rock.inst) 
{{-29.933395, -13.390024, -25.482903}, {30.003204, 6.759514, 31.683998}}, // (Rock.001.inst) 
{{-19.174805, -13.390017, -25.482903}, {30.003204, 13.261921, 30.914383}}, // (Rock.002.inst) 
{{-15.867348, -21.999542, -5.716667}, {3.573647, 53.502243, 10.156860}}, // (Watergrass.002.inst) 
{{-1.107788, -9.607552, -10.389290}, {13.390160, 49.918880, 0.000000}}, // (Watergrass.003.inst) 
{{-29.933395, -13.390017, -25.482903}, {30.003204, 5.335236, 31.683998}}, // (Rock.inst.001) 
{{0.000000, -66.621208, 0.000000}, {2.139816, 49.910568, 1.821327}}, // (Reed.001.inst.001) 
{{-29.933395, -13.390024, -25.482903}, {30.003204, 6.759514, 31.683998}}, // (Rock.001.inst.001) 
{{0.000000, -8.391581, -7.322273}, {18.022156, 63.362251, 0.000000}}, // (Watergrass.001.inst.001) 
{{-19.668732, -34.420197, -5.539627}, {6.624069, 56.590496, 9.426155}}, // (Watergrass.inst.001) 
{{0.000000, -66.621223, 0.000000}, {2.139816, 49.910568, 1.821327}}, // (Reed.001.inst.002) 
{{-11.701584, 0.000000, -12.729034}, {15.457535, 0.000000, 15.827751}}, // (Lilypad.inst) 
{{-14.035149, 0.000000, -16.148872}, {14.504128, 0.000000, 11.302299}}, // (Lilypad.inst.001) 
{{-13.432961, 0.000000, -11.844177}, {14.300766, 0.000000, 16.635475}}, // (Lilypad.inst.002) 
{{0.000000, -8.391581, -7.322273}, {18.022156, 63.362251, 0.000000}}, // (Watergrass.001.inst.002) 
{{-15.867348, -21.999542, -5.716667}, {3.573647, 53.502243, 10.156860}}, // (Watergrass.002.inst.001) 
{{-1.107788, -9.607558, -10.389290}, {13.390160, 49.918880, 0.000000}}, // (Watergrass.003.inst.001) 
{{-19.668732, -34.420197, -5.539627}, {6.624069, 56.590496, 9.426155}}, // (Watergrass.inst.002) 
{{-2195.758789, -65.354500, -1914.777954}, {0.000000, 0.000000, 0.000000}}, // (Water.001) 
{{-1845.410767, -67.102318, -1981.791016}, {0.000000, 0.000000, 0.000000}}, // (Water.002) 
{{-1395.317261, -69.267746, -2053.417725}, {0.000000, 0.000000, 0.000000}}, // (Water.003) 
{{-1233.899170, -65.354500, -2053.417725}, {0.000000, 0.000000, 0.000000}}, // (Water.004) 
{{-236.518539, -54.389256, -15.605621}, {236.913010, 18.310425, 15.605621}}, // (Wall.inst.001) 
{{-128.671188, -3.864530, -40.110741}, {129.328537, 52.202320, 48.230095}}, // (Bench) 
{{-337.233582, -3.864531, -334.646881}, {813.144226, 0.000000, 26.948891}}, // (Path) 
{{-29.389343, 0.000000, -29.389343}, {29.389343, 2.438395, 29.389343}}, // (Paver.inst) 
{{-236.518585, -54.389256, -15.605621}, {236.913071, 96.539619, 15.605621}}, // (WallTall.inst) 
{{-236.518539, -54.389256, -15.605621}, {236.913010, 96.539619, 15.605621}}, // (WallTall.inst.001) 
{{-232.346878, -29.036779, 0.000000}, {162.644470, 141.263748, 387.966888}}, // (Hedge.inst) 
{{-232.346817, -29.036779, 0.000000}, {162.644470, 141.263748, 387.966888}}, // (Hedge.inst.001) 
{{-972.713501, -168.049377, -1911.581177}, {0.000000, 0.000000, 0.000000}}, // (CollisionGround.003) 
{{-2182.876221, -133.764282, -2054.058838}, {0.000000, 0.000000, 0.000000}}, // (CollisionGround.004) 
{{-1230.510132, -93.962471, -2413.904053}, {0.000000, 0.000000, 0.000000}}, // (CollisionGround.001) 
{{-962.034973, -143.872101, -2476.222168}, {0.000000, 0.000000, 0.000000}}, // (CollisionGround.002) 
{{-2198.944824, -65.643578, -2559.913818}, {0.000000, 0.102139, 0.000000}}, // (Ground.014) 
{{-2197.419434, -192.916824, -1639.372070}, {0.000000, 0.000000, 0.000000}}, // (CollisionGround.005) 
{{-29.389343, 0.000000, -29.389343}, {29.389343, 2.438395, 29.389343}}, // (Paver.inst.013) 
{{-41.540108, 0.000000, -41.540165}, {41.540108, 2.438395, 41.540165}}, // (Paver.inst.001) 
{{-41.540108, 0.000000, -41.540165}, {41.540108, 2.438395, 41.540138}}, // (Paver.inst.004) 
{{-41.540108, 0.000000, -41.540138}, {41.540108, 2.438395, 41.540138}}, // (Paver.inst.002) 
{{-41.540108, 0.000000, -41.540138}, {41.540108, 2.438395, 41.540150}}, // (Paver.inst.003) 
{{-41.540108, 0.000000, -41.540150}, {41.540108, 2.438395, 41.540150}}, // (Paver.inst.005) 
{{-41.540108, 0.000000, -41.540142}, {41.540108, 2.438395, 41.540142}}, // (Paver.inst.006) 
{{-41.540108, 0.000000, -41.540142}, {41.540108, 2.438395, 41.540146}}, // (Paver.inst.007) 
{{-41.540108, 0.000000, -41.540142}, {41.540108, 2.438395, 41.540150}}, // (Paver.inst.008) 
{{-41.540108, 0.000000, -41.540150}, {41.540108, 2.438395, 41.540138}}, // (Paver.inst.011) 
{{-41.540108, 0.000000, -41.540138}, {41.540108, 2.438395, 41.540138}}, // (Paver.inst.010) 
{{-41.540108, 0.000000, -41.540138}, {41.540108, 2.438395, 41.540165}}, // (Paver.inst.009) 

};

GameObject garden_map_data[] = {
{0, // object id (Bush.inst)
{-1107.730835, 11.748462, -64.801270}, // position
{0.000000, 0.000000, -0.000000}, // rotation
BushModel, // modelType
0, // subtype
},
{1, // object id (Bush.inst.001)
{-994.538513, 11.748466, -18.517456}, // position
{0.000000, 0.000000, -0.000000}, // rotation
BushModel, // modelType
0, // subtype
},
{2, // object id (Bush.inst.002)
{-1005.235413, 11.748462, -147.547226}, // position
{0.000000, 0.000000, -0.000000}, // rotation
BushModel, // modelType
0, // subtype
},
{3, // object id (GardenerCharacter)
{-1042.592285, 0.000000, -553.012817}, // position
{0.000000, 0.000000, -0.000000}, // rotation
GardenerCharacterModel, // modelType
0, // subtype
},
{4, // object id (Wall.inst)
{-1533.462036, 53.700020, -691.284058}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WallModel, // modelType
0, // subtype
},
{5, // object id (Planter.inst)
{-1390.277832, 28.706844, -404.714294}, // position
{0.000000, 0.000000, -0.000000}, // rotation
PlanterModel, // modelType
0, // subtype
},
{6, // object id (Planter.inst.001)
{-1390.277832, 28.706844, -127.979004}, // position
{0.000000, 0.000000, -0.000000}, // rotation
PlanterModel, // modelType
0, // subtype
},
{7, // object id (Bush.inst.007)
{-1931.657593, 11.748433, -464.240753}, // position
{0.000000, 0.000000, -0.000000}, // rotation
BushModel, // modelType
0, // subtype
},
{8, // object id (Goose)
{-1320.555664, -62.705948, -1328.402832}, // position
{0.000000, 0.000000, -0.000000}, // rotation
GooseModel, // modelType
0, // subtype
},
{9, // object id (BookItem)
{-900.784485, 0.000007, -241.499634}, // position
{0.000000, 0.000000, -0.000000}, // rotation
BookItemModel, // modelType
0, // subtype
},
{10, // object id (Ground.001)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
0, // subtype
},
{11, // object id (Ground.002)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
1, // subtype
},
{12, // object id (Ground.003)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
2, // subtype
},
{13, // object id (Ground.004)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
3, // subtype
},
{14, // object id (Ground.005)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
4, // subtype
},
{15, // object id (Ground.006)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
5, // subtype
},
{16, // object id (Ground)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
6, // subtype
},
{17, // object id (Water)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WaterModel, // modelType
0, // subtype
},
{18, // object id (Rock.inst)
{-1422.893188, -65.354454, -1455.859619}, // position
{0.000000, 172.253710, 0.000000}, // rotation
RockModel, // modelType
0, // subtype
},
{19, // object id (Rock.001.inst)
{-1420.412354, -65.354454, -1376.857300}, // position
{0.000000, 172.253710, 0.000000}, // rotation
RockModel, // modelType
1, // subtype
},
{20, // object id (Rock.002.inst)
{-1471.690308, -62.778938, -1366.834717}, // position
{0.000000, 172.253710, 0.000000}, // rotation
RockModel, // modelType
2, // subtype
},
{21, // object id (Watergrass.002.inst)
{-1489.577515, -65.926643, -1381.857910}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WatergrassModel, // modelType
0, // subtype
},
{22, // object id (Watergrass.003.inst)
{-1488.054565, -72.866158, -1377.465454}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WatergrassModel, // modelType
1, // subtype
},
{23, // object id (Rock.inst.001)
{-961.716492, -65.354446, -1419.150635}, // position
{0.000000, 172.253710, 0.000000}, // rotation
RockModel, // modelType
0, // subtype
},
{24, // object id (Reed.001.inst.001)
{-1095.969238, -43.459892, -1379.145752}, // position
{0.000000, 0.000000, 0.000000}, // rotation
ReedModel, // modelType
0, // subtype
},
{25, // object id (Rock.001.inst.001)
{-1030.242310, -65.354454, -1397.586060}, // position
{0.000000, 172.253710, 0.000000}, // rotation
RockModel, // modelType
1, // subtype
},
{26, // object id (Watergrass.001.inst.001)
{-1511.936646, -74.177361, -1359.434570}, // position
{0.000000, -9.583076, -0.000000}, // rotation
WatergrassModel, // modelType
2, // subtype
},
{27, // object id (Watergrass.inst.001)
{-1511.162231, -81.396347, -1364.991577}, // position
{0.000000, -9.583076, -0.000000}, // rotation
WatergrassModel, // modelType
3, // subtype
},
{28, // object id (Reed.001.inst.002)
{-1050.088013, -43.459900, -1382.839844}, // position
{0.000000, 0.000000, 0.000000}, // rotation
ReedModel, // modelType
0, // subtype
},
{29, // object id (Lilypad.inst)
{-1244.464233, -62.969227, -1603.408447}, // position
{0.000000, 0.000000, 0.000000}, // rotation
LilypadModel, // modelType
0, // subtype
},
{30, // object id (Lilypad.inst.001)
{-1167.187622, -62.969227, -1626.878052}, // position
{0.000000, 123.995268, 0.000000}, // rotation
LilypadModel, // modelType
0, // subtype
},
{31, // object id (Lilypad.inst.002)
{-1251.871460, -62.969227, -1643.819336}, // position
{0.000000, -40.211082, 0.000000}, // rotation
LilypadModel, // modelType
0, // subtype
},
{32, // object id (Watergrass.001.inst.002)
{-991.317871, -73.228722, -1377.973145}, // position
{0.000000, -9.583076, -0.000000}, // rotation
WatergrassModel, // modelType
2, // subtype
},
{33, // object id (Watergrass.002.inst.001)
{-1054.851196, -65.926651, -1385.552124}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WatergrassModel, // modelType
0, // subtype
},
{34, // object id (Watergrass.003.inst.001)
{-1053.328125, -72.866165, -1381.159668}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WatergrassModel, // modelType
1, // subtype
},
{35, // object id (Watergrass.inst.002)
{-990.543457, -80.447701, -1383.530029}, // position
{0.000000, -9.583076, -0.000000}, // rotation
WatergrassModel, // modelType
3, // subtype
},
{36, // object id (Water.001)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WaterModel, // modelType
1, // subtype
},
{37, // object id (Water.002)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WaterModel, // modelType
2, // subtype
},
{38, // object id (Water.003)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WaterModel, // modelType
3, // subtype
},
{39, // object id (Water.004)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WaterModel, // modelType
4, // subtype
},
{40, // object id (Wall.inst.001)
{-900.363281, 53.700020, -691.284058}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WallModel, // modelType
0, // subtype
},
{41, // object id (Bench)
{-1266.641479, -6.660190, -1274.815430}, // position
{0.000000, 4.622635, 0.000000}, // rotation
BenchModel, // modelType
0, // subtype
},
{42, // object id (Path)
{-1400.278442, 6.734142, -1014.376465}, // position
{0.000000, 0.000000, -0.000000}, // rotation
PathModel, // modelType
0, // subtype
},
{43, // object id (Paver.inst)
{-1185.147827, -1.930627, -690.732849}, // position
{0.000000, 0.000000, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{44, // object id (WallTall.inst)
{-1525.567139, 53.700020, 516.286011}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WallTallModel, // modelType
0, // subtype
},
{45, // object id (WallTall.inst.001)
{-890.475952, 53.700020, 516.286011}, // position
{0.000000, 0.000000, 0.000000}, // rotation
WallTallModel, // modelType
0, // subtype
},
{46, // object id (Hedge.inst)
{-1530.732056, 21.818190, 114.756088}, // position
{0.000000, 0.000000, -0.000000}, // rotation
HedgeModel, // modelType
0, // subtype
},
{47, // object id (Hedge.inst.001)
{-825.179138, 21.818190, 114.756088}, // position
{0.000000, 0.000000, -0.000000}, // rotation
HedgeModel, // modelType
0, // subtype
},
{48, // object id (CollisionGround.003)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
CollisionGroundModel, // modelType
0, // subtype
},
{49, // object id (CollisionGround.004)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
CollisionGroundModel, // modelType
1, // subtype
},
{50, // object id (CollisionGround.001)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
CollisionGroundModel, // modelType
2, // subtype
},
{51, // object id (CollisionGround.002)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
CollisionGroundModel, // modelType
3, // subtype
},
{52, // object id (Ground.014)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
GroundModel, // modelType
7, // subtype
},
{53, // object id (CollisionGround.005)
{0.000000, 0.000000, -0.000000}, // position
{0.000000, 0.000000, 0.000000}, // rotation
CollisionGroundModel, // modelType
4, // subtype
},
{54, // object id (Paver.inst.013)
{-1248.190674, -1.454250, -689.521545}, // position
{0.000000, 0.000000, -0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{55, // object id (Paver.inst.001)
{-1218.977661, -1.454253, -586.532104}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{56, // object id (Paver.inst.004)
{-1217.810913, 0.489220, -483.006195}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{57, // object id (Paver.inst.002)
{-1221.044678, 1.046229, -378.281494}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{58, // object id (Paver.inst.003)
{-1220.111084, 0.941538, -271.197998}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{59, // object id (Paver.inst.005)
{-1216.846924, 0.941538, -172.458679}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{60, // object id (Paver.inst.006)
{-1218.886963, 0.941538, -68.823212}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{61, // object id (Paver.inst.007)
{-1219.294922, 0.941538, 43.345615}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{62, // object id (Paver.inst.008)
{-1220.927002, 0.941538, 152.320206}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{63, // object id (Paver.inst.011)
{-1221.743042, 0.941538, 243.715271}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{64, // object id (Paver.inst.010)
{-1216.438843, 0.941538, 369.791504}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},
{65, // object id (Paver.inst.009)
{-1213.174805, 0.941538, 503.212006}, // position
{0.000000, -46.890755, 0.000000}, // rotation
PaverModel, // modelType
0, // subtype
},

};

#define GARDEN_MAP_COUNT 66

#endif /* GARDEN_MAP_H */
