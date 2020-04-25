#ifndef KRUSKAL_NAIVE_MST_H
#define KRUSKAL_NAIVE_MST_H

#include <functional>    // std::less
#include <unordered_set> // std::unordered_set

#include "AdjacencyMapGraph.h"
#include "DFSCycleDetection.h"

template <typename Label, typename Weight>
auto kruskal_simple_mst(AdjacencyMapGraph<Label, Weight>&& adj_map_graph) noexcept -> std::unordered_set<Edge<Label, Weight>, custom_hash::edge_hash> {
    const size_t n = adj_map_graph.vertexes_size();

	// adjacency map that will store the Minimum Spanning Tree
	AdjacencyMapGraph<Label, Weight> mst_set_graph({}, n);

	// sort edges in non-decreasing order of weight in O(m * log(m)) time
	auto edges = adj_map_graph.get_sorted_edges(std::less<>{});

	// object that detects cycles in a graph using Depth First Search.
	// dfs uses mst_set_graph in read-only mode.
	DFSCycleDetection<Label, Weight> dfs(&mst_set_graph);

	// Iterate over the edges, stop early if the MST reached its maximum size (n vertexes).
	// The mst is populated in O(m*n) time
	for (auto it = edges.cbegin(); !(it == edges.cend() && n == mst_set_graph.vertexes_size()); ++it) {
        // edge is the object pointed by the current iterator
	    const auto& edge = *it;

        // add the current edge to the spanning tree
		mst_set_graph.add_edge(edge);

        // detect a cycle in O(n' + m'), where n' is the number of vertexes and m'
        // it the number of edges currently present in mst_set_graph
		if (dfs.has_cycle()) {
    		// remove edge from the spanning tree in O(1) because it introduced a cycle.
    		// This is much cheaper than constructing a new AdjacencyMapGraph
    		// object every time we need to detect a cycle
			mst_set_graph.remove_edge(edge);
    	}
	}

	// std::unordered_set representing a Minimum Spanning Tree, returned in O(1) time
	return mst_set_graph.get_edges();
}

#endif  // KRUSKAL_NAIVE_MST_H
