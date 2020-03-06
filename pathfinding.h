#include "constants.h"
#include "vec3d.h"

#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

typedef enum NodeStateCategory {
  ClosedNodeStateCategory,
  OpenNodeStateCategory,
  UnvisitedNodeStateCategory,
  MAX_A_STAR_NODE_CATEGORY
} NodeStateCategory;

typedef struct Node {
  int id;
  Vec3d position;
} Node;

typedef struct EdgeList {
  int size;
  // array of adjacent ids
  int* elements;
} EdgeList;

typedef struct Graph {
  int size;
  Node* nodes;
  // array indexed by id of EdgeLists
  EdgeList* edges;
} Graph;

typedef struct NodeState {
  int nodeID;
  Node* node;
  Node* reachedViaNode;
  float costSoFar;
  float estimatedTotalCost;
  NodeStateCategory category;
} NodeState;

typedef struct PathfindingState {
  Node* start;
  Node* end;
  NodeState* nodeStates;
  int nodeStateSize;
  int open;
  int* result;
  int resultSize;
} PathfindingState;

EdgeList* Path_getNodeEdgesByID(Graph* graph, int nodeID);

Node* Path_getNodeByID(Graph* graph, int nodeID);

int Path_findAStar(Graph* graph, PathfindingState* state);

void Path_initState(Graph* graph,
                    PathfindingState* state,
                    Node* start,
                    Node* end,
                    NodeState* nodeStates,
                    int nodeStateSize,
                    int* result);

#endif /* !_PATHFINDING_H_ */
