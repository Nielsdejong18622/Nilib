#ifndef _GRAPH_REP_HPP
#define _GRAPH_REP_HPP

#include <vector>
#include <list>
#include "Nilib/Math/Matrix.hpp"
#include <functional>

namespace Nilib
{
    // IDs.
    using nodeID = std::uint_fast64_t;

    // struct nodeID
    // {
    //     uint16_t id;

    //     nodeID(uint16_t id) : id(id) {}

    //     // Implict conversion to size_t
    //     operator uint16_t() const { return id; }
    //     bool operator==(nodeID const other) const { return id == other.id; }
    // };

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

    // template <>
    // struct hash<Nilib::nodeID>
    // {
    //     size_t operator()(Nilib::nodeID const &s) const
    //     {
    //         return s.id;
    //     }
    // };
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
    struct GraphI
    {
        // using arcID = size_t;

        // Adding and removing nodes.
        virtual nodeID addNode() = 0;
        virtual std::vector<nodeID> addNodes(size_t const n) = 0;
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

        // Print the representation to LOG.
        virtual void print() const = 0;

        // Traversal
        virtual GraphRange<nodeID> down_stream(nodeID const node) const = 0;
        virtual GraphRange<nodeID> up_stream(nodeID const node) const = 0;
        virtual GraphRange<nodeID> adjacent(nodeID const node) const = 0;

        virtual GraphRange<nodeID> nodes() const = 0;
        virtual GraphRange<arcID> arcs() const = 0;
    };

    // Some constructors.
    void erdos_renyi(GraphI &graph, size_t const numnodes, float const prob);
    void ring_world(GraphI &graph, size_t const numnodes, size_t const C);
    void small_world(GraphI &graph, size_t const numnodes, size_t const C, float const rewire_prob);

    // struct AdjacencyRep : GraphI
    // {
    //     Nilib::Matrixf adjacency;
    // };

    // struct Incidence : GraphI
    // {
    // };

    struct AdjacencyList : public GraphI
    {
        // nodeid -> nodejd -> nodekd
        std::unordered_map<nodeID, std::list<nodeID>> data;

        // Adding and removing nodes.
        nodeID addNode() override;
        std::vector<nodeID> addNodes(size_t const n) override;
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
        GraphRange<nodeID> down_stream(nodeID const node) const override;
        GraphRange<nodeID> up_stream(nodeID const node) const override;
        GraphRange<nodeID> adjacent(nodeID const node) const override;

        // Print the representation to LOG.
        void print() const override;

        // Traversal.
        class NodeIteratorImpl : public GraphIteratorImpl<nodeID>
        {
            using Iter = std::unordered_map<nodeID, std::list<nodeID>>::const_iterator;
            Iter iter_;

        public:
            NodeIteratorImpl(Iter it) : iter_(it) {}

            std::unique_ptr<GraphIteratorImpl<nodeID>> clone() const override
            {
                return std::make_unique<NodeIteratorImpl>(*this);
            }

            nodeID operator*() const override { return iter_->first; }

            GraphIteratorImpl<nodeID> &operator++() override
            {
                ++iter_;
                return *this;
            }

            bool equals(const GraphIteratorImpl<nodeID> &other) const override
            {
                const auto *o = dynamic_cast<const NodeIteratorImpl *>(&other);
                return o && iter_ == o->iter_;
            }
        };
        class NeighborIteratorImpl : public GraphIteratorImpl<nodeID>
        {
            using Iter = std::list<nodeID>::const_iterator;
            Iter iter_;

        public:
            NeighborIteratorImpl(Iter it) : iter_(it) {}

            std::unique_ptr<GraphIteratorImpl<nodeID>> clone() const override
            {
                return std::make_unique<NeighborIteratorImpl>(*this);
            }

            nodeID operator*() const override { return *iter_; }

            GraphIteratorImpl<nodeID> &operator++() override
            {
                ++iter_;
                return *this;
            }

