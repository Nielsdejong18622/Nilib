#ifndef _VRP_INSTANCE_HPP
#define _VRP_INSTANCE_HPP

#include "Nilib/Math/Matrix.hpp"
#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Core/Serializer.hpp"
#include "Nilib/Core/Deserializer.hpp"

// An instance holds a graph adjacency matrix. And a matrix with node attributes.
class Instance
{
public:
    Matrix<float> A; // Adjacency matrix.
    Matrix<float> X; // Attributes
    size_t vehcap; // vehicle capacity. (Assume all nodes have demand 1).

    friend class Solution;
    Instance() = default;
    
    Instance(size_t const nnodes, size_t const vehcap, Matrix<float> const &A, Matrix<float> const &X);

    // Draw the instance to a window. 
    // - Uses the first two features of X matrix as x and y coordinates.
    void draw(Window &win) const;

    // Creates a simple random instance. 
    static Instance createRandom(size_t const nnodes, size_t const vehcap);

    
    // Load an instance from stream.
    static Instance load(std::istream &out);

    // Get number of locations
    size_t numlocations() const;
    size_t vehcapacity() const;

    Matrix<float> distances() const;
    
    //static bool serialize(Serializer &serializer. Instance &inst);
    //static Instance deserialize(Deserializer &deserializer);
    bool serialize(Serializer &serializer);
    bool deserialize(Deserializer &deserializer);
};


#endif