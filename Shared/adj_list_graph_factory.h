#pragma once

#include <iostream> // std::cin
#include <vector>   // std::vector
#include <type_traits>
#include "AdjListGraph.h"
#include "map_index.h"

/**
 * Helper that reads a graph from a text file.
 * We assume that the nodes are labeled with a label x,
 * where 1 <= x <= n (n is the number of nodes).
 * The nodes are going to be saved into the adjacency list
 * class AdjListGraph with their label decremented by 1
 * (thus 0 <= x' <= n-1), to simplify the book-keeping in the
 * MST algorithm implementations.
 *
 * Label is the type of a node label, which must be an unsigned integer.
 * Weight is the type of an edge weight, which must be a signed integer.
 */
template <typename Label, typename Weight,
    typename = typename std::enable_if<
        std::is_unsigned<Label>::value
    >::type,
    typename = typename std::enable_if<
        std::is_signed<Weight>::value
    >::type
>
AdjListGraph<Label, Weight> adj_list_graph_factory() {
    size_t n; // number of vertexes
    size_t m; // number of edges
    std::cin >> n >> m;

    std::vector<Edge<Label, Weight>> edge_list;
    edge_list.reserve(m);

    Label u;
    Label v;
    Weight w;

    size_t i = m;
    while (i > 0) {
        std::cin >> u >> v >> w;
        edge_list.emplace_back(u - 1, v - 1, w);
        i--;
    }

    // after the creation of adjListGraph, edgeList is no longer accessible
    AdjListGraph<Label, Weight> adj_list_graph(std::move(edge_list), n);

    return adj_list_graph;
}
