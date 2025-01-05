#include <iostream>

#include "Nilib/Renderer/Renderer.h"
#include "Nilib/Renderer/ShaderProgram.h"

#include "Nilib/Renderer/Window.hpp"


int main() 
{
    using namespace Nilib;

    Window window = Window(1024, 812, "RenderTest2");
    window.bindkey(std::bind(&Window::close, &window), Key::Escape);

    window.open();

    window.clearColor(Colors::Gray);

    // Create and compile the shader program
    // ShaderProgram triangleshader = ShaderProgram::createFromFiles("shaders/vertex.hlsl", "shaders/fragment.hlsl");

    // if (triangleshader) {
    //     LOG_ERROR("Shader Program creation failed!");
    //     return -1; 
    // }

    // Renderer renderer;

    // renderer.submitTriangle({{ 0.0f, 0.5f, 0.0f}, Colors::Cyan},
    //                         {{-0.2f,-0.5f, 0.0f}, Colors::Coral},
    //                         {{ 0.5f,-0.4f, 0.0f}, Colors::Maroon});
    
    // renderer.submitTriangle({{ 0.0f, 0.5f, 0.0f}, Colors::Red},
    //                         {{-0.5f,-0.5f, 1.0f}, Colors::Blue},
    //                         {{ 0.5f,-0.5f, 0.0f}, Colors::Lavender});
    // renderer.submitTriangle({{-1.0f, -1.0f, 0.0f}, Colors::Green},
    //                         {{-0.5f, -0.5f, 0.0f}, Colors::Grey},
    //                         {{ 0.0f, -1.0f, 0.0f}, Colors::Purple});

    window.setXlim(-12.0f,   200.0f);
    window.setYlim(-10.0f, 10.0f);
    // Check if window is open.
    while (window.opened())
    {
        // Window 1. 
        window.startScene(); 
        
        window.color(Colors::Magenta);
        window.drawArc({0.0, -1.0}, {1.0, 0.0});
        window.drawTriangleUp({1.0f, 2.0f});
        window.drawCross({10.0f, 10.0f});

        window.endScene(); // Renders the geometry. 

        Window::updateidletasks();
    }


    LOG_SUCCESS("Termination of Example!");
    return EXIT_SUCCESS;
}