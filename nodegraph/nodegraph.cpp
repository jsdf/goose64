
#include <algorithm>
#include <cctype>
#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "nodegraph.hpp"

NodeGraphEdge::NodeGraphEdge(const NodeGraphEdge& other) {
  from = other.from;
  to = other.to;
}
NodeGraphEdge::NodeGraphEdge(int from_, int to_) {
  from = from_;
  to = to_;
}

void NodeGraph::load(Graph* graph) {
  int i;
  Node* node;

  for (i = 0, node = graph->nodes;  //
       i < graph->size;             //
       i++, node++                  //
  ) {
    nodes.push_back(*node);
    assert(nodes[i].id == i);

    addEdges(i, Path_getNodeEdgesByID(graph, i));
  }
}

bool NodeGraph::edgeExists(int nodeID, int otherNodeID) {
  for (auto edge = edges.begin(); edge != edges.end(); ++edge) {
    if ((edge->from == nodeID && otherNodeID == edge->to) ||
        (edge->from == otherNodeID && nodeID == edge->to)) {
      return true;
    }
  }

  return false;
}

void NodeGraph::addEdges(int nodeID, EdgeList* edgesToAdd) {
  int i;
  int* otherNodeID;
  for (                                           //
      i = 0, otherNodeID = edgesToAdd->elements;  //
      i < edgesToAdd->size;                       //
      i++, otherNodeID++                          //
  ) {
    if (!edgeExists(nodeID, *otherNodeID)) {
      edges.push_back(NodeGraphEdge(nodeID, *otherNodeID));
    }
  }
}

void NodeGraph::addEdge(int from, int to) {
  edges.push_back(NodeGraphEdge(from, to));  //
}

void NodeGraph::addNewNode(Vec3d* position) {
  Node newNode;
  newNode.id = nodes.size();
  newNode.position = *position;
  nodes.push_back(newNode);
}

void NodeGraph::deleteNodeAndEdges(float nodeID) {
  int i;

  // remove node
  nodes.erase(nodes.begin() + nodeID);

  // fix shifted node ids
  i = 0;
  for (auto node = nodes.begin(); node != nodes.end(); ++node) {
    node->id = i;
    i++;
  }

  // remove invalid edges
  edges.erase(std::remove_if(edges.begin(), edges.end(),
                             [nodeID](NodeGraphEdge edge) {
                               return edge.from == nodeID || edge.to == nodeID;
                             }),
              edges.end());

  // fix shifted node ids of remaining edges
  for (auto edge = edges.begin(); edge != edges.end(); ++edge) {
    if (edge->from >= nodeID) {
      edge->from -= 1;
    }
    if (edge->to >= nodeID) {
      edge->to -= 1;
    }
  }
}

std::string NodeGraph::serializeNode(Node* node) {
  char buffer[150];
  sprintf(buffer, "{%d, {%.7f, %.7f, %.7f}}", node->id, node->position.x,
          node->position.y, node->position.z);
  return buffer;
}

std::string NodeGraph::serializeNodeEdges(
    std::set<int, std::less<int>> nodeEdges) {
  std::string joined;

  for (auto edge = nodeEdges.begin(); edge != nodeEdges.end(); ++edge) {
    joined += std::to_string(*edge) + ",";
  }
  // dumb
  if (joined.size() > 0 && joined[joined.size() - 1] == ',') {
    joined.pop_back();
  }
  return "{" + joined + "}";
}

