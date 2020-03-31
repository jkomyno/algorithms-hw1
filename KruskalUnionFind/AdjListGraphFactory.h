#pragma once

#include <iostream> // std::cin
#include <vector>   // std::vector
#include "AdjListGraph.h"

template <typename Label, typename Weight>
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
        edge_list.emplace_back(Edge<Label, Weight>(u, v, w));
        i--;
    }

    // after the creation of adjListGraph, edgeList is no longer accessible
    AdjListGraph<Label, Weight> adj_list_graph(std::move(edge_list), n);

    return adj_list_graph;
}
