#include "Nilib/Structures/GraphRepresentation.hpp"

using namespace Nilib;

void Nilib::erdos_renyi(GraphI &graph, size_t const numnodes, float const prob)
{
    LOG_INFO("Constructing erdos_renyi graph!");

    graph.clear();
    graph.addNodes(numnodes);

    for (size_t node_i = 0; node_i < numnodes; ++node_i)
    {
        for (size_t node_j = 0; node_j < numnodes; ++node_j)
        {
            if (RNG::prob() < prob)
            {
                graph.addArc(node_i, node_j);
            }
        }
    }
}

// C describes how many neighbours there are.
void Nilib::ring_world(GraphI &graph, size_t const numnodes, size_t const C)
{
    LOG_INFO("Constructing ring world graph!");
    graph.clear();

    graph.addNodes(numnodes);

    size_t c = C / 2;
    for (nodeID idx = 0; idx < numnodes; idx++)
        for (nodeID add = 0; add < C + 1; ++add)
            if (idx != ((idx + add - c) + numnodes) % numnodes)
                graph.addArc(idx, ((idx + add - c) + numnodes) % numnodes);
}

void Nilib::small_world(GraphI &graph, size_t const numnodes, size_t const C, float const rewire_prob)
{
    ring_world(graph, numnodes, C);

    // for (auto &&i : graph.nodes())
    // {

    // }
}

std::vector<nodeID> AdjacencyList::addNodes(size_t const n)
{
    std::vector<nodeID> res;
    for (size_t i = 0; i < n; ++i)
    {
        res.push_back(addNode());
    }
    return res;
};

nodeID AdjacencyList::addNode()
{
    nodeID node_id = nodeID{data.size()};
    data[node_id];
    return node_id;
};

void AdjacencyList::remove(nodeID node)
{
    data.erase(node);
};

// Adding and removing arcs/edges.
void AdjacencyList::addArc(nodeID node_A, nodeID node_B)
{
    ASSERT(data.contains(node_A), node_A, "not in graph!");
    ASSERT(data.contains(node_B), node_B, "not in graph!");
    data[node_A].push_back(node_B);
};

void AdjacencyList::remove(nodeID node_A, nodeID node_B)
{
    auto iter_B = std::find(data.at(node_A).begin(), data.at(node_A).end(), node_B);
    CORE_ASSERT(iter_B != data[node_A].end());
    data[node_A].erase(iter_B);
};

// Make the empty graph.
void AdjacencyList::clear()
{
    data.clear();
};

// Properties.
size_t AdjacencyList::numnodes() const
{
    return data.size();
};
size_t AdjacencyList::numedges() const
{
    size_t num_edges = 0;
    for (auto &&[nodeid, list] : data)
        num_edges += list.size();
    return num_edges;
};
bool AdjacencyList::empty() const
{
    return data.size() == 0;
};
bool AdjacencyList::contains(nodeID node) const
{
    return node < data.size();
};

bool AdjacencyList::contains(nodeID nodeA, nodeID nodeB) const
{
    return adjacent(nodeA, nodeB);
};

bool AdjacencyList::adjacent(nodeID nodeA, nodeID nodeB) const
{
    return std::find(data.at(nodeA).begin(), data.at(nodeA).end(), nodeB) != data.at(nodeA).end();
};

size_t AdjacencyList::indegree(nodeID node) const
{
    size_t indegree = 0;
    for (auto &&[nodeA, list] : data)
        for (auto &&nodeB : list)
            indegree += (nodeB == node);
    return indegree;
};
size_t AdjacencyList::outdegree(nodeID node) const
{
    return data.at(node).size();
};

// Traversal
GraphRange<nodeID> AdjacencyList::down_stream(nodeID const node) const
{
    return GraphRange<nodeID>(GraphIterator<nodeID>(std::make_unique<NeighborIteratorImpl>(data.at(node).begin())),
                              GraphIterator<nodeID>(std::make_unique<NeighborIteratorImpl>(data.at(node).end())));
};

GraphRange<nodeID> AdjacencyList::up_stream(nodeID const node) const
{
    return GraphRange<nodeID>(GraphIterator<nodeID>(std::make_unique<RevNeighborIteratorImpl>(node, data.begin(), data.end())),
                              GraphIterator<nodeID>(std::make_unique<RevNeighborIteratorImpl>(node, data.end(), data.end())));
};

GraphRange<nodeID> AdjacencyList::adjacent(nodeID const node) const
{
    return down_stream(node);
};

void AdjacencyList::print() const
{
    LOG_DEBUG("Printing Graph", this, "with", numnodes(), "nodes and", numedges(), "edges.");
    for (auto &&[nodeA, list] : data)
    {
        LOG_DEBUG() << nodeA << "-> [";
        for (auto &&nodeB : list)
        {
            if (nodeB != list.back())
                LOG_DEBUG() << nodeB << ',';
            else
                LOG_DEBUG() << nodeB;
        }
        LOG_DEBUG() << ']' << '\n';
    }
}