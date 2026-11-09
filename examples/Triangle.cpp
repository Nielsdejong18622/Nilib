#include "Nilib/Renderer/Window.hpp"


int main() 
{
    using namespace Nilib;

    Window window = Window(1024, 812, "RenderTest2");
    window.bindkey(std::bind(&Window::close, &window), Key::Escape);

    window.open();

    window.clearColor(Colors::Gray);

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