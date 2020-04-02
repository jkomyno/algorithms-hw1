#pragma once

/**
 * Utility function that maps a vertex to an index. The image of this function should
 * be [0, n-1], where n is the number of vertexes in a graph.
 * This function is necessary because some data structures (e.g. DisjointSet or BinaryHeap)
 * assume that the nodes are given as size_t numbers in [0, n-1], whereas the graph nodes given
 * in input are in the domain [1, n].
 */
template <typename T>
size_t map_to_index(const T& vertex) noexcept {
    return vertex - 1;
}

/**
 * Inverse of "map_to_index", used to convert back an index in [0, n-1] to the original
 * vertex representation.
 */
template <typename T>
T map_from_index(const size_t index) noexcept {
    return index + 1;
}
