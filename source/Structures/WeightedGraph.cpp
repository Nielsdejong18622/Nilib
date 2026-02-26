#include "Nilib/Structures/WeightedGraph.hpp"

namespace Nilib
{
    template <>
    Nilib::Weighted<SparseGraph> Nilib::Weighted<SparseGraph>::empty()
    {
        return Nilib::Weighted<SparseGraph>();
    }

    template <>
    Nilib::Weighted<DenseGraph> Nilib::Weighted<DenseGraph>::empty()
    {
        return Nilib::Weighted<DenseGraph>();
    }

    template <>
    Nilib::Weighted<SparseGraph> Nilib::Weighted<SparseGraph>::random(size_t const numNodes, size_t const numEdges)
    {
        Weighted<SparseGraph> res(SparseGraph::random(numNodes, numEdges));
        res.weights.resize(numEdges);
        for (uint32_t idx = 0; idx < numEdges; ++idx)
        {
            res.weights[idx] = Nilib::RNG::rounded_uniform(0, numNodes, 2);
        }
        return res;
    }

    template <>
    Nilib::Weighted<DenseGraph> Nilib::Weighted<DenseGraph>::random(size_t const numNodes, size_t const numEdges)
    {
        Weighted<DenseGraph> res(DenseGraph::random(numNodes, numEdges));
        res.weights.resize(numNodes * numNodes);
        for (uint32_t idx = 0; idx < numNodes * numNodes; ++idx)
        {
            res.weights[idx] = Nilib::RNG::rounded_uniform(0, numNodes, 2) * res.adj(idx);
        };
        return res;
    }

    template <>
    void Nilib::Weighted<DenseGraph>::addEdge(node_t const node1, node_t const node2, weight_t const weight)
    {
        LOG_DEBUG("Adding weighted edge!", node1, node2, weight, weights.size());
        DenseGraph::addEdge(node1, node2);
        edge_t const e_idx = node1 * numNodes() + node2;
        if (e_idx < weights.size())
        {
            weights[e_idx] = weight;
        }
        else
        {
            weights.resize(numNodes() * numNodes());
            weights[e_idx] = weight;
        }
    }

    template <>
    void Nilib::Weighted<SparseGraph>::addEdge(node_t const node1, node_t const node2, weight_t const weight)
    {
        SparseGraph::addEdge(node1, node2);
        weights.push_back(weight);
    }

    template <>
    void Nilib::Weighted<DenseGraph>::print()
    {
        LOG_DEBUG("Weighted DenseGraph", this);
        LOG_DEBUG("Adjacency:", adj);
        adj.print();
        LOG_DEBUG("Weights:", weights);
    }

    template <>
    void Nilib::Weighted<SparseGraph>::print()
    {
        LOG_DEBUG("Weighted SparseGraph", this);
        LOG_DEBUG("Head:", head);
        LOG_DEBUG("Edges:", edges);
        LOG_DEBUG("Weights:", weights);
    }
}; // namespace Nilib