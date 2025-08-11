#include "Nilib/Structures/GraphRepresentation.hpp"

using namespace Nilib;

nodeID AdjacencyList::addNode()
{
    size_t nnodes = data.size();
    data[nodeID{nnodes}];
    return nodeID{nnodes};
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
std::vector<nodeID> AdjacencyList::down_stream(nodeID node) const
{
    return std::vector<nodeID>(data.at(node).begin(), data.at(node).end());
};

std::vector<nodeID> AdjacencyList::up_stream(nodeID node) const
{
    std::vector<nodeID> upstream;
    for (auto &&[nodeid, list] : data)
        for (auto &&node_l : list)
            if (node_l == node)
                upstream.push_back(nodeid);
    return upstream;
};

std::vector<nodeID> AdjacencyList::adjacent(nodeID node) const
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