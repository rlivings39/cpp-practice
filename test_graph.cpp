#include "graph.hpp"
#include <gtest/gtest.h>
#include <limits>

TEST(Graph, AdjacencyListConstruct) {
  ry::AdjacencyListGraph g;
  ASSERT_EQ(g.getNodes(), std::vector<ry::nodeId_t>());
}

TEST(Graph, AdjacencyListAddNodes) {
  ry::AdjacencyListGraph g;
  g.add_node(1);
  g.add_node(3);
  ASSERT_EQ(g.getNodes(), std::vector<ry::nodeId_t>({1, 3}));
}

TEST(Graph, AdjacencyListAddEdges) {
  ry::AdjacencyListGraph g;
  g.add_node(1);
  g.add_node(3);
  ASSERT_EQ(g.getNodes(), std::vector<ry::nodeId_t>({1, 3}));
  ASSERT_FALSE(g.isAdjacent(1, 3));
  ASSERT_FALSE(g.isAdjacent(3, 1));

  g.add_edge(1, 3);
  ASSERT_TRUE(g.isAdjacent(1, 3));
  // Note: Our graph is directed
  ASSERT_FALSE(g.isAdjacent(3, 1));
}

TEST(Graph, AdjacencyListNeighbors) {
  ry::AdjacencyListGraph g;
  g.add_node(1);
  g.add_node(3);
  g.add_node(17);

  g.add_edge(1, 3);
  g.add_edge(1, 17);
  ASSERT_EQ(g.getNeighbors(1), std::vector<ry::nodeId_t>({3, 17}));
  ASSERT_EQ(g.getNeighbors(3), std::vector<ry::nodeId_t>());
  ASSERT_EQ(g.getNeighbors(17), std::vector<ry::nodeId_t>());
}

TEST(Graph, AdjacencyListRemoveNode) {
  ry::AdjacencyListGraph g;
  g.add_node(1);
  g.add_node(3);
  g.add_node(17);

  g.add_edge(1, 3);
  g.add_edge(1, 17);
  g.add_edge(3, 17);
  ASSERT_EQ(g.getNeighbors(1), std::vector<ry::nodeId_t>({3, 17}));
  ASSERT_EQ(g.getNeighbors(3), std::vector<ry::nodeId_t>({17}));
  ASSERT_EQ(g.getNodes()[2], 17);

  g.remove_node(17);
  ASSERT_EQ(g.getNodes().size(), 2);
  ASSERT_EQ(g.getNeighbors(1), std::vector<ry::nodeId_t>({3}));
  ASSERT_EQ(g.getNeighbors(3), std::vector<ry::nodeId_t>());
}

TEST(Graph, AdjacencyListBfs) {
  ry::AdjacencyListGraph g;
  g.add_node(0);
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  g.add_node(4);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 4);
  g.add_edge(4, 1);
  ASSERT_EQ(ry::bfs(g), std::vector<ry::nodeId_t>({0, 1, 2, 3, 4}));
  ASSERT_EQ(ry::bfs2(g), std::vector<ry::nodeId_t>({0, 1, 2, 3, 4}));
}

TEST(Graph, AdjacencyListDfs) {
  ry::AdjacencyListGraph g;
  g.add_node(0);
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  g.add_node(4);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 4);
  g.add_edge(4, 1);
  ASSERT_EQ(ry::dfsPreorder(g), std::vector<ry::nodeId_t>({0, 2, 4, 1, 3}));
  ASSERT_EQ(ry::dfsPreorder2(g), std::vector<ry::nodeId_t>({0, 2, 4, 1, 3}));
  ASSERT_EQ(ry::dfsPostorderRecursive(g),
            std::vector<ry::nodeId_t>({4, 3, 1, 2, 0}));
  ASSERT_EQ(ry::dfsPostorderRecursive2(g),
            std::vector<ry::nodeId_t>({4, 3, 1, 2, 0}));
  ASSERT_EQ(ry::dfsPostorderIterative(g),
            std::vector<ry::nodeId_t>({4, 3, 1, 2, 0}));
  ASSERT_EQ(ry::dfsPostorderIterative2(g),
            std::vector<ry::nodeId_t>({4, 3, 1, 2, 0}));
}

TEST(Graph, AdjacencyListDfsOrders) {
  ry::AdjacencyListGraph g;
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  g.add_node(4);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 4);

  ASSERT_EQ(ry::dfsPreorder(g), std::vector<ry::nodeId_t>({1, 3, 4, 2}));
  ASSERT_EQ(ry::dfsPostorderRecursive(g),
            std::vector<ry::nodeId_t>({4, 2, 3, 1}));
  ASSERT_EQ(ry::dfsPostorderIterative(g),
            std::vector<ry::nodeId_t>({4, 2, 3, 1}));
}

TEST(Graph, AdjacencyListCycleDetection) {
  ry::AdjacencyListGraph g;
  g.add_node(1);
  g.add_node(2);
  g.add_node(3);
  g.add_node(4);
  g.add_edge(1, 2);
  g.add_edge(1, 3);
  g.add_edge(2, 4);
  g.add_edge(3, 4);

  auto idmax = std::numeric_limits<ry::nodeId_t>::max();
  ASSERT_EQ(ry::detectCyclesDfs(g), std::make_tuple(false, idmax, idmax));

  g.add_edge(4, 1);
  ASSERT_EQ(ry::detectCyclesDfs(g), std::make_tuple(true, 4, 1));
}
