#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ry {

using nodeId_t = std::size_t;

class Graph {
public:
  explicit Graph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const = 0;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const = 0;
  virtual void add_node(nodeId_t node) = 0;
  virtual void remove_node(nodeId_t node) = 0;
  virtual void add_edge(nodeId_t src, nodeId_t dest) = 0;
  virtual std::vector<nodeId_t> getNodes() const = 0;
};

class AdjacencyMatrixGraph : public Graph {
public:
  explicit AdjacencyMatrixGraph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const override;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const override;
  virtual void add_node(nodeId_t node) override;
  virtual void remove_node(nodeId_t node) override;
  virtual void add_edge(nodeId_t src, nodeId_t dest) override;
};

struct InvalidNodeIdError {
  InvalidNodeIdError(std::string message) : fMessage(message) {}
  std::string what() const {
    return fMessage;
  }

private:
  std::string fMessage = nullptr;
};
class AdjacencyListGraph : public Graph {
public:
  explicit AdjacencyListGraph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const override;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const override;
  virtual void add_node(nodeId_t node) override;
  virtual void remove_node(nodeId_t node) override;
  virtual void add_edge(nodeId_t src, nodeId_t dest) override;
  virtual std::vector<nodeId_t> getNodes() const override;

  using AdjacencyList_t = std::unordered_map<nodeId_t, std::unordered_set<nodeId_t>>;

private:
  // Maps a given node id, src to the list of adjacent node ids, dst[i]
  // such that there is an edge from src to dst[i] for each i
  AdjacencyList_t fAdjacencyList;
};

class NodeAndEdgeGraph : public Graph {
public:
  explicit NodeAndEdgeGraph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const override;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const override;
  virtual void add_node(nodeId_t node) override;
  virtual void remove_node(nodeId_t node) override;
  virtual void add_edge(nodeId_t src, nodeId_t dest) override;
};

// Various visitation methods that all return a vector of
// node IDs in the order they were visited
std::vector<nodeId_t> bfs(const Graph &g);
std::vector<nodeId_t> dfsPreorder(const Graph &g);
std::vector<nodeId_t> dfsPostorderRecursive(const Graph &g);
std::vector<nodeId_t> dfsPostorderIterative(const Graph &g);

/**
 * @brief Detects if g contains a cycle. First tuple element
 * is true if a cycle is found and false otherwise. If true
 * there was a back edge fround from tuple[1] to tuple[2]
 *
 * @param g
 * @return std::tuple<bool, nodeId_t, nodeId_t>
 */
std::tuple<bool, nodeId_t, nodeId_t> detectCyclesDfs(const Graph &g);
} // namespace ry

