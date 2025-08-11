#include "Nilib/Structures/GraphFunctions.hpp"

#include "Nilib/Core/Assert.hpp"
#include "Nilib/Renderer/Window.hpp"

using namespace Nilib;

// Shortest path form source -> destination.
GraphFunctions::Dijkstra::Dijkstra(Graph<> const &graph, nodeID const source, nodeID const destination)
{
    CORE_ASSERT(graph.contains(source));
    CORE_ASSERT(graph.contains(destination));
    LOG_DEBUG("Searching for shortest path from", source, "to", destination, "using Dijkstra's algorithm.");
}

void GraphFunctions::drawGraph(Window &window, GraphI const &graph)
{
    LOG_DEBUG("Drawing graph to window!");
    LOG_DEBUG("Constructing GraphLayout object...");
}

// void Graph::dijkstra()
// {
// }