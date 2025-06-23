#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <vector>
#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Math/RNG.hpp"
#include <map>

namespace Nilib
{

    template <typename NodeData, typename CoordPrec>
    class GraphLayout;

    // An all purpose Graph implementation.
    template <typename NodeData>
    class Graph
    {

    public:
        typedef unsigned char node_id;

        Graph() : d_nodes(), d_edges(), d_fragmented(0) {};

        void clear()
        {
            d_nodes.clear();
            d_edges.clear();
            d_fragmented = 0;
        }

        friend std::ostream &operator<<(std::ostream &os, Graph const &g)
        {
            return os << "<Graph:" << &g
                      << ", nodes:" << +g.node_count()
                      << '(' << sizeof(NodeData) << "b)"
                      << ", cap:" << +g.capacity() << '/' << +g.maxcapacity()
                      << ", edges:" << +g.d_edges.size()
                      << '(' << sizeof(d_edges.begin()->first) + sizeof(d_edges.begin()->second) << "b)"
                      << ", cap:" << +g.d_edges.size() << '/' << +g.d_edges.max_size()
                      << ", memory:" << +g.bytesize() << 'b'
                      << ">";
        }

        size_t bytesize() const
        {
            size_t edge_size_b = sizeof(d_edges.begin()->first) + sizeof(d_edges.begin()->second);
            size_t node_size_b = sizeof(NodeData);
            return d_edges.size() * edge_size_b + d_nodes.size() * node_size_b + sizeof(node_id); // +d_fragmented
        }
        node_id maxcapacity() const { return std::numeric_limits<node_id>::max(); }

        node_id capacity() const { return d_nodes.size(); }
        node_id node_count() const { return d_nodes.size() - d_fragmented; }
        size_t edge_count() const { return d_edges.size(); }

        size_t out_degree(node_id const A) const { return d_edges.count(A); }
        size_t in_degree(node_id const A) const
        {
            size_t res = 0;

            for (auto iter = d_edges.begin(); iter != d_edges.end(); iter++)
            {
                if (iter->second == A)
                    res += 1;
            }
            return res;
        }

        void addEdge(node_id const A, node_id const B)
        {
            CORE_ASSERT(A < capacity() && d_nodes[A].occupied)
            CORE_ASSERT(B < capacity() && d_nodes[B].occupied)
            d_edges.insert(std::pair<node_id, node_id>(A, B));
        }

        bool exists(node_id const A) const { return (A < capacity()) ? d_nodes[A].occupied : false; }
        bool exists(node_id const A, node_id const B) const
        {
            auto range = d_edges.equal_range(A);
            for (auto i = range.first; i != range.second; ++i)
                if (i->second == B)
                    return true;
            return false;
        }

        void removeEdges(node_id const A, node_id const B)
        {
            auto range = d_edges.equal_range(A);
            for (auto iter = range.first; iter != range.second;)
            {
                if (iter->first == A && iter->second == B)
                {
                    // LOG_DEBUG() << "Removing Edge " << iter->first << " -- " << iter->second << ".\n";
                    iter = d_edges.erase(iter);
                }
                else
                    iter++;
            }
        }

        void removeEdge(node_id const A, node_id const B)
        {
            auto range = d_edges.equal_range(A);
            for (auto iter = range.first; iter != range.second; ++iter)
                if (iter->first == A && iter->second == B)
                {
                    // LOG_DEBUG() << "Removing Edge " << A << " -- " << B << ".\n";
                    d_edges.erase(iter);
                    return;
                }
        }
        // // Reserve space for nodes.
        void reserve(node_id const nodeid)
        {
            d_nodes.reserve(nodeid);
        }

        // Return the node_id of the first node that equals node. Returns capacity if not found.
        node_id find(NodeData const &data) const
        {
            auto lambda = [data](NodeBlock const &b)
            { return (b.data == data && b.occupied); };
            auto it = std::find_if(d_nodes.begin(), d_nodes.end(), lambda);
            return it - d_nodes.begin();
        }

        // Retrieve the nodes. TODO: iterator or coroutine.
        std::vector<node_id> nodes() const
        {
            std::vector<node_id> res;
            for (node_id i = 0; i < d_nodes.size(); i++)
            {
                if (!d_nodes[i].occupied)
                    continue;
                res.emplace_back(i);
            }
            return res;
        }

        std::vector<node_id> addNodes(std::vector<NodeData> const &nodes)
        {
            std::vector<node_id> res;
            for (NodeData const &node : nodes)
            {
                res.push_back(addNode(node));
            }

            return res;
        }

