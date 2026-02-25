#ifndef _DENSE_GRAPH_H
#define _DENSE_GRAPH_H

#include <vector>
#include <cstdint>
#include "Nilib/Math/Matrix.hpp"

namespace Nilib
{

    // Mostly static graph. Stored as a compressed sparse row representation.
    // Directed. Compact memory layout. Excellent for traversal (BFS, DFS, Dijkstra).
    // Super cache friendly. All neighbours are stored contiguous.
    // Allows multi edge.
    class DenseGraph
    {
    public:
        using node_t = uint32_t;
        using edge_t = uint32_t;

        void reserve(size_t const numNodes, size_t const numEdges);

        size_t numNodes() const;
        bool contains(node_t const node_id) const;
        bool contains(node_t const node_A, node_t const node_B) const;
        size_t numEdges() const;

        size_t out_degree(node_t const node_id) const;
        size_t in_degree(node_t const node_id) const;

        // Relatively expensive operations.
        void addNode();
        void disconnectNode(node_t const node_id);
        void removeNode(node_t const node_id);
        void addEdge(node_t const from, node_t const to);
        void remEdge(node_t const from, node_t const to);

        static DenseGraph random(size_t const numNodes, size_t const numEdges);
        static DenseGraph empty();

        bool operator==(DenseGraph const &other) const;

        void print();

    protected:
        using StoreMatrix = Nilib::Matrix<float, DynamicMatrixData<float>>;
        StoreMatrix adj;

        // Iterators.
        struct EdgeView
        {
            StoreMatrix const &adj;
            struct iterator
            {
                StoreMatrix const &adj;
                edge_t edge_idx;

                iterator(StoreMatrix const &adj, edge_t idx)
                    : adj(adj), edge_idx(idx)
                {
                    skip_zero();
                }

                void skip_zero()
                {
                    while (edge_idx < adj.size() && adj(edge_idx) == 0.0f)
                        ++edge_idx;
                }

                std::tuple<node_t, node_t, edge_t> operator*() const
                {
                    node_t from = edge_idx / adj.cols();
                    node_t to = edge_idx % adj.cols();
                    return {from, to, edge_idx};
                }

                iterator &operator++()
                {
                    ++edge_idx;
                    skip_zero();
                    return *this;
                }

                bool operator!=(iterator const &other) const
                {
                    return edge_idx != other.edge_idx;
                }
            };
            iterator begin() const { return {adj, 0}; }
            iterator end() const { return {adj, static_cast<edge_t>(adj.size() - 1)}; }
        };

    public:
        EdgeView arcs() const { return EdgeView{adj}; }
        // NodeView nodes() const { return NodeView{head, edges}; }
    };

}; // namespace Nilib

#endif
