#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <set>
#include <string>
#include <vector>

#include "imgui.h"
#include "pathfinding.h"

class NodeGraphEdge {
 public:
  int from;
  int to;
  NodeGraphEdge(const NodeGraphEdge& other);
  NodeGraphEdge(int from_, int to_);
};

class NodeGraph {
 public:
  std::vector<Node> nodes;
  std::vector<NodeGraphEdge> edges;

  bool save(std::string path, std::string name);
  void load(Graph* graph);

  bool edgeExists(int nodeID, int otherNodeID);

  void addEdges(int nodeID, EdgeList* edgesToAdd);

  void addEdge(int from, int to);

  void addNewNode(Vec3d* position);

  std::string serializeNode(Node* node);

  std::string serializeNodeEdges(std::set<int, std::less<int>> nodeEdges);
};

void drawNodeGraphGUI(NodeGraph& nodeGraph,
                      Vec3d* position,
                      std::string path,
                      std::string name);
#endif /* !NODEGRAPH */
