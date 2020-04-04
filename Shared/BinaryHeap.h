#pragma once

#include <vector>     // std::vector
#include <utility>    // std::forward
#include <algorithm>  // std::swap
#include <functional> // std::function
#include <cassert>    // std::assert

/**
 * Generic Binary Heap implementation. Depending on the given comparator function, it can
 * either be a Min Heap or a Max Heap.
 *
 * T: the type of the objects stored in the heap.
 * is_heap_on_init: constexpr flag. If true, build_heap() won't be called after the class is constructed.
 *                  If false, the given vector is considered an already valid heap.
 */
template <typename T, bool is_heap_on_init = false>
class BinaryHeap {
    using Compare = std::function<bool(T, T)>;

    // returns the parent of nodes[i]
    static size_t parent(size_t i) {
        return (i - 1) >> 1;
    }

    // returns the left child of nodes[i]
    static size_t left(size_t i) {
        return (i << 1) + 1;
    }

    // returns the right child of nodes[i]
    static size_t right(size_t i) {
        return (i << 1) + 2;
    }

    // returns true iff the nodes[i] is a leaf
    [[nodiscard]] bool is_leaf(size_t i) const {
        /**
         * The first leaf in a Heap starts from (n >> 1) + 1,
         * where n is the size of nodes.
         */
        const size_t bound = (size() >> 1) + 1;
        return i >= bound;
    }

protected:
    // protected constructor to let the subclass set the comparator
    explicit BinaryHeap(std::vector<T>&& inputs) noexcept :
        nodes(inputs) {
        // spend O(n) to build the heap only if the given vector wasn't already a
        // valid heap from the beginning
        if constexpr (!is_heap_on_init) {
            build_heap();
        }
    }

    // keep the values in the heap
    std::vector<T> nodes;

    // comparison function
    // std::greater<> -> Min Heap
    // std::less<>    -> Max Heap
    Compare comp;

    // swap 2 nodes in the heap
    virtual void swap_nodes(size_t i, size_t j) {
        using std::swap;
        swap(nodes.at(i), nodes.at(j));
    }

    // starting from a Heap with a misplaced node at the given index,
    // recover the shape and heap properties in O(logn) bubbling the node down
    void heapify_down(const size_t index_to_fix) {
        const size_t len = size();
        size_t i = index_to_fix;

        while (!is_leaf(i)) {
            const size_t l = left(i);
            const size_t r = right(i);

            // comp_est is the biggest element in a Max Heap,
            // or the smallest element in a Min Heap
            size_t comp_est = i;

            if (l < len && comp(nodes.at(i), nodes.at(l))) {
                comp_est = l;
            }
            if (r < len && comp(nodes.at(comp_est), nodes.at(r))) {
                comp_est = r;
            }

            if (comp_est == i) {
                return;
            }

            swap_nodes(i, comp_est);
            i = comp_est;
        }
    }

    // starting from a Heap with a misplaced node at the given index,
    // recover the shape and heap properties in O(logn) bubbling the node up
    void heapify_up(size_t index_to_fix) {
        size_t i = index_to_fix;

        size_t p = parent(i);
        while (i > 0 && comp(nodes.at(p), nodes.at(i))) {
            swap_nodes(i, p);
            i = p;
            p = parent(i);
        }
    }

public:
    BinaryHeap() = delete;

    /**
     * Compare is the type of a comparison function.
     * Use std::greater<> to obtain a Min Heap, use std::less<> to obtain a Max Heap.
     */
    explicit BinaryHeap(std::vector<T>&& inputs, Compare comp) noexcept :
        nodes(inputs),
        comp(comp) {

        // spend O(n) to build the heap only if the given vector wasn't already a
        // valid heap from the beginning
        if constexpr (!is_heap_on_init) {
            build_heap();
        }
    }

    BinaryHeap(const BinaryHeap<T, is_heap_on_init>& rhs) = delete;
    BinaryHeap& operator=(const BinaryHeap<T, is_heap_on_init>& rhs) = delete;

    BinaryHeap(BinaryHeap<T, is_heap_on_init>&& rhs) = default;
    BinaryHeap& operator=(BinaryHeap<T, is_heap_on_init>&& rhs) = default;

    // virtual destructor because this is a base class
    virtual ~BinaryHeap() = default;

    // transform the internal vector in a heap in O(n)
    void build_heap() {
        for (size_t i = (size() >> 1) + 1; i > 0; i--) {
            heapify_down(i - 1);
        }
    }

    // return the number of elements in the heap
    [[nodiscard]] size_t size() const {
        return nodes.size();
    }

    // return true iff the heap is empty
    [[nodiscard]] bool empty() const {
        return size() == 0;
    }

    // if BinaryHeap is instantiated as a Min Heap, returns the minimum element, otherwise
    // returns the maximum.
    [[nodiscard]] T top() const {
        assert(size() > 0);

        return nodes.at(0);
    }

    // if BinaryHeap is instantiated as a Min Heap, removes the minimum element, otherwise
    // removes the maximum.
    void pop() {
        assert(size() > 0);

        // replace root of the heap with the last element of the vector
        nodes.at(0) = nodes.back();

        // remove the last element of the vector
        nodes.pop_back();

        // the root violates the heap property. Let's fix that
        heapify_down(0);
    }

    // adds a new element to the heap
    template <class... Args>
    void push(Args&&... args) {
        const size_t index_to_fix = size();

        // insert new node at the end of the vector
        nodes.emplace_back(std::forward<Args>(args)...);

        // the root violates the heap property. Let's fix that
        heapify_up(index_to_fix);
    }
};


// create a Min Heap
template <typename T>
BinaryHeap<T, false> make_min_heap(std::vector<T>&& inputs) {
    return BinaryHeap<T, false>(std::forward(inputs), std::greater<>{});
}

// create a Max Heap
template <typename T>
BinaryHeap<T, false> make_max_heap(std::vector<T>&& inputs) {
    return BinaryHeap<T, false>(std::forward(inputs), std::less<>{});
}
