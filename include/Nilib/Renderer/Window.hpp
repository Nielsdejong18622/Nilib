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

// In this order!
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Nilib/Logger/Log.hpp"
#include "Nilib/Renderer/Color.h"
#include "Nilib/Renderer/Keys.h"
#include "Nilib/Math/Matrix.hpp"


namespace Nilib {


// Typedefs. 
typedef std::function<void(void)> Callback;
typedef std::tuple<int, int, int, int> KeyEvent;

class Window;

struct Windowdata {
    int width = 800;
    int height = 800;
    int screenposx = 0;
    int screenposy = 0;
    //size_t framebufx;
    //size_t framebufy;
    int minwidth = 0; 
    int minheight = 0;
    Color clearColor;
    char const *title;
    Window* owner = nullptr;
    std::map<KeyEvent, Callback> keybindings;
};

class Window
{
public:

    Window(size_t width, size_t height, char const *title);
    Window(size_t width, size_t height, char const *title, size_t minwidth, size_t minheight, bool , bool decorated, bool resizeable);
    ~Window();
	
    // Set Windowtitle.
    void title(char const *title) const;
    char const* title() const;

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
    void bindkey(Callback const &fun, int key, int action, int mods);
    void bindkey(Callback const &bindfun, int key);

protected:

    GLFWwindow *d_window = nullptr;
	GLFWmonitor* d_monitor = nullptr;
	static size_t s_windowsactive;

	Windowdata d_data;

    void setCallbacks() const;
    void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void static drop_callback(GLFWwindow* window, int path_count, const char* paths[]);
    void static cursorPos_callback(GLFWwindow* window, double xpos, double ypos);
    void static framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void static windowPos_callback(GLFWwindow *window, int xpos, int ypos);
    void static windowSize_callback(GLFWwindow *window, int width, int height);
    void static mouseButton_callback(GLFWwindow *window, int button, int action, int mods);
    void static windowFocus_callback(GLFWwindow *window, int focused);
    void static windowMaximize_callback(GLFWwindow *window, int maximized);
    void static windowContentScale_callback(GLFWwindow *window, float xscale, float yscale);
    void static windowIconify_callback(GLFWwindow *window, int iconified);
    void static cursorEnter_callback(GLFWwindow *window, int entered);
    void static scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void static error_callback(int code, char const*description);
    void static windowClose_callback(GLFWwindow *window);

    // Utility function.
    static Window &windowFromPtr(GLFWwindow *window);
public:

    // Poll and handle events and block the calling thread. 
    void static updateidletasks();
    
    // Poll and handle events.
    void static update();

    void clearColor(Color const &color);

    // Draw functions.
    void drawArc(float const x1, float const y1, float const x2, float const y2, float const linewidth) const;
};

} // Nilib;
#endif