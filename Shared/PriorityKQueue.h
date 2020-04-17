#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <algorithm>      // std::swap, std::find
#include <cassert>        // std::assert
#include <functional>     // std::function
#include <unordered_map>  // std::unordered_map

#include "KHeap.h"

/**
 * Priority Queue based on a K-ary Heap. Whether it is based on a MinHeap or a MaxHeap
 * depends on CompareFactory object used for initialization. This class shouldn't be invoked directly.
 * Instead, the factories make_min_priority_queue and make_max_priority_queue should be used.
 *
 * K: number of children that each node stored in the heap may have. K=2 is semantically equal to using a BinaryHeap.
 * Key: type of the keys used to mantain the elements in a valid heap
 * T: type of the elements stored in the priority queue
 * is_heap_on_init: constexpr flag. If true, build_heap() won't be called after the class is constructed.
 *                  If false, the given vector is considered an already valid heap.
 * T_Hash: functor used to hash values of type T. Necessary only if T is a non-trivial type.
 */
template <size_t K, typename Key, typename T, bool is_heap_on_init = false, typename T_Hash = std::hash<T>>
class PriorityKQueue : private KHeap<K, T, is_heap_on_init> {
	using super = KHeap<K, T, is_heap_on_init>;
	using key_map_type = std::unordered_map<T, Key, T_Hash>;
	using index_map_type = std::unordered_map<T, size_t, T_Hash>;
	using Compare = std::function<bool(T, T)>;
	using CompareFactory = std::function<Compare(key_map_type&)>;

	// keep track of the value of the keys of each node.
	key_map_type key_map;

	// keep track of the index of the nodes
	index_map_type index_map;

	// initialize the key map
	template <typename U, typename U_Key, typename U_Hash = std::hash<U>>
	static std::unordered_map<size_t, U_Key, U_Hash> build_key_map(std::vector<Key>&& keys, std::vector<U>& node_list) noexcept {
		assert(keys.size() == node_list.size());

		std::unordered_map<U, U_Key, U_Hash> local_key_map(node_list.size());

		size_t index = 0;
		for (const auto& node : node_list) {
			local_key_map[node] = keys[index];
			index++;
		}

		return local_key_map;
	}

	// initialize index map
	template <typename U, typename U_Key, typename U_Hash = std::hash<U>>
	static std::unordered_map<U, size_t, U_Hash> build_index_map(std::vector<U>& node_list) noexcept {
		std::unordered_map<U, size_t, U_Hash> local_index_map(node_list.size());

		size_t index = 0;
		for (const auto& node : node_list) {
			local_index_map[node] = index;
			index++;
		}

		return local_index_map;
	}

protected:
	void swap_nodes(size_t i, size_t j) override {
		using std::swap;

		auto node_i = this->nodes.at(i);
		auto node_j = this->nodes.at(j);

		// swap(key_map.at(node_i), key_map.at(node_j));
		swap(index_map.at(node_i), index_map.at(node_j));
		super::swap_nodes(i, j);
	}

public:
	PriorityKQueue() = delete;

	explicit PriorityKQueue(std::vector<Key>&& keys, std::vector<T>&& inputs, CompareFactory&& comp) :
		super(std::move(inputs)),
		key_map(build_key_map<T, Key, T_Hash>(std::move(keys), this->nodes)),
		index_map(build_index_map<T, size_t, T_Hash>(this->nodes))
	{
		this->comp = comp(key_map);
		this->build_heap();
	}

	explicit PriorityKQueue(const PriorityKQueue<K, Key, T>& rhs) = delete;
	PriorityKQueue& operator=(const PriorityKQueue<K, Key, T>& rhs) = delete;

	~PriorityKQueue() = default;

	// return the number of elements in the heap
	[[nodiscard]] size_t size() const noexcept {
		return super::size();
	}

	// return true iff the heap is empty
	[[nodiscard]] bool empty() const noexcept  {
		return super::empty();
	}

	// adds a new element to the heap
	void push(const Key& key, const T& node) {
		const size_t index_to_fix = this->size();
		index_map[node] = index_to_fix;
		key_map[node] = key;

		super::push(node);
	}

	// update the key of a node in the priority queue.
	// node must exist in the priority queue
	void update_key(const Key& key, const T& node) {
		const size_t index_to_fix = index_map[node];
		key_map[node] = key;

		// recover the heap property
		this->heapify_up(index_to_fix);
	}

	// returns the value of the key of the given node
	[[nodiscard]] const Key& key_at(const T& node) const {
		return key_map.at(node);
	}

	// returns true iff the given node is in the priority queue
	[[nodiscard]] bool contains(const T& node) const noexcept {
		const auto search = index_map.find(node);
		return search != index_map.end();
	}

	// return the top element
	[[nodiscard]] T top() const {
		return super::top();
	}

	// remove the top element
	void pop() {
		assert(this->size() > 0);

		const size_t index_to_remove = 0;
		const auto node = this->nodes.at(index_to_remove);
		index_map.erase(node);
		key_map.erase(node);

		// replace root of the heap with the last element of the vector
		this->nodes.at(0) = this->nodes.back();

		// remove the last element of the vector
		this->nodes.pop_back();

		// no need to recover the heap property if the heap is empty
		if (this->size() > 0) {
			const auto front = this->nodes.front();
			index_map[front] = 0;

			// the root violates the heap property. Let's fix that
			this->heapify_down(0);
		}
	}
};

// create a Priority KQueue based on a Min Heap
template <size_t K, typename Key, typename Value, bool is_heap_on_init = false>
PriorityKQueue<K, Key, Value, is_heap_on_init> make_min_priority_k_queue(std::vector<Key>&& keys, std::vector<Value>&& inputs) {
	return PriorityKQueue<K, Key, Value, is_heap_on_init>(std::move(keys), std::move(inputs), [](auto& key_map) {
		return [&key_map](auto&& a, auto&& b) -> bool {
			return key_map.at(a) > key_map.at(b);
		};
	});
}

// create a Priority KQueue based on a Max Heap
template <size_t K, typename Key, typename Value, bool is_heap_on_init = false >
PriorityKQueue<K, Key, Value, is_heap_on_init> make_max_priority_k_queue(std::vector<Key>&& keys, std::vector<Value>&& inputs) {
	return PriorityKQueue<K, Key, Value, is_heap_on_init>(std::move(keys), std::move(inputs), [](auto& key_map) {
		return [&key_map](auto&& a, auto&& b) -> bool {
			return key_map.at(a) < key_map.at(b);
		};
	});
}

#endif  // PRIORITY_QUEUE_H
