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

    // All pairs shortest paths.
    struct BellManFord
    {

        typedef float (*ArcCost)(nodeID const, nodeID const);

        BellManFord(GraphI const &graph, nodeID const source, ArcCost arccost)
            : costs(graph.numnodes(), std::numeric_limits<float>::max()),
              predecessor(graph.numnodes(), -1)
        {
            auto k = graph.numnodes();

            costs[source] = 0;
            predecessor[source] = source;

            for (size_t iter = 0; iter < k - 1; ++iter)
            {
                for (nodeID idx = 0; idx < k; ++idx)
                {
                    for (nodeID jdx = 0; jdx < k; ++jdx)
                    {
                        if (!graph.contains(idx, jdx))
                            continue;

                        float const weight = arccost(idx, jdx);
                        if (costs[idx] + weight < costs[jdx])
                        {
                            costs[jdx] = costs[idx] + weight;
                            predecessor[jdx] = idx;
                        }
                    }
                }
            }
        }

        std::vector<float> costs;
        std::vector<nodeID> predecessor;
    };

    struct MinCostFlow
    {

        // Using successive shortest path.
        MinCostFlow(GraphI const &graph)
        {
        }
    };

    struct Dijkstra
    {

        typedef float (*ArcCost)(nodeID const, nodeID const);

        Dijkstra(GraphI const &graph,
                 nodeID source,
                 nodeID destination)
            : Dijkstra(graph, source, destination,
                       &Dijkstra::arcCost) // default cost
        {
        }

        // General graph.
        // template <typename NodeData, typename ArcData>
        // Dijkstra(Graph<NodeData, ArcData> const &graph,
        //          nodeID const source,
        //          nodeID const destination)
        Dijkstra(GraphI const &graph,
                 nodeID const source,
                 nodeID const destination,
                 ArcCost const arccost)
            // requires std::is_base_of_v<GraphI, Graph<NodeData, ArcData>>
            : costs(graph.numnodes(), std::numeric_limits<float>::infinity()), predecessor(graph.numnodes(), -1)
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
            costs[source] = 0;

            while (!Q.empty())
            {
                auto [cost, node] = Q.top();
                // LOG_DEBUG("Popped nodeID", node);

                // Explore the down stream neighbours.
                for (auto &&neigh : graph.down_stream(node))
                {
                    // LOG_DEBUG("Exploring neighbourhood of", node, ":", neigh);

                    // Skip admissable arcs. ONLY for Graph with arc data.
                    if (!admissableArc(node, neigh))
                        continue;

                    float const cost_node_neigh = arccost(node, neigh);

                    ASSERT(cost_node_neigh >= 0.0, "Dijkstra's shortest path requires non-negative arc costs! Got:", cost_node_neigh, "from", node, "to", neigh);
                    if (costs[node] + cost_node_neigh < costs[neigh])
                    {
                        costs[neigh] = costs[node] + cost_node_neigh;
                        predecessor[neigh] = node;
                        Q.push({costs[neigh], neigh});
                    }
                }
                Q.pop();
            }
            // If a path exists!
            if (costs[destination] < std::numeric_limits<float>::infinity())
            {
                // Constructing path source -> sink.
                // LOG_DEBUG("Found Shortest path!");
                nodeID current = destination;
                while (current != source)
                {
                    path.push_back(current);
                    current = predecessor[current];
                }
                path.push_back(current);
                std::reverse(path.begin(), path.end());
                cost = costs[destination];
            }
            else
            {
                LOG_DEBUG("Unable to find path from", source, "to", destination);
            }
        }

        static float arcCost(nodeID const A, nodeID const B);

        virtual bool admissableArc(nodeID const A, nodeID const B) const;

        std::vector<nodeID> predecessor;
        std::vector<nodeID> path;
        std::vector<float> costs;
        int status;
        float cost;
    };

    void drawGraph(Window &window, GraphI const &graph);
}; // namespace Nilib

#endif
