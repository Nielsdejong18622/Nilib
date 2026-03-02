#ifndef _WEIGHTED_GRAPH_H
#define _WEIGHTED_GRAPH_H

#include "Nilib/Structures/SparseGraph.hpp"
#include "Nilib/Structures/DenseGraph.hpp"

namespace Nilib
{

    // Decorator for a grahp type.
    template <typename Graph>
    class Weighted : public Graph
    {
    public:
        using weight_t = float;
        using node_t = Graph::node_t;
        std::vector<weight_t> weights; // m

        void addEdge(node_t node1, node_t node2, weight_t weight);

        static Weighted empty();
        static Weighted random(size_t numNodes, size_t numEdges);
        static Weighted fromVec2f(std::vector<Vec2f> const &x);

        void print();

    private:
        using Graph::addEdge;
        using Graph::empty;
        using Graph::random;
    };

}; // namespace Nilib

#endif
