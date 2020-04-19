#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <type_traits> // std::enable_if, std::is_unsigned
#include <vector>      // std::vector

#include "DisjointSetBase.h"

/**
 * Simple union-by-size implementation of a Disjoint-Set.
 * DisjointSet accepts element of type T, which must be an unsigned integer type.
 */
template <typename T, typename = typename std::enable_if<
	std::is_unsigned<T>::value>::type
>
class DisjointSet : public DisjointSetBase<T> {
	using super = DisjointSetBase<T>;

	// vector that keeps track of the size of every element in the DisjointSet
	std::vector<size_t> sizes;
public:
	// x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
	explicit DisjointSet(std::vector<T>&& x_list) :
		super(std::move(x_list)),
		sizes(x_list.size(), 1) // fill size with value 1
	{ }

	// default move constructor and assignment operator
	DisjointSet(DisjointSet&& disjoint_set) noexcept = default;
	DisjointSet& operator=(DisjointSet&& rhs) noexcept = default;

	// disable copy constructor and assignment operator
	DisjointSet(const DisjointSet& rhs) = delete;
	DisjointSet& operator=(const DisjointSet& rhs) = delete;

	/**
	 * Returns the index of the representative of the unique set containing the given item.
	 * T must be castable to size_t.
	 * O(logn)
	 */
	size_t find(const T& element) noexcept override {
		auto x = element;
		
		while (x != this->parents[x]) {
			// move towards the parent
			x = this->parents[x];
		}
		return x;
	}

	/**
	 * Unites the dynamic sets that contain x and y into a new set that is the union
	 * of these two sets.
	 * x should be different than y.
	 * Uses union-by-size policty: it makes the node whose set size is smaller point to
	 * the node whose set size is bigger, breaking ties arbitrarily.
	 * The complexity of this method is the one of find + O(1)
	 */
	void unite(const T& x, const T& y) noexcept override {
		const size_t i = find(x);
		const size_t j = find(y);

		// if the two nodes are the same set, do nothing
		if (i != j) {
			if (this->sizes[i] < this->sizes[j]) {
				this->parents[i] = j;
				this->sizes[j] += this->sizes[i];
			} else {
				this->parents[j] = i;
				this->sizes[i] += this->sizes[j];
			}
		}
	}

	auto get_rank_updates_comparisons() const noexcept {
		auto max_rank = std::max_element(this->sizes.begin(), this->sizes.end());
		auto min_rank = std::min_element(this->sizes.begin(), this->sizes.end());
		return std::pair(*max_rank, *min_rank);
	}
};

#endif // DISJOINT_SET_H
