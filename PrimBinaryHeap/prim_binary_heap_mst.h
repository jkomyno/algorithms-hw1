#pragma once

#include <vector>    // std::vector
#include <algorithm> // std::sort
#include "AdjListGraph.h"
#include "BinaryHeap.h"

template <typename Label, typename Weight>
auto prim_binary_heap_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {
    std::vector<Edge<Label, Weight>> mst;

    auto& vertexes = adj_list_graph.get_vertexes();
    auto& edges = adj_list_graph.get_edges();

    for (const auto& vertex : vertexes) {
        // ...
    }

    // As it is, this probably isn't right. heap should sort vertexes according to their
    // weight, so that heap.extract_min is a vertex incident to a light edge for the cut
    BinaryHeap<Label> heap(std::move(vertexes));

    while (heap.size() > 0) {
        const auto& u = heap.extract_min();
        // ...
    }

    return mst;
}
