#include <iostream>
#include "adj_list_graph_factory.h"
#include "prim_binary_heap_mst.h"
#include "map_index.h"
#include "sum_weights.h"

int main()
{
    typedef size_t Label; // nodes are identified by size_t type
    typedef long Weight; // weights are of type long

    AdjListGraph<Label, Weight> adj_list_graph(adj_list_graph_factory<Label, Weight>());

    // compute Minimum Spanning Tree with Prim algorithm using Binary Heap
    // const auto& mst = prim_binary_heap_mst(std::move(adj_list_graph), map_to_index<Label>, map_from_index<Label>);
    
    // total weight of the mst found by Prim algorithm with Binary Heap
    // const auto total_weight = sum_weights<Label, Weight>(mst.cbegin(), mst.cend());

    // use std::fixed to avoid displaying numbers in scientific notation
    // std::cout << std::fixed << total_weight << std::endl;
}
