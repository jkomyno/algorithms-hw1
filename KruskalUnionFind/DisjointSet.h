#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <algorithm>    // std::transform
#include <functional>   // std::function
#include <type_traits>  // std::enable_if, std::is_unsigned
#include <vector>       // std::vector

/**
 * Union-by-size implementation of a Disjoint-Set.
 * DisjointSet accepts element of type T, which must be an unsigned integer type.
 * TODO:
 * - path compression may be used to obtain even better performance
 */
template <typename T, typename = typename std::enable_if<
    std::is_unsigned<T>::value>::type>
class DisjointSet {
    std::vector<size_t> parents;
    std::vector<size_t> sizes;

   public:
    // x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
    explicit DisjointSet(std::vector<T>&& x_list) : sizes(x_list.size(), 1)  // fill size with value 1
    {
        parents.reserve(x_list.size());
        std::transform(x_list.cbegin(), x_list.cend(), std::back_inserter(parents), [&x_list](const auto& x_elem) {
            // pointer arithmetic to obtain the index of the current element in x_list
            const size_t index = &x_elem - &(x_list[0]);
            return index;
        });
    }

    // default move constructor
    DisjointSet(DisjointSet&& disjoint_set) noexcept = default;

    // disable copy constructor and assignment operator
    DisjointSet(const DisjointSet& rhs) = delete;
    DisjointSet& operator=(const DisjointSet& rhs) = delete;

    // default move assignment operator
    DisjointSet& operator=(DisjointSet&& rhs) = default;

    // default destructor
    ~DisjointSet() = default;

    // returns the index of the representative of the unique set containing the given item.
    // T must be castable to size_t.
    size_t find(const T& item) {
        auto x = item;
        while (x != parents[x]) {
            x = parents[x];
        }
        return x;
    }

    // unites the dynamic sets that contain x and y into a new
    // set that is the union of these two sets.
    void unionBySize(const T& x, const T& y) {
        const size_t i = find(x);
        const size_t j = find(y);

        if (i != j) {
            const size_t sum_of_sizes = sizes[i] + sizes[j];
            if (sizes[i] >= sizes[j]) {
                parents[j] = i;
                sizes[i] = sum_of_sizes;
            } else {
                parents[i] = j;
                sizes[j] = sum_of_sizes;
            }
        }
    }
};

#endif  // DISJOINT_SET_H
