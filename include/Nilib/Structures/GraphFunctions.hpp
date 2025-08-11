#ifndef _GRAPH_FUNCTIONS_H
#define _GRAPH_FUNCTIONS_H

#include "Nilib/Structures/Graph.hpp"
#include "Nilib/Structures/GraphRepresentation.hpp"

#include <stack>
#include <queue>

namespace Nilib
{
    class Window;

    // // Search for all node agreeing with a predicate in a graph.
    // struct DepthFirstSearch
    // {
    //     std::stack<nodeID> list;

    //     using nodePredicateFunction = bool;

    //     DepthFirstSearch(GraphI const &graph, nodeID const start, nodePredicateFunction function);
    // };
    // struct BreadthFirstSearch
    // {
    //     std::queue<nodeID> list;

    //     using nodePredicateFunction = bool;

    //     BreadthFirstSearch(GraphI const &graph, nodeID const start, nodePredicateFunction function);
    // };

    class Dijkstra
    {

    public:
        // General graph.
        template <typename NodeData, typename ArcData>
        Dijkstra(Graph<NodeData, ArcData> const &graph,
                 nodeID const source,
                 nodeID const destination)
            : costs(graph.numnodes(), 0), predecessor(graph.numnodes(), 0)
        {
            LOG_PROGRESS("Searching for shortest path from", source, "to", destination, "using Dijkstra's algorithm.");
            CORE_ASSERT(graph.contains(source));
            CORE_ASSERT(graph.contains(destination));

            // Min-heap priority queue: (cost, node)
            std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> Q;
            // Init Queue with vertex priority.
            Q.push({0, source});

            // Init mincost array with
            predecessor[source] = source;

            while (!Q.empty())
            {
                auto [cost, node] = Q.top();
                Q.pop();

                // Skip this evaluation, we already got here via a better path.
                if (cost > costs[node])
                    continue;

                // Explore the down stream neighbours.
                for (auto &&neigh : graph.down_stream(node))
                {
                    // Skip admissable arcs. ONLY for Graph with arc data.
                    if (!admissableArc(node, neigh))
                        continue;

                    float cost_node_neigh = arcCost(node, neigh);
                    if (costs[node] + cost_node_neigh < costs[neigh])
                    {
                        costs[neigh] = costs[node] + cost_node_neigh;
                        predecessor[neigh] = node;
                        Q.push({costs[neigh], neigh});
                    }
                }
            }
        }

        float arcCost(nodeID const A, nodeID const B) const;

        bool admissableArc(nodeID const A, nodeID const B) const;

        std::vector<nodeID> predecessor;
        std::vector<float> costs;
        int status;
        int steps;
        float cost;
    };

    void drawGraph(Window &window, GraphI const &graph);
}; // namespace Nilib

#endif
