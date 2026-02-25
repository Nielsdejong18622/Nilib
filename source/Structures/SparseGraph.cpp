#include "Nilib/Structures/SparseGraph.hpp"
#include "Nilib/Core/Assert.hpp"
#include "Nilib/Math/RNG.hpp"

void Nilib::SparseGraph::reserve(size_t const numNodes, size_t const numEdges)
{
    head.reserve(numNodes + 1);
    edges.reserve(numEdges);
}

bool Nilib::SparseGraph::contains(node_t const node_id) const
{
    return node_id < numNodes();
}
bool Nilib::SparseGraph::contains(node_t const node_A, node_t const node_B) const
{
    return contains(node_A) and
           contains(node_B) and
           std::binary_search(edges.begin() + head[node_A], edges.begin() + head[node_A + 1], node_B);
}

size_t Nilib::SparseGraph::numNodes() const
{
    return head.size() - 1;
}

size_t Nilib::SparseGraph::numEdges() const
{
    return edges.size();
}

size_t Nilib::SparseGraph::out_degree(node_t const node_id) const
{
    ASSERT(node_id < numNodes(), "Non-existant node", node_id, "has no outdegree");
    return head[node_id + 1] - head[node_id];
}

size_t Nilib::SparseGraph::in_degree(node_t const node_id) const
{
    ASSERT(node_id < numNodes(), "Non-existant node", node_id, "has no indegree");
    return std::count(edges.begin(), edges.end(), node_id);
}

void Nilib::SparseGraph::addNode()
{
    if (numNodes() == 0)
        head.push_back(numEdges());
    head.push_back(numEdges());
}

void Nilib::SparseGraph::print()
{
    LOG_DEBUG("SparseGraph", this);
    LOG_DEBUG("Head:", head);
    LOG_DEBUG("Edges:", edges);
}

// Removing a node will destroy the persistent index of the graph labels.
void Nilib::SparseGraph::disconnectNode(uint32_t const node_id)
{
    ASSERT(node_id < numNodes(), "Can not remove non-existant node", node_id);
    size_t const node_out_deg = out_degree(node_id);

    // Remove outgoing edges.
    for (size_t idx = 0; idx < node_out_deg; ++idx)
    {
        remEdge(node_id, edges[head[node_id]]);
    }

    // Remove ingoing edges.
    size_t const node_in_deg = in_degree(node_id);
    for (node_t idx = 0; idx < node_in_deg; ++idx)
    {
        auto first = std::find(edges.begin(), edges.end(), node_id);
        edges.erase(first);
        // Decrement head.
        for (edge_t jdx = first + 1 - edges.begin(); jdx < numNodes() + 1; jdx++)
        {
            head[jdx] -= head[jdx] != node_id;
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
    head.erase(head.begin() + node_id);

    // Bump all node_ids higher than node_id down 1.
    for (edge_t idx = 0; idx < numEdges(); idx++)
    {
        edges[idx] -= edges[idx] > node_id;
    }
}

void Nilib::SparseGraph::addEdge(node_t const from, node_t const to)
{
    ASSERT(from < numNodes(), "Can not relate edge to", from);
    ASSERT(to < numNodes(), "Can not relate edge to", to);

    // Get the first edge of the from-node.
    node_t const out_start = head[from];
    node_t const out_end = head[from + 1];

    // Lower_bound ensures we can binary search over outgoing neigbors.
    // Also makes representations comparable.
    edges.insert(std::lower_bound(edges.begin() + out_start, edges.begin() + out_end, to), to);

    // Add +1 to all heads[from + 1:]
    for (node_t idx = from + 1; idx < numNodes() + 1; ++idx)
    {
        head[idx]++;
    }
}

// Does not remove multi-edges, only one edge!
void Nilib::SparseGraph::remEdge(node_t const from, node_t const to)
{
    ASSERT(from < numNodes(), "Can not relate edge to", from);
    ASSERT(to < numNodes(), "Can not relate edge to", to);
    ASSERT(head[from] != head[from + 1], "Node", from, "has no outgoing edges to remove!");

    auto it = std::lower_bound(edges.begin() + head[from], edges.begin() + head[from + 1], to);
    if (it != edges.end())
        edges.erase(it);

    // Add -1 to all heads[from + 1:]
    for (node_t idx = from + 1; idx < numNodes() + 1; ++idx)
    {
        head[idx]--;
    }
}

Nilib::SparseGraph Nilib::SparseGraph::random(size_t const numNodes, size_t const numEdges)
{
    SparseGraph rand;
    rand.head.resize(numNodes + 1); // Default constructed (0).
    rand.edges.resize(numEdges);    // Default constructed (0).

    // Heads should be an increasing sequence from 0 -> numNodes + 1
    for (node_t idx = 1; idx < numNodes + 1; ++idx)
    {
        node_t const inc = Nilib::RNG::prob() * numNodes > 1.0;
        rand.head[idx] = std::min(rand.head[idx - 1] + inc, static_cast<node_t>(numNodes + 1));
    }

    // Edges can be sampled from Nilib::RNG::index(numNodes + 1)
    for (edge_t idx = 0; idx < numEdges; ++idx)
    {
        rand.edges[idx] = Nilib::RNG::index(numNodes);
    }

    return rand;
}

Nilib::SparseGraph Nilib::SparseGraph::empty()
{
    return SparseGraph();
}

bool Nilib::SparseGraph::operator==(Nilib::SparseGraph const &other) const
{
    return head == other.head && edges == other.edges;
}
