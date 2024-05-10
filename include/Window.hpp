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
#include "../lib/GLFW3_4/glfw3.h"
#include "Color.h"
#include "Log.hpp"

// Typedefs. 
typedef std::function<void(void)> Callback;
typedef std::tuple<int, int, int, int> KeyEvent;

class Window
{
public:

    Window(size_t width, size_t height, char const *title, bool eventbased);
    ~Window();
    //Window(Window& win)=delete;
	//Window(Window&& win)=delete;
	
	void open();
	void close();
	bool closed() const;
	bool opened() const;

    // Start the scene. After this we can draw. 
	void startScene();

    // Set color. 
    void drawColor(Color const &col) const;

    // Draw arc.
    void drawArc(float const x1, float const y1, float const x2, float const y2, float const linewidth) const;

    // Draw shapes. 
    void drawSquare(float const x, float const y, float const size) const;
    void drawCircle(float const x, float const y, float const radius) const;
    void drawCross(float const x, float const y, float const size) const;
    void drawUpTriangle(float const x, float const y, float const size) const;
    void drawDownTriangle(float const x, float const y, float const size) const;
	void drawCubicBezier(float const x1, float const y1, float const x2, float const y2, float const x3, float const y3, float const x4, float const y4) const;
    // Submit all the drawings to the GPU. 
    void endScene();
    
    // Poll events in queue. 
    void updateidletasks();
    void update();

    // Bind Key Callback. 
    void bindkey(Callback const &fun, int key, int action, int mods);

    // Set Windowtitle.
    void setTitle(char const *title) const;

protected:
    struct Windowdata {
        size_t width;
        size_t height;
        size_t screenposx;
        size_t screenposy;
        size_t framebufx;
        size_t framebufy;
        bool eventbased;
        bool open;
        Window* owner;
        std::map<KeyEvent, Callback> keybindings;

        float minX, minY;
        float maxX, maxY;
    };

	GLFWwindow *d_window;
	GLFWmonitor* d_monitor;
	static size_t s_windowsactive;
	Windowdata d_windowData;
    
    void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    void processX(float const x);
    void processY(float const y);
    void resetWorldView();
};

#endif