#ifndef _GRAPH_REP_HPP
#define _GRAPH_REP_HPP

#include <vector>
#include <list>
#include "Nilib/Math/Matrix.hpp"
#include <functional>

namespace Nilib
{
    // IDs.
    struct nodeID
    {
        size_t id;
        
        nodeID(size_t id) : id(id) {}
        
        // Implict conversion to size_t
        operator size_t() const { return id; }
        bool operator==(nodeID const other) const { return id == other.id; }
    };

    struct arcID
    {
        nodeID a, b;

        friend bool operator==(arcID const other, arcID const self)
        {
            return other.a == self.a && other.b == self.b;
        }
    };

};

namespace std
{
    template <>
    struct hash<Nilib::arcID>
    {
        size_t operator()(Nilib::arcID const &s) const
        {
            size_t h1 = hash<size_t>()(static_cast<size_t>(s.a)); // Hash the 'a' nodeID
            size_t h2 = hash<size_t>()(static_cast<size_t>(s.b)); // Hash the 'b' nodeID
            return h1 + 31 * h2;                                  // Combine the hashes using a prime multiplier (31)
        }
    };

    template <>
    struct hash<Nilib::nodeID>
    {
        size_t operator()(Nilib::nodeID const &s) const
        {
            return s.id;
        }
    };
}

namespace Nilib
{
    // Interface of everything we are allowed to do with a graph class.
    struct GraphI
    {
        // using arcID = size_t;

        // Adding and removing nodes.
        virtual nodeID addNode() = 0;
        virtual void remove(nodeID node) = 0;

        // Adding and removing arcs/edges.
        virtual void addArc(nodeID node_A, nodeID node_B) = 0;
        // virtual void removeArcs(nodeID node_A, nodeID node_B) = 0;
        virtual void remove(nodeID node_A, nodeID node_B) = 0;
        // virtual void removeArc(arcID) = 0;

        // Make the empty graph.
        virtual void clear() = 0;

        // Properties.
        virtual size_t numnodes() const = 0;
        virtual size_t numedges() const = 0;
        virtual bool empty() const = 0;

        // Simple checks and operations.
        virtual bool contains(nodeID node) const = 0;
        virtual bool contains(nodeID nodeA, nodeID nodeB) const = 0;
        virtual bool adjacent(nodeID nodeA, nodeID nodeB) const = 0;
        // virtual bool incident(arcID arcA, arcID arcB) const = 0;ß
        virtual size_t indegree(nodeID node) const = 0;
        virtual size_t outdegree(nodeID node) const = 0;
        size_t degree(nodeID node) const { return indegree(node) + outdegree(node); };

        // Traversal
        virtual std::vector<nodeID> down_stream(nodeID node) const = 0;
        virtual std::vector<nodeID> up_stream(nodeID node) const = 0;
        virtual std::vector<nodeID> adjacent(nodeID node) const = 0;

        // Print the representation to LOG.
        virtual void print() const = 0;
    };

    // struct AdjacencyRep : GraphI
    // {
    //     Nilib::Matrixf adjacency;
    // };

    // struct Incidence : GraphI
    // {
    //     Nilib::Matrixf adjacency;
    // };

    struct AdjacencyList : public GraphI
    {
        // nodeid -> nodejd -> nodekd
        std::unordered_map<nodeID, std::list<nodeID>> data;

        // Adding and removing nodes.
        nodeID addNode() override;
        void remove(nodeID node) override;

        // Adding and removing arcs/edges.
        void addArc(nodeID node_A, nodeID node_B) override;
        void remove(nodeID node_A, nodeID node_B) override;

        // Make the empty graph.
        void clear() override;

        // Properties.
        size_t numnodes() const override;
        size_t numedges() const override;
        bool empty() const override;

        // Some simple operations and checks.
        bool contains(nodeID node) const override;
        bool contains(nodeID nodeA, nodeID nodeB) const override;
        bool adjacent(nodeID nodeA, nodeID nodeB) const override;
        size_t indegree(nodeID node) const override;
        size_t outdegree(nodeID node) const override;

        // Traversal
        std::vector<nodeID> down_stream(nodeID node) const override;
        std::vector<nodeID> up_stream(nodeID node) const override;
        std::vector<nodeID> adjacent(nodeID node) const override;

        // Print the representation to LOG.
        void print() const override;
    };

} // namespace Nilib

#endif