#ifndef _GRAPH_FUNCTIONS_H
#define _GRAPH_FUNCTIONS_H

#include "Nilib/Structures/Graph.hpp"
#include "Nilib/Structures/GraphRepresentation.hpp"
#include "Nilib/Core/Profiler.hpp"

#include <stack>
#include <queue>

namespace Nilib
{
    class Window;

    // All pairs shortest paths.
    struct BellManFord
    {
        using Cost = float;

        template <typename Graph, typename ArcCost, typename AdmissableArc>
        BellManFord(Graph const &graph, nodeID const source, ArcCost arccost, AdmissableArc admissable)
            : costs(graph.nodes_capacity(), std::numeric_limits<Cost>::max()),
              predecessor(graph.nodes_capacity(), -1)
        {
            auto k = graph.numnodes();

            costs[source] = 0;
            predecessor[source] = source;

            for (size_t iter = 0; iter < k - 1; ++iter)
            {
                bool change = false;
                for (auto &&[idx, jdx] : graph.arcs())
                {

                    if (!admissable(idx, jdx))
                        continue;

                    Cost const weight = arccost(idx, jdx);
                    if (costs[idx] + weight < costs[jdx])
                    {
                        costs[jdx] = costs[idx] + weight;
                        predecessor[jdx] = idx;
                        change = true;
                    }
                }
                if (!change)
                    break;
            }
        }

        std::vector<Cost> costs;
        std::vector<nodeID> predecessor;
    };

    struct Dijkstra
    {

        using Cost = float;

        Dijkstra(GraphI const &graph,
                 nodeID source,
                 nodeID destination)
            : Dijkstra(graph, source, destination,
                       &Dijkstra::arcCost, &Dijkstra::admissableArc) // default cost and admissable
        {
        }

        template <typename Graph, typename ArcCost, typename AdmissableArc>
        Dijkstra(Graph const &graph,
                 nodeID const source,
                 nodeID const destination,
                 ArcCost const arccost = &Dijkstra::arcCost,
                 AdmissableArc const admissablearc = &Dijkstra::admissableArc)
            requires std::is_base_of_v<GraphI, Graph>
            : costs(graph.nodes_capacity(), std::numeric_limits<Cost>::infinity()), predecessor(graph.nodes_capacity(), -1), cost(0.0)
        {
            // LOG_PROGRESS("Searching for shortest path from", source, "to", destination, "using Dijkstra's algorithm.");
            CORE_ASSERT(graph.contains(source));
            CORE_ASSERT(graph.contains(destination));

            // Min-heap priority queue: (cost, node)
            std::priority_queue<std::pair<Cost, int>, std::vector<std::pair<Cost, int>>, std::greater<>> Q;
            // Init Queue with vertex priority.
            Q.push({0, source});

            // Init mincost array with
            predecessor[source] = source;
            costs[source] = 0;

            while (!Q.empty())
            {
                auto [cost, node] = Q.top();
                Q.pop();

                if (cost > costs[node])
                    continue; // Skip stale entry

                // if (node == destination)
                //     break;;

                // Explore the down stream neighbours.
                for (auto &&neigh : graph.down_stream(node))
                {
                    // Skip admissable arcs. ONLY for Graph with arc data.
                    if (!admissablearc(node, neigh))
                        continue;

                    Cost const cost_node_neigh = arccost(node, neigh);
                    ASSERT(cost_node_neigh >= 0.0, "Dijkstra's shortest path requires non-negative arc costs! Got:", cost_node_neigh, "from", node, "to", neigh);

                    if (costs[node] + cost_node_neigh < costs[neigh])
                    {
                        costs[neigh] = costs[node] + cost_node_neigh;
                        predecessor[neigh] = node;
                        Q.push({costs[neigh], neigh});
                    }
                }
            }

            // If a path exists!
            if (costs[destination] < std::numeric_limits<float>::infinity())
            {
                // Constructing path source -> sink.
                nodeID current = destination;
                while (current != source)
                {
                    d_path.push_back(current);
                    path_arcs.push_back({predecessor[current], current});
                    current = predecessor[current];
                }
                d_path.push_back(current);
                cost = costs[destination];
                // LOG_DEBUG("Found Shortest path!", path, cost);
            }
            else
            {
                LOG_WARNING("Unable to find path from", source, "to", destination);
            }
        }

        auto arcs()
        {
            return std::ranges::subrange(path_arcs.rbegin(), path_arcs.rend());
        }

        auto path()
        {
            return std::ranges::subrange(d_path.rbegin(), d_path.rend());
        }
        static constexpr Dijkstra::Cost arcCost(nodeID const A, nodeID const B) { return 1.0f; };

        static constexpr bool admissableArc(nodeID const A, nodeID const B) { return true; };

        std::vector<float> costs;
        std::vector<nodeID> predecessor;
        float cost;

    private:
        std::vector<nodeID> d_path;
        std::vector<ArcID> path_arcs;
    };

    struct MinCostFlow
    {

        // Using successive shortest path.
        // Single source and single terminal
        template <typename Graph>
        MinCostFlow(Graph &graph, int const req_flow, nodeID const source, nodeID const terminal)
        {
            PROFILE_FUNCTION();
            // LOG_DEBUG("MinCostFlow!");
            int flow = 0;
            while (flow < req_flow)
            {
                // Find shortest path source->terminal. Over admissable arcs.
                auto bellman = BellManFord(graph, source, [&graph](nodeID a, nodeID b)
                                           { return graph.arcData[{a, b}].cost; }, [&graph](nodeID a, nodeID b)
                                           { return graph.arcData[{a, b}].flow < graph.arcData[{a, b}].capacity; });

                // Push as much flow as possible over that path.
                int min_flow = 1;
                flow += min_flow;

                // LOG_DEBUG(bellman.costs, bellman.predecessor);
                // Alter flows,
                nodeID current = terminal;
                // std::vector<nodeID> st_path;
                while (current != source)
                {
                    // st_path.push_back(current);
                    nodeID previous = bellman.predecessor[current];
                    graph.arcData[{previous, current}].flow += 1;
                    graph.arcData[{current, previous}].flow -= 1;
                    current = previous;
                }
                // st_path.push_back(source);
                // std::reverse(st_path.begin(), st_path.end());
            }
        }
    };

    void drawGraph(Window &window, GraphI const &graph);
}; // namespace Nilib

#endif
