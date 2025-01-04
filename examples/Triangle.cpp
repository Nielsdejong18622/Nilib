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


    struct TriangleVertex 
    {
        Vec3f position;
        Vec4f color;
        // Texture coordinate. 
    };

    TriangleVertex triangle_vertices[] = {
       {{ 0.0f, 0.5f, 0.0f}, Colors::Blue},
       {{-0.5f,-0.5f, 0.0f}, Colors::Blue},
       {{ 0.5f,-0.5f, 0.0f}, Colors::Blue},

       {{-1.0f, -1.0f, 0.0f}, Colors::Red},
       {{-0.5f, -0.5f, 0.0f}, Colors::Red},
       {{ 0.0f, -1.0f, 0.0f}, Colors::Red}
    };

    GLuint VBO, VAO_triangles;
    glGenVertexArrays(1, &VAO_triangles);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO_triangles);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    // Attribute position.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)0);
    // Attribute color.
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO

    glBindVertexArray(0); // Unbind VAO_triangles

    while (window.opened())
    {
        // Window 1.
        window.startScene(); // Check if window is open.
        
        // Use our shaderprogram. 
        shaderprogram.bind();
        
        glBindVertexArray(VAO_triangles);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.endScene(); // Renders the geometry. 

        Window::update();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO_triangles);
    glDeleteBuffers(1, &VBO);

    LOG_SUCCESS("Termination of Example!");
    return EXIT_SUCCESS;
}