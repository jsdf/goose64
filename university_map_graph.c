#include "university_map_graph.h"

Node university_map_graphNodes[] = {
    {0, {10.0000000, 10.0000000, 100.0000000}},
    {1, {0.0000000, 10.0000000, 300.0000000}},
    {2, {300.0000000, 10.0000000, 300.0000000}},
    {3, {600.0000000, 10.0000000, 300.0000000}},
    {4, {-395.6669922, 10.0010004, -302.6180115}},
    {5, {-598.6290283, 10.0010004, -585.0130005}},
    {6, {-939.5869751, 10.0010004, -556.0570068}},
    {7, {-1291.4169922, 10.0010004, -625.1380005}},
    {8, {-1296.9329834, 10.0010004, -792.5040283}},
    {9, {-947.6099854, 10.0010004, -820.0809937}},
    {10, {-632.4359741, 10.0010004, -775.9580078}},
};

int university_map_graph_edges_node0[] = {1,2,4};
int university_map_graph_edges_node1[] = {0,2};
int university_map_graph_edges_node2[] = {0,1,3};
int university_map_graph_edges_node3[] = {2};
int university_map_graph_edges_node4[] = {0,5};
int university_map_graph_edges_node5[] = {4,6,10};
int university_map_graph_edges_node6[] = {5,7};
int university_map_graph_edges_node7[] = {6,8};
int university_map_graph_edges_node8[] = {7,9};
int university_map_graph_edges_node9[] = {8,10};
int university_map_graph_edges_node10[] = {5,9};

EdgeList university_map_graphEdges[] = {
    {/*size*/ 3, university_map_graph_edges_node0},
    {/*size*/ 2, university_map_graph_edges_node1},
    {/*size*/ 3, university_map_graph_edges_node2},
    {/*size*/ 1, university_map_graph_edges_node3},
    {/*size*/ 2, university_map_graph_edges_node4},
    {/*size*/ 3, university_map_graph_edges_node5},
    {/*size*/ 2, university_map_graph_edges_node6},
    {/*size*/ 2, university_map_graph_edges_node7},
    {/*size*/ 2, university_map_graph_edges_node8},
    {/*size*/ 2, university_map_graph_edges_node9},
    {/*size*/ 2, university_map_graph_edges_node10},
};

Graph university_map_graph = {
    UNIVERSITY_MAP_GRAPH_SIZE,  // int size;
    university_map_graphNodes,  // Node* nodes;
    university_map_graphEdges,  // EdgeList* edges;
};

PathfindingState university_map_graph_pathfinding_state;

NodeState university_map_graph_pathfinding_node_states[UNIVERSITY_MAP_GRAPH_SIZE];
int university_map_graph_pathfinding_result[UNIVERSITY_MAP_GRAPH_SIZE];
