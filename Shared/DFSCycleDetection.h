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

    // return true iff there exist a path that links the source and target vertexes.
    // This method assumes that the underlying graph doesn't have any cycle.
    bool are_connected_helper(const Label& source, const Label& target,
                              std::unordered_set<Label>& visited) const {
        // mark the source node as visited
        visited.insert(source);

        // the source node will be the first to be processed
        std::stack<Label> stack;
        stack.push(source);

        while (!stack.empty()) {
            const auto curr_source = stack.top();
            stack.pop();

            // if the current source vertex is the same as the target vertex, or if it is
            // directly linked to the target node, then source and target are connected
            if (curr_source == target || adj_map_graph_ptr->has_edge(curr_source, target)) {
                return true;
            }

            // there may not be any edge in the graph containing the current source vertex
            if (adj_map_graph_ptr->has_vertex(curr_source)) {
                // visit the neighbors of the current source vertex
                for (const auto& [u, _] : adj_map_graph_ptr->adj_map.at(curr_source)) {
                    // we shouldn't consider any path that touches vertexes we already met
                    // previously, as they don't provide a path to reach the target node.
                    if (!visited.count(u)) {
                        visited.insert(u);
                        stack.push(u);
                    }
                }
            }
        }

        return false;
    }

    // this method is left here for reference. are_connected_helper is ~100% faster than
    // are_connected_helper_rec
    [[deprecated("use are_connected_helper instead")]] bool are_connected_helper_rec(
        const Label& source, const Label& target, std::unordered_set<Label>& visited) const {
        if (source == target || adj_map_graph_ptr->has_edge(source, target)) {
            return true;
        }

        // mark the source node as visited
        visited.insert(source);

        // there may not be any edge containing the source vertex
        if (adj_map_graph_ptr->has_vertex(source)) {
            // visit the neighbors of the current node
            for (const auto& [u, _] : adj_map_graph_ptr->adj_map.at(source)) {
                if (!visited.count(u) && are_connected_helper_rec(u, target, visited)) {
                    return true;
                }
            }
        }

        return false;
    }

    // return true iff there is a cycle in the graph starting from the source node and exploring
    // the graph using Depth First Search.
    // After benchmarking, we discovered that the recursive version of DFS is consistently faster
    // than the iterative version.
    [[deprecated("use are_connected_helper instead")]] bool has_cycle_helper_rec(
        const Label& v, std::unordered_set<Label>& visited,
        const Label& parent = std::numeric_limits<Label>::max()) const {
        // mark the current node as discovered
        visited.insert(v);

        // visit the neighbors of the current node, except the current node itself
        for (const auto& [u, _] : adj_map_graph_ptr->adj_map.at(v)) {
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
    [[deprecated("use are_connected_helper instead")]] bool has_cycle_helper(
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
     * Returns true iff there is a path from the source vertex to the target vertex in the graph
     * pointed by adj_map_graph_ptr.
     * Time: O(n + m)
     * Space: O(n)
     */
    bool are_connected(const Label& source, const Label& target) const {
        const auto n = adj_map_graph_ptr->vertexes_size();

        // set that keeps track of the visited nodes
        std::unordered_set<Label> visited;
        visited.reserve(n);

        return are_connected_helper(source, target, visited);
    }

    /**
     * Returns true iff the graph pointed by adj_map_graph_ptr has a loop.
     * Time: O(n + m)
     * Space: O(n)
     */
    [[deprecated("Use are_connected instead")]] bool has_cycle() const {
        const auto n = adj_map_graph_ptr->vertexes_size();

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
