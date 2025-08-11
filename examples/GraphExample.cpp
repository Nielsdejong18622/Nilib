#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Structures/Graph.hpp"
#include "Nilib/Structures/GraphFunctions.hpp"

int main()
{
    using namespace Nilib;
    using namespace Nilib::GraphFunctions;

    // Step 1. Make a simple graph that stores relations.
    {
        Graph graph2;

        // Add two nodes
        LOG_DEBUG("Graph2 empty?:", graph2.empty());
        nodeID nodeA = graph2.addNode();
        nodeID nodeB = graph2.addNode();
        nodeID nodeC = graph2.addNode();

        LOG_DEBUG("Graph2 empty?:", graph2.empty());

        graph2.remove(nodeC);
        nodeID nodeD = graph2.addNode();

        graph2.addArc(nodeA, nodeB);
        graph2.addArc(nodeA, nodeD);
        // graph2.addArc(nodeA, nodeB);

        graph2.print();
        auto res = Dijkstra(graph2, nodeA, nodeB);

        res.path;
        res.status;
        res.steps;
        res.cost;

        Graph graph = graph2;
        LOG_DEBUG("Constructed graph with", graph.numnodes(), "nodes and", graph.numedges(), "edges!");

        LOG_DEBUG("First downstream node of nodeA", nodeA, ':', graph.down_stream(nodeA).front(), graph.down_stream(nodeA).back());
        LOG_DEBUG("First downstream node of nodeB", nodeB, ':', graph.up_stream(nodeB).front());

        Nilib::Window window(1024, 800, "GraphDrawing");

        drawGraph(window, graph);
    }

    struct NodeData
    {
        int potential;
        char letter;
    };

    struct ArcData
    {
        int capacity;
        float flow;
    };

    // This stores NodeData per nodeid.
    {
        Graph<NodeData> graph;

        graph.print();
    }

    // This stores data per arc.
    {
        Graph<void, std::string> graph;

        auto b = graph.addNode();
        auto a = graph.addNode();

        for (size_t node_i = 0; node_i < 10; node_i++)
        {
            graph.addNode();
        }

        graph.addArc(a, 1, "test");

        graph.print();
    }

    // This stores data per node and arc.
    {
        Graph<NodeData, ArcData> graph;
        auto node1 = graph.addNode(NodeData{5, 'a'});
        auto node2 = graph.addNode(NodeData{72, 'c'});

        graph.addArc(node1, node2, ArcData{.capacity = 5, .flow = 10.0f});
        auto res = Dijkstra(graph, node1, node2);
    }

    LOG_SUCCESS("Completed Graph Example!");
    return 0;
}