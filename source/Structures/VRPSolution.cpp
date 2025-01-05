#include "Nilib/Structures/VRPSolution.hpp"
#include "gurobi_c++.h"

using namespace Nilib;

bool Solution::feasible() const
{

    return true;
}

float Solution::optimalityGap() const
{
    float obj = objective();
    if (obj <= d_opt_objval)
        return 0.0;
    // CORE_ASSERT(obj >= d_opt_objval)
    if (d_opt_objval == 0)
        LOG_WARNING() << "Optimal Objective value not calculated!\n";
    return (obj - d_opt_objval) / d_opt_objval;
}

float Solution::objective() const
{
    float objval = 0;
    for (size_t ri = 0; ri < d_X.rows(); ++ri)
    {
        for (size_t ci = 0; ci < d_X.cols(); ++ci)
        {
            if (d_X(ri, ci) > 0.5)
            {
                objval += d_instance.distances()(ri, ci);
            }
        }
    }
    return objval;
}
Solution::Solution(Instance const &inst, Matrixf const &X, float const opt_objval)
    : d_instance(inst), d_X(X), d_opt_objval(opt_objval)
{
}

Solution::Solution(Instance const &inst, Matrixf const &X)
    : Solution(inst, X, 0.0)
{
}

void Solution::random()
{
    d_X = Matrixf::rand(d_X.rows(), d_X.cols());
    d_X.apply([](float const t)
              { return (t > 0.8); });
}

Solution Solution::feasibleSolution(Instance const &inst)
{
    size_t const numLocations = inst.numlocations();
    if (numLocations == 1)
    {
        return Solution(inst, Matrixf::ones(1, 1));
    }
    CORE_ASSERT(numLocations > 1)

    size_t const VEHICLE_CAP = inst.vehcap;

    Matrixf d_X = Matrixf::zeros(numLocations, numLocations);

    std::vector<int> customers(numLocations - 1);

    for (size_t i = 0; i < numLocations - 1; ++i)
    {
        customers.at(i) = i + 1; // Customers are indexed from 1 to numCustomers
    }

    // Shuffle the customers to create a random order
    random_shuffle(customers.begin(), customers.end());

    // Start creating routes
    int currentIndex = 0; // Tracks the index of the customer

    while (currentIndex < numLocations - 1)
    {
        std::vector<int> route;
        route.push_back(0); // Start from the depot

        int currentCapacity = 0;

        // Add customers to the route while respecting the vehicle capacity
        while (currentIndex < numLocations - 1 && currentCapacity < VEHICLE_CAP)
        {
            route.push_back(customers.at(currentIndex));
            currentCapacity++;
            currentIndex++;
        }
        route.push_back(0); // Return to the depot

        // Update the adjacency matrix to reflect the route
        for (size_t i = 0; i < route.size() - 1; ++i)
        {
            d_X(route[i], route[i + 1]) = 1;
            // d_X(route[i+1], route[i]) = 1;
        }
    }
    return Solution(inst, d_X);
}

Solution Solution::randomSolution(Instance const &inst)
{
    // ASSUMES Node 0 is always the depot!
    size_t const numnodes = inst.numlocations();

    // Really random.
    Matrixf X = Matrixf::rand(numnodes, numnodes);
    X.apply([](float const t)
            { return (t > 0.8); });

    // Feasible random.

    return Solution(inst, X);
}

bool Solution::operator==(Solution const &other) const
{
    ASSERT(false, "Comparison of Solutions is not yet available!");
    return false; //(d_X == other.d_X);
}

