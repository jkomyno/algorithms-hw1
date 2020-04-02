#pragma once

#include <vector>    // std::vector
#include <algorithm> // std::sort
#include "AdjListGraph.h"

template <typename Label, typename Weight>
auto kruskal_simple_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {
    std::vector<Edge<Label, Weight>> mst;

    auto& edges = adj_list_graph.get_edges();
    const size_t n = adj_list_graph.vertexes_size();
    const size_t n_stop = n - 1;

    /**
     * Sort edges in non-decreasing order. edges is modified after the process.
     */
    auto comparator = [](const auto& l, const auto& r) {
        return l.get_weight() < r.get_weight();
    };
    std::sort(edges.begin(), edges.end(), comparator);

    for (const auto& edge : edges) {
        // a Minimum Spanning Tree can have (n - 1) edges at maximum.
        if (mst.size() == n_stop) {
            return mst;
        }

        // ...
    }

    return mst;
}
