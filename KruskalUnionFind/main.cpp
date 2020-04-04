// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include "adj_list_graph_factory.h"
#include "kruskal_mst.h"
#include "sum_weights.h"

int main() {
    typedef size_t Label; // nodes are identified by size_t type
    typedef long Weight; // weights are of type long

    AdjListGraph<Label, Weight> adj_list_graph(adj_list_graph_factory<Label, Weight>());

    /*
    std::cout << adj_list_graph;

    const auto& vertexes = adj_list_graph.get_vertexes();
    const auto& edges = adj_list_graph.get_edges();

    std::cout << std::endl << "Vertexes: " << std::endl;
    for (const auto& v : vertexes) {
        std::cout << v << "; ";
    }

    std::cout << std::endl;
    std::cout << std::endl << "Edges: " << std::endl;
    for (const auto& e : edges) {
        std::cout << e;
    }
    */

    // compute Minimum Spanning Tree with Kruskal algorithm using Disjoint-Set data structure
    const auto& mst = kruskal_mst(std::move(adj_list_graph));

    // total weight of the mst found by Kruskal's algorithm
    const auto total_weight = sum_weights<Label, Weight>(mst.cbegin(), mst.cend());

    // use std::fixed to avoid displaying numbers in scientific notation
    std::cout << std::fixed << total_weight << std::endl;
}