Solution Solution::optimalSolution(Instance const &inst)
{
    // 0 is the depot.
    int n = inst.numlocations(); // Number of nodes
    Matrixf d_X = Matrixf(n, n);
    double objValue = 0.0;
    try
    {
        static GRBEnv env = GRBEnv(true);
        env.start();

        GRBModel model = GRBModel(env);

        // Silence the solver output
        model.set(GRB_IntParam_OutputFlag, 0);

        // Create decision variables
        std::vector<std::vector<GRBVar>> x(n, std::vector<GRBVar>(n));
        std::vector<GRBVar> u(n); // Auxiliary variables for MTZ constraints

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                float const xdist = std::pow(inst.X(i, 0) - inst.X(j, 0), 2.0f);
                float const ydist = std::pow(inst.X(i, 1) - inst.X(j, 1), 2.0f);
                float const dist = std::sqrt(xdist + ydist);
                x[i][j] = model.addVar(0.0, 1.0, dist, GRB_BINARY, "x_" + std::to_string(i) + "_" + std::to_string(j));
            }
        }

        for (int i = 1; i < n; ++i)
        {
            u[i] = model.addVar(1, n - 1, 0.0, GRB_CONTINUOUS, "u_" + std::to_string(i));
        }

        // MTZ Subtour Elimination Constraints
        for (int i = 1; i < n; ++i)
        {
            for (int j = 1; j < n; ++j)
            {
                if (i != j)
                {
                    model.addConstr(u[i] - u[j] + (n - 1) * x[i][j] <= n - 2, "MTZ_" + std::to_string(i) + "_" + std::to_string(j));
                }
            }
            model.addConstr(u[i] <= inst.vehcapacity());
        }

        // Constraints
        // Each node must be entered and exited exactly once
        for (int i = 1; i < n; ++i)
        {
            GRBLinExpr entry = 0;
            GRBLinExpr exit = 0;
            for (int j = 0; j < n; ++j)
            {
                if (i != j)
                {
                    entry += x[j][i];
                    exit += x[i][j];
                }
            }
            model.addConstr(entry == 1, "entry_" + std::to_string(i));
            model.addConstr(exit == 1, "exit_" + std::to_string(i));
        }

        // Add constraints for vehicle capacity
        // Assuming k is the vehicle capacity constraint (simplified)
        // Adding additional constraints to ensure vehicle capacity constraint

        // Optimize the model
        model.optimize();
        // Print the solution
        if (model.get(GRB_IntAttr_Status) == GRB_OPTIMAL)
        {
            // std::cout << "Optimal solution found!" << std::endl;
            objValue = model.get(GRB_DoubleAttr_ObjVal);

            // Retrieve and print the solution
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    d_X(i, j) = (x[i][j].get(GRB_DoubleAttr_X) > 0.5);
                    // d_X(j, i) = (x[i][j].get(GRB_DoubleAttr_X) > 0.5);
                }
            }
        }
        else
        {
            // std::cout << "No optimal solution found." << std::endl;
        }
    }
    catch (GRBException &e)
    {
        // std::cerr << "Error code = " << e.getErrorCode() << std::endl;
        // std::cerr << e.getMessage() << std::endl;
    }
    catch (...)
    {
        // std::cerr << "Exception during optimization" << std::endl;
    }

    Solution sol = Solution(inst, d_X, objValue);
    return sol;
}

void Solution::draw(Window const &window) const
{
    ASSERT(false, "Reimplement Solution::draw, please.");
    /*
    size_t const numnodes = d_instance.numlocations();
    //window.drawColor(Colors::Black);
    for (size_t from = 0; from < numnodes; ++from)
    {
        for (size_t to = 0; to < numnodes; ++to)
        {
            if (d_X(from, to) < 0.5) continue;
            Vec2D<float> from_pos( {d_instance.X(from, 0), d_instance.X(from,1)});
            Vec2D<float> to_pos( {d_instance.X(to,0), d_instance.X(to,1)});
            window.drawArc(from_pos, to_pos, 2.0f);
        }
    }
    */
}

bool Solution::serialize(Serializer &serializer)
{
    LOG_DEBUG() << "Serializing Solution!\n";
    // serializer.writeObject(d_instance);
    // serializer.writeObject(d_X);
    // LOG_SUCCESS("Serialized Instance!\n");
    return true;
}

bool Solution::deserialize(Deserializer &deserializer)
{
    LOG_DEBUG() << "Deserializing Instance!\n";
    // deserializer.readObject(d_instance);
    // deserializer.readObject(d_X);
    return true;
}