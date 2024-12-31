#ifndef _ML_ENV_H
#define _ML_ENV_H

#include "Nilib/Renderer/Window.hpp"
class Matrixf;

namespace Nilib
{

    // variables: [options]
    // Move:[UP, DOWN], RAISE[[0, amount]]
    // Load:[amount[0,10]], WAIT[NO, YES]
    class ActionSpace
    {

    };
 
    class StateSpace
    {

    };

    class DiscreteStateSpace : public StateSpace
    {

    };

    class ContinuosStateSpace : public StateSpace
    {

    };

    // General environment for reinforcement learning.
    class MLEnv
    {

        typedef float Reward;
        typedef Matrixf Action;
        typedef Matrixf State;

        // Counter of how many times an action was taken. 
        size_t actions_taken = 0;

    public:
        // Obtain a reference to the current state. 
        virtual State const &state() = 0;

        // Render the current state to the window. 
        virtual void render(Window const &window) = 0;

        // Reset the environment to an initial (random) state.
        virtual void reset() = 0;

        // Take action and simulate the environment forward.
        virtual Reward step(Action const &action) = 0;

        virtual bool isterminal() const = 0;
    };

    class MLEnvImpl : public MLEnv
    {
        static MLEnv GridMaze();

        // Creates the double Pendulum environment.
        static MLEnv doublePendulum();

        // Stock trading environment.
        static MLEnv trading();

        // VRP solving environment.
        static MLEnv VRPsolving();
        
        // VRP solving environment.
        static MLEnv ArmedBandits(size_t k);
    };
} // namespace Nilib




#endif