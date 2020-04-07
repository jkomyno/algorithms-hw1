#pragma once

#include <unordered_set> // std::unordered_set
#include <limits>        // std::numeric_limits
#include <stack>         // std::stack
#include <utility>       // std::pair
#include "AdjListGraph.h"

/**
 * Utility class to detect cycles in a graph using Depth First Search
 */
template <typename Label, typename Weight>
class DFSCycleDetection {
	// constant pointer to a non constant graph represented as an Adjacent List
	AdjListGraph<Label, Weight>* const adj_list_graph_ptr;

	// return true iff there is a cycle in the graph starting from the source node and exploring
	// the graph using Depth First Search
	bool has_cycle_helper(const Label& source, std::unordered_set<Label>& visited) const {
		// pair of nodes organized as (Son, Father)
		using son_father_pair = std::pair<Label, Label>;

		// stack of (son, father) nodes
		std::stack<son_father_pair> stack;

		// set the father of the source node to a sentinel value, in this case infinity
		stack.push(std::make_pair(source, std::numeric_limits<Label>::max()));

		// mark the source node as discovered
		visited.insert(source);

		while (!stack.empty()) {
			const auto [v, father_v] = stack.top();
			stack.pop();

			for (const auto& uw : adj_list_graph_ptr->get_adjacent_vertexes(v)) {
				const auto& u = uw.vertex;

				// if we hadn't met the node u before, we mark that we discovered it "through" v,
				// which we call its father
				if (!visited.count(u)) {
					stack.push(std::make_pair(u, v));
					visited.insert(u);
				}
				// if we already met u but u isn't v's father, it means there's a cycle
				else if (u != father_v) {
					return true;
				}
			}
		}

		// no cycle loop found
		return false;
	}

public:
	/**
	 * DFSCycleDetection expects a constant pointer to a non constant AdjListGraph object.
	 * The AdjListGraph object can be freely updated outside this class, but all updates
	 * on that object will be reflected in this class. This avoids expensive copies and
	 * limits memory usage.
	 */
	DFSCycleDetection(AdjListGraph<Label, Weight>* const adj_list_graph_ptr) :
		adj_list_graph_ptr(adj_list_graph_ptr) {}

	// we should not deallocate adj_list_graph_ptr, as it should reside on the stack, not the heap
	~DFSCycleDetection() {}

	// returns true iff the graph pointed by adj_list_graph_ptr has a loop
	bool has_cycle() const {
		const auto n = adj_list_graph_ptr->vertexes_size();
		// if the graph has less than 3 vertexes, there can't possibly exist a cycle
		if (n < 3) {
			return false;
		}
		
		const auto vertexes = adj_list_graph_ptr->get_vertexes();

		// set that keeps track of the visited nodes
		std::unordered_set<Label> visited;
		visited.reserve(n);

		for (const auto v : vertexes) {
			if (!visited.count(v) && has_cycle_helper(v, visited)) {
				return true;
			}
		}

		// no cycle loop found
		return false;
	}
};
