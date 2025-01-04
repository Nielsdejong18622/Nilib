#include <iostream>

#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Renderer/ShaderProgram.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>



// Function to compile shaders
GLuint compileShader(GLenum shaderType, const char* shaderSource) {
    LOG_DEBUG("Compiling Shader!");
    ASSERT(glfwGetCurrentContext(), "No associated GLAD context!");

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        LOG_ERROR("[SHADER] COMPILATION_FAILED", infoLog);
        return 0;
    }
    return shader;
}



int main() 
{
    using namespace Nilib;

    Window window = Window(1024, 812, "RenderTest2");
    window.bindkey(std::bind(&Window::close, &window), Key::Escape);

    window.open();
    window.clearColor(Colors::Gray);

    // Create and compile the shader program
    ShaderProgram shaderprogram = ShaderProgram::createFromFiles("shaders/vertex.hlsl", "shaders/fragment.hlsl");

    if (shaderprogram) {
        return -1;  // Shader program creation failed
    }

    
    // Set up a simple triangle
    float vertices[] = {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO

    glBindVertexArray(0); // Unbind VAO

    while (window.opened())
    {
        // Window 1.
        window.startScene(); // Check if window is open.
        
        // Use our shaderprogram. 
        shaderprogram.bind();
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.endScene(); // Renders the geometry. 

        Window::update();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    LOG_SUCCESS("Termination of Example!");
    return EXIT_SUCCESS;
}