#include "Nilib/Logger/Log.hpp"

#include <Nilib/Renderer/Window.hpp>


int main(int argc, char** argv)
{
    using namespace Nilib;
    RNG::seed(127);
    Window win1(1024, 800, "Window1 (Hold R and A)");
    Window win2(250, 100, "Fullsize (Press R and A)");
    

    // Add some keybindings.
    win1.bindkey(std::bind(&Window::close, &win1), Key::Escape, 0, 0);
    win1.bindkey(std::bind(&Window::open, &win2), Key::A, 0, 0);
    win1.bindkey([&]() { win1.clearColor(Color::RandomHSL()); }, Key::R, Key::Hold, 0);

    win2.bindkey(std::bind(&Window::close, &win2), Key::Escape, 0, 0);
    win2.bindkey(std::bind(&Window::open, &win1), Key::A, 0, 0);
    win2.bindkey([&]() { win2.clearColor(Color::RandomHSL()); }, Key::R, Key::Press, 0);
    

    win1.open();
    win2.open();

    win1.clearColor(Color::RGBA(90, 200, 80, 255));
    win2.clearColor(Color::RGBA(100, 40, 90, 200));

    while (win1.opened() || win2.opened())
    {
        // Render for window 1.
        win1.startScene();
        win1.drawArc(0, 0, 100, 100, 4);
        win1.endScene();

        // Render for window 2. 
        win2.startScene();
        win2.drawArc(0, 0, -100, 100, 4);
        win2.endScene();
        Window::updateidletasks();

        // win1.clearColor(Color::RandomHSL());
        // win2.clearColor(Color::RandomRGB());
    }
    
}