#ifndef _GRAPH_H
#define _GRAPH_H

#include "Nilib/Structures/AdjacencyList.hpp"
#include "Nilib/Structures/Persistent.hpp"
#include <vector>
#include <unordered_map>

namespace Nilib
{

    // General Graph type
    template <typename Node = void, typename Arc = void, typename GraphRep = AdjacencyList>
        requires std::is_base_of_v<GraphI, GraphRep>
    class Graph : public GraphRep
    {
    public:
        using GraphRep::addNode;
        using GraphRep::addArc;

        // Node data storage if Node != void
        std::conditional_t<!std::is_void_v<Node>,
                           PersistentIndexVector<Node, nodeID>,
                           std::monostate>
            nodeData;

        // Arc data storage if Arc != void
        std::conditional_t<!std::is_void_v<Arc>,
                           std::unordered_map<ArcID, Arc>,
                           std::monostate>
            arcData;

        // Only available when Node is void
        template <typename N = Node>
            requires(std::is_void_v<N>)
        nodeID addNode()
        {
            return GraphRep::addNode();
        }

        // Only available when Node is NOT void
        template <typename N = Node>
            requires(!std::is_void_v<N>)
        nodeID addNode(N const &node)
        {
            nodeID id1 = GraphRep::addNode();
            // nodeID id2 = nodeData.add(node);
            CORE_ASSERT(false); // Something does not add up here. 
            return id1;
        }

        void remove(nodeID const node) override
        {
            GraphRep::remove(node);
            if constexpr (!std::is_void_v<Node>)
                nodeData.remove(node);
        }

        // In case Arc == VOID
        template <typename A = Arc>
            requires(std::is_void_v<A>)
        void addArc(nodeID const from, nodeID const to)
        {
            GraphRep::addArc(from, to);
        }

        // In case Arc != VOID
        template <typename A = Arc>
            requires(!std::is_void_v<A>)
        void addArc(nodeID const from, nodeID const to, A const &arc)
        {
            GraphRep::addArc(from, to);
            arcData[{from, to}] = std::move(arc);
        }

        void remove(nodeID const from, nodeID const to) override
        {
            GraphRep::remove(from, to);
            if constexpr (!std::is_void_v<Arc>)
                arcData.erase(ArcID{from, to});
        }
    };

    // Some constructors.
    template <typename Node>
    void erdos_renyi(Graph<void, void> &graph, size_t const numnodes, float const prob)
    {
        LOG_INFO("Constructing erdos_renyi graph!");

        graph.clear();
        for (size_t i = 0; i < numnodes; i++)
            if constexpr (std::is_void_v<Node>)
                graph.addNode();
            else
                graph.addNode(Node{});

        for (size_t node_i = 0; node_i < numnodes; ++node_i)
        {
            for (size_t node_j = 0; node_j < numnodes; ++node_j)
            {
                if (RNG::prob() < prob)
                {
                    graph.addArc(node_i, node_j);
                }
            }
        }
    }

    // C describes how many neighbours there are.
    template <typename Node>
    void ring_world(Graph<Node, void> &graph, size_t const numnodes, size_t const C)
    {
        LOG_INFO("Constructing ring world graph!");
        graph.clear();

        for (size_t i = 0; i < numnodes; i++)
            if constexpr (std::is_void_v<Node>)
                graph.addNode();
            else
                graph.addNode(Node{});

        size_t c = C / 2;
        for (nodeID idx = 0; idx < numnodes; idx++)
            for (nodeID add = 0; add < C + 1; ++add)
                if (idx != ((idx + add - c) + numnodes) % numnodes)
                    graph.addArc(idx, ((idx + add - c) + numnodes) % numnodes);
    }

    template <typename Node>
    void small_world(Graph<Node, void> &graph, size_t const numnodes, size_t const C, float const rewire_prob)
    {
        ring_world<Node, void>(graph, numnodes, C);
    }

}; // namespace Nilib

#endif
