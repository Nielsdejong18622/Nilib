// Own headers
#include "Nilib/Renderer/Window.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>


using namespace Nilib;

size_t Window::s_windowsactive = 0;

Window::Window(size_t width,
               size_t height,
               char const *title,
               size_t minwidth,
               size_t minheight,
               bool fullscreen,
               bool decorated,
               bool resizeable)
{
    /* Initialize the glfw library */
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize the GLFW libary!");

    // If this is the first active window.
    if (s_windowsactive == 0)
    {
        int major, minor, rev;
        glfwGetVersion(&major, &minor, &rev);
        LOG_INFO() << "Initialized GLFW version " << major << '.' << minor << '.' << rev << ".\n";

        glfwSetErrorCallback(Window::error_callback);
    }

    glfwWindowHint(GLFW_VISIBLE, false); // By default start hidden.
    glfwWindowHint(GLFW_RESIZABLE, resizeable);
    glfwWindowHint(GLFW_DECORATED, decorated);
    glfwWindowHint(GLFW_MAXIMIZED, fullscreen);
    glfwWindowHint(GLFW_CENTER_CURSOR, fullscreen);
    d_data.width = width;
    d_data.height = height;
    d_data.minheight = minheight;
    d_data.minwidth = minwidth;
    d_data.title = title;
    d_data.owner = this;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    GLFWvidmode const *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_SAMPLES, 4); // Enable Multisampling.
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    if (fullscreen)
        d_window = glfwCreateWindow(mode->width, mode->height, d_data.title, monitor, nullptr);
    else
        d_window = glfwCreateWindow(d_data.width, d_data.height, d_data.title, nullptr, nullptr);
    // Window creation failed.
    if (!d_window)
    {
        glfwDestroyWindow(d_window);
        throw std::runtime_error("Failed to create Window!");
    }
    glfwSetWindowShouldClose(d_window, GLFW_TRUE);
    glfwSetWindowUserPointer(d_window, this);
    setCallbacks();

    // Render parameters.
    glfwMakeContextCurrent(d_window);

    Window::s_windowsactive++;
    LOG_DEBUG("Constructed Window", title, width, height, minwidth, minheight, fullscreen, decorated, resizeable);
}

Window::Window(size_t width, size_t height, char const *title)
    : Window(width, height, title, width, height, false, true, true)
{
}

// Utility function.
Window &Window::windowFromPtr(GLFWwindow *window)
{
    return *reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
}

char const *Window::title() const
{
    return d_data.title;
}

void Window::title(char const *title) const
{
    glfwSetWindowTitle(d_window, title);
}

void Window::setCallbacks() const
{
    // Callbacks.
    // glfwSetCharCallback()
    // glfwSetCharModsCallback();
    // glfwSetJoystickCallback();
    glfwSetKeyCallback(d_window, Window::key_callback);
    glfwSetDropCallback(d_window, Window::drop_callback);
    glfwSetScrollCallback(d_window, Window::scroll_callback);
    glfwSetCursorPosCallback(d_window, Window::cursorPos_callback);
    glfwSetWindowPosCallback(d_window, Window::windowPos_callback);
    glfwSetWindowSizeCallback(d_window, Window::windowSize_callback);
    glfwSetCursorEnterCallback(d_window, Window::cursorEnter_callback);
    glfwSetMouseButtonCallback(d_window, Window::mouseButton_callback);
    glfwSetWindowFocusCallback(d_window, Window::windowFocus_callback);
    glfwSetWindowCloseCallback(d_window, Window::windowClose_callback);
    glfwSetWindowIconifyCallback(d_window, Window::windowIconify_callback);
    glfwSetWindowMaximizeCallback(d_window, Window::windowMaximize_callback);
    glfwSetFramebufferSizeCallback(d_window, Window::framebuffer_size_callback);
    glfwSetWindowContentScaleCallback(d_window, Window::windowContentScale_callback);
}

void Window::framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    LOG_DEBUG("[WEVENT] Resizing framebuffer ", width, 'x', height);
    glViewport(0, 0, width, height);
};

