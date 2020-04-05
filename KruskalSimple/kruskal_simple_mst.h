#ifndef KRUSKAL_SIMPLE_MST_H
#define KRUSKAL_SIMPLE_MST_H

#include <algorithm>  // std::sort, std::copy
#include <vector>     // std::vector

#include "AdjListGraph.h"
#include "dfs.h"

template <typename Label, typename Weight>
auto kruskal_simple_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {
    // object representing a Minimum Spanning Tree
    std::vector<Edge<Label, Weight>> mst;

    auto edges = adj_list_graph.get_edges();
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
		const size_t mst_size = mst.size();
        if (mst_size == n_stop) {
            return mst;
        }

    	// pushing a node to the mst vector and maybe pop it back is much more
    	// performant and memory-efficient than constructing a whole new vector
    	// any time, copying mst in it and adding the current edge to it.
		mst.emplace_back(edge);

        // dfs is a utility object that uses Depth First Search to determine whether there is
        // any loop in the given graph.
        DFS<Label, Weight> dfs(mst, mst_size);

        if (!dfs.is_acyclic()) {
            // if there's a cycle in (mst + edge), remove the last inserted edge from mst
			mst.pop_back();
        }
    }

    return mst;
}

#endif  // KRUSKAL_SIMPLE_MST_H
