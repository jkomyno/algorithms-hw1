#ifndef DISJOINT_SET_COMPRESSED_H
#define DISJOINT_SET_COMPRESSED_H

#include <functional>  // std::function
#include <type_traits> // std::enable_if, std::is_unsigned
#include <vector>      // std::vector
#include <algorithm>

#include "DisjointSetBase.h"

/**
 * Compressed union-by-rank implementation of a Disjoint-Set.
 * DisjointSetCompressed accepts element of type T, which must be an unsigned integer type.
 */
template <typename T, typename = typename std::enable_if<
	std::is_unsigned<T>::value>::type
>
class DisjointSetCompressed : public DisjointSetBase<T> {
	using super = DisjointSetBase<T>;

	// vector that keeps track of the rank of every element in the DisjointSet
	std::vector<size_t> ranks;
public:
	// x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
	explicit DisjointSetCompressed(std::vector<T>&& x_list) :
		super(std::move(x_list)),
		ranks(x_list.size(), 0) // fill size with value 0
	{ }

	// default move constructor and assignment operator
	DisjointSetCompressed(DisjointSetCompressed&& disjoint_set) noexcept = default;
	DisjointSetCompressed& operator=(DisjointSetCompressed&& rhs) noexcept = default;

	// disable copy constructor and assignment operator
	DisjointSetCompressed(const DisjointSetCompressed& rhs) = delete;
	DisjointSetCompressed& operator=(const DisjointSetCompressed& rhs) = delete;

	/**
	 * Returns the index of the representative of the unique set containing the given item.
	 * T must be castable to size_t.
	 * find implements path compression via path splitting: every time find is called on a node, it
	 * makes every node in the path point to its grandparent, effectively making the trees
	 * flat or almost flat.
	 * O(lg*(n)) (basically linear, lg*(265536) == 5)
	 */
	size_t find(const T& element) noexcept override {
		auto x = element;
		
		while (x != this->parents[x]) {
			// skip parent and point to grandparent
			this->parents[x] = this->parents[this->parents[x]];

			// move to grandparent
			x = this->parents[x];
		}

		return x;
	}

	/**
	 * Unites the dynamic sets that contain x and y into a new set that is the union
	 * of these two sets.
	 * x should be different than y.
	 * Uses union-by-rank policty: it makes the node whose set rank is smaller point to
	 * the node whose set size is bigger, breaking ties arbitrarily. In case of tie,
	 * the resulting set's rank is increased by 1.
	 * The complexity of this method is the one of find + O(1)
	 */
	void unite(const T& x, const T& y) noexcept override {
		auto i = find(x);
		auto j = find(y);

		if (i != j) {
			// if two sets are united and have different ranks,
			// the resulting set's rank is the larger of the two
			if (this->ranks[i] < this->ranks[j]) {
				using std::swap;
				swap(i, j);
			}

			this->parents[j] = i;

			if (this->ranks[i] != this->ranks[j]) {
				return;
			}

			// if two sets are united and have the same rank, the resulting
			// set's rank is one larger
			++(this->ranks[i]);
		}
	}
};

#endif // DISJOINT_SET_COMPRESSED_H
