#include "Nilib/Structures/GraphFunctions.hpp"

#include "Nilib/Core/Assert.hpp"
#include "Nilib/Renderer/Window.hpp"

using namespace Nilib;

#include <queue>

bool Dijkstra::admissableArc(nodeID const A, nodeID const B)
{
    return true;
}

float Dijkstra::arcCost(nodeID const A, nodeID const B)
{
    return 1.0f;
}

// Shortest path form source -> destination.


void drawGraph(Window &window, GraphI const &graph)
{
    LOG_DEBUG("Drawing graph to window!");
    LOG_DEBUG("Constructing GraphLayout object...");
}

// void Graph::dijkstra()
// {
// }