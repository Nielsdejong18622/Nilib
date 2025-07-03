#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <vector>
#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Math/RNG.hpp"
#include <map>

namespace Nilib
{
    namespace Graph
    {

        // Interface of everything we are allowed to do with a graph class.
        class GraphI
        {
        public:
            using node_id = int;

            // Adding and removing nodes/edges.
            virtual void add(node_id node) = 0;
            virtual void remove(node_id node) = 0;

            virtual void add(node_id node_A, node_id node_B) = 0;
            virtual void remove(node_id node_A, node_id node_B) = 0;

            virtual void clear() = 0;

            // Properties.
            virtual size_t numnodes() const = 0;
            virtual size_t numedges() const = 0;
            virtual bool empty() const = 0;

            // Traversal
            virtual std::vector<node_id> down_stream(node_id node) const = 0;
            virtual std::vector<node_id> up_stream(node_id node) const = 0;
            virtual std::vector<node_id> adjacent(node_id node) const = 0;
        };

        namespace Representation
        {

            struct AdjacencyRep : GraphI
            {
                Nilib::Matrixf adjacency;
            };

            
            struct Incidence : GraphI
            {
                Nilib::Matrixf adjacency;
            };

        }; // namespace Representation


        // Structure that maps node_id -> Custom data. 
        class GraphNodeMap
        {

        };


        class DiGraph
        {
            GraphI *rep; // Make it unique ptr. 

        };


    }; // namespace Graph

}; // namespace Nilib

#endif
