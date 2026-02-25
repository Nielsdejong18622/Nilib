#ifndef _WEIGHTED_GRAPH_H
#define _WEIGHTED_GRAPH_H

#include "Nilib/Structures/SparseGraph.hpp"
#include "Nilib/Structures/DenseGraph.hpp"

namespace Nilib
{

    // Decorator for a grahp type.
    template <typename Graph>
    class Weighted : public Graph
    {
    public:
        using weight_t = float;
        std::vector<weight_t> weights; // m

        static Weighted empty();
        static Weighted random(size_t const numNodes, size_t const numEdges);

        void print();

    private:
        using Graph::addEdge;
        using Graph::empty;
        using Graph::random;
    };

    // Weighted (Sparse) Graph.
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

#endif
