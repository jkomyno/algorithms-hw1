#pragma once

#include <vector>     // std::vector
#include <algorithm>  // std::transform
#include <functional> // std::function

/**
 * Union-by-size implementation of a Disjoint-Set.
 * TODO:
 * - path compression may be used to obtain even better performance
 */
template <typename Element>
class DisjointSet {
    typedef std::function<size_t(const Element&)> ToIndexF;

    std::vector<size_t> parents;
    std::vector<size_t> sizes;

    ToIndexF to_index_f;

public:
    explicit DisjointSet(std::vector<Element>&& x_list, ToIndexF&& to_index) :
        sizes(x_list.size(), 1), // fill size with 1s
        to_index_f(to_index)
    {
        parents.reserve(x_list.size());
        std::transform(x_list.cbegin(), x_list.cend(), std::back_inserter(parents), [&x_list](const auto& x_elem) {
            // pointer arithmetic to obtain the index of the current element in x_list
            const size_t index = &x_elem - &(x_list[0]);
            return index;
        });
    }

    DisjointSet(DisjointSet&& disjoint_set) noexcept {
        parents = disjoint_set.parents;
        sizes = disjoint_set.sizes;
        to_index_f = disjoint_set.to_index_f;
    }

    // disable copy constructor and assignment operator
    DisjointSet(const DisjointSet& rhs) = delete;
    DisjointSet& operator=(const DisjointSet& rhs) = delete;

    // default move assignment operator
    DisjointSet& operator=(DisjointSet&& rhs) = default;

    // default destructor
    ~DisjointSet() = default;

    size_t find(const Element& element) {
        size_t x = to_index_f(element);
        while (x != parents[x]) {
            x = parents[x];
        }
        return x;
    }

    void unionBySize(const Element& x, const Element& y) {
        const size_t i = find(x);
        const size_t j = find(y);

        if (i != j) {
            const size_t sum_of_sizes = sizes[i] + sizes[j];
            if (sizes[i] >= sizes[j]) {
                parents[j] = i;
                sizes[i] = sum_of_sizes;
            }
            else {
                parents[i] = j;
                sizes[j] = sum_of_sizes;
            }
        }
    }
};

template <typename T>
size_t map_to_index(const T& element) noexcept {
    return element - 1;
}

template <typename T>
DisjointSet<T> disjoint_set_factory(std::vector<T>&& x_list) noexcept {
    return DisjointSet<T>(std::move(x_list), map_to_index<T>);
}
