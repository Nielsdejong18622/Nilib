#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Structures/Graph.hpp"

int main()
{
    using namespace Nilib::Graph;

    // Step 1. Make a simple graph that stores relations. 
    Graph<AdjacencyRep> graph;

    graph.addNode(1);
    graph.addNode(2);
    graph.addEdge(1, 2);


    // Step 2. Use graphmaps to create graphWithDatatype
    
    //DiGraphWithData<ListRepresentation, Location> g;
    //DiGraphWithData<ListRepresentation, Location, Arc> g;

    // node_id n1 = g.addNode({1, 2});
    // node_id dep = g.addNode({1, 2});
    // edge_id e1 = g.addEdge(n1, dep);

    return 0;
}