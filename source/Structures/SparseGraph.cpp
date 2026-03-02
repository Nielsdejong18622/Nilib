#include "Nilib/Structures/SparseGraph.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Math/RNG.hpp"

void Nilib::SparseGraph::reserve(size_t const numNodes, size_t const numEdges)
{
    d_head.reserve(numNodes + 1);
    d_edges.reserve(numEdges);
}

bool Nilib::SparseGraph::contains(node_t const node_id) const
{
    return node_id < numNodes();
}
bool Nilib::SparseGraph::contains(node_t const node_A, node_t const node_B) const
{
    return contains(node_A) and
           contains(node_B) and
           std::binary_search(d_edges.begin() + d_head[node_A], d_edges.begin() + d_head[node_A + 1], node_B);
}

size_t Nilib::SparseGraph::numNodes() const
{
    return d_head.size() - 1;
}

size_t Nilib::SparseGraph::numEdges() const
{
    return d_edges.size();
}

size_t Nilib::SparseGraph::out_degree(node_t const node_id) const
{
    ASSERT(node_id < numNodes(), "Non-existant node", node_id, "has no outdegree");
    return d_head[node_id + 1] - d_head[node_id];
}

size_t Nilib::SparseGraph::in_degree(node_t const node_id) const
{
    ASSERT(node_id < numNodes(), "Non-existant node", node_id, "has no indegree");
    return std::count(d_edges.begin(), d_edges.end(), node_id);
}

void Nilib::SparseGraph::addNode()
{
    if (numNodes() == 0)
        d_head.push_back(numEdges());
    d_head.push_back(numEdges());
}

void Nilib::SparseGraph::print()
{
    LOG_DEBUG("SparseGraph", this);
    LOG_DEBUG("Head:", d_head);
    LOG_DEBUG("Edges:", d_edges);
}

// Removing a node will destroy the persistent index of the graph labels.
void Nilib::SparseGraph::disconnectNode(uint32_t const node_id)
{
    ASSERT(node_id < numNodes(), "Can not remove non-existant node", node_id);
    size_t const node_out_deg = out_degree(node_id);

    // Remove outgoing edges.
    for (size_t idx = 0; idx < node_out_deg; ++idx)
    {
        remEdge(node_id, d_edges[d_head[node_id]]);
    }

    // Remove ingoing edges.
    size_t const node_in_deg = in_degree(node_id);
    for (node_t idx = 0; idx < node_in_deg; ++idx)
    {
        auto first = std::find(d_edges.begin(), d_edges.end(), node_id);
        d_edges.erase(first);
        // Decrement head.
        for (edge_t jdx = first + 1 - d_edges.begin(); jdx < numNodes() + 1; jdx++)
        {
            d_head[jdx] -= d_head[jdx] != node_id;
        }
    }
    // Note: This leaves a [0, 1, ..., node_id, node_id, ...,  numNodes-1] behind.
}

void Nilib::SparseGraph::removeNode(uint32_t const node_id)
{
    ASSERT(node_id < numNodes(), "Can not remove non-existant node", node_id);
    disconnectNode(node_id);

    // This leaves a [0, 1, ..., node_id, node_id, ...,  numNodes-1] behind.
    // Delete the node_id, node_id.
    d_head.erase(d_head.begin() + node_id);

    // Bump all node_ids higher than node_id down 1.
    for (edge_t idx = 0; idx < numEdges(); idx++)
    {
        d_edges[idx] -= d_edges[idx] > node_id;
    }
}

void Nilib::SparseGraph::addEdge(node_t const from, node_t const to)
{
    ASSERT(from < numNodes(), "Can not relate edge to", from);
    ASSERT(to < numNodes(), "Can not relate edge to", to);

    // Get the first edge of the from-node.
    node_t const out_start = d_head[from];
    node_t const out_end = d_head[from + 1];

    // Lower_bound ensures we can binary search over outgoing neigbors.
    // Also makes representations comparable.
    d_edges.insert(std::lower_bound(d_edges.begin() + out_start, d_edges.begin() + out_end, to), to);

    // Add +1 to all heads[from + 1:]
    for (node_t idx = from + 1; idx < numNodes() + 1; ++idx)
    {
        d_head[idx]++;
    }
}

// Does not remove multi-edges, only one edge!
void Nilib::SparseGraph::remEdge(node_t const from, node_t const to)
{
    ASSERT(from < numNodes(), "Can not relate edge to", from);
    ASSERT(to < numNodes(), "Can not relate edge to", to);
    ASSERT(d_head[from] != d_head[from + 1], "Node", from, "has no outgoing edges to remove!");

    auto it = std::lower_bound(d_edges.begin() + d_head[from], d_edges.begin() + d_head[from + 1], to);
    if (it != d_edges.end())
        d_edges.erase(it);

    // Add -1 to all heads[from + 1:]
    for (node_t idx = from + 1; idx < numNodes() + 1; ++idx)
    {
        d_head[idx]--;
    }
}

Nilib::SparseGraph Nilib::SparseGraph::random(size_t const numNodes, size_t const numEdges)
{
    SparseGraph rand;
    rand.d_head.resize(numNodes + 1); // Default constructed (0).
    rand.d_edges.resize(numEdges);    // Default constructed (0).

    // Heads should be an increasing sequence from 0 -> numNodes + 1
    for (node_t idx = 1; idx < numNodes + 1; ++idx)
    {
        node_t const increment = Nilib::RNG::prob() > 1.0f / static_cast<float>(numNodes + 1);
        rand.d_head[idx] = std::min(rand.d_head[idx - 1] + increment, static_cast<node_t>(numNodes + 1));
    }

    // Edges can be sampled from Nilib::RNG::index(numNodes + 1)
    for (edge_t idx = 0; idx < numEdges; ++idx)
    {
        rand.d_edges[idx] = Nilib::RNG::index(numNodes);
    }

    return rand;
}

Nilib::SparseGraph Nilib::SparseGraph::empty()
{
    return SparseGraph();
}

bool Nilib::SparseGraph::operator==(Nilib::SparseGraph const &other) const
{
    return d_head == other.d_head && d_edges == other.d_edges;
}
