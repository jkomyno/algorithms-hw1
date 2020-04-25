#ifndef ADJACENCY_MAP_GRAPH_FACTORY_H
#define ADJACENCY_MAP_GRAPH_FACTORY_H

#include <iostream>     // std::cin
#include <type_traits>  // std::enable_if, std::is_unsigned, std::is_signed
#include <vector>       // std::vector

#include "AdjacencyMapGraph.h"

/**
 * Helper that reads a graph from a text file.
 * We assume that the nodes are labeled with a label x,
 * where 1 <= x <= n (n is the number of nodes).
 * The nodes are going to be saved into the adjacency map
 * class AdjacencySetGraph with their label decremented by 1
 * (thus 0 <= x' <= n-1), to simplify the book-keeping in the
 * MST algorithm implementations.
 * Creating a graph representation from an input file takes O(m * (n + m)) time.
 *
 * Label is the type of a node label, which must be an unsigned integer.
 * Weight is the type of an edge weight, which must be a signed integer.
 */
template <typename Label, typename Weight,
          typename = typename std::enable_if<std::is_unsigned<Label>::value>::type,
          typename = typename std::enable_if<std::is_signed<Weight>::value>::type>
AdjacencyMapGraph<Label, Weight> adjacency_map_graph_factory() {
    size_t n;  // number of vertexes
    size_t m;  // number of edges
    std::cin >> n >> m;

    // vector that stores the edges read from the input file
    std::vector<Edge<Label, Weight>> edge_list;
    edge_list.reserve(m);

    Label u;
    Label v;
    Weight w;

    size_t i = m;
    while (i > 0) {
        /**
         * For each row of the input file, we read a weighted edge.
         * The edge between u and v weights w.
         * Time: O(m)
         */
        std::cin >> u >> v >> w;
        edge_list.emplace_back(u - 1, v - 1, w);
        i--;
    }

    // after the creation of adj_map_graph, edge_list is no longer accessible
    AdjacencyMapGraph<Label, Weight> adj_map_graph(std::move(edge_list), n);

    return adj_map_graph;
}

#endif  // ADJACENCY_MAP_GRAPH_FACTORY_H
