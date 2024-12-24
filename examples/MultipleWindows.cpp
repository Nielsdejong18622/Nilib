#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Nilib/Logger/Log.hpp"

#include <Nilib/Renderer/Window.hpp>

int main(int argc, char** argv)
{

    Window win1(1024, 800, "Window1");
    Window win2(1024, 800, "Window2");

    win1.open();

    while (win1.opened() || win2.opened())
    {
        glfwPollEvents();
    }
    
    /**
    int xpos, ypos, height, width;
    const char* description;

    GLFWwindow* windows[4];

    if (!glfwInit())
    {
        glfwGetError(&description);
        printf("Error: %s\n", description);
        exit(EXIT_FAILURE);
    }

    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &width, &height);
    
    LOG_DEBUG("Monitor work area", xpos, ypos, width, height);

    for (int i = 0;  i < 4;  i++)
    {
        LOG_DEBUG("In Loop", i);
        int const size = height / 5;
        struct Color
        {
            float r, g, b;
        } colors[] =
        {
            { 0.95f, 0.32f, 0.11f },
            { 0.50f, 0.80f, 0.16f },
            {   0.f, 0.68f, 0.94f },
            { 0.98f, 0.74f, 0.04f }
        };

        if (i > 0)
            glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

        LOG_DEBUG("Window",i, "X:", xpos + size * (1 + (i & 1)), "Y:", ypos + size * (1 + (i >> 1)));
        glfwWindowHint(GLFW_POSITION_X, xpos + size * (1 + (i & 1)));
        glfwWindowHint(GLFW_POSITION_Y, ypos + size * (1 + (i >> 1)));

        windows[i] = glfwCreateWindow(size, size, "Multi-Window Example", NULL, NULL);
        if (!windows[i])
        {
            glfwGetError(&description);
            printf("Error: %s\n", description);
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        LOG_SUCCESS("Created Window", i, '!');

        glfwSetInputMode(windows[i], GLFW_STICKY_KEYS, GLFW_TRUE);

        glfwMakeContextCurrent(windows[i]);
        LOG_SUCCESS("Made Context Current!");
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        LOG_INFO("Color", colors[i].r, colors[i].g, colors[i].b, 1.f);
        glClearColor(colors[i].r, colors[i].g, colors[i].b, 1.f);
    }
    LOG_INFO("Starting render loops!");
    
    
    while (true)
    {

        for (int i = 0;  i < 4;  i++)
        {
            if (glfwWindowShouldClose(windows[i])) continue;

            glfwMakeContextCurrent(windows[i]);
            //LOG_INFO("Window", i, "is contextcurrent!");
        
            if (glfwGetKey(windows[i], GLFW_KEY_ESCAPE)== GLFW_PRESS || glfwWindowShouldClose(windows[i])) {
                LOG_TRACE("Window", i, "should close!");
                glfwSetWindowShouldClose(windows[i], GLFW_TRUE);
            }
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(windows[i]);
        }

        glfwWaitEvents();
    }
}
*/
}