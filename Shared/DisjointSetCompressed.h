#ifndef DISJOINT_SET_COMPRESSED_H
#define DISJOINT_SET_COMPRESSED_H

#include <functional>  // std::function
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
class DisjointSetCompressed : public DisjointSetBase<T> {
	using super = DisjointSetBase<T>;

public:
	// x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
	explicit DisjointSetCompressed(std::vector<T>&& x_list) :
		super(std::move(x_list)) { }

	// default move constructor and assignment operator
	DisjointSetCompressed(DisjointSetCompressed&& disjoint_set) noexcept = default;
	DisjointSetCompressed& operator=(DisjointSetCompressed&& rhs) noexcept = default;

	// disable copy constructor and assignment operator
	DisjointSetCompressed(const DisjointSetCompressed& rhs) = delete;
	DisjointSetCompressed& operator=(const DisjointSetCompressed& rhs) = delete;

	// default destructor
	~DisjointSetCompressed() = default;

	/**
	 * Returns the index of the representative of the unique set containing the given item.
	 * T must be castable to size_t.
	 * find implements path compression via path halving: every time find is called on a node, it
	 * makes every node in the path point to its grandparent, effectively making the trees
	 * flat or almost flat.
	 * O(n + lg*(n)) (basically linear, lg*(265536) == 5)
	 */
	size_t find(const T& item) noexcept override {
		auto u = this->parents[item];

		while (this->parents[u] != u) {
			this->parents[u] = this->parents[this->parents[u]];
			u = this->parents[u];
		}

		return u;
	}
};

#endif // DISJOINT_SET_COMPRESSED_H