void Window::drop_callback(GLFWwindow *window, int path_count, const char *paths[])
{
    LOG_INFO("[WEVENT] Window", window, "dropping", path_count, "files");
    for (int path = 0; path < path_count; ++path)
        LOG_DEBUG(paths[path]);
};
void Window::error_callback(int code, char const *description)
{
    LOG_ERROR("[WEVENT] Error ", code, description);
}
void Window::windowClose_callback(GLFWwindow *window)
{
    LOG_DEBUG("[WEVENT] Closing window", windowFromPtr(window).title());
    windowFromPtr(window).close();
}

void Window::windowPos_callback(GLFWwindow *window, int xpos, int ypos)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "position:", xpos, ypos);
}

void Window::windowSize_callback(GLFWwindow *window, int width, int height)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "(re)size:", width, height);
}

void Window::mouseButton_callback(GLFWwindow *window, int button, int action, int mods)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "mouse button:", button, "action:", action, "mods:", mods);
}

void Window::windowFocus_callback(GLFWwindow *window, int focused)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "focus:", focused);
}

void Window::windowIconify_callback(GLFWwindow *window, int iconified)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "iconified:", iconified);
}

void Window::windowMaximize_callback(GLFWwindow *window, int maximized)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "maximized:", maximized);
}

void Window::windowContentScale_callback(GLFWwindow *window, float xscale, float yscale)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "content scale:", xscale, yscale);
}
void Window::cursorEnter_callback(GLFWwindow *window, int entered)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "cursor enter", entered);
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "scroll:", xoffset, yoffset);
}

void Window::cursorPos_callback(GLFWwindow *window, double xpos, double ypos)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "cursurpos", xpos, ypos);
}

void Window::open()
{
    CORE_ASSERT(d_window);
    // If already open.
    if (!glfwWindowShouldClose(d_window))
    {
        LOG_WARNING("Window", title(), "already open!");
        return;
    }

    // If window already exists but is hidden.
    glfwSetWindowShouldClose(d_window, GLFW_FALSE);
    glfwShowWindow(d_window);
    LOG_DEBUG("Showing window", title());
};

Window::~Window()
{
    CORE_ASSERT(d_window);
    glfwDestroyWindow(d_window);
    LOG_DEBUG("Window", title(), "destructed!");
    d_window = nullptr;
    if (--Window::s_windowsactive == 0)
    {
        glfwTerminate();
        LOG_DEBUG("Destructed GLFW library");
    }
};

void Window::requestAttention() const
{
    ASSERT(d_window, "Window not init!");
    glfwRequestWindowAttention(d_window);
}

void Window::linewidth(float const lw) const
{
    glLineWidth(lw);
}

void Window::startScene()
{
    if (!d_window || glfwWindowShouldClose(d_window))
    {
        LOG_WARNING("Starting scene on unopened Window", title());
        return;
    }
    glfwMakeContextCurrent(d_window);
    glfwSwapInterval(1);

    // // Renderer.
    float R, G, B, A;
    d_data.clearColor.RGBAf(&R, &G, &B, &A);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(R, G, B, A);
};

void Window::endScene()
{
    glfwSwapBuffers(d_window);
};

void Window::updateidletasks()
{
    glfwWaitEvents();
};

void Window::update()
{
    glfwPollEvents();
};

void Window::screenshotPNG(char const *filename)
{
    int width = d_data.width;
    int height = d_data.height;
    LOG_PROGRESS("Saving Framebuffer screenshot to file", filename, "dimensions", width, 'x', height);
    // Step 1: Create a buffer to store pixel data
    unsigned char* pixels = new unsigned char[3 * width * height];

    // Step 2: Read the pixels from the framebuffer
    glfwSwapBuffers(d_window);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Step 3: Flip the image vertically (OpenGL's origin is bottom-left)
    unsigned char* flippedPixels = new unsigned char[3 * width * height];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIdx = (y * width + x) * 3;
            int dstIdx = ((height - y - 1) * width + x) * 3;
            flippedPixels[dstIdx] = pixels[srcIdx];       // R
            flippedPixels[dstIdx + 1] = pixels[srcIdx + 1]; // G
            flippedPixels[dstIdx + 2] = pixels[srcIdx + 2]; // B
        }
    }

    // Step 4: Save the flipped pixel data to a file as PNG
    stbi_write_png(filename, width, height, 3, flippedPixels, width * 3);

    // Step 5: Cleanup
    delete[] pixels;
    delete[] flippedPixels;
}

