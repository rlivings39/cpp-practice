#include <unordered_map>
#include <vector>
#pragma "once"

#include <list>

namespace ry {

using nodeId_t = std::size_t;

class Graph {
public:
  explicit Graph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const = 0;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const = 0;
  virtual void add_node(nodeId_t node) = 0;
  virtual void remove_vertex(nodeId_t node) = 0;
  virtual void add_edge(nodeId_t src, nodeId_t dest) = 0;
  virtual std::vector<nodeId_t> getNodes() const = 0;
};

class AdjacencyMatrixGraph : public Graph {
public:
  explicit AdjacencyMatrixGraph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const override;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const override;
  virtual void add_node(nodeId_t node) override;
  virtual void remove_vertex(nodeId_t node) override;
  virtual void add_edge(nodeId_t src, nodeId_t dest) override;
};

class AdjacencyListGraph : public Graph {
public:
  explicit AdjacencyListGraph() {}

  virtual bool isAdjacent(nodeId_t src, nodeId_t dest) const override;
  virtual std::vector<nodeId_t> getNeighbors(nodeId_t node) const override;
  virtual void add_node(nodeId_t node) override;
  virtual void remove_vertex(nodeId_t node) override;
  virtual void add_edge(nodeId_t src, nodeId_t dest) override;
  virtual std::vector<nodeId_t> getNodes() const override;

  using AdjacencyList_t = std::unordered_map<nodeId_t, std::vector<nodeId_t>>;
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
  virtual void remove_vertex(nodeId_t node) override;
  virtual void add_edge(nodeId_t src, nodeId_t dest) override;
};
} // namespace ry
