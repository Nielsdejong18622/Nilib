#ifndef _GRAPH_ADJACENCY_LIST_HPP
#define _GRAPH_ADJACENCY_LIST_HPP

#include "Nilib/Structures/Persistent.hpp"
#include "Nilib/Structures/GraphRepresentation.hpp"

namespace Nilib
{

    class AdjacencyList : public GraphI
    {

    protected:
        // nodeid -> [nodejd, nodekd]
        PersistentIndexVector<std::list<nodeID>, nodeID> d_data;
        // PersistentIndexVector<ArcID, arcID> d_arc_data;

        // Adding and removing nodes.
        nodeID addNode() override;
        std::vector<nodeID> addNodes(size_t const n) override;

        void remove(nodeID node) override;

        // Adding and removing arcs/edges.
        void addArc(nodeID node_A, nodeID node_B) override;
        void remove(nodeID node_A, nodeID node_B) override;

    public:
        // Make the empty graph.
        void clear() override;

        // Properties.
        size_t numnodes() const override;
        size_t nodes_capacity() const override;
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
            using Iter = PersistentIndexVector<std::list<nodeID>, nodeID>::const_iterator;
            Iter iter_;

        public:
            NodeIteratorImpl(Iter it) : iter_(it) {}

            std::unique_ptr<GraphIteratorImpl<nodeID>> clone() const override
            {
                return std::make_unique<NodeIteratorImpl>(*this);
            }

            nodeID operator*() const override { return iter_.getIndex(); }

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
            using Iter = PersistentIndexVector<std::list<nodeID>, nodeID>::const_iterator;
            using IterTail = std::list<nodeID>::const_iterator;

            nodeID node_;
            Iter iter_, iter_end;

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
                return iter_.getIndex();
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
                    const auto &neighbors = *iter_;
                    if (std::find(neighbors.begin(), neighbors.end(), node_) != neighbors.end())
                    {
                        break; // Found a predecessor
                    }
                    ++iter_;
                }
            }
        };

        GraphRange<nodeID> nodes() const override
        {
            return GraphRange<nodeID>(GraphIterator<nodeID>(std::make_unique<NodeIteratorImpl>(d_data.begin())),
                                      GraphIterator<nodeID>(std::make_unique<NodeIteratorImpl>(d_data.end())));
        }

        class ArcIteratorImpl : public GraphIteratorImpl<ArcID>
        {
            using IterHead = PersistentIndexVector<std::list<nodeID>, nodeID>::const_iterator;
            IterHead iter_head;
            IterHead iter_head_end;

            using IterTail = std::list<nodeID>::const_iterator;
            IterTail iter_tail;

        public:
            ArcIteratorImpl(IterHead it, IterHead it2) : iter_head(it), iter_head_end(it2)
            {
                if (iter_head != iter_head_end)
                {
                    iter_tail = iter_head->begin();
                    advance_to_valid();
                }
            }

            std::unique_ptr<GraphIteratorImpl<ArcID>> clone() const override
            {
                return std::make_unique<ArcIteratorImpl>(*this);
            }

            ArcID operator*() const override { return ArcID{iter_head.getIndex(), *iter_tail}; }

            GraphIteratorImpl<ArcID> &operator++() override
            {
                ++iter_tail;
                advance_to_valid();
                return *this;
            }

            bool equals(GraphIteratorImpl<ArcID> const &other) const override
            {
                const auto *o = dynamic_cast<const ArcIteratorImpl *>(&other);
                return o && (iter_tail == o->iter_tail || iter_head == o->iter_head);
            }

        private:
            void advance_to_valid()
            {
                while (iter_head != iter_head_end && iter_tail == iter_head->end())
                {
                    ++iter_head;
                    if (iter_head != iter_head_end)
                    {
                        iter_tail = iter_head->begin();
                    }
                }
            }
        };

        GraphRange<ArcID> arcs() const override
        {
            return GraphRange<ArcID>(GraphIterator<ArcID>(
                                         std::make_unique<ArcIteratorImpl>(d_data.begin(), d_data.end())),
                                     GraphIterator<ArcID>(
                                         std::make_unique<ArcIteratorImpl>(d_data.end(), d_data.end())));
        };
    };

} // namespace Nilib
#endif