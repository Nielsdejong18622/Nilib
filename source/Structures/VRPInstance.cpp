#include "Nilib/Structures/VRPInstance.hpp"
#include "Nilib/Core/Log.hpp"

Instance::Instance(size_t const nnodes, size_t const vehcap, Matrix<float> const &A, Matrix<float> const &X)
    : A(A), X(X), vehcap(vehcap)
{

}

Instance Instance::createRandom(size_t const nnodes, size_t const vehcap)
{
    CORE_ASSERT(vehcap >= 1);
    // Feature matrix X;
    // (x, y, isdepot, nnodes, vehcap)
    Matrix<float> Pos = Matrix<float>::create_randn(nnodes, 2, 0, 1);
    Matrix<float> isdepot = Matrix<float>::create_zeros(nnodes, 1);
    Matrix<float> Nodes = Matrix<float>::all(nnodes, 1, nnodes);
    Matrix<float> V = Matrix<float>::all(nnodes, 1, vehcap);
    //Matrix<float> numnodes = Matrix<float>::all(nnodes, 1, nnodes);
    isdepot(0, 0) = 1.0;
    Matrix<float> X = cbind(Pos, isdepot); 
    X = cbind(X, Nodes); 
    X = cbind(X, V); 
    
    // Adjacency matrix A;
    Matrix<float> A = Matrix<float>::create_ones(nnodes, nnodes) - Matrix<float>::diag(nnodes, nnodes, 1.0);
    A.apply([](float const t) {return t > 0.5;});

    // Make sure that depot is reachable.

    return Instance(nnodes, vehcap, A, X);
}

void Instance::draw(Window &window) const
{
    for (size_t idx = 0; idx < X.rows(); ++idx)
    {  
        //Log::debug("Drawing node ") << idx << ' ' << X(idx, 0) << ',' << X(idx, 1) << " D:" << X(idx, 2) << '\n';
        if (X(idx, 2) > 0.5)
        {
            window.drawColor(Colors::Grey);
            window.drawSquare(Vec2D<float>({X(idx,0), X(idx, 1)}), 10);
        }
        else
        {
            window.drawColor(Colors::Blue);
            window.drawCircle(Vec2D<float>({X(idx, 0), X(idx, 1)}), 10);
        }
    }
    
}

// Return a matrix containing the distance between every node.
Matrix<float> Instance::distances() const
{   
    Matrix<float> dist(A);
    for (size_t i = 0; i < A.rows(); i++)
    {
        for (size_t j = 0; j < A.cols(); j++)
        {
            dist(i, j) = std::sqrt((X(i,0) - X(j,0)) * (X(i,0) - X(j,0)) + (X(i,1) - X(j,1)) * (X(i,1) - X(j,1)));
        }
    }
    return dist;
}
size_t Instance::numlocations() const
{
    return A.cols();
}


size_t Instance::vehcapacity() const
{
    return vehcap;
}


bool Instance::serialize(Serializer &serializer)
{
    Log::debug("Serializing Instance!\n");
    serializer.writeObject(A);
    serializer.writeObject(X);
    serializer.writeRaw(vehcap);
    //Log::success("Serialized Instance!\n");
    return true;
}

bool Instance::deserialize(Deserializer &deserializer)
{
    Log::debug("Deserializing Instance!\n");
    deserializer.readObject(A);
    deserializer.readObject(X);
    deserializer.readRaw(vehcap);
    return true;
}