            bool equals(const GraphIteratorImpl<nodeID> &other) const override
            {
                const auto *o = dynamic_cast<const NeighborIteratorImpl *>(&other);
                return o && iter_ == o->iter_;
            }
        };
        class RevNeighborIteratorImpl : public GraphIteratorImpl<nodeID>
        {
            using Iter = std::unordered_map<nodeID, std::list<nodeID>>::const_iterator;
            using IterTail = std::list<nodeID>::const_iterator;

            Iter iter_, iter_end;
            nodeID node_;

        public:
            RevNeighborIteratorImpl(nodeID const node, Iter it, Iter it_end)
                : node_(node), iter_(it), iter_end(it_end)
            {
                advance_to_valid();
            }

            std::unique_ptr<GraphIteratorImpl<nodeID>> clone() const override
            {
                return std::make_unique<RevNeighborIteratorImpl>(*this);
            }

            nodeID operator*() const override
            {
                return iter_->first;
            }

            GraphIteratorImpl<nodeID> &operator++() override
            {
                ++iter_;
                advance_to_valid();
                return *this;
            }

            bool equals(const GraphIteratorImpl<nodeID> &other) const override
            {
                const auto *o = dynamic_cast<const RevNeighborIteratorImpl *>(&other);
                return o && iter_ == o->iter_ && node_ == o->node_;
            }

        private:
            void advance_to_valid()
            {
                while (iter_ != iter_end)
                {
                    const auto &neighbors = iter_->second;
                    if (std::find(neighbors.begin(), neighbors.end(), node_) != neighbors.end())
                    {
                        break; // Found a predecessor
                    }
                    ++iter_;
                }
            }
        };
        class ArcIteratorImpl : public GraphIteratorImpl<arcID>
        {
            using IterHead = std::unordered_map<nodeID, std::list<nodeID>>::const_iterator;
            IterHead iter_head;
            IterHead iter_head_end;

            using IterTail = std::list<nodeID>::const_iterator;
            IterTail iter_tail;

        public:
            ArcIteratorImpl(IterHead it, IterHead it2) : iter_head(it), iter_head_end(it2)
            {
                if (iter_head != iter_head_end)
                {
                    iter_tail = iter_head->second.begin();
                    advance_to_valid();
                }
            }

            std::unique_ptr<GraphIteratorImpl<arcID>> clone() const override
            {
                return std::make_unique<ArcIteratorImpl>(*this);
            }

            arcID operator*() const override { return arcID{iter_head->first, *iter_tail}; }

            GraphIteratorImpl<arcID> &operator++() override
            {
                ++iter_tail;
                advance_to_valid();
                return *this;
            }

            bool equals(const GraphIteratorImpl<arcID> &other) const override
            {
                const auto *o = dynamic_cast<const ArcIteratorImpl *>(&other);
                return o && (iter_tail == o->iter_tail || iter_head == o->iter_head);
            }

        private:
            void advance_to_valid()
            {
                while (iter_head != iter_head_end && iter_tail == iter_head->second.end())
                {
                    ++iter_head;
                    if (iter_head != iter_head_end)
                    {
                        iter_tail = iter_head->second.begin();
                    }
                }
            }
        };

        GraphRange<nodeID> nodes() const override
        {
            return GraphRange<nodeID>(GraphIterator<nodeID>(std::make_unique<NodeIteratorImpl>(data.begin())),
                                      GraphIterator<nodeID>(std::make_unique<NodeIteratorImpl>(data.end())));
        }

        GraphRange<arcID> arcs() const override
        {
            return GraphRange<arcID>(GraphIterator<arcID>(
                                         std::make_unique<ArcIteratorImpl>(data.begin(), data.end())),
                                     GraphIterator<arcID>(
                                         std::make_unique<ArcIteratorImpl>(data.end(), data.end())));
        };
    };

} // namespace Nilib

#endif