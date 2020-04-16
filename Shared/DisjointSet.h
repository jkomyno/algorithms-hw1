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

public:
	// x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
	explicit DisjointSet(std::vector<T>&& x_list) :
		super(std::move(x_list)) { }

	// default move constructor and assignment operator
	DisjointSet(DisjointSet&& disjoint_set) noexcept = default;
	DisjointSet& operator=(DisjointSet&& rhs) noexcept = default;

	// disable copy constructor and assignment operator
	DisjointSet(const DisjointSet& rhs) = delete;
	DisjointSet& operator=(const DisjointSet& rhs) = delete;

	// default destructor
	~DisjointSet() = default;

	/**
	 * returns the index of the representative of the unique set containing the given item.
	 * T must be castable to size_t.
	 * O(logn)
	 */
	size_t find(const T& element) noexcept override {
		auto x = element;
		
		while (x != this->parents[x]) {
			// move to parent
			x = this->parents[x];
		}
		return x;
	}
};

#endif // DISJOINT_SET_H
