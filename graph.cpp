#include "graph.hpp"
#include <algorithm>
#include <iterator>
#include <limits>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>

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
  return it->second.contains(dest);
}

std::vector<nodeId_t> AdjacencyListGraph::getNeighbors(nodeId_t node) const {
  auto it = fAdjacencyList.find(node);
  if (it == std::end(fAdjacencyList)) {
    return {};
  }
  std::vector<nodeId_t> res(it->second.begin(), it->second.end());
  std::sort(res.begin(), res.end());
  return res;
}

void AdjacencyListGraph::remove_node(nodeId_t node) {
  auto it = fAdjacencyList.find(node);
  if (it == fAdjacencyList.end()) {
    return;
  }
  fAdjacencyList.erase(node);
  for (auto &[_, edgeSet] : fAdjacencyList) {
    edgeSet.erase(node);
  }
}

auto checked_node_find(AdjacencyListGraph::AdjacencyList_t &l, nodeId_t node) {
  auto it = l.find(node);
  if (it == std::end(l)) {
    throw new InvalidNodeIdError("Non-existent node: " + std::to_string(node));
  }
  return it;
}
void AdjacencyListGraph::add_edge(nodeId_t src, nodeId_t dest) {
  auto itSrc = checked_node_find(fAdjacencyList, src);
  checked_node_find(fAdjacencyList, dest);

  itSrc->second.insert(dest);
}

std::vector<nodeId_t> bfs(const Graph &g) {
  std::queue<nodeId_t> workQueue;
  std::unordered_set<nodeId_t> visited;

  std::vector<nodeId_t> res;

  auto startNode = g.getNodes()[0];
  workQueue.push(startNode);
  visited.insert(startNode);
  while (!workQueue.empty()) {
    auto node = workQueue.front();
    workQueue.pop();
    res.push_back(node);
    auto neighbors = g.getNeighbors(node);
    for (auto neighbor : neighbors) {
      auto [_, isUnvisited] = visited.insert(neighbor);
      if (isUnvisited) {
        workQueue.push(neighbor);
      }
    }
  }
  return res;
}

std::vector<nodeId_t> dfsPreorder(const Graph &g) {
  std::stack<nodeId_t> workStack;
  std::unordered_set<nodeId_t> visited;
  std::vector<nodeId_t> res;

  auto startNode = g.getNodes()[0];
  workStack.push(startNode);
  while (!workStack.empty()) {
    auto node = workStack.top();
    workStack.pop();
    auto [_, isUnvisited] = visited.insert(node);
    if (isUnvisited) {
      res.push_back(node);
      auto neighbors = g.getNeighbors(node);
      for (auto neighbor : neighbors) {
        workStack.push(neighbor);
      }
    }
  }
  return res;
}

void dfsPostorderWorker(const Graph &g, std::vector<nodeId_t> &res,
                        std::unordered_set<nodeId_t> &visited, nodeId_t node) {
  auto [_, isUnvisited] = visited.insert(node);
  if (!isUnvisited) {
    return;
  }
  auto neighbors = g.getNeighbors(node);
  for (auto neighbor : neighbors) {
    if (!visited.contains(neighbor)) {
      dfsPostorderWorker(g, res, visited, neighbor);
    }
  }
  res.push_back(node);
}

std::vector<nodeId_t> dfsPostorderRecursive(const Graph &g) {
  std::unordered_set<nodeId_t> visited;
  std::vector<nodeId_t> res;

  auto startNode = g.getNodes()[0];
  dfsPostorderWorker(g, res, visited, startNode);
  return res;
}

std::vector<nodeId_t> dfsPostorderIterative(const Graph &g) {
  // Visited set for when we first visit a node
  std::unordered_set<nodeId_t> visited;
  // Pairs {nodeId, isSecondVisit}. isSecondVisit == true means
  // we're seeing this the second time and ready to store to result
  // to mimic postorder
  std::stack<std::pair<nodeId_t, bool>> workStack;
  std::vector<nodeId_t> res;

  auto startNode = g.getNodes()[0];
  workStack.emplace(startNode, false);
  while (!workStack.empty()) {
    auto node = workStack.top();
    auto nodeId = node.first;
    workStack.pop();
    if (node.second) {
      res.push_back(nodeId);
    } else if (!visited.contains(nodeId)) {
      visited.insert(nodeId);
      workStack.emplace(nodeId, true);
      auto neighbors = g.getNeighbors(nodeId);
      // Reverse to match recursive neighbor visitation order
      for (auto neighborIt = neighbors.rbegin(); neighborIt != neighbors.rend();
           ++neighborIt) {
        workStack.emplace(*neighborIt, false);
      }
    }
  }

  return res;
}

std::tuple<bool, nodeId_t, nodeId_t> detectCyclesDfs(const Graph &g) {
  std::stack<nodeId_t> workStack;
  std::unordered_set<nodeId_t> visited;
  std::unordered_set<nodeId_t> recursiveStack;

  auto startNode = g.getNodes()[0];
  workStack.push(startNode);
  while (!workStack.empty()) {
    // Look at next element
    auto node = workStack.top();
    workStack.pop();
    if (recursiveStack.contains(node)) {
      // We've recursed on this node and are now returning from recursion
      recursiveStack.erase(node);
    } else {
      // If unvisited, recurse on this node. I.e. push neighbors and
      // check for back edges
      auto [_, isUnvisited] = visited.insert(node);
      if (isUnvisited) {
        // "Recurse" on node
        recursiveStack.insert(node);
        workStack.push(node);
        auto neighbors = g.getNeighbors(node);
        for (auto neighbor : neighbors) {
          if (recursiveStack.contains(neighbor)) {
            return {true, node, neighbor};
          }
          workStack.push(neighbor);
        }
      }
    }
  }
  return {false, std::numeric_limits<nodeId_t>::max(),
    std::numeric_limits<nodeId_t>::max()};
}

// TODO matrix multiply optimization
// TODO basic constant folding
} // namespace ry
