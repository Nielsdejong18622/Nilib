#include <GLFW/glfw3.h>
#include <gurobi_c++.h>
#include <iostream>

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }
#ifdef WIN32
    // Get and print GLFW version
    const char *glfwVersion = glfwGetVersionString();
    std::cout << "GLFW Version: " << glfwVersion << std::endl;

    // Gurobi: Initialize and print version information
    try
    {
        GRBEnv env = GRBEnv(true); // Create a Gurobi environment
        env.start();               // Start the environment

        // Get Gurobi version
        int major, minor, patch;
        GRBversion(&major, &minor, &patch); // Get version as major, minor, patch

        std::cout << "Gurobi Version: " << major << "." << minor << "." << patch << std::endl;
    }
    catch (GRBException &e)
    {
        std::cerr << "Error code " << e.getErrorCode() << ": " << e.getMessage() << std::endl;
        return -1;
    }
#endif
    // Clean up and exit
    glfwTerminate();

    return 0;
}
