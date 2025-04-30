#include "graph.hpp"
#include <algorithm>
#include <iterator>

/**
 * Algorithms to review / practice
 *
 * https://codedamn.com/news/programming/top-10-graph-algorithms-every-software-developer-must-know
 * https://www.geeksforgeeks.org/graph-data-structure-and-algorithms/
 * https://en.wikipedia.org/wiki/Category:Graph_algorithms
 *
 * Terms and definitions:
 * DAG - DAG has no directed cycles/loops. Equivalently it has a topological
 * ordering. Connected component - Maximal connected subgraph of a graph
 * Weighted graph - Edges have a weight (i.e. number associated to them)
 *
 * Representations:
 * Adjacency matrix
 * Adjacency list
 * Adjacency set
 * Incidence matrix
 * Pointer chasing nodes and edges
 *
 * Algorithms:
 * Breadth-first search
 * Depth-first search
 * Pre/in/post-order traversal
 * Topological Sort
 * Cycle Detection
 * Dijkstra's algorithm
 * Bellman-Ford algorithm
 * Floyd-Warshall Algorithm
 * Kruskal’s Algorithm
 * Prim’s Algorithm
 * Strongly Connected Components
 *
 */

namespace ry {
std::vector<nodeId_t> AdjacencyListGraph::getNodes() const {
  std::vector<nodeId_t> res(fAdjacencyList.size());
  std::size_t i = 0;
  for (auto [nodeId, _] : fAdjacencyList) {
    res[i++] = nodeId;
  }
  std::sort(std::begin(res), std::end(res));
  return res;
}

auto maybe_add_node(AdjacencyListGraph::AdjacencyList_t &map, nodeId_t node) {
    return map.emplace(node, 0).first;
}

void AdjacencyListGraph::add_node(nodeId_t node) {
    maybe_add_node(fAdjacencyList, node);
}

bool AdjacencyListGraph::isAdjacent(nodeId_t src, nodeId_t dest) const {
  auto it = fAdjacencyList.find(src);
  if (it == std::end(fAdjacencyList)) {
    return false;
  }
  auto &adjList = it->second;
  return std::find(std::begin(adjList), std::end(adjList), dest) !=
         std::end(adjList);
}

std::vector<nodeId_t> AdjacencyListGraph::getNeighbors(nodeId_t node) const {
  return {};
}

void AdjacencyListGraph::remove_vertex(nodeId_t node) {}

void AdjacencyListGraph::add_edge(nodeId_t src, nodeId_t dest) {
  auto itSrc = maybe_add_node(fAdjacencyList, src);
  // TODO not unique
  itSrc->second.push_back(dest);
}

} // namespace ry
