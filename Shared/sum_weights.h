#ifndef SUM_WEIGHTS_H
#define SUM_WEIGHTS_H

#include <numeric>  // std::accumulate
#include <vector>   // std::vector

#include "AdjListGraph.h"

/**
 * Compute the sum of the weights of every edge in mst. It expects in input the
 * cbegin() and cend() iterator of the data structure holding the MST.
 */
template <typename Label, typename Weight, typename It, typename = typename std::enable_if<
	std::is_same<typename std::iterator_traits<It>::value_type,
	Edge<Label, Weight>>::value
>::type>
Weight sum_weights(const It& cbegin, const It& cend) noexcept {
    return std::accumulate(cbegin, cend, Weight(0), [](const auto acc, const auto elem) {
        return acc + elem.get_weight();
    });
}

#endif  // SUM_WEIGHTS_H
