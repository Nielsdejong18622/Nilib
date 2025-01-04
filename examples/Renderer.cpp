
#include "Nilib/Renderer/Window.hpp"

int main()
{
    using namespace Nilib;
    Window window = Window(1024, 812, "RenderTest");

    window.bindkey(std::bind(&Window::close, window), Key::Escape, 0, 0);


    window.clearColor(Colors::Gray);

    while (window.opened())
    {
        // Window 1.
        window.startScene(); // Check if window is open.

        window.endScene(); // Renders the geometry. 
    }

}