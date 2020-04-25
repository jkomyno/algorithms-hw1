#ifndef SUM_WEIGHTS_H
#define SUM_WEIGHTS_H

#include <iterator>     // std::iterator_traits
#include <numeric>      // std::accumulate
#include <type_traits>  // std::enable_if, std::is_same

/**
 * Compute the sum of the weights of every edge in mst. It expects in input the
 * cbegin() and cend() iterator of the data structure holding the MST.
 */
template <typename Label, typename Weight, typename It,
          typename = typename std::enable_if<std::is_same<
              typename std::iterator_traits<It>::value_type, Edge<Label, Weight>>::value>::type>
Weight sum_weights(const It& cbegin, const It& cend) noexcept {
    return std::accumulate(cbegin, cend, Weight(0), [](const auto& acc, const auto& elem) {
        return acc + elem.weight;
    });
}

#endif  // SUM_WEIGHTS_H
