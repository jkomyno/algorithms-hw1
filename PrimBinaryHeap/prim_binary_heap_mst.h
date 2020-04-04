#pragma once

#include <algorithm>     // std::transform
#include <limits>        // std::numeric_limits
#include <vector>        // std::vector
#include <queue>         // std::priority_queue
#include "AdjListGraph.h"
#include "BinaryHeap.h"

template <typename Label, typename Weight>
auto prim_binary_heap_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {

    // type corresponding to the pair (weight, vertex). When pairs are ordered, they follow
    // the lexicographical order, so e.g. (1,5) < (1,7) < (2,1).
    // When used inside a Min Heap, this allows us to extract the vertex with the minimum
    // associated weight.
    // using vw_pair = std::pair<Weight, Label>;

    auto& vertexes = adj_list_graph.get_vertexes();
    size_t n_stop = vertexes.size() - 1;

    std::vector<Edge<Label, Weight>> mst(n_stop);

    // Keys are used to pick the lightest edge in cut
    std::vector<Weight> key;
    key.reserve(vertexes.size());

    // initialize keys of all vertexes as infinite.
    std::transform(vertexes.cbegin(), vertexes.cend(), std::back_inserter(key), [](const auto& v) {
        return std::numeric_limits<Weight>::max();
    });

    // keep track of vertexes added to the mst.
    // This is needed to make sure we don't include the same vertex in
    // "min_heap" multiple times.
    // All entries in in_mst are initialized as false.
    std::vector<bool> in_mst(vertexes.size(), false);

    // the source vertex can be randomly chosen. For simplicity, we choose the first vertex available
    const Label SOURCE_VERTEX = vertexes.at(0);

    // the first vertex is distant 0 from itself
    key.at(SOURCE_VERTEX) = Weight(0);

    /*
    std::vector<vw_pair> vw_pairs;
    vw_pairs.reserve(vertexes.size());
    std::transform(vertexes.cbegin(), vertexes.cend(), std::back_inserter(vw_pairs), [&key](const auto& v) {
        return std::make_pair(key.at(v), v);
    });
    */

    // Min Heap ordered by weight, from smaller to larger.
    // auto min_heap = make_min_heap(std::move(vw_pairs));

    // Min Heap that uses nodes' keys as comparison criterion
    auto key_comp = [&key](auto&& a, auto&& b) -> bool {
        return key.at(a) > key.at(b);
    };
    BinaryHeap<Label, decltype(key_comp)> min_heap(std::move(vertexes), std::move(key_comp));

    // insert first vertex to the priority queue
    // min_heap.push(std::make_pair(key.at(SOURCE_VERTEX), SOURCE_VERTEX));

    // variable that keeps track of the number of nodes in the Minimum Spanning Tree
    size_t n_in_mst = 0;

    while (n_in_mst < n_stop) {
        // u is the vertex with minimum key that belongs to the lightest edge of the cut
        auto u = min_heap.top();
        min_heap.pop();

        // the vertex u will now be put in the mst
        in_mst.at(u) = true;
        n_in_mst++;

        // traverse all vertexes which are adjacent to u
        for (const auto vw : adj_list_graph.get_adjacent_vertexes(u)) {
            const auto v = vw.vertex;
            const auto weight = vw.weight; // w(u, v)

            // if v is not in MST and w(u, v) is smaller than the current key of v
            if (!in_mst.at(v) && weight < key.at(v)) {
                key.at(v) = weight;

                // TODO: reheap is used to ensure the heap property is preserved after a key
                // has changed. This operation costs O(N), where N is the number of elements in the heap.
                // We should make it cost O(logN) using an std::unordered_map to keep track of the
                // indices of each node
                min_heap.reheap();

                mst.at(v) = Edge<Label, Weight>(u, v, weight);
            }
        }
    }

    return mst;
}
