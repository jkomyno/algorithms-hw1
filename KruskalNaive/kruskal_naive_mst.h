#ifndef KRUSKAL_NAIVE_MST_H
#define KRUSKAL_NAIVE_MST_H

#include <functional>    // std::less
#include <vector>        // std::vector
#include <unordered_set> // std::unordered_set

#include "AdjacencyMapGraph.h"
#include "DFSCycleDetection.h"

template <typename Label, typename Weight>
auto kruskal_naive_mst(AdjacencyMapGraph<Label, Weight>&& adj_map_graph) noexcept -> std::unordered_set<Edge<Label, Weight>, custom_hash::edge_hash> {
	const size_t n_stop = adj_map_graph.vertexes_size() - 1;

	// sort edges in non-decreasing order of weight in O(mlogm) time
	auto edges = adj_map_graph.get_sorted_edges(std::less<>{});

	// adjacency map that will store the Minimum Spanning Tree
	AdjacencyMapGraph<Label, Weight> mst_set_graph({}, n_stop + 1);

	// object that detects cycles in a graph using Depth First Search.
	// It requires a constant pointer to mst_list_graph.
	DFSCycleDetection<Label, Weight> dfs(&mst_set_graph);

	size_t mst_size = 0;

	// a Minimum Spanning Tree can have (n - 1) edges at maximum.
	// The mst is populated in O(m*n) time
	for (auto it = edges.cbegin(); !(it == edges.cend() || mst_size == n_stop); ++it) {
		const auto& edge = *it;

		mst_set_graph.add_edge(edge);

		if (dfs.has_cycle()) {
    		// remove the last inserted entry in the adjacency map of nodes
    		// edge.from and edge.to. This is cheaper than constructing a new
    		// AdjacencySetGraph<> object every time we need to detect a cycle
			mst_set_graph.remove_edge(edge);
    	} else {
			// we added a new edge that didn't result in a cycle. Our mst is thus one node bigger.
			mst_size++;
    	}

	}

	// object representing a Minimum Spanning Tree, returned in O(1) time
	return mst_set_graph.get_edges();
}

#endif  // KRUSKAL_NAIVE_MST_H
