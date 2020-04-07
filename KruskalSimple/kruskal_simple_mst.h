#ifndef KRUSKAL_SIMPLE_MST_H
#define KRUSKAL_SIMPLE_MST_H

#include <algorithm>  // std::sort
#include <vector>     // std::vector

#include "AdjListGraph.h"
#include "DFSCycleDetection.h"

template <typename Label, typename Weight>
auto kruskal_simple_mst(AdjListGraph<Label, Weight>&& adj_list_graph) noexcept -> std::vector<Edge<Label, Weight>> {

	auto edges = adj_list_graph.get_edges(false);
    const size_t n = adj_list_graph.vertexes_size();
    const size_t n_stop = n - 1;

    /**
     * Sort edges in non-decreasing order of weight. edges is modified after the process.
     */
    std::sort(edges.begin(), edges.end(), [](const auto& l, const auto& r) {
		return l.get_weight() < r.get_weight();
	});

	// adjacency list 
	AdjListGraph<Label, Weight> mst_list_graph;

	// object that detects cycles in a graph using Depth First Search.
	// It requires a constant pointer to mst_list_graph.
	DFSCycleDetection<Label, Weight> dfs(&mst_list_graph);

	size_t mst_size = 0;
	
    for (auto& edge : edges) {
        // a Minimum Spanning Tree can have (n - 1) edges at maximum.
    
        if (mst_size == n_stop) {
			break;
        }
		
		mst_list_graph.add_edge(edge);

    	if (dfs.has_cycle()) {
    		// remove the last inserted entry in the adjacency list of nodes
    		// edge.get_from() and edge.get_to(). This is cheaper than constructing a new
    		// AdjListGraph<> object every time we need to detect a cycle
			mst_list_graph.remove_edge(edge);
    	} else {
			mst_size++;
    	}
    }

	// object representing a Minimum Spanning Tree
	return mst_list_graph.get_edges(false);
}

#endif  // KRUSKAL_SIMPLE_MST_H
