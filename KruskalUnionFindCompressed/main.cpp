#include <iostream>

#include "AdjacencyMapGraph.h"
#include "adjacency_map_graph_factory.h"
#include "sum_weights.h"
#include "kruskal_mst_compressed.h"

int main() noexcept {
	typedef size_t Label; // nodes are identified by size_t type
	typedef long Weight;  // weights are of type long

	AdjacencyMapGraph<Label, Weight> adj_map_graph(adjacency_map_graph_factory<Label, Weight>());

	// compute Minimum Spanning Tree with Kruskal algorithm using compressed Disjoint-Set data structure
	const auto& mst = kruskal_mst_compressed(std::move(adj_map_graph));

	// total weight of the mst found by Kruskal's algorithm
	const auto total_weight = sum_weights<Label, Weight>(mst.cbegin(), mst.cend());

	// use std::fixed to avoid displaying numbers in scientific notation
	std::cout << std::fixed << total_weight << std::endl;
}
