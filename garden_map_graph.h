#ifndef GARDEN_MAP_GRAPH_H
#define GARDEN_MAP_GRAPH_H 1

#include "constants.h"
#include "pathfinding.h"

#define GARDEN_MAP_GRAPH_SIZE 29

extern Graph garden_map_graph;
extern PathfindingState garden_map_graph_pathfinding_state;
extern NodeState
    garden_map_graph_pathfinding_node_states[GARDEN_MAP_GRAPH_SIZE];
extern int garden_map_graph_pathfinding_result[GARDEN_MAP_GRAPH_SIZE];

#endif /* !GARDEN_MAP_GRAPH_H */
