#ifndef PRIM_K_HEAP_MST_H
#define PRIM_K_HEAP_MST_H

#include <algorithm>  // std::transform
#include <limits>     // std::numeric_limits
#include <vector>     // std::vector

#include "AdjacencyMapGraph.h"
#include "PriorityQueue.h"

template <typename Label, typename Weight>
std::vector<Edge<Label, Weight>> prim_k_heap_mst(AdjacencyMapGraph<Label, Weight>&& adj_map_graph) noexcept {
	auto vertexes = adj_map_graph.get_vertexes();

	const size_t n_stop = vertexes.size();
	std::vector<Edge<Label, Weight>> mst(n_stop);

	// Keys are used to pick the lightest edge in cut.
	// Initially, keys of all vertexes are set to infinity.
	std::vector<Weight> keys;
	keys.reserve(vertexes.size());
	std::transform(vertexes.cbegin(), vertexes.cend(), std::back_inserter(keys), [](const auto&) {
		return std::numeric_limits<Weight>::max();
		});

	// the source vertex can be randomly chosen. For simplicity, we choose the first vertex available.
	// the first vertex is distant 0 from itself
	keys.at(0) = Weight(0);

	// Priority Queue based on a Min Heap ordered by keys, from smaller to larger.
	// We don't need the O(N) first to reorder the vertexes as a heap, since keys is already
	// a valid heap. The priority queue creation thus takes O(1) time
	constexpr size_t K = 4;
	constexpr bool IsAlreadyHeap = true;
	auto min_pq(priority_queue::make_min_k_priority_queue<K, IsAlreadyHeap>(std::move(keys), std::move(vertexes)));
	
	while (!min_pq.empty()) {
		// u is the vertex with minimum key that belongs to the lightest edge of the cut
		auto u = min_pq.top();
		min_pq.pop();

		// traverse all vertexes which are adjacent to u
		for (const auto [v, weight] : adj_map_graph.adjacent_vertexes(u)) {
			// if v is not in MST and w(u, v) is smaller than the current key of v
			if (min_pq.contains(v) && weight < min_pq.key_at(v)) {
				// update the key associated with node v in O(logN), where N is the number
				// of elements stored in the priority queue
				min_pq.update_key(weight, v);

				// add the edge (u, v) to the Minimum Spanning Tree
				mst.at(v) = Edge<Label, Weight>(u, v, weight);
			}
		}
	}

	return mst;
}

#endif  // PRIM_K_HEAP_MST_H
