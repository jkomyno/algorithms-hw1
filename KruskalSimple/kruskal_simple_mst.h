#ifndef KRUSKAL_SIMPLE_MST_H
#define KRUSKAL_SIMPLE_MST_H

#include <algorithm>  // std::sort, std::copy
#include <vector>     // std::vector

#include "../Shared/AdjListGraph.h"
#include "../Shared/dfs.h"

template <typename Label, typename Weight>
auto kruskal_simple_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {
    // object representing a Minimum Spanning Tree
    std::vector<Edge<Label, Weight>> mst;

    auto& edges = adj_list_graph.get_edges();
    const size_t n = adj_list_graph.vertexes_size();
    const size_t n_stop = n - 1;

    /**
     * Sort edges in non-decreasing order of weight. edges is modified after the process.
     */
    auto comparator = [](const auto& l, const auto& r) {
        return l.get_weight() < r.get_weight();
    };
    std::sort(edges.begin(), edges.end(), std::move(comparator));

    for (const auto& edge : edges) {
        // a Minimum Spanning Tree can have (n - 1) edges at maximum.
        if (mst.size() == n_stop) {
            return mst;
        }

        // mst_and_edge is a copy of mst + the current edge
        std::vector<Edge<Label, Weight>> mst_and_edge(mst.size());
        mst_and_edge.reserve(mst.size() + 1);
        std::copy(mst.cbegin(), mst.cend(), mst_and_edge.begin());
        mst_and_edge.emplace_back(edge);

        // graph representation of mst_and_edge
        AdjListGraph<Label, Weight> tmp_adj_list_graph(std::move(mst_and_edge), mst.size());

        // dfs is a utility object that uses Depth First Search to determine whether there is
        // any loop in the given graph.
        DFS<Label, Weight> dfs(std::move(tmp_adj_list_graph));

        if (dfs.is_acyclic()) {
            // if there's no cycle in (mst + edge), add edge to the Minimum Spanning Tree
            mst.emplace_back(edge);
        }
    }

    return mst;
}

#endif  // KRUSKAL_SIMPLE_MST_H
