#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Get and print GLFW version
    const char* glfwVersion = glfwGetVersionString();
    std::cout << "GLFW Version: " << glfwVersion << std::endl;

    // Clean up and exit
    glfwTerminate();
    return 0;
}
