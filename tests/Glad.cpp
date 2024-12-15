#include <iostream>
#include <GLAD/glad.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>

const GLuint WIDTH = 800, HEIGHT = 600;

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    // Create a windowed GLFW window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's OpenGL context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Load OpenGL functions using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return -1;
    }

    // Print OpenGL version
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Loaded OpenGL: " << version << std::endl;

    // Main rendering loop (simplified)
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll and process events
        glfwPollEvents();

        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Cleanup and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