bool NodeGraph::save(std::string path, std::string name) {
  std::string nameUpper = name;
  std::transform(nameUpper.begin(), nameUpper.end(), nameUpper.begin(),
                 ::toupper);

  int nodeID = 0;
  std::ofstream fout_h(path + ".h");

  fout_h << "#ifndef " << nameUpper << "_H \n";
  fout_h << "#define " << nameUpper << "_H 1\n\n";
  fout_h << "#include \"constants.h\"\n";
  fout_h << "#include \"pathfinding.h\"\n\n";

  fout_h << "#define " << nameUpper << "_SIZE " << nodes.size() << "\n\n";

  fout_h << "extern Graph " << name << ";\n";
  fout_h << "extern PathfindingState " << name << "_pathfinding_state;\n";
  fout_h << "extern NodeState " << name << "_pathfinding_node_states["
         << nameUpper << "_SIZE];\n";
  fout_h << "extern int " << name << "_pathfinding_result[" << nameUpper
         << "_SIZE];\n\n";
  fout_h << "#endif /* !" << nameUpper << "_H */\n";
  fout_h.close();

  std::ofstream fout_c(path + ".c");

  fout_c << "#include \"" << name << ".h\"\n\n";

  fout_c << "Node " << name << "Nodes[] = {\n";
  for (auto node = nodes.begin(); node != nodes.end(); ++node) {
    fout_c << "    " << serializeNode(&(*node)) << ",\n";
  }
  fout_c << "};\n\n";

  std::vector<std::set<int, std::less<int>>> nodesEdges;
  nodesEdges.resize(nodes.size());

  for (auto edge = edges.begin(); edge != edges.end(); ++edge) {
    nodesEdges[edge->from].insert(edge->to);
    nodesEdges[edge->to].insert(edge->from);
  }

  nodeID = 0;
  for (auto nodeEdges = nodesEdges.begin(); nodeEdges != nodesEdges.end();
       ++nodeEdges) {
    fout_c << "int " << name << "_edges_node" << nodeID
           << "[] = " << serializeNodeEdges(*nodeEdges) << ";\n";
    nodeID++;
  }
  fout_c << "\n";

  fout_c << "EdgeList " << name << "Edges[] = {\n";
  nodeID = 0;
  for (auto nodeEdges = nodesEdges.begin(); nodeEdges != nodesEdges.end();
       ++nodeEdges) {
    fout_c << "    {/*size*/ " << nodeEdges->size() << ", " << name
           << "_edges_node" << nodeID << "},\n";
    nodeID++;
  }
  fout_c << "};\n\n";

  fout_c << "Graph " << name << " = {\n";
  fout_c << "    " << nameUpper << "_SIZE,  // int size;\n";
  fout_c << "    " << name << "Nodes,  // Node* nodes;\n";
  fout_c << "    " << name << "Edges,  // EdgeList* edges;\n";
  fout_c << "};\n\n";

  fout_c << "PathfindingState " << name << "_pathfinding_state;\n\n";
  fout_c << "NodeState " << name << "_pathfinding_node_states[" << nameUpper
         << "_SIZE];\n";
  fout_c << "int " << name << "_pathfinding_result[" << nameUpper
         << "_SIZE];\n";
  fout_c.close();

  return true;
}

void drawNodeGraphGUI(NodeGraph& nodeGraph,
                      Vec3d* position,
                      std::string path,
                      std::string name,
                      int& selectedNode) {
  int i;

  bool hasValidSelection =
      selectedNode > -1 && selectedNode < nodeGraph.nodes.size();

  ImGui::Begin("NodeGraph Nodes");  // Create a window
  if (hasValidSelection) {
    ImGui::Text("Selected Node");
    ImGui::InputFloat3((std::to_string(i) + "##selected").c_str(),
                       (float*)&(nodeGraph.nodes[selectedNode].position),
                       "%.3f");

    ImGui::Text("All Nodes");
  }

  i = 0;
  for (auto node = nodeGraph.nodes.begin(); node != nodeGraph.nodes.end();
       ++node) {
    ImGui::InputFloat3(std::to_string(i).c_str(), (float*)&(node->position),
                       "%.3f");
    i++;
  }
  ImGui::End();

  ImGui::Begin("NodeGraph Edges");  // Create a window

  if (hasValidSelection) {
    ImGui::Text("Selected Node's edges");
    i = 0;
    for (auto edge = nodeGraph.edges.begin(); edge != nodeGraph.edges.end();
         ++edge) {
      if (edge->from == selectedNode || edge->to == selectedNode) {
        ImGui::InputInt2((std::to_string(i) + "##selected").c_str(),
                         (int*)&(*edge));
      }
      i++;
    }

    ImGui::Text("All Nodes deges");
  }

  i = 0;
  for (auto edge = nodeGraph.edges.begin(); edge != nodeGraph.edges.end();
       ++edge) {
    ImGui::InputInt2(std::to_string(i).c_str(), (int*)&(*edge));
    i++;
  }

  ImGui::End();

  ImGui::Begin("NodeGraph Tools");  // Create a window

  ImGui::Text("File");
  if (ImGui::Button("Save")) {
    nodeGraph.save(path, name);
  }

  ImGui::Text("Nodes");
  if (ImGui::Button("Add Node")) {
    nodeGraph.addNewNode(position);
  }
  if (ImGui::Button("Add Node and Edge")) {
    nodeGraph.addNewNode(position);
    if (nodeGraph.nodes.size() >= 2) {
      // edge between last 2 nodes created
      nodeGraph.addEdge(nodeGraph.nodes.size() - 2, nodeGraph.nodes.size() - 1);
    }
  }
  if (hasValidSelection) {
    if (ImGui::Button("Delete selected Node and Edges")) {
      nodeGraph.deleteNodeAndEdges(selectedNode);
      selectedNode = -1;
    }
  } else {
    if (ImGui::Button("Delete last Node")) {
      nodeGraph.deleteNodeAndEdges(nodeGraph.nodes.size() - 1);
    }
  }

  ImGui::Text("Edges");
  if (ImGui::Button("Add Edge")) {
    if (nodeGraph.nodes.size() >= 2) {
      // edge between last 2 nodes created
      nodeGraph.addEdge(nodeGraph.nodes.size() - 2, nodeGraph.nodes.size() - 1);
    }
  }
  if (ImGui::Button("Delete last Edge")) {
    if (nodeGraph.edges.size() > 0) {
      nodeGraph.edges.pop_back();
    }
  }
  ImGui::End();
}
