#ifndef _WINDOW_H
#define _WINDOW_H
#pragma once

// External headers.
#include <GLFW/glfw3.h>

#include <map>

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Renderer/Color.h"
#include "Nilib/Renderer/Keys.h"
#include "Nilib/Renderer/Camera.h"
#include "Nilib/Math/Matrix.hpp"

namespace Nilib
{

    /*
     * Barebones window class. Bind keybindings, Plot some shapes.
     * Non-resizable.
     * Name is given.
     * Immediate mode opengl.
     */
    class Window
    {
        // Typedefs.
        typedef std::function<void(void)> Callback;
        typedef std::tuple<int, int, int, int> KeyEvent;

        struct Windowdata
        {
            int width = 800;
            int height = 800;
            int screenposx = 0;
            int screenposy = 0;
            // size_t framebufx;
            // size_t framebufy;
            int minwidth = 0;
            int minheight = 0;
            Color clearColor;
            char const *title;
            Window *owner = nullptr;
            std::map<KeyEvent, Callback> keybindings;

            // Immediate mode rendering.
            float xmin = -1.0f;
            float xmax = +1.0f;

            float ymin = -1.0f;
            float ymax = +1.0f;
        };

    public:
        Window(size_t width, size_t height, char const *title);
        Window(size_t width, size_t height, char const *title, size_t minwidth, size_t minheight, bool, bool decorated, bool resizeable);
        ~Window();

        // Set Windowtitle.
        void title(char const *title) const;
        void title(std::string const &title) const;
        char const *title() const;

        // Open/close the window.
        void open();
        void close();
        bool opened() const;

        // Start the scene. After this we can draw.
        void startScene();

        // Submit all the drawings to the GPU.
        void endScene();

        // Ping the user.
        void requestAttention() const;

        // Bind Key Callbacks.
        void bindkey(Callback const &fun, int key, int action, int mods = 0);
        void bindkey(Callback const &bindfun, int key);

        inline float getXlim_min() const { return d_data.xmin; };
        inline float getXlim_max() const { return d_data.xmax; };
        inline float getYlim_min() const { return d_data.ymin; };
        inline float getYlim_max() const { return d_data.ymax; };

    protected:
        GLFWwindow *d_window = nullptr;
        GLFWmonitor *d_monitor = nullptr;
        static size_t s_windowsactive;

        Windowdata d_data;

        void setCallbacks() const;
        void static key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        void static drop_callback(GLFWwindow *window, int path_count, const char *paths[]);
        void static cursorPos_callback(GLFWwindow *window, double xpos, double ypos);
        void static framebuffer_size_callback(GLFWwindow *window, int width, int height);
        void static windowPos_callback(GLFWwindow *window, int xpos, int ypos);
        void static windowSize_callback(GLFWwindow *window, int width, int height);
        void static mouseButton_callback(GLFWwindow *window, int button, int action, int mods);
        void static windowFocus_callback(GLFWwindow *window, int focused);
        void static windowMaximize_callback(GLFWwindow *window, int maximized);
        void static windowContentScale_callback(GLFWwindow *window, float xscale, float yscale);
        void static windowIconify_callback(GLFWwindow *window, int iconified);
        void static cursorEnter_callback(GLFWwindow *window, int entered);
        void static scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
        void static error_callback(int code, char const *description);
        void static windowClose_callback(GLFWwindow *window);

        // Utility function.
        static Window &windowFromPtr(GLFWwindow *window);

    public:
        // Poll and handle events and block the calling thread.
        void static updateidletasks();

        // Poll and handle events.
        void static update();

        void screenshotPNG(char const *filename = "Screenshot.png");

        // Below is all for immediate mode.
        // Calling these functions while shaders/Render exist might not give the expected result.
        // Immediate mode 2D Draw functions.

        void setXlim(float const xmin, float const xmax);
        void setYlim(float const ymin, float const ymax);

        // Set the background color.
        void clearColor(Color const &color);
        void linewidth(float const lw) const;

        void drawArc(Vec2f const &A, Vec2f const &B, float const linewidth = 1.0f) const;
        void drawCircle(Vec2f const &centre, float const radius = 1.0f, float const linewidth = 1.0f, unsigned int sides = 12) const;
        void drawFilledCircle(Vec2f const &centre, float const radius = 1.0f, float const linewidth = 1.0f, unsigned int sides = 12) const;
        void drawTriangleUp(Vec2f const &centre, float const size = 1.0f) const;
        void drawSquare(Vec2f const &centre, float const size = 1.0f) const;
        void drawRectangle(Vec2f const &leftup, Vec2f const &rightdown) const;
        void drawDiamond(Vec2f const &center, float const radius = 1.0f, float const linewidth = 1.0f) const;
        void drawCross(Vec2f const &centre, float const size = 1.0f) const;
        void drawTriangleDown(Vec2f const &centre, float const size = 1.0f) const;
        void drawPole(Vec2f const &centre, float const size) const;
        void color(Color const &color);

    private:
        void transform2D(float &x, float &y) const;
        void transform2DDist(float &dist) const;
    };

} // Nilib;
#endif