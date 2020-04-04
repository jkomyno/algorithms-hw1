#pragma once

#include <vector>    // std::vector
#include <utility>   // std::forward
#include <algorithm> // std::swap, std::find
#include <unordered_map> // std::unordered_map
#include <optional>  // std::optional
#include <cassert>  // std::assert

// support comparator
template <typename T, class Compare = std::greater<>>
class BinaryHeap {
    // keep the values in the heap
    std::vector<T> nodes;

    // comparison function
    // std::greater<> -> Min Heap
    // std::less<>    -> Max Heap
    Compare comp;

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

    // create a Heap in O(n)
    void build_heap() {
        for (size_t i = (size() >> 1) + 1; i > 0; i--) {
            heapify_down(i - 1);
        }
    }

    // starting from a Heap with a misplaced node at the given index,
    // recover the shape and heap properties in O(logn) bubbling the node down
    void heapify_down(const size_t index_to_fix) {
        const size_t len = size();
        size_t i = index_to_fix;

        while (!is_leaf(i)) {
            const size_t l = left(i);
            const size_t r = right(i);

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

            using std::swap;
            swap(nodes.at(i), nodes.at(comp_est));

            i = comp_est;
        }
    }

    // starting from a Heap with a misplaced node at the given index,
    // recover the shape and heap properties in O(logn) bubbling the node up
    void heapify_up(size_t index_to_fix) {
        size_t i = index_to_fix;
        using std::swap;

        size_t p = parent(i);
        while (i > 0 && comp(nodes.at(p), nodes.at(i))) {
            swap(nodes.at(i), nodes.at(p));
            i = p;
        }
    }

public:
    BinaryHeap() = delete;

    explicit BinaryHeap(std::vector<T>&& inputs) noexcept :
        nodes(inputs),
        comp{} {
        build_heap();
    }

    explicit BinaryHeap(std::vector<T>&& inputs, Compare&& comp) noexcept :
        nodes(inputs),
        comp(comp) {
        build_heap();
    }

    ~BinaryHeap() = default;

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
    [[nodiscard]] const T& top() const {
        assert(size() > 0);

        return nodes.at(0);
    }

    void reheap() {
        build_heap();
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

    /*
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
    */

    /*
    template <typename UnaryPredicate>
    [[nodiscard]] std::optional<size_t> find(const UnaryPredicate& predicate) const {
        const auto it = std::find_if(nodes.begin(), nodes.end(), predicate);
        if (it != nodes.end()) {
            size_t index = std::distance(nodes.begin(), it);
            return std::optional{ index };
        }
        return std::nullopt;
    }

    T remove(const size_t k) {
        const size_t len = size();

        assert(len > 0);

        // replace deleted node with the right-most leaf to keep
        // the binary tree in shape
        const auto last = nodes.back();

        if (k == len - 1) {
            nodes.pop_back();
            return last;
        }

        nodes.pop_back();
        auto result = nodes.at(k);
        nodes.at(k) = last;

        const size_t p = parent(k);

        if (k == 0 || nodes.at(p) < nodes.at(k)) {
            // move the k-th node down the tree
            heapify_down(k);
        } else {
            // move the k-th node up the tree
            heapify_up(k);
        }

        return result;
    }
    */
};

// create a Min Heap
template <typename T>
BinaryHeap<T, decltype(std::greater<>{})> make_min_heap(std::vector<T>&& inputs) {
    return BinaryHeap<T, std::greater<>>(std::forward(inputs));
}

// create a Max Heap
template <typename T>
BinaryHeap < T, decltype(std::less<>{}) > make_max_heap(std::vector<T>&& inputs) {
    return BinaryHeap<T, std::less<>>(std::forward(inputs));
}
