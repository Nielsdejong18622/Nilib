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

        template <typename ArcCost, typename AdmissableArc>
        BellManFord(GraphI const &graph, nodeID const source, ArcCost arccost, AdmissableArc admissable)
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
                        if (!admissable(idx, jdx))
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

            // // Constructing path source -> all other nodes.
            // for (nodeID destination : graph.nodes())
            // {

            //     nodeID current = destination;
            //     while (current != source)
            //     {
            //         path.push_back(current);
            //         path_arcs.push_back({predecessor[current], current});
            //         current = predecessor[current];
            //     }
            //     path.push_back(current);
            //     std::reverse(path.begin(), path.end());
            //     std::reverse(path_arcs.begin(), path_arcs.end());
            //     cost = costs[destination];
            //     LOG_DEBUG("Found Shortest path!", path, cost);
            // }
        }

        std::vector<float> costs;
        std::vector<nodeID> predecessor;
    };

    struct Dijkstra
    {

        // typedef float (*ArcCost)(nodeID const, nodeID const);

        Dijkstra(GraphI const &graph,
                 nodeID source,
                 nodeID destination)
            : Dijkstra(graph, source, destination,
                       &Dijkstra::arcCost, &Dijkstra::admissableArc) // default cost and admissable
        {
        }

        template <typename ArcCost, typename AdmissableArc>
        Dijkstra(GraphI const &graph,
                 nodeID const source,
                 nodeID const destination,
                 ArcCost const arccost = &Dijkstra::arcCost,
                 AdmissableArc const admissablearc = &Dijkstra::admissableArc)
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

                    // Skip admissable arcs. ONLY for Graph with arc data.
                    if (!admissablearc(node, neigh))
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
                nodeID current = destination;
                while (current != source)
                {
                    path.push_back(current);
                    path_arcs.push_back({predecessor[current], current});
                    current = predecessor[current];
                }
                path.push_back(current);
                std::reverse(path.begin(), path.end());
                std::reverse(path_arcs.begin(), path_arcs.end());
                cost = costs[destination];
                LOG_DEBUG("Found Shortest path!", path, cost);
            }
            else
            {
                LOG_DEBUG("Unable to find path from", source, "to", destination);
            }
        }

        static float arcCost(nodeID const A, nodeID const B);

        static bool admissableArc(nodeID const A, nodeID const B);

        std::vector<nodeID> predecessor;
        std::vector<nodeID> path;
        std::vector<arcID> path_arcs;
        std::vector<float> costs;
        int status;
        float cost;

        std::vector<nodeID>::const_iterator begin() const { return path.begin(); };
        std::vector<nodeID>::const_iterator end() const { return path.end(); };
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
