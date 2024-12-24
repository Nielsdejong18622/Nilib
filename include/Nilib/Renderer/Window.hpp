/* 
 * Barebones window class. Bind keybindings, Plot some shapes.
 * Non-resizable. 
 * Name is given. 
 * Old opengl. 
*/
#ifndef _WINDOW_H
#define _WINDOW_H
#pragma once

// External headers.
#include <exception>
#include <string>
#include <cmath>
#include <map>
#include <functional>

//#include "../lib/GLEW/glew.h"
#include <GLFW/glfw3.h>
#include "Nilib/Logger/Log.hpp"
#include "Nilib/Renderer/Color.h"
#include "Nilib/Math/Matrix.hpp"

// Typedefs. 
typedef std::function<void(void)> Callback;
typedef std::tuple<int, int, int, int> KeyEvent;

class Window;

struct Windowdata {
    int width, height;
    int screenposx, screenposy;
    //size_t framebufx;
    //size_t framebufy;
    char const *title;
    bool open;
    bool current;
    Window* owner;
    std::map<KeyEvent, Callback> keybindings;
};

class Window
{
public:

    Window(size_t width, size_t height, char const *title);
    ~Window();
    Window(Window& win)=delete;
	Window(Window&& win)=delete;
	
    // Open/close the window.
	void open();
	void close();
	bool opened() const;

    // Start the scene. After this we can draw. 
	void startScene();

    // Submit all the drawings to the GPU. 
    void endScene();
    
    // Poll events in queue. 
    void updateidletasks();
    void update();

    // Bind Key Callback. 
    void bindkey(Callback const &fun, int key, int action, int mods);

    // Set Windowtitle.
    void setTitle(char const *title) const;

    // Set View Frustrum bounds. F1 is left uppercorner, F2 right lower corner (in world coordinates).
    void xlim(float const minx, float const max);
    void ylim(float const minx, float const max);

    Windowdata const &windata() {return d_data; }

protected:

    GLFWwindow *d_window;
	GLFWmonitor* d_monitor;
	static size_t s_windowsactive;
	Windowdata d_data;

    void setCallbacks() const;
    void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void static error_callback(int code, char const*description);
    
public:
    /*
    // Set color. 
    void drawColor(Color const &col) const;

    // Draw arc.
    void drawArc(Vec2D<float> const x1, Vec2D<float> const x2, float const linewidth) const;

    // Draw shapes. 
    void drawSquare(Vec2D<float> const x1, float size) const;
    void drawDiamond(Vec2D<float> const x1, float const radius) const;
    void drawCircle(Vec2D<float> const x, float const radius) const;
    void drawCircle(Vec2D<float> const x1, float const radius, int sides) const;
    void drawCross(Vec2D<float> const x, float const size) const;
    void drawUpTriangle(Vec2D<float> const x, float const size) const;
    void drawDownTriangle(Vec2D<float> const x, float const size) const;
	void drawCubicBezier(Vec2D<float> const x1, Vec2D<float> const x2, Vec2D<float> const x3, Vec2D<float> const x4) const;
    */
};

#endif