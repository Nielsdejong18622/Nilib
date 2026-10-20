#include "Nilib/Structures/WeightedGraph.hpp"
#include "Nilib/Math/LinAlg.hpp"

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
            res.weights[idx] = Nilib::RNG::rounded_uniform(0.0f, 10.0f, 2);
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
            res.weights[idx] = Nilib::RNG::rounded_uniform(0.0f, 10.0f, 2) * res.d_adj(idx);
        };
        return res;
    }

    template <>
    void Nilib::Weighted<DenseGraph>::addEdge(node_t const node1, node_t const node2, weight_t const weight)
    {
        // LOG_DEBUG("Adding weighted edge!", node1, node2, weight, weights.size());
        DenseGraph::addEdge(node1, node2);
        edge_t const e_idx = node1 * numNodes() + node2;
        weights[e_idx] = weight;
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
    }

    template <>
    void Nilib::Weighted<SparseGraph>::print()
    {
    }
}; // namespace Nilib