        // Add a node with NodeData, returns an unique handle id.
        node_id addNode(NodeData const &node)
        {
            // LOG_DEBUG() << "Adding node:" << &node << " fragmentation status:" << d_fragmented << '\n';
            if (d_fragmented > 0)
            {
                // Loop through the nodes vector see if a spot is available.
                auto avail = std::find_if(d_nodes.begin(), d_nodes.end(), [](NodeBlock &b)
                                          { return !b.occupied; });
                node_id spot = avail - d_nodes.begin();
                d_nodes[spot].occupied = true;
                d_nodes[spot].data = node;
                d_fragmented--;
                // LOG_DEBUG() << "Adding node:" << spot << ':' << &node << '\n';
                return spot;
            }

            d_nodes.emplace_back(true, node);
            LOG_DEBUG() << "Adding node:" << d_nodes.size() - 1 << ':' << &node << '\n';
            CORE_ASSERT(d_nodes.size() <= std::numeric_limits<node_id>::max())
            return d_nodes.size() - 1;
        }

        // Remove Node by node_id.
        void removeNode(node_id const idx)
        {
            // First remove any edges this Node is part of.
            for (auto iter = d_edges.begin(); iter != d_edges.end();)
            {
                if (iter->first == idx || iter->second == idx)
                {
                    // LOG_DEBUG() << "Removing Edge " << iter->first << " -- " << iter->second << ".\n";
                    iter = d_edges.erase(iter);
                }
                else
                    iter++;
            }

            CORE_ASSERT(idx < d_nodes.size())
            ASSERT(d_nodes[idx].occupied, "Warning: Node was already removed!")
            d_fragmented += (idx < d_nodes.size() - 1);
            // LOG_DEBUG() << "Removed indx:" << idx << '\n';
            d_nodes[idx].occupied = false;
        }

        // Filters nodes that are equal to data.
        void filterNodes(NodeData const &data)
        {
            auto lambda = [data](NodeBlock const &b)
            { return (b.data == data && b.occupied); };
            auto it = std::find_if(d_nodes.begin(), d_nodes.end(), lambda);
            while (it != d_nodes.end())
            {
                removeNode(it - d_nodes.begin());
                it = std::find_if(it, d_nodes.end(), lambda);
            }
        }

        std::vector<node_id> randomWalk(node_id start, size_t const max_steps) const
        {
            ASSERT(!empty(), "No walk on an empty graph!")
            node_id a = start; // start node.
            std::vector<node_id> res(a);
            if (down_stream(a).size() == 0)
                return res;
            for (size_t step = 0; step < max_steps; step++)
            {
                res.emplace_back(+a);
                if (down_stream(a).size() == 0)
                    return res;
                a = down_stream(a)[RNG::index(down_stream(a).size())];
            }
            return res;
        }

        // Find any downstream neigbours of node_id.
        std::vector<node_id> down_stream(node_id const idx) const
        {
            auto begin = d_edges.lower_bound(idx);
            auto end = d_edges.upper_bound(idx);
            if (begin == d_edges.end())
                return std::vector<node_id>();

            std::vector<node_id> neigbours;
            for (auto it = begin; it != end; ++it)
            {
                neigbours.emplace_back(it->second);
            }
            return neigbours;
        }

        [[nodiscard]] bool empty() const { return d_nodes.empty(); }

    private:
        struct NodeBlock
        {
            bool occupied; // Indicate if the Nodeblock is occupied in memory.
            NodeData data;
        };

        std::vector<NodeBlock> d_nodes;
        std::multimap<node_id, node_id> d_edges;

        node_id d_fragmented;

        template <typename, typename>
        friend class GraphLayout;

    public:
        // Some premade graphs.
        static Graph erdos_reini(std::vector<NodeData> nodes, float const p)
        {
            Graph<NodeData> g;
            LOG_DEBUG() << "Constructing erdos_reini!\n";
            auto handles = g.addNodes(nodes);

            for (node_id i : handles)
            {
                for (node_id j : handles)
                {
                    if (RNG::prob<float>() > 1 - p)
                        g.addEdge(i, j);
                }
            }
            LOG_DEBUG() << "Constructed erdos_reini!\n";
            return g;
        }

        static Graph erdos_reini(size_t const num_nodes, float const p)
        {
            return erdos_reini(std::vector<NodeData>(num_nodes), p);
        }
    };

}; // namespace Nilib

#endif
