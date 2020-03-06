#ifndef UNIVERSITY_MAP_GRAPH_H
#define UNIVERSITY_MAP_GRAPH_H 1

#include "constants.h"
#include "pathfinding.h"

#define UNIVERSITY_MAP_GRAPH_SIZE 11

extern Graph university_map_graph;

extern PathfindingState university_map_graph_pathfinding_state;

extern NodeState
    university_map_graph_pathfinding_node_states[UNIVERSITY_MAP_GRAPH_SIZE];
extern int university_map_graph_pathfinding_result[UNIVERSITY_MAP_GRAPH_SIZE];

#endif /* UNIVERSITY_MAP_GRAPH_H */