void Window::close()
{
    // if (glfwWindowShouldClose(d_window)) return;
    glfwSetWindowShouldClose(d_window, GLFW_TRUE);
    glfwHideWindow(d_window);
    LOG_DEBUG() << "Closed window.\n";
};

bool Window::opened() const
{
    return !glfwWindowShouldClose(d_window);
};

// Default bindkey for Key::Press / No modifications.
void Window::bindkey(Callback const &bindfun, int key)
{
    bindkey(bindfun, key, Key::Press, 0);
}

void Window::bindkey(Callback const &bindfun, int key, int action, int mods)
{
    if (!glfwInit())
    {
        LOG_ERROR("Binding key while GLFW is not initialized!");
        return;
    }
    // Convert scancode to non-platform specific.
    int scancode = glfwGetKeyScancode(key);
    LOG_DEBUG("[WEVENT] Binding key", key, "scancode", scancode, "mods", mods);
    KeyEvent keyevent = {key, scancode, action, mods};
    d_data.keybindings[keyevent] = bindfun;
}

// Required function by the glfw render library. Handles user key input.
void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Window &win = Window::windowFromPtr(window);
    std::array<char const *, 3> const actionnames = {"release", "press", "hold"};
    LOG_DEBUG("[WEVENT] Window", win.title(), "key", key, "action", actionnames[action], "scancode", scancode, "mods", mods);

    // Look in the keymap for the key Callback and call it.
    KeyEvent keyev = {key, scancode, action, mods};
    std::map<KeyEvent, Callback> keymap = win.d_data.keybindings;
    if (keymap.find(keyev) != keymap.end())
        win.d_data.keybindings.at(keyev)();
};

void Window::drawArc(Vec2f const &A, Vec2f const &B, float const linewidth) const
{
    // Line segment from this to adjacent.
    glLineWidth(linewidth);
    glBegin(GL_LINES);
    float ax = A.x();
    float ay = A.y();
    float bx = B.x();
    float by = B.y();

    transform2D(ax, ay);
    transform2D(bx, by);
    glVertex3f(ax, ay, 0.0f);
    glVertex3f(bx, by, 0.0f);
    glEnd();
}

void Window::drawCircle(Vec2f const &centre, float const radius, float const linewidth, unsigned int sides) const
{
    float arc = 0.0f;
    // Start on the right.
    float cx = centre.x();
    float cy = centre.y();
    float arcx = centre.x() + radius;
    float arcy = centre.y();

    glBegin(GL_LINES);
    while (arc <= 360.0f)
    {
        float warcx = arcx;
        float warcy = arcy;
        transform2D(warcx, warcy);
        glVertex3f(warcx, warcy, 0.0f);
        arc += 360.0f / sides;
        arcx = cx + radius * cos(arc * 3.14159265f / 180.0f);
        arcy = cy + radius * sin(arc * 3.14159265f / 180.0f);
        float nwarcx = arcx;
        float nwarcy = arcy;
        transform2D(nwarcx, nwarcy);
        glVertex3f(nwarcx, nwarcy, 0.0f);
    }
    glEnd();
}

void Window::drawFilledCircle(Vec2f const &centre, float const radius, float const linewidth, unsigned int sides) const
{
    float arc = 0.0f;
    // Start on the right.
    float cx = centre.x();
    float cy = centre.y();
    float arcx = centre.x() + radius;
    float arcy = centre.y();

    glBegin(GL_TRIANGLE_FAN);
    while (arc <= 360.0f)
    {
        float warcx = arcx;
        float warcy = arcy;
        transform2D(warcx, warcy);
        glVertex3f(warcx, warcy, 0.0f);
        arc += 360.0f / sides;
        arcx = cx + radius * cos(arc * 3.14159265f / 180.0f);
        arcy = cy + radius * sin(arc * 3.14159265f / 180.0f);
        float nwarcx = arcx;
        float nwarcy = arcy;
        transform2D(nwarcx, nwarcy);
        glVertex3f(nwarcx, nwarcy, 0.0f);
    }
    glEnd();
}

