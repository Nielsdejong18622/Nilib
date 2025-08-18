#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Structures/Graph.hpp"
#include "Nilib/Structures/GraphFunctions.hpp"

int main()
{
    using namespace Nilib;

    Nilib::RNG::seed();

    // Step 1. Make a simple graph that stores relations.
    {
        Graph graph2;

        // Add two nodes
        LOG_DEBUG("Graph2 empty?:", graph2.empty());
        nodeID nodeA = graph2.addNode();
        nodeID nodeB = graph2.addNode();
        nodeID nodeC = graph2.addNode();
        nodeID nodeD = graph2.addNode();
        nodeID nodeE = graph2.addNode();

        graph2.addArc(nodeA, nodeB);
        graph2.addArc(nodeB, nodeC);
        graph2.addArc(nodeC, nodeD);
        graph2.addArc(nodeA, nodeE);
        graph2.addArc(nodeA, nodeB);
        graph2.addArc(nodeE, nodeA);

        char const name[6] = "ABCDE";

        graph2.print();

        Graph graph = graph2;
        LOG_DEBUG("Constructed graph with", graph.numnodes(), "nodes and", graph.numedges(), "edges!");

        LOG_DEBUG("First downstream node of nodeA", nodeA, ':', *graph.down_stream(nodeA).begin(), *graph.down_stream(nodeA).end());
        LOG_DEBUG("First downstream node of nodeB", nodeB, ':', *graph.up_stream(nodeB).begin());
        auto res = Dijkstra(graph2, nodeA, nodeE);

        LOG_INFO("Dijkstra Result:", res.status, res.path, res.cost);

        Nilib::Window window(1024, 800, "GraphDrawing");

        // drawGraph(window, graph);

        for (auto node : graph.nodes())
        {
            LOG_SUCCESS(name[node]);
        }

        LOG_PROGRESS("Iterating over down_stream neighbors of", name[0]);
        for (auto node_it : graph.down_stream(0))
        {
            LOG_PROGRESS(name[node_it]);
        }

        LOG_PROGRESS("Iterating over up_stream neighbors of", name[0]);
        for (auto node_it : graph.up_stream(0))
        {
            LOG_PROGRESS(name[node_it]);
        }

        // LOG_PROGRESS("Random walk over the graph starting at", name[0]);
        // for (auto node_it : graph.random_walk(0))
        // {
        //     LOG_PROGRESS(name[node_it]);
        // }

        LOG_PROGRESS("Iterating over arcs!");
        for (auto arc : graph.arcs())
        {
            LOG_PROGRESS(name[arc.a], name[arc.b]);
            // LOG_PROGRESS(name[(*arc_it).a], name[(*arc_it).b]);
        }
    }

    LOG_SUCCESS("Completed Graph Example!");
    return EXIT_SUCCESS;
    struct NodeData
    {
        char letter;
    };

    struct ArcData
    {
        int capacity;
        float flow;
        float cost;
    };

    // This stores NodeData per nodeid.
    {
        Graph<NodeData> graph;
        // erdos_renyi(graph, 100u, 0.05f);
        ring_world(graph, 100, 1);
        graph.print();

        auto res = Dijkstra(graph, 0, 99);

        // if (res.status)
        {
            LOG_DEBUG("Found shortest path!");
            LOG_DEBUG(res.path);
            // LOG_DEBUG(res.predecessor);
        }
    }

    // This stores data per arc.
    {
        Graph<NodeData, ArcData> graph;

        auto source = graph.addNode(NodeData{.letter = 's'});
        auto terminal = graph.addNode(NodeData{.letter = 't'});

        size_t n_orders = 2;
        size_t n_nodes = 4;

        for (size_t node_i = 0; node_i < n_nodes; node_i++)
        {
            auto node = graph.addNode(NodeData{.letter = node_i});
            // The first nodes are the orders.
            if (node_i < n_orders)
            {
                // source -> orders
                graph.addArc(source, node, ArcData{.capacity = 1, .flow = 0, .cost = 0});
            }
            // The remainder are couriers.
            else
            {
                // courier -> terminal
                int kc = 1; // Courier capacity.
                graph.addArc(node, terminal, ArcData{.capacity = kc, .flow = 0, .cost = 0});
            }
        }

        for (size_t oid = 0; oid < n_orders; oid++)
            for (size_t cid = n_orders; cid < n_nodes; cid++)
            {
                //
                float cij = 10;
                graph.addArc(oid, cid, ArcData{.capacity = 1, .flow = 0, .cost = cij});
            }

        // graph.addArc(a, 1, "test");

        graph.print();
    }

    // This stores data per node and arc.
    {
        Graph<NodeData, ArcData> graph;
        auto node1 = graph.addNode(NodeData{5});
        auto node2 = graph.addNode(NodeData{7});
        auto node3 = graph.addNode(NodeData{7});

        graph.addArc(node1, node2, ArcData{.capacity = 5, .flow = 10.0f, .cost = 10.0f});
        graph.addArc(node2, node3, ArcData{.capacity = 4, .flow = 1.0f, .cost = 10.0f});
        auto res = Dijkstra(graph, node1, node3);

        // if (res.status)
        {
            LOG_DEBUG("Found shortest path!");
            LOG_DEBUG(res.costs);
            LOG_DEBUG(res.predecessor);
        }
        res.cost;
    }
}