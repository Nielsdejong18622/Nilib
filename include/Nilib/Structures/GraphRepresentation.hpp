#ifndef _GRAPH_REP_HPP
#define _GRAPH_REP_HPP

#include <vector>
#include <list>
#include "Nilib/Math/Matrix.hpp"
#include <functional>
#include <memory>

namespace Nilib
{
    // IDs.
    using nodeID = std::uint_fast16_t;
    // using arcID = std::uint_fast64_t;

    // struct nodeID
    // {
    //     uint16_t id;

    //     nodeID(uint16_t id) : id(id) {}

    //     // Implict conversion to size_t
    //     operator uint16_t() const { return id; }
    //     bool operator==(nodeID const other) const { return id == other.id; }
    // };

    struct ArcID
    {
        nodeID from, to;

        friend bool operator==(ArcID const &other, ArcID const &self)
        {
            return other.from == self.from && other.to == self.to;
        }

        friend std::ostream &operator<<(std::ostream &os, ArcID const &self)
        {
            return os << self.from << '-' << self.to;
        }
        nodeID first() { return from; }
        nodeID second() { return to; }
    };
};

namespace std
{
    template <>
    struct hash<Nilib::ArcID>
    {
        std::size_t operator()(const Nilib::ArcID &a) const
        {
            uintmax_t hash = std::hash<Nilib::nodeID>{}(a.from);
            hash <<= sizeof(uintmax_t) * 4;
            hash ^= std::hash<Nilib::nodeID>{}(a.to);
            return std::hash<uintmax_t>{}(hash);
        }
    };
}

namespace Nilib
{
    template <typename T>
    class GraphIteratorImpl
    {
    public:
        virtual ~GraphIteratorImpl() = default;

        virtual std::unique_ptr<GraphIteratorImpl<T>> clone() const = 0;
        virtual T operator*() const = 0;
        virtual GraphIteratorImpl &operator++() = 0;
        virtual bool equals(const GraphIteratorImpl &other) const = 0;
    };

    template <typename T>
    class GraphIterator
    {
        std::unique_ptr<GraphIteratorImpl<T>> impl_;

    public:
        GraphIterator(std::unique_ptr<GraphIteratorImpl<T>> impl)
            : impl_(std::move(impl)) {}

        GraphIterator(const GraphIterator &other)
            : impl_(other.impl_->clone()) {}

        GraphIterator &operator=(const GraphIterator &other)
        {
            if (this != &other)
            {
                impl_ = other.impl_->clone();
            }
            return *this;
        }

        GraphIterator(GraphIterator &&) noexcept = default;
        GraphIterator &operator=(GraphIterator &&) noexcept = default;

        T operator*() const { return **impl_; }
        GraphIterator &operator++()
        {
            ++(*impl_);
            return *this;
        }

        bool operator==(const GraphIterator &other) const
        {
            return impl_->equals(*other.impl_);
        }

        bool operator!=(const GraphIterator &other) const
        {
            return !(*this == other);
        }
    };

    template <typename T>
    class GraphRange
    {
        GraphIterator<T> _begin;
        GraphIterator<T> _end;

    public:
        GraphRange(GraphIterator<T> const begin, GraphIterator<T> const end)
            : _begin(begin), _end(end) {}
        GraphIterator<T> begin() { return _begin; };
        GraphIterator<T> end() { return _end; };
    };

    // Interface of everything we are allowed to do with a graph class.
    class GraphI
    {
    protected:
        // Adding and removing nodes.
        virtual nodeID addNode() = 0;
        virtual void remove(nodeID node) = 0;
        virtual std::vector<nodeID> addNodes(size_t const n) = 0;

        // Adding and removing arcs/edges.
        virtual void addArc(nodeID node_A, nodeID node_B) = 0;
        // virtual void removeArcs(nodeID node_A, nodeID node_B) = 0;
        virtual void remove(nodeID node_A, nodeID node_B) = 0;

    public:
        // Make the empty graph.
        virtual void clear() = 0;

        // Properties.
        virtual size_t numnodes() const = 0;
        virtual size_t nodes_capacity() const = 0;
        virtual size_t numedges() const = 0;
        virtual bool empty() const = 0;

        // Simple checks and operations.
        virtual bool contains(nodeID node) const = 0;
        virtual bool contains(nodeID nodeA, nodeID nodeB) const = 0;
        virtual bool adjacent(nodeID nodeA, nodeID nodeB) const = 0;
        // virtual bool incident(arcID arcA, arcID arcB) const = 0;
        virtual size_t indegree(nodeID node) const = 0;
        virtual size_t outdegree(nodeID node) const = 0;
        size_t degree(nodeID node) const { return indegree(node) + outdegree(node); };

        // Print the representation to LOG.
        virtual void print() const = 0;

        // Traversal
        virtual GraphRange<nodeID> down_stream(nodeID const node) const = 0;
        virtual GraphRange<nodeID> up_stream(nodeID const node) const = 0;
        virtual GraphRange<nodeID> adjacent(nodeID const node) const = 0;

        virtual GraphRange<nodeID> nodes() const = 0;
        virtual GraphRange<ArcID> arcs() const = 0;
    };

} // namespace Nilib

#endif