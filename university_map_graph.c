#include "university_map_graph.h"

Node university_map_graphNodes[] = {
    {0, {10, 10, 100}},
    {1, {0, 10, 300}},
    {2, {300, 10, 300}},
    {3, {600, 10, 300}},
    {4, {-395.667, 10.001, -302.618}},
    {5, {-598.629, 10.001, -585.013}},
    {6, {-939.587, 10.001, -556.057}},
    {7, {-1291.417, 10.001, -625.138}},
    {8, {-1296.933, 10.001, -792.504}},
    {9, {-947.610, 10.001, -820.081}},
    {10, {-632.436, 10.001, -775.958}},
};

int university_map_graph_edges_node0[] = {1, 2, 4};
int university_map_graph_edges_node1[] = {0, 2};
int university_map_graph_edges_node2[] = {0, 1, 3};
int university_map_graph_edges_node3[] = {2};
int university_map_graph_edges_node4[] = {0, 5};
int university_map_graph_edges_node5[] = {4, 6, 10};
int university_map_graph_edges_node6[] = {5, 7};
int university_map_graph_edges_node7[] = {6, 8};
int university_map_graph_edges_node8[] = {7, 9};
int university_map_graph_edges_node9[] = {8, 10};
int university_map_graph_edges_node10[] = {9, 5};

EdgeList university_map_graphEdges[] = {
    {/*size*/ 3, university_map_graph_edges_node0},   //
    {/*size*/ 2, university_map_graph_edges_node1},   //
    {/*size*/ 3, university_map_graph_edges_node2},   //
    {/*size*/ 1, university_map_graph_edges_node3},   //
    {/*size*/ 2, university_map_graph_edges_node4},   //
    {/*size*/ 3, university_map_graph_edges_node5},   //
    {/*size*/ 2, university_map_graph_edges_node6},   //
    {/*size*/ 2, university_map_graph_edges_node7},   //
    {/*size*/ 2, university_map_graph_edges_node8},   //
    {/*size*/ 2, university_map_graph_edges_node9},   //
    {/*size*/ 2, university_map_graph_edges_node10},  //
};

Graph university_map_graph = {
    UNIVERSITY_MAP_GRAPH_SIZE,  // int size;
    university_map_graphNodes,  // Node* nodes;
    university_map_graphEdges,  // EdgeList* edges;
};

PathfindingState university_map_graph_pathfinding_state;

NodeState
    university_map_graph_pathfinding_node_states[UNIVERSITY_MAP_GRAPH_SIZE];
int university_map_graph_pathfinding_result[UNIVERSITY_MAP_GRAPH_SIZE];
