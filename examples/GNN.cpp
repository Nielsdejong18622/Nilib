#include "Nilib/ML/ML.h"

#include "Nilib/ML/MLData.h"
#include "Nilib/ML/MLP.h"
#include "Nilib/Core/Signal.hpp"

#include "Nilib/Logger/CSV.hpp"

int main()
{
    Nilib::RNG::seed();

    using namespace Nilib;

    static size_t f = 2;  // Node feature dimension
    static size_t f2 = 8; // Hidden dimension size.
    static size_t f1 = 1; // Target Node feature dimension.

    struct GraphObs
    {
        size_t n;       // Num nodes.
        Matrixf Adj;    // Adjacency matrix.
        Matrixf Xnodes; // X features.
        Matrixf Ynodes; // Y features.
    };

    struct GraphDegree : public GraphObs
    {

        GraphDegree(size_t nodes)
        {
            n = nodes;
            Adj = Matrixf::rand(n, n);
            Adj.apply([](float t)
                      { return t > 0.5; });
            Xnodes = Matrixf::randn(n, f, 0.0f, 1.0f);
            // Create target node features.
            Ynodes = Matrixf::zeros(n, f1);
            for (size_t i = 0; i < n; i++)
                for (size_t j = 0; j < n; ++j)
                    Ynodes(i) += Adj(i, j);
        }
    };

    struct GraphDist : public GraphObs
    {
        GraphDist(size_t nodes)
        {
            n = nodes;
            Adj = Matrixf::rand(n, n);
            Adj.apply([](float t)
                      { return t > 0.5; });
            Xnodes = Matrixf::randn(n, f, 0.0f, 1.0f);
            // Create target node features.
            Ynodes = Matrixf::zeros(n, f1);
            for (size_t i = 0; i < n; i++)
                for (size_t j = 0; j < n; ++j)
                    Ynodes(i) += (Nilib::manhattan(Vec2f{Xnodes(i, 0), Xnodes(i, 1)}, Vec2f{Xnodes(j, 0), Xnodes(j, 1)})) / n;
        }
    };

    // Predict the adjacency matrix
    struct GraphC : public GraphObs
    {
        GraphC(size_t nodes)
        {
            n = nodes;
            Adj = Matrixf::rand(n, n);
            Adj.apply([](float t)
                      { return t > 0.5; });
            Xnodes = Matrixf::randn(n, f, 0.0f, 1.0f);
            // Create target node features.
            Ynodes = Matrixf::zeros(n, n);
            for (size_t i = 0; i < n; i++)
                for (size_t j = 0; j < n; ++j)
                    Ynodes(i, j) = (Nilib::manhattan(Vec2f{Xnodes(i, 0), Xnodes(i, 1)}, Vec2f{Xnodes(j, 0), Xnodes(j, 1)}));
        }
    };

    // Predict the Distance matrix.
    {

        std::vector<GraphC> training;
        for (size_t obs = 0; obs < 64; obs++)
            training.emplace_back(2 + Nilib::RNG::index(5));

        // Create the computational graph.
        Input A, X, Y;

        Weight W(f, f2);

        DenseGraphLayer H1(&A, &X, &W);
        Relu H2(&H1, 0.001f);

        Weight Wt(f2, 1);
        Weight Wb(f2, 1);
        Weight b(1, 1);
        NN_outerdecoder H3(&H2, &Wt, &Wb, &b);
        Relu H4(&H3, 0.001f);

        MSELoss L(&H4, &Y);

        float slr = 0.0005;

        for (size_t iter = 0; iter < 1'000'000; ++iter)
        {
            if (Signal::receivedCtrlC())
                break;
            float const lr = slr * exp(-0.0001 * iter);
            float iter_loss = 0.0;
            for (auto &&obs : training)
            {
                Matrixf seed = Matrixf::ones(obs.n, obs.n);

                // Input the Matrices in the Compu graph.
                A.set(obs.Adj), X.set(obs.Xnodes), Y.set(obs.Ynodes);

                L.evaluate();
                // L.derive(seed);
                iter_loss += L.value.sum() / obs.n;
            }
            LOG_DEBUG("Constructed Cgraph!");
            // if (iter % 500 == 0)
            //     LOG_DEBUG("Iteration:", iter, iter_loss / training.size(), W.partial.sum());

            // float const Wgrad = W.partial.sum();

            // CSV("training.csv", iter, iter_loss, Wgrad, lr);

            // W.value -= lr * W.partial;
            // W.partial.zero();
        }
        LOG_DEBUG("End W matrix:");
        W.value.print();
        LOG_DEBUG("Xnodes");
        X.value.print();
        LOG_DEBUG("Prediction:");
        H4.value.print();
        LOG_DEBUG("Target:");
        Y.value.print();
    }

    if (false)
    {
        // Simple examplle.
        std::vector<GraphDist> training;
        for (size_t obs = 0; obs < 64; obs++)
            training.emplace_back(2 + Nilib::RNG::index(5));

        // Create the computational graph.
        Input A, X, Y;

        Weight W(f, f2);
        Weight W2(f2, f1);

        DenseGraphLayer H1(&A, &X, &W);
        Relu H2(&H1, 0.001f);

        DenseGraphLayer H3(&A, &H2, &W2);
        Relu H4(&H3, 0.001f);

        MSELoss L(&H4, &Y);

        float slr = 0.0005;

        for (size_t iter = 0; iter < 1'000'000; ++iter)
        {
            if (Signal::receivedCtrlC())
                break;
            float const lr = slr * exp(-0.0001 * iter);
            float iter_loss = 0.0;
            for (auto &&obs : training)
            {
                Matrixf seed = Matrixf::ones(obs.n, f1);

                // Input the Matrices in the Compu graph.
                A.set(obs.Adj), X.set(obs.Xnodes), Y.set(obs.Ynodes);

                L.evaluate();
                L.derive(seed);
                iter_loss += L.value.sum() / obs.n;
            }
            if (iter % 500 == 0)
                LOG_DEBUG("Iteration:", iter, iter_loss / training.size(), W.partial.sum());

            float const Wgrad = W.partial.sum();
            float const W2grad = W2.partial.sum();

            CSV("training.csv", iter, iter_loss, Wgrad, W2grad, lr);

            W.value -= lr * W.partial;
            W2.value -= lr * W2.partial;
            W.partial.zero();
            W2.partial.zero();
        }
        LOG_DEBUG("End W matrix:");
        W.value.print();
        LOG_DEBUG("Xnodes");
        X.value.print();
        LOG_DEBUG("Prediction:");
        H4.value.print();
        LOG_DEBUG("Target:");
        Y.value.print();
    }
    return 0;
}