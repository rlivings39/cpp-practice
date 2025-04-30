#include "graph.hpp"
#include <gtest/gtest.h>

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
    ASSERT_FALSE(g.isAdjacent(3,1));

    g.add_edge(1, 3);
    ASSERT_TRUE(g.isAdjacent(1, 3));
    // Note: Our graph is directed
    ASSERT_FALSE(g.isAdjacent(3, 1));
}
