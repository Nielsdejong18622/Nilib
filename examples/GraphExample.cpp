#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Structures/SparseGraph.hpp"
#include "Nilib/Structures/DenseGraph.hpp"
#include "Nilib/Structures/WeightedGraph.hpp"
#include "Nilib/Structures/GraphFunctions.hpp"

template <typename Graph>
void test()
{
    using namespace Nilib;

    Graph graph1;
    graph1.addNode();
    graph1.addNode();
    graph1.addNode();
    graph1.addNode();
    // graph1.addNode();

    graph1.addEdge(0, 2);
    graph1.addEdge(1, 0);
    graph1.addEdge(1, 1);
    graph1.addEdge(3, 1);
    graph1.addEdge(3, 2);
    graph1.addEdge(2, 1);
    graph1.addEdge(2, 3);
    graph1.addEdge(2, 3);
    graph1.print();
    Graph graph2(graph1);
    // graph2.remEdge(1, 1);
    graph2.disconnectNode(1);

    graph2.addEdge(1, 1);
    graph2.addEdge(1, 0);
    graph2.addEdge(2, 1);
    graph2.addEdge(3, 1);
    LOG_DEBUG("Constructed Graph2!");
    graph2.print();

    if (graph1 == graph2)
    {
        LOG_SUCCESS("Test [graph/construct/destroy] Passed!");
    }
    else
    {
        LOG_ERROR("Test [graph/construct/destroy] failed!");
    }

    Graph graph3(graph2);
    graph3.removeNode(1);

    Graph rgraph = Graph::random(10, 40);

    Weighted<Graph> wrgraph = Weighted<Graph>::random(5'000, 1'000'000);

    // wrgraph.print();

    BellManFord shortestpaths = BellManFord(wrgraph, 0);
}

int main()
{
    using namespace Nilib;

    RNG::seed(127);
    test<SparseGraph>();
}