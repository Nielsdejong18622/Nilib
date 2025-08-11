#ifndef _GRAPH_FUNCTIONS_H
#define _GRAPH_FUNCTIONS_H

#include "Nilib/Structures/Graph.hpp"

namespace Nilib
{
    class Window;

    namespace GraphFunctions
    {
        // A functor that accumulates for the graph it traversls.
        // Visitor pattern.
        struct PathAccumulator
        {
            void operator()(nodeID const node);
            void operator()(nodeID const nodeA, nodeID const nodeB);
        };
        struct StepCount : public PathAccumulator
        {
            int cost = 0;
            void operator()(nodeID const node) { cost += 1; };
            void operator()(nodeID const nodeA, nodeID const nodeB) {};
        };
        struct Dijkstra
        {
            Dijkstra(GraphI const &graph,
                     nodeID const source,
                     nodeID const destination,
                     PathAccumulator );

            std::vector<nodeID> path;
            int status;
            int steps;
            float cost;
        };

        void drawGraph(Window &window, GraphI const &graph);
    }; // namespace GraphFunctions

}; // namespace Nilib

#endif
