#pragma once

#include <limits>         // std::numeric_limits
#include <stack>          // std::stack
#include <unordered_set>  // std::unordered_set
#include <utility>        // std::pair

#include "AdjacencyMapGraph.h"

/**
 * Utility class to detect cycles in a graph using Depth First Search
 */
template <typename Label, typename Weight>
class DFSCycleDetection {
    // constant pointer to a non constant graph represented as an Adjacency Map
    AdjacencyMapGraph<Label, Weight>* const adj_map_graph_ptr;

    // return true iff there is a cycle in the graph starting from the source node and exploring
    // the graph using Depth First Search.
    // After benchmarking, we discovered that the recursive version of DFS is consistently faster
    // than the iterative version.
    bool has_cycle_helper_rec(const Label& v, std::unordered_set<Label>& visited,
                              const Label& parent = std::numeric_limits<Label>::max()) const {
        // mark the current node as discovered
        visited.insert(v);

        // visit the neighbours of the current node, except the current node itself
        for (const auto& uw : adj_map_graph_ptr->adj_map.at(v)) {
            const auto& u = uw.first;

            // if the current node is different than its parent, we need to check for either
            // one of two cases:
            // 1) if node u was already encountered, we fell into a cycle
            // 2) if the recursive call of has_cycle_helper_rec with u as current node returned
            // true, we should propagate the result.
            if (u != parent && (visited.count(u) || has_cycle_helper_rec(u, visited, v))) {
                return true;
            }
        }

        return false;
    }

    // return true iff there is a cycle in the graph starting from the source node and exploring
    // the graph using Depth First Search
    [[deprecated("use has_cycle_helper_rec instead")]] bool has_cycle_helper(
        const Label& source, std::unordered_set<Label>& visited) const {
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

            // visit the neighbours of the current node, except the current node itself
            for (const auto& [u, _] : adj_map_graph_ptr->adj_map.at(v)) {
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
     * Time: O(1)
     * Space: O(1)
     */
    DFSCycleDetection(AdjacencyMapGraph<Label, Weight>* const adj_map_graph_ptr) :
        adj_map_graph_ptr(adj_map_graph_ptr) {
    }

    // we should not deallocate adj_list_graph_ptr, as it resides on the stack, not the heap
    ~DFSCycleDetection() {
    }

    /**
     * Returns true iff the graph pointed by adj_map_graph_ptr has a loop.
     * Time: O(n + m)
     * Space: O(n)
     */
    bool has_cycle() const {
        const auto n = adj_map_graph_ptr->vertexes_size();
        // if the graph has less than 3 vertexes, there can't possibly exist a cycle
        if (n < 3) {
            return false;
        }

        // set that keeps track of the visited nodes
        std::unordered_set<Label> visited;
        visited.reserve(n);

        for (const auto& v : adj_map_graph_ptr->adj_map) {
            if (!visited.count(v.first) && has_cycle_helper_rec(v.first, visited)) {
                return true;
            }
        }

        // no cycle loop found
        return false;
    }
};
