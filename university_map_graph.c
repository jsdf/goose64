#include "university_map_graph.h"

Node university_map_graphNodes[] = {
    {0, {10.0000000, 10.0000000, 100.0000000}},
    {1, {0.0000000, 10.0000000, 300.0000000}},
    {2, {300.0000000, 10.0000000, 300.0000000}},
    {3, {600.0000000, 10.0000000, 300.0000000}},
    {4, {-395.6669922, 10.0010004, -302.6180115}},
    {5, {-598.6290283, 10.0010004, -585.0130005}},
    {6, {-939.5869751, 10.0010004, -556.0570068}},
    {7, {-1229.8011475, 10.0009995, -605.9559326}},
    {8, {-1235.6510010, 10.0009995, -689.8064575}},
    {9, {-1232.8931885, 10.0009995, -784.2147217}},
    {10, {-940.8439941, 10.0009995, -840.3145752}},
    {11, {-601.7558594, 10.0009995, -760.5780640}},
    {12, {-1551.4914551, 10.0009995, -841.2996826}},
    {13, {-1851.8679199, 10.0009995, -792.4689331}},
    {14, {-1851.8679199, 10.0009995, -634.3679810}},
    {15, {-1538.6076660, 10.0009995, -555.2544556}},
    {16, {-1237.5518799, 10.0009995, -289.1876526}},
    {17, {-1397.0313721, 10.0009995, -257.3352356}},
    {18, {-1544.4617920, 10.0009995, -278.0179138}},
    {19, {-1572.4715576, 10.0009995, 15.1090002}},
    {20, {-1212.1336670, 10.0009995, 30.1054840}},
    {21, {-871.9851074, 10.0009995, -11.3354816}},
    {22, {-902.3599243, 10.0009995, -336.9341125}},
};

int university_map_graph_edges_node0[] = {1,2,4,21};
int university_map_graph_edges_node1[] = {0,2};
int university_map_graph_edges_node2[] = {0,1,3};
int university_map_graph_edges_node3[] = {2};
int university_map_graph_edges_node4[] = {0,5,21,22};
int university_map_graph_edges_node5[] = {4,6,11,22};
int university_map_graph_edges_node6[] = {5,7,16,22};
int university_map_graph_edges_node7[] = {6,8,15,16,22};
int university_map_graph_edges_node8[] = {7,9};
int university_map_graph_edges_node9[] = {8,10,12};
int university_map_graph_edges_node10[] = {9,11};
int university_map_graph_edges_node11[] = {5,10};
int university_map_graph_edges_node12[] = {9,13};
int university_map_graph_edges_node13[] = {12,14};
int university_map_graph_edges_node14[] = {13,15};
int university_map_graph_edges_node15[] = {7,14,18};
int university_map_graph_edges_node16[] = {6,7,17,20,21,22};
int university_map_graph_edges_node17[] = {16,18};
int university_map_graph_edges_node18[] = {15,17,19};
int university_map_graph_edges_node19[] = {18,20};
int university_map_graph_edges_node20[] = {16,19,21,22};
int university_map_graph_edges_node21[] = {0,4,16,20,22};
int university_map_graph_edges_node22[] = {4,5,6,7,16,20,21};

EdgeList university_map_graphEdges[] = {
    {/*size*/ 4, university_map_graph_edges_node0},
    {/*size*/ 2, university_map_graph_edges_node1},
    {/*size*/ 3, university_map_graph_edges_node2},
    {/*size*/ 1, university_map_graph_edges_node3},
    {/*size*/ 4, university_map_graph_edges_node4},
    {/*size*/ 4, university_map_graph_edges_node5},
    {/*size*/ 4, university_map_graph_edges_node6},
    {/*size*/ 5, university_map_graph_edges_node7},
    {/*size*/ 2, university_map_graph_edges_node8},
    {/*size*/ 3, university_map_graph_edges_node9},
    {/*size*/ 2, university_map_graph_edges_node10},
    {/*size*/ 2, university_map_graph_edges_node11},
    {/*size*/ 2, university_map_graph_edges_node12},
    {/*size*/ 2, university_map_graph_edges_node13},
    {/*size*/ 2, university_map_graph_edges_node14},
    {/*size*/ 3, university_map_graph_edges_node15},
    {/*size*/ 6, university_map_graph_edges_node16},
    {/*size*/ 2, university_map_graph_edges_node17},
    {/*size*/ 3, university_map_graph_edges_node18},
    {/*size*/ 2, university_map_graph_edges_node19},
    {/*size*/ 4, university_map_graph_edges_node20},
    {/*size*/ 5, university_map_graph_edges_node21},
    {/*size*/ 7, university_map_graph_edges_node22},
};

Graph university_map_graph = {
    UNIVERSITY_MAP_GRAPH_SIZE,  // int size;
    university_map_graphNodes,  // Node* nodes;
    university_map_graphEdges,  // EdgeList* edges;
};

NodeState university_map_graph_pathfinding_node_states[UNIVERSITY_MAP_GRAPH_SIZE];
int university_map_graph_pathfinding_result[UNIVERSITY_MAP_GRAPH_SIZE];

PathfindingState university_map_graph_pathfinding_state = {
    NULL, // Node* start;
    NULL, // Node* end;
    university_map_graph_pathfinding_node_states,
    UNIVERSITY_MAP_GRAPH_SIZE, // int nodeStateSize;
    0, // int open;
    university_map_graph_pathfinding_result, // int* result;
    0, // int resultSize;
};

