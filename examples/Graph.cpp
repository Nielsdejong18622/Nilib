//#include "GraphLayout.hpp"
#include "Nilib/Renderer/Window.hpp"
#include "Nilib/Structures/Graph.hpp"
#//include "LinAlg.hpp"


struct Ndata
{
    int a;
};

int main() {
    RNG::seed(127);
    Window win(1024, 812, "Graph window!", false, true);
    win.bindkey(std::bind(&Window::close, &win), GLFW_KEY_ESCAPE, GLFW_PRESS, 0);
    
    // Tests. 
    std::vector<Ndata> locations(20, Ndata());
    Graph<Ndata> erdos_reini;
    
    LOG_DEBUG() << "G:" << erdos_reini << '\n';
    erdos_reini.reserve(100);
    LOG_DEBUG() << "G:" << erdos_reini << '\n';
    erdos_reini = Graph<Ndata>::erdos_reini(254, 0.05);

    LOG_DEBUG() << "G:" << erdos_reini << '\n';
    
    LOG_DEBUG() << "Starting random walk\n";
    for (Graph<Ndata>::node_id v: erdos_reini.randomWalk(0, 10)) {
        LOG_DEBUG() << "Random walk:" << +v << '\n';
    }
    // Bounding box. 
    auto min = Vec2D<double>({win.windata().f1X, win.windata().f1Y});
    auto max = Vec2D<double>({win.windata().f2X, win.windata().f2Y});

    /*
    GraphLayout layout = GraphLayout<Ndata>::random(erdos_reini, min, max);
    win.bindkey(std::bind(&GraphLayout<Ndata>::update, &layout), GLFW_KEY_U, GLFW_PRESS, 0);
    win.bindkey(std::bind(&Graph<Ndata>::removeEdge, &erdos_reini, RNG::uniform(0, 100), RNG::uniform(0, 100)), GLFW_KEY_P, GLFW_PRESS, 0);
    win.bindkey(std::bind(&GraphLayout<Ndata>::update, &layout), GLFW_KEY_U, GLFW_REPEAT, 0);

    win.bindkey(std::bind(&GraphLayout<Ndata>::shuffle, &layout, erdos_reini, min, max), GLFW_KEY_R, GLFW_PRESS, 0);

    while (!win.closed())
    */
    {
        win.startScene();
        //layout.draw(win);
        //LOG_DEBUG("") << erdos_reini << '\n'; 
        win.endScene();
    }
    
    return 0;
}