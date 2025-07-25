#ifndef _VRP_INSTANCE_HPP
#define _VRP_INSTANCE_HPP

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Structures/Graph.hpp"

#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

namespace Nilib
{
    namespace VRP
    {

        // An instance holds a graph adjacency matrix. And a matrix with node attributes.
        class Instance
        {
        public:
            Nilib::Matrixf A; // Adjacency matrix.
            Nilib::Matrixf X; // Attributes
            size_t vehcap;    // vehicle capacity. (Assume all nodes have demand 1).

            friend class Solution;
            Instance() = default;

            Instance(size_t const nnodes, size_t const vehcap, Nilib::Matrixf const &A, Nilib::Matrixf const &X);

            // Draw the instance to a window.
            // - Uses the first two features of X matrix as x and y coordinates.
            void draw(Nilib::Window &win) const;

            // Creates a simple random instance.
            static Instance createRandom(size_t const nnodes, size_t const vehcap);

            // Load an instance from stream.
            static Instance load(std::istream &out);

            // Get number of locations
            size_t numlocations() const;
            size_t vehcapacity() const;

            Nilib::Matrixf distances() const;

            // static bool serialize(Serializer &serializer. Instance &inst);
            // static Instance deserialize(Deserializer &deserializer);
            bool serialize(Serializer &serializer);
            bool deserialize(Deserializer &deserializer);
        };

        template <typename Location, typename Vehicle>
        struct DataInstance
        {
            // If graph is empty, assume full connectivity.
            // Nilib::Graph<Location> graph;
            std::vector<Location> locations;

            // If fleet is empty, assume unlimited vehicles.
            std::vector<Vehicle> fleet;

            void draw(Nilib::Window &window) const;

            bool serialize(Serializer &serializer);
            bool deserialize(Deserializer &deserializer);

            static DataInstance empty()
            {
                return DataInstance();
            };
        };

    };
};
#endif