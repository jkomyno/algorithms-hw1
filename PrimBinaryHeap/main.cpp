#include <iostream>
#include "adj_list_graph_factory.h"
#include "prim_binary_heap_mst.h"
#include "sum_weights.h"

int main()
{
    typedef size_t Label; // nodes are identified by size_t type
    typedef size_t Weight; // weights are of type size_t

    AdjListGraph<size_t, size_t> adj_list_graph(adj_list_graph_factory<Label, Weight>());

    // compute Minimum Spanning Tree with Prim algorithm using Binary Heap
    const auto& mst = prim_binary_heap_mst(std::move(adj_list_graph));

    // total weight of the mst found by Kruskal's algorithm
    const auto total_weight = sum_weights(mst);

    // use std::fixed to avoid displaying numbers in scientific notation
    std::cout << std::fixed << total_weight << std::endl;
}
