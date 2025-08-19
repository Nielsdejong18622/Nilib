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

    struct NodeData
    {
        nodeID letter;
    };

    struct ArcData
    {
        int capacity;
        int flow;
        float cost;
    };

    // This stores NodeData per nodeid.
    {
        Graph<NodeData> graph;
        // erdos_renyi(graph, 100u, 0.05f);
        ring_world(graph, 100, 4);
        graph.print();

        auto res = Dijkstra(graph, 0, 40);

        // if (res.status)
        {
            LOG_DEBUG("Found shortest path!");
            LOG_DEBUG(res.path);
            LOG_DEBUG(res.path_arcs);
            // LOG_DEBUG(res.predecessor);
        }
    }

    // This stores data per arc.
    {
        PROFILE_SCOPE("Creating MinCostFlow!");

        struct ArcData
        {
            int capacity;
            int flow;
            float cost;
        };

        Graph<void, ArcData> graph;

        auto source = graph.addNode();
        auto terminal = graph.addNode();
        auto dummy_courier = graph.addNode();

        int n_orders = 20;   // Excluding dummy order.
        int n_couriers = 20; // Excluding dummy courier.
        int n_nodes = n_orders + n_couriers;

        for (int node_i = 0; node_i < n_nodes; node_i++)
        {
            auto node = graph.addNode();
            // The first nodes are the orders.
            if (node_i < n_orders)
            {
                // source -> orders
                graph.addArc(source, node, ArcData{.capacity = 1, .flow = 0, .cost = 0});
                graph.addArc(node, source, ArcData{.capacity = 1, .flow = 1, .cost = -0});
            }
            // The remainder are couriers.
            else
            {
                // courier -> terminal
                int kc = 1; // Courier capacity.
                graph.addArc(node, terminal, ArcData{.capacity = kc, .flow = 0, .cost = 0});
                graph.addArc(terminal, node, ArcData{.capacity = kc, .flow = kc, .cost = -0});
            }
        }

        for (int oid = 3; oid < n_orders + 3; oid++)
            for (int cid = n_orders + 3; cid < n_nodes + 3; cid++)
            {
                //
                float cij = 10;
                graph.addArc(oid, cid, ArcData{.capacity = 1, .flow = 0, .cost = cij});
                graph.addArc(cid, oid, ArcData{.capacity = 1, .flow = 1, .cost = -cij});
            }
        // graph.arcData[{3, 5}].cost = 2;
        // graph.arcData[{5, 3}].cost = -2;

        // graph.arcData[{4, 6}].cost = 20;
        // graph.arcData[{6, 4}].cost = -20;

        // Order -> Dummy courier arcs.
        for (int oid = 3; oid < n_orders + 3; oid++)
        {
            graph.addArc(oid, dummy_courier, ArcData{.capacity = 1, .flow = 0, .cost = 100});
            graph.addArc(dummy_courier, oid, ArcData{.capacity = 1, .flow = 1, .cost = -100});
        }
        // Dummy -> terminal arcs.
        graph.addArc(dummy_courier, terminal, ArcData{.capacity = n_orders, .flow = 0, .cost = -0});
        graph.addArc(terminal, dummy_courier, ArcData{.capacity = n_orders, .flow = n_orders, .cost = 0});

        for (auto &&arc : graph.arcs())
        {
            // LOG_DEBUG(arc, "flow:", graph.arcData[arc].flow, "capacity:", graph.arcData[arc].capacity, "cost:", graph.arcData[arc].cost);
        }

        graph.print();
        // auto res = Dijkstra(graph, 0, 1, [&graph](nodeID a, nodeID b)
        //                     { return graph.arcs[{a, b}].cost; }, [&graph](nodeID a, nodeID b)
        //                     { return graph.arcs[{a, b}].flow < graph.arcs[{a, b}].capacity; });
        auto res = MinCostFlow(graph, n_orders, source, terminal);

        // Print solution.
        for (auto &&arc : graph.arcs())
        {
            if (graph.arcData[arc].flow > 0 && !(arc.a < 3 || arc.b < 3) && arc.a < arc.b)
            {
                LOG_INFO("Active arcs:", arc);
            }
        }
    }

    LOG_SUCCESS("Completed Graph Example!");
    return EXIT_SUCCESS;
    // // This stores data per node and arc.
    // {
    //     Graph<NodeData, ArcData> graph;
    //     auto node1 = graph.addNode(NodeData{5});
    //     auto node2 = graph.addNode(NodeData{7});
    //     auto node3 = graph.addNode(NodeData{7});

    //     graph.addArc(node1, node2, ArcData{.capacity = 5, .flow = 10.0f, .cost = 10.0f});
    //     graph.addArc(node2, node3, ArcData{.capacity = 4, .flow = 1.0f, .cost = 10.0f});
    //     auto res = Dijkstra(graph, node1, node3);

    //     // if (res.status)
    //     {
    //         LOG_DEBUG("Found shortest path!");
    //         LOG_DEBUG(res.costs);
    //         LOG_DEBUG(res.predecessor);
    //     }
    //     res.cost;
    // }
}