#pragma once

#include <vector>    // std::vector
#include <numeric>   // std::accumulate
#include "AdjListGraph.h"

/**
 * Compute the sum of the weights of every edge in mst.
 */
template <typename Label, typename Weight>
auto sum_weights(const std::vector<Edge<Label, Weight>>& mst) -> Weight {
    return std::accumulate(mst.cbegin(), mst.cend(), Weight(0), [](const auto acc, const auto elem) {
        return acc + elem.get_weight();
    });
}
