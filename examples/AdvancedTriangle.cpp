#include <iostream>

#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Renderer/ShaderProgram.h"
#include "Nilib/Renderer/Renderer.h"

int main()
{
    using namespace Nilib;

    Window window = Window(1024, 812, "RenderTest2");
    Renderer renderer;

    window.open();

    window.clearColor(Colors::Gray);

    window.bindkey(std::bind(&Window::close, &window), Key::Escape);
    window.bindkey(std::bind(&Renderer::toggleWireFrameMode, &renderer), Key::R);

    // Pan camera.
    window.bindkey(std::bind(&Camera::move_left, &renderer.camera), Key::A, Key::Hold);
    window.bindkey(std::bind(&Camera::move_right, &renderer.camera), Key::D, Key::Hold);
    window.bindkey(std::bind(&Camera::move_up, &renderer.camera), Key::W, Key::Hold);
    window.bindkey(std::bind(&Camera::move_down, &renderer.camera), Key::S, Key::Hold);
    
    window.bindkey(std::bind(&Camera::move_left, &renderer.camera), Key::A, Key::Press);
    window.bindkey(std::bind(&Camera::move_right, &renderer.camera), Key::D, Key::Press);
    window.bindkey(std::bind(&Camera::move_up, &renderer.camera), Key::W, Key::Press);
    window.bindkey(std::bind(&Camera::move_down, &renderer.camera), Key::S, Key::Press);

    renderer.submitQuad({{0.0f, 0.0f, 0.0f}, Colors::Red},
                        {{0.0f, -0.5f, 0.0f}, Colors::Green},
                        {{-0.5f, -0.5f, 0.0f}, Colors::Blue},
                        {{-0.5f, 0.0f, 0.0f}, Colors::Yellow});

    renderer.submitQuad({{0.0f, 0.0f, 0.0f}, Colors::Red},
                        {{0.0f, 1.f, 0.0f}, Colors::Green},
                        {{0.9f, 1.f, 0.0f}, Colors::Blue},
                        {{1.f, 0.0f, 0.0f}, Colors::Yellow});

    renderer.submitTriangle({{0.0f, 0.5f, 0.0f}, Colors::Cyan},
                            {{-0.2f, -0.5f, 0.0f}, Colors::Coral},
                            {{0.5f, -0.4f, 0.0f}, Colors::Maroon});

    while (window.opened())
    {
        // Window 1.
        window.startScene(); // Check if window is open.

        renderer.drawCalls();

        // Some immediate mode rendering.
        window.color(Colors::Magenta);
        window.drawArc({0.0, -1.0}, {1.0, 0.0}, 5.0);

        window.endScene(); // Renders the geometry.

        Window::update();
    }

    LOG_SUCCESS("Termination of Example!");
    return EXIT_SUCCESS;
}