#pragma once

#include <vector>    // std::vector
#include <algorithm> // std::swap
#include <assert.h>  // std::assert

template <typename T>
class BinaryHeap {
    std::vector<T> nodes;

    // returns the parent of nodes[i]
    static size_t parent(size_t i) {
        return (i - 1) / 2;
    }

    // returns the left child of nodes[i]
    static size_t left(size_t i) {
        return 2 * i + 1;
    }

    // returns the right child of nodes[i]
    static size_t right(size_t i) {
        return 2 * i + 2;
    }

    // adjusts the node at index i and possibly its 2 children, which
    // violate the heap property
    void heapify_down(size_t i) {
        const size_t l = left(i);
        const size_t r = right(i);

        const size_t len = size();

        // index of the smallest node among the node at index i and its
        // possibly 2 children
        size_t smallest = i;

        if (l < len && nodes.at(l) < nodes.at(i)) {
            smallest = l;
        }

        if (r < len && nodes.at(r) < nodes.at(smallest)) {
            smallest = r;
        }

        if (smallest != i) {
            std::swap(nodes.at(i), nodes.at(smallest));
            heapify_down(smallest);
        }
    }

    void heapify_up(size_t i) {
        const size_t p = parent(i);
        if (i != 0 && nodes.at(p) > nodes.at(i)) {
            std::swap(nodes.at(i), nodes.at(p));
            heapify_up(p);
        }
    }

public:
    BinaryHeap() = delete;

    explicit BinaryHeap(std::vector<T>&& inputs) noexcept {
        nodes.reserve(inputs.size());
        for (const auto& input : inputs) {
            insert(input);
        }
    }

    ~BinaryHeap() = default;

    size_t size() {
        return nodes.size();
    }

    T& get_min() const {
        assert(size() > 0);

        return nodes.at(0);
    }

    T extract_min() {
        assert(size() > 0);

        const T min = nodes.at(0);

        // replace root of the heap with the last element of the vector
        nodes.at(0) = nodes.back();

        // remove the last element of the vector
        nodes.pop_back();

        // the root violates the heap property. Let's fix that
        heapify_down(0);

        return min;
    }

    void insert(const T& node) {
        const size_t new_index = size();

        // insert new node at the end of the vector
        nodes.emplace_back(node);
        heapify_up(new_index);
    }
};