void Window::drawDiamond(Vec2f const &center, float const radius, float const linewidth) const
{
    return drawCircle(center, radius, linewidth, 4);
}

void Window::drawPole(Vec2f const &centre, float const size) const
{
    glBegin(GL_LINES);

    float lx = centre.x() - 1 * size;
    float ly = centre.y();
    float rx = centre.x() + 1 * size;
    float ry = centre.y();

    float cx = centre.x();
    float cy = centre.y();

    float tx = centre.x();
    float ty = centre.y() + 2 * size;

    transform2D(lx, ly);
    transform2D(rx, ry);
    transform2D(cx, cy);
    transform2D(tx, ty);

    glVertex3f(rx, ry, 0.0f);
    glVertex3f(lx, ly, 0.0f);

    glVertex3f(cx, cy, 0.0f);
    glVertex3f(tx, ty, 0.0f);

    glEnd();
}

void Window::drawTriangleUp(Vec2f const &centre, float const size) const
{
    glBegin(GL_TRIANGLES);
    float ax = centre.x();
    float ay = centre.y() + size;
    float bx = centre.x() + size;
    float by = centre.y() - size;
    float cx = centre.x() - size;
    float cy = centre.y() - size;
    transform2D(ax, ay);
    transform2D(bx, by);
    transform2D(cx, cy);

    glVertex3f(ax, ay, 0.0f); // Top
    glVertex3f(bx, by, 0.0f); // Bottom right
    glVertex3f(cx, cy, 0.0f); // Bottom left
    glEnd();
}

void Window::drawSquare(Vec2f const &centre, float const size) const
{
    glBegin(GL_QUADS);
    float a = centre.x() - size;
    float c = centre.x() + size;
    float b = centre.y() + size;
    float d = centre.y() - size;
    transform2D(a, b);
    transform2D(c, d);
    glVertex3f(a, b, 0.0f);
    glVertex3f(c, b, 0.0f);
    glVertex3f(c, d, 0.0f);
    glVertex3f(a, d, 0.0f);
    glEnd();
}

void Window::drawRectangle(Vec2f const &leftup, Vec2f const &rightdown) const
{
    CORE_ASSERT("Not yet implemented!");
}

void Window::drawCross(Vec2f const &centre, float const size) const
{
    glBegin(GL_LINES);
    float a = centre.x() - size;
    float c = centre.x() + size;
    float b = centre.y() + size;
    float d = centre.y() - size;
    transform2D(a, b);
    transform2D(c, d);
    glVertex3f(a, d, 0.0f);
    glVertex3f(c, b, 0.0f);
    glVertex3f(a, b, 0.0f);
    glVertex3f(c, d, 0.0f);
    glEnd();
};

void Window::drawTriangleDown(Vec2f const &centre, float const size) const
{
    glBegin(GL_TRIANGLES);
    float ax = centre.x();
    float ay = centre.y() + size;
    float bx = centre.x() + size;
    float by = centre.y() - size;
    float cx = centre.x() - size;
    float cy = centre.y() - size;
    transform2D(ax, ay);
    transform2D(bx, by);
    transform2D(cx, cy);

    glVertex3f(ax, by, 0.0f); // Top
    glVertex3f(bx, ay, 0.0f); // Bottom right
    glVertex3f(cx, ay, 0.0f); // Bottom left
    glEnd();
}

// For immediate mode drawing.
void Window::color(Color const &color)
{
    float r, g, b, a;
    color.RGBAf(&r, &g, &b, &a);
    glColor4f(r, g, b, a);
}

void Window::clearColor(Color const &color)
{
    d_data.clearColor = color;
    LOG_DEBUG("Set clear color:", color);
}

void Window::transform2D(float &x, float &y) const
{
    x = (x - d_data.xmin) * 2.0f / (d_data.xmax - d_data.xmin) - 1.0f;
    y = (y - d_data.ymin) * 2.0f / (d_data.ymax - d_data.ymin) - 1.0f;
}

void Window::setXlim(float const xmin, float const xmax)
{
    d_data.xmin = xmin;
    d_data.xmax = xmax;
}

void Window::setYlim(float const ymin, float const ymax)
{
    d_data.ymin = ymin;
    d_data.ymax = ymax;
}