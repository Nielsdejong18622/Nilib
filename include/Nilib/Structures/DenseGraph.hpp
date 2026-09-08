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
        using StoreMatrix = Nilib::Matrix<float, DynamicMatrixData<float>>;

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
        void removeEdge(node_t const from, node_t const to);

        static DenseGraph random(size_t const numNodes, size_t const numEdges);
        static DenseGraph connected(size_t numNodes, bool self_loops = false);
        static DenseGraph empty();

        bool operator==(DenseGraph const &other) const;

        void print();
        StoreMatrix const &adjMat() const { return d_adj; }

    protected:
        StoreMatrix d_adj;

        // Iterators.
        struct EdgeView
        {
            StoreMatrix const &d_adj;
            struct iterator
            {
                StoreMatrix const &adj;
                size_t edge_idx;

                iterator(StoreMatrix const &adjMatr, size_t eidx)
                    : adj(adjMatr), edge_idx(eidx)
                {
                    skip_zero();
                }

                void skip_zero()
                {
                    const auto size = adj.size();

                    while (edge_idx < size && adj(edge_idx) == 0.0f)
                        ++edge_idx;
                }

                std::tuple<node_t, node_t, size_t> operator*() const
                {
                    const auto cols = adj.cols();

                    node_t from = static_cast<node_t>(edge_idx / cols);
                    node_t to = static_cast<node_t>(edge_idx % cols);

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

            iterator begin() const
            {
                return {d_adj, 0};
            }

            iterator end() const
            {
                return {d_adj, d_adj.size()};
            }
        };

    public:
        EdgeView arcs() const { return EdgeView{d_adj}; }
        // NodeView nodes() const { return NodeView{head, edges}; }
    };

}; // namespace Nilib

#endif
