#ifndef _GRAPH_H
#define _GRAPH_H

#include "Nilib/Structures/GraphRepresentation.hpp"
#include <vector>

namespace Nilib
{

    // General Graph type
    template <typename Node = void, typename Arc = void, typename GraphRep = AdjacencyList>
    class Graph : public GraphRep
    {

    private:
        nodeID addNode() override { return nodeID{0}; };
        void addArc(nodeID a, nodeID b) override {};

    public:
        std::unordered_map<arcID, Arc> arcs;
        std::unordered_map<nodeID, Node> nodes;

        nodeID addNode(Node const &node)
        {
            nodeID id = GraphRep::addNode();
            nodes[id] = node;
            return id;
        };
        void addArc(nodeID a, nodeID b, Arc const &arc)
        {
            arcs[arcID{a, b}] = arc;
            GraphRep::addArc(a, b);
        };
    };

    // Simples graph, only thin wrapper around GraphRep.
    template <typename GraphRep>
    class Graph<void, void, GraphRep> : public GraphRep
    {
    };

    // Partial Specialization: Graph with only Node type (for cases like Graph<int>)
    template <typename Node, typename GraphRep>
    class Graph<Node, void, GraphRep> : public GraphRep
    {
    private:
        nodeID addNode() override { return nodeID{0}; };

    public:
        std::unordered_map<nodeID, Node> nodes;

        nodeID addNode(Node const &node)
        {
            nodeID id = GraphRep::addNode();
            nodes[id] = node;
            return id;
        };
    };

    // Partial Specialization: Graph with only Arc type (for cases like Graph<void, int>)
    template <typename Arc, typename GraphRep>
    class Graph<void, Arc, GraphRep> : public GraphRep
    {
    private:
        void addArc(nodeID a, nodeID b) override {};

    public:
        std::unordered_map<arcID, Arc> arcs;

        void addArc(nodeID a, nodeID b, Arc const &arc)
        {
            arcs[arcID{a, b}] = arc;
            GraphRep::addArc(a, b);
        };
    };

}; // namespace Nilib

#endif
