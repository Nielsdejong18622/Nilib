#ifndef _GRAPH2_H
#define _GRAPH2_H

#include "Nilib/Math/Matrix.hpp"
#include <vector>
#include <cstdint>

namespace Nilib
{

    // Mostly static graph. Stored as a compressed sparse row representation.
    // Directed. Compact memory layout. Excellent for traversal (BFS, DFS, Dijkstra).
    // Super cache friendly. All neighbours are stored contiguous.
    // Allows multi edge.
    class SparseGraph
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

        static SparseGraph random(size_t const numNodes, size_t const numEdges);
        static SparseGraph empty();

        bool operator==(SparseGraph const &other) const;

        void print();

    protected:
        // Node i's outgoing neigbours are: edges[ head[i] ... head[i+1] )
        std::vector<node_t> head;  // n + 1
        std::vector<edge_t> edges; // m

        // Iterators.
        struct EdgeView
        {
            std::vector<node_t> const &head;
            std::vector<edge_t> const &edges;

            struct iterator
            {
                std::vector<node_t> const &head;
                std::vector<edge_t> const &edges;
                node_t from;
                node_t edge_idx;

                iterator(std::vector<node_t> const &head, std::vector<node_t> const &edges, node_t from, node_t idx)
                    : head(head),
                      edges(edges),
                      from(from),
                      edge_idx(idx) {}

                std::tuple<node_t, node_t, edge_t> operator*() const
                {
                    return {from, edges[edge_idx], edge_idx};
                }

                iterator &operator++()
                {
                    ++edge_idx;
                    while (from + 1 < head.size() && edge_idx >= head[from + 1])
                        ++from;
                    return *this;
                }

                bool operator!=(iterator const &other) const
                {
                    return edge_idx != other.edge_idx;
                }
            };

            iterator begin() const { return {head, edges, 0, 0}; }
            iterator end() const { return {head, edges, static_cast<node_t>(head.size() - 1), static_cast<edge_t>(edges.size())}; }
        };

        // struct NodeView
        // {
        //     std::vector<node_t> const &head;
        //     std::vector<node_t> const &edges;

        //     struct iterator
        //     {
        //         std::vector<node_t> const &head;
        //         std::vector<node_t> const &edges;
        //         node_t from;
        //         node_t edge_idx;

        //         iterator(std::vector<node_t> const &head, std::vector<node_t> const &edges, node_t from, node_t idx)
        //             : head(head),
        //               edges(edges),
        //               from(from),
        //               edge_idx(idx) {}

        //         std::tuple<node_t> operator*() const
        //         {
        //             return {node_t};
        //         }

        //         iterator &operator++()
        //         {
        //             ++edge_idx;
        //             while (from + 1 < head.size() && edge_idx >= head[from + 1])
        //                 ++from;
        //             return *this;
        //         }

        //         bool operator!=(iterator const &other) const
        //         {
        //             return edge_idx != other.edge_idx;
        //         }
        //     };

        //     iterator begin() const { return {head, edges, 0, 0}; }
        //     iterator end() const { return {head, edges, static_cast<uint32_t>(head.size() - 1), static_cast<uint32_t>(edges.size())}; }
        // };

    public:
        EdgeView arcs() const { return EdgeView{head, edges}; }
        // NodeView nodes() const { return NodeView{head, edges}; }
    };

    // Mostly static graph.
    struct DenseGraph
    {
        Nilib::Matrix<bool, DynamicMatrixData<bool>> adj;
    };

    class WeightedSparseGraph : public SparseGraph
    {
    public:
        using weight_t = float;
        std::vector<weight_t> weights; // m

        static WeightedSparseGraph empty();
        static WeightedSparseGraph random(size_t const numNodes, size_t const numEdges);

        void print();

    private:
        using SparseGraph::addEdge;
        using SparseGraph::empty;
        using SparseGraph::random;
    };

    // Dynamic graph.
    struct ForwardStarGraph
    {
        struct Edge
        {
            uint32_t head;
            uint32_t nextEdge;
        };
        std::vector<Edge> edges;
    };

    struct DirectedTree
    {
        uint32_t numNodes; // n
    };

}; // namespace Nilib

#endif
