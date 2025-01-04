#include <iostream>

#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Renderer/ShaderProgram.h"
#include "Nilib/Renderer/Renderer.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>




int main() 
{
    using namespace Nilib;

    Window window = Window(1024, 812, "RenderTest2");
    window.bindkey(std::bind(&Window::close, &window), Key::Escape);

    window.open();
    window.clearColor(Colors::Gray);

    // Create and compile the shader program
    ShaderProgram triangleshader = ShaderProgram::createFromFiles("shaders/vertex.hlsl", "shaders/fragment.hlsl");

    if (triangleshader) {
        LOG_ERROR("Shader Program creation failed!");
        return -1; 
    }

    Renderer renderer;

    renderer.submitTriangle({{ 0.0f, 0.5f, 0.0f}, Colors::Cyan},
                            {{-0.2f,-0.5f, 0.0f}, Colors::Coral},
                            {{ 0.5f,-0.4f, 0.0f}, Colors::Maroon});
    
    renderer.submitTriangle({{ 0.0f, 0.5f, 0.0f}, Colors::Red},
                            {{-0.5f,-0.5f, 1.0f}, Colors::Blue},
                            {{ 0.5f,-0.5f, 0.0f}, Colors::Lavender});
    renderer.submitTriangle({{-1.0f, -1.0f, 0.0f}, Colors::Green},
                            {{-0.5f, -0.5f, 0.0f}, Colors::Grey},
                            {{ 0.0f, -1.0f, 0.0f}, Colors::Purple});


    while (window.opened())
    {
        // Window 1. 
        window.startScene(); // Check if window is open.
        
        // Use our triangleshader. 
        triangleshader.bind();
        
        renderer.drawCalls();

        window.endScene(); // Renders the geometry. 

        Window::updateidletasks();
    }


    LOG_SUCCESS("Termination of Example!");
    return EXIT_SUCCESS;
}