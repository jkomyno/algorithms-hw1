#ifndef DISJOINT_SET_BASE_H
#define DISJOINT_SET_BASE_H

#include <algorithm>   // std::transform
#include <functional>  // std::function
#include <type_traits> // std::enable_if, std::is_unsigned
#include <vector>      // std::vector

/**
 * Abstract base class of Disjoint-Set. It implements the union-by-size policy.
 * DisjointSet accepts element of type T, which must be an unsigned integer type.
 */
template <typename T, typename = typename std::enable_if<
	std::is_unsigned<T>::value>::type
>
class DisjointSetBase {
	// vector that keeps track of the size of every element in the DisjointSet
	std::vector<size_t> sizes;

protected:
	// vector that keeps track of the parent of every element in the DisjointSet
	std::vector<size_t> parents;

public:
	// x_list must contain unsigned integers of distinct value in the range [0, x_list.size())
	explicit DisjointSetBase(std::vector<T>&& x_list) :
		sizes(x_list.size(), 1) // fill size with value 1
	{
		parents.reserve(x_list.size());

		// initially every item is the parent of itself
		std::transform(x_list.cbegin(), x_list.cend(), std::back_inserter(parents), [&x_list](const auto& x_elem) {
			// pointer arithmetic to obtain the index of the current element in x_list
			const size_t index = &x_elem - &(x_list[0]);
			return index;
		});
	}

	// default move constructor and assignment operator
	DisjointSetBase(DisjointSetBase&& disjoint_set) noexcept = default;
	DisjointSetBase& operator=(DisjointSetBase&& rhs) noexcept = default;

	// disable copy constructor and assignment operator
	DisjointSetBase(const DisjointSetBase& rhs) = delete;
	DisjointSetBase& operator=(const DisjointSetBase& rhs) = delete;

	// it's a base class, so the destructor must be virtual
	virtual ~DisjointSetBase() { }

	/**
	 * returns the index of the representative of the unique set containing the given item.
	 * T must be castable to size_t.
	 */
	virtual size_t find(const T& element) noexcept = 0;

	// unites the dynamic sets that contain x and y into a new
	// set that is the union of these two sets.
	// The complexity of this method is the complexity of find + O(1)

	/**
	 * unites the dynamic sets that contain x and y into a new set that is the union
	 * of these two sets.
	 * The complexity of this method is the one of find + O(1)
	 */
	void unite(const T& x, const T& y) noexcept {
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

#endif  // DISJOINT_SET_BASE_H
