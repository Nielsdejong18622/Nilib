#ifndef _SPARSE_GRAPH_H
#define _SPARSE_GRAPH_H

#include "Nilib/Logger/Log.hpp"

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
                node_t head_idx = 0; // Points to the current from node..
                node_t deg_left = 0; // Counts the out_degree of from node, if reached: increment head_idx, to_idx.
                node_t edge_idx = 0;

                iterator(std::vector<node_t> const &head, std::vector<node_t> const &edges, node_t head_idx, edge_t edge_idx)
                    : head(head),
                      edges(edges),
                      head_idx(head_idx),
                      edge_idx(edge_idx)
                {

                    // Skip disconnected nodes.
                    while (head_idx + 1 < head.size() and head[head_idx + 1] - head[head_idx])
                    {
                        head_idx++;
                        deg_left = head[head_idx + 1] - head[head_idx];
                    }
                    // LOG_DEBUG("Constructor Arc iterator.", head_idx, edge_idx, deg_left);
                }

                std::tuple<node_t, node_t, edge_t> operator*() const
                {
                    return {head_idx, edges[edge_idx], edge_idx};
                }

                iterator &operator++()
                {
                    // We traverse to the next edge.
                    ++edge_idx;
                    // We consume one outgoing degree.
                    --deg_left;
                    while (deg_left == 0)
                    {
                        head_idx++;
                        deg_left = head[head_idx + 1] - head[head_idx];
                    }
                    return *this;
                }

                bool operator!=(iterator const &other) const
                {
                    return edge_idx != other.edge_idx;
                }
            };

            iterator begin() const { return {head, edges, 0, 0}; }
            iterator end() const { return {head, edges, static_cast<node_t>(head.size()), static_cast<edge_t>(edges.size())}; }
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

}; // namespace Nilib

#endif
