#ifndef DFS_H
#define DFS_H

#include <limits>         // std::limits
#include <stack>          // std::stack
#include <unordered_map>  // std::unordered_map
#include <utility>        // std::pair
#include <vector>         // std::vector

#include "AdjListGraph.h"

// Depth First Search operations
template <typename Label, typename Weight>
class DFS {
    // the discovered nodes are modeled as an unordered_map because this class may be used
    // only on a subset of the nodes (as it's the case in kruskal_simple_mst.h),
    // thus creating a vector from 0 to n-1 might be too wasteful.
    using discovered_map = std::unordered_map<Label, bool>;

    using Son = Label;
    using Father = Label;

    // pair of nodes organized as (Son, Father)
    using son_father_pair = std::pair<Son, Father>;

    // input graph represented as a undirected graph in an adjacency list
    AdjListGraph<Label, Weight> adj_list_graph;

    // vector of nodes of the input graphs, numbered from 0 to n-1
    std::vector<Label> vertexes;

    // return one node in the same connected component as source
    Label connected_components_helper(const Label& source, discovered_map& discovered) const {
        std::stack<Label> stack;
        stack.push(source);

        // this will be the node in the same connected component as source that
        // will be returned
        Label node_in_cc = source;

        while (!stack.empty()) {
            // extract node from the stack
            auto v = stack.top();
            stack.pop();

            // ignore all discovered nodes
            if (!discovered[v]) {
                // mark the current node as discovered,
                // process the undiscovered adjacent nodes

                discovered[v] = true;
                node_in_cc = v;

                for (const auto& uw : adj_list_graph.get_adjacent_vertexes(v)) {
                    const auto& u = uw.vertex;

                    if (!discovered[u]) {
                        stack.push(u);
                    }
                }
            }
        }

        return node_in_cc;
    }

    // return true iff there is a cycle in the same connected component where the source node resides
    bool is_acyclic_helper(const Label& source, discovered_map& discovered) const {
        // stack of (son, father) nodes
        std::stack<son_father_pair> stack;
        const auto& inf = std::numeric_limits<Father>::max();

        // set the father of the source node to a sentinel value, in this case infinity
        stack.push(std::make_pair(source, inf));

        // mark the source node as discovered
        discovered[source] = true;

        while (!stack.empty()) {
            const auto [v, father_v] = stack.top();
            stack.pop();

            for (const auto& uw : adj_list_graph.get_adjacent_vertexes(v)) {
                const auto& u = uw.vertex;

                // if we hadn't met the node u before, we mark that we discovered it "through" v,
                // which we call its father
                if (!discovered[u]) {
                    stack.push(std::make_pair(u, v));
                    discovered[u] = true;
                } else if (u != father_v) {
                    // if we already met u but u isn't v's father, it means there's a loop
                    return false;
                }
            }
        }

        // no loop was found in source's connected component
        return true;
    }

   public:
    DFS(AdjListGraph<Label, Weight>&& adj_list_graph)
    noexcept : adj_list_graph(adj_list_graph),
               vertexes(adj_list_graph.get_vertexes()) {}

    // default destructor
    ~DFS() = default;

    /**
     * Return one node for each connected component of the graph.
     * Each of these nodes can be used as source node for the has_cycle method.
     * The size of the returned vector is the number of connected components in the graph.
     */
    [[nodiscard]] std::vector<Label> find_connected_components() const {
        discovered_map discovered;
        discovered.reserve(vertexes.size());

        std::vector<Label> result;

        for (const auto& v : vertexes) {
            if (!discovered[v]) {
                auto node_in_cc = connected_components_helper(v, discovered);
                result.emplace_back(node_in_cc);
            }
        }

        return result;
    }

    /**
     * Returns true iff there's no cycle in each of the connected components of
     * the graph
     */
    [[nodiscard]] bool is_acyclic() const {
        discovered_map discovered;
        discovered.reserve(vertexes.size());

        bool result = true;
        for (const auto& source : find_connected_components()) {
            result = result && is_acyclic_helper(source, discovered);

            if (!result) {
                return false;
            }
        }

        return true;
    }
};

#endif  // DFS_H
