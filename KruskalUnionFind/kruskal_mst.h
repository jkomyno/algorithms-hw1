#ifndef KRUSKAL_MST_H
#define KRUSKAL_MST_H

#include <algorithm>  // std::sort
#include <vector>     // std::vector

#include "AdjListGraph.h"
#include "DisjointSet.h"

template <typename Label, typename Weight>
auto kruskal_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {
    std::vector<Edge<Label, Weight>> mst;

    auto vertexes = adj_list_graph.get_vertexes();
    auto edges = adj_list_graph.get_edges();
    const size_t n = vertexes.size();
    const size_t n_stop = n - 1;

    /**
     * Create a new Disjoint-Set data structure to store the vertexes.
     * Initially, every vertex is in a separate set.
     * vertexes is no longer accessible after the process.
     */
    DisjointSet<Label> disjoint_set(std::move(vertexes));

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

        const auto& v = edge.get_from();
        const auto& w = edge.get_to();

        if (disjoint_set.find(v) != disjoint_set.find(w)) {
            mst.push_back(edge);
            disjoint_set.unionBySize(v, w);
        }
    }

    return mst;
}

#endif  // KRUSKAL_MST_H
