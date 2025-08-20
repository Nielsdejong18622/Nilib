#include "Nilib/Structures/AdjacencyList.hpp"
#include "Nilib/Core/Assert.hpp"

using namespace Nilib;

std::vector<nodeID> AdjacencyList::addNodes(size_t const n)
{
    std::vector<nodeID> res;
    res.reserve(n);
    for (size_t i = 0; i < n; ++i)
        res.push_back(addNode());
    return res;
};

nodeID AdjacencyList::addNode()
{
    nodeID const node_id = d_data.add({});
    return node_id;
};

void AdjacencyList::remove(nodeID node)
{
    // Removes all outgoing arcs from node and remove the node itself.
    d_data.remove(node);
    // Remove all incoming arcs from a node.
    for (auto it = d_data.begin(); it != d_data.end(); ++it)
        std::erase(d_data[it.getIndex()], node);
};

// Adding and removing arcs/edges.
void AdjacencyList::addArc(nodeID node_A, nodeID node_B)
{
    ASSERT(d_data.contains(node_A), node_A, "not in graph!");
    ASSERT(d_data.contains(node_B), node_B, "not in graph!");
    d_data[node_A].push_back(node_B);
    // return d_arc_data.add({node_A, node_B});
};

void AdjacencyList::remove(nodeID node_A, nodeID node_B)
{
    auto iter_B = std::find(d_data[node_A].begin(), d_data[node_A].end(), node_B);
    CORE_ASSERT(iter_B != d_data[node_A].end());
    d_data[node_A].erase(iter_B);
};

// Make the empty graph.
void AdjacencyList::clear()
{
    d_data.clear();
};

// Properties.
size_t AdjacencyList::numnodes() const
{
    return d_data.size();
};

size_t AdjacencyList::nodes_capacity() const
{
    return d_data.capacity();
}

size_t AdjacencyList::numedges() const
{
    size_t num_edges = 0;
    for (auto &list : d_data)
        num_edges += list.size();
    return num_edges;
};
bool AdjacencyList::empty() const
{
    return d_data.empty();
};
bool AdjacencyList::contains(nodeID node) const
{
    return d_data.contains(node);
};

bool AdjacencyList::contains(nodeID nodeA, nodeID nodeB) const
{
    return d_data.contains(nodeA) && d_data.contains(nodeB) && std::find(d_data[nodeA].begin(), d_data[nodeA].end(), nodeB) != d_data[nodeA].end();
};

bool AdjacencyList::adjacent(nodeID nodeA, nodeID nodeB) const
{
    return std::find(d_data[nodeA].begin(), d_data[nodeA].end(), nodeB) != d_data[nodeA].end();
};

size_t AdjacencyList::indegree(nodeID node) const
{
    size_t indegree = 0;
    for (auto &&list : d_data)
        for (auto &&nodeB : list)
            indegree += (nodeB == node);
    return indegree;
};
size_t AdjacencyList::outdegree(nodeID node) const
{
    return d_data[node].size();
};

// Traversal
GraphRange<nodeID> AdjacencyList::down_stream(nodeID const node) const
{
    return GraphRange<nodeID>(GraphIterator<nodeID>(std::make_unique<NeighborIteratorImpl>(d_data[node].begin())),
                              GraphIterator<nodeID>(std::make_unique<NeighborIteratorImpl>(d_data[node].end())));
};

GraphRange<nodeID> AdjacencyList::up_stream(nodeID const node) const
{
    return GraphRange<nodeID>(GraphIterator<nodeID>(std::make_unique<RevNeighborIteratorImpl>(node, d_data.begin(), d_data.end())),
                              GraphIterator<nodeID>(std::make_unique<RevNeighborIteratorImpl>(node, d_data.end(), d_data.end())));
};

GraphRange<nodeID> AdjacencyList::adjacent(nodeID const node) const
{
    return down_stream(node);
};

void AdjacencyList::print() const
{
    LOG_DEBUG("Printing Graph", this, "with", numnodes(), "nodes and", numedges(), "edges.");
    for (auto it = d_data.cbegin(); it != d_data.cend(); ++it)
    {
        LOG_DEBUG() << it.getIndex() << "-> [";
        bool first = true;
        for (auto nodeB = it->cbegin(); nodeB != it->cend(); ++nodeB)
        {
            if (!first)
                LOG_DEBUG() << ',' << *nodeB;
            else
                LOG_DEBUG() << *nodeB;
            first = false;
        }
        LOG_DEBUG() << ']' << '\n';
    }
}