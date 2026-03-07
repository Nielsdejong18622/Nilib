#include "Nilib/Structures/DenseGraph.hpp"

void Nilib::DenseGraph::reserve(size_t const numNodes, size_t const numEdges)
{
    CORE_ASSERT(numNodes * numNodes == numEdges);
    d_adj = Nilib::Matrixf::zeros(numNodes, numNodes);
}

bool Nilib::DenseGraph::contains(node_t const node_id) const
{
    return node_id < numNodes();
}
bool Nilib::DenseGraph::contains(node_t const node_A, node_t const node_B) const
{
    return contains(node_A) and
           contains(node_B) and
           d_adj[node_A * numNodes() + node_B] == 1.0;
}

size_t Nilib::DenseGraph::numNodes() const
{
    return d_adj.cols();
}

size_t Nilib::DenseGraph::numEdges() const
{
    size_t sum = 0;
    size_t const n = numNodes();
    for (size_t idx = 0; idx < n; ++idx)
    {
        for (size_t jdx = 0; jdx < n; ++jdx)
        {
            sum += static_cast<size_t>(d_adj(idx, jdx));
        }
    }
    return sum;
}

size_t Nilib::DenseGraph::out_degree(node_t const node_id) const
{
    ASSERT(node_id < numNodes(), "Non-existant node", node_id, "has no outdegree");
    size_t sum = 0;
    for (size_t jdx = 0; jdx < numEdges(); ++jdx)
    {
        sum += d_adj(node_id, jdx);
    }
    return sum;
}

size_t Nilib::DenseGraph::in_degree(node_t const node_id) const
{
    ASSERT(node_id < numNodes(), "Non-existant node", node_id, "has no indegree");
    size_t sum = 0;
    for (size_t jdx = 0; jdx < numEdges(); ++jdx)
    {
        sum += d_adj(jdx, node_id) > 0.5;
    }
    return sum;
}

void Nilib::DenseGraph::addNode()
{
    size_t const n = numNodes();
    StoreMatrix new_adj = StoreMatrix::zeros(n + 1, n + 1);
    for (size_t idx = 0; idx < n; ++idx)
    {
        for (size_t jdx = 0; jdx < n; ++jdx)
        {
            new_adj(idx, jdx) = d_adj(idx, jdx);
        }
    }
    d_adj = new_adj;
}

void Nilib::DenseGraph::print()
{
    LOG_DEBUG("DenseGraph", this);
    LOG_DEBUG("Adjacency:", d_adj);
}

void Nilib::DenseGraph::disconnectNode(uint32_t const node_id)
{
    ASSERT(node_id < numNodes(), "Can not remove non-existant node", node_id);

    for (size_t jdx = 0; jdx < numEdges(); ++jdx)
    {
        d_adj(jdx, node_id) = 0.0;
        d_adj(node_id, jdx) = 0.0;
    }
}

// Removing a node will destroy the persistent index of the graph labels.
void Nilib::DenseGraph::removeNode(uint32_t const node_id)
{
    ASSERT(node_id < numNodes(), "Can not remove non-existant node", node_id);
    ASSERT(node_id > 0, "Can not remove non-existant node", node_id);
    size_t const n = numNodes();
    StoreMatrix new_adj = StoreMatrix::zeros(n - 1, n - 1);
    for (size_t idx = 0; idx < n - 1; ++idx)
    {
        for (size_t jdx = 0; jdx < n - 1; ++jdx)
        {
            new_adj(idx, jdx) = d_adj(idx, jdx);
        }
    }
    d_adj = new_adj;
}

void Nilib::DenseGraph::addEdge(node_t const from, node_t const to)
{
    ASSERT(from < numNodes(), "Can not relate edge to", from);
    ASSERT(to < numNodes(), "Can not relate edge to", to);

    d_adj(from, to) = 1.0;
}

// Does not remove multi-edges, only one edge!
void Nilib::DenseGraph::remEdge(node_t const from, node_t const to)
{
    ASSERT(from < numNodes(), "Can not relate edge to", from);
    ASSERT(to < numNodes(), "Can not relate edge to", to);
    ASSERT(d_adj(from, to) == 1.0, "Edge from", from, "to", to, "not present");
    d_adj(from, to) = 0.0;
}

Nilib::DenseGraph Nilib::DenseGraph::random(size_t const numNodes, size_t const numEdges)
{
    ASSERT(numEdges <= numNodes * numNodes, "A dense graph can not have more than n^2 edges!");
    DenseGraph rand;
    rand.d_adj = StoreMatrix::rand(numNodes, numNodes);
    rand.d_adj.apply([](float const t)
                     { return (t > 0.5) ? 1.0f : 0.0f; });
    return rand;
}

Nilib::DenseGraph Nilib::DenseGraph::connected(size_t numNodes, bool self_loops)
{
    DenseGraph full;
    full.d_adj = StoreMatrix::all(numNodes, numNodes, 1.0f);
    for (size_t node_id = 0; node_id < numNodes and self_loops; node_id++)
    {
        full.d_adj(node_id, node_id) = 0.0f;
    }

    return full;
}
Nilib::DenseGraph Nilib::DenseGraph::empty()
{
    return DenseGraph();
}

bool Nilib::DenseGraph::operator==(Nilib::DenseGraph const &other) const
{
    size_t const n = numNodes();
    if (other.numNodes() != numNodes() or other.numEdges() != numEdges())
        return false;
    for (size_t idx = 0; idx < n; ++idx)
    {
        for (size_t jdx = 0; jdx < n; ++jdx)
        {
            if (other.d_adj(idx, jdx) != d_adj(idx, jdx))
                return false;
        }
    }
    return true;
}
