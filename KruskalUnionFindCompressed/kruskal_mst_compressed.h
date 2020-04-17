#ifndef KRUSKAL_MST_COMPRESSED_H
#define KRUSKAL_MST_COMPRESSED_H

#include <functional> // std::less
#include <vector>     // std::vector

#include "AdjacencyMapGraph.h"
#include "DisjointSetCompressed.h"

template <typename Label, typename Weight>
auto kruskal_mst_compressed(AdjacencyMapGraph<Label, Weight>&& adj_map_graph) noexcept -> std::vector<Edge<Label, Weight>> {
	// this vector will store the Minimum Spanning Tree
	std::vector<Edge<Label, Weight>> mst;
	const size_t n_stop = adj_map_graph.vertexes_size() - 1;
	mst.reserve(n_stop);

	// sort edges in non-decreasing order of weight in O(mlogm) time
	auto edges = adj_map_graph.get_sorted_edges(std::less<>{});

	// generate vector of vertexes in O(n) time
    auto vertexes = adj_map_graph.get_vertexes();

	/**
	 * Create a new Disjoint-Set data structure to store the vertexes.
	 * Initially, every vertex is in a separate set.
	 * vertexes is no longer accessible after the process.
	 */
	DisjointSetCompressed<Label> disjoint_set(std::move(vertexes));

	// a Minimum Spanning Tree can have (n - 1) edges at maximum.
	// The mst is populated in O(mlog*(n)) time
	for (auto it = edges.cbegin(); !(it == edges.cend() || mst.size() == n_stop); ++it) {
		const auto& edge = *it;
		const auto& [v, w, _] = edge;

		// O(log*(n)) ~ O(1)
		if (!disjoint_set.are_connected(v, w)) {
			mst.push_back(edge);
			disjoint_set.unite(v, w);
		}
	}

	return mst;
}

#endif  // KRUSKAL_MST_COMPRESSED_H
