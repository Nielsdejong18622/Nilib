//Own headers
#include "Nilib/Renderer/Window.hpp"

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
    if (s_windowsactive == 0) {
        int major, minor, rev;
        glfwGetVersion(&major, &minor, &rev);
        LOG_INFO() << "Initialized GLFW version " << major << '.' <<  minor << '.' << rev << ".\n";
        
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
    return *reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
}

char const *Window::title() const
{
    return d_data.title; 
}

void Window::title(char const *title) const{
    glfwSetWindowTitle(d_window, title);
}

void Window::setCallbacks() const
{
    // Callbacks. 
    //glfwSetCharCallback()
    //glfwSetCharModsCallback();
    //glfwSetJoystickCallback();
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

void Window::framebuffer_size_callback(GLFWwindow*, int width, int height){ 
    LOG_DEBUG("[WEVENT] Resizing framebuffer ", width, 'x', height);
    //glViewport(0, 0, width, height);
};

void Window::drop_callback(GLFWwindow* window, int path_count, const char* paths[])
{
    LOG_INFO("[WEVENT] Window", window, "dropping", path_count, "files");
    for (int path = 0; path < path_count; ++path)
        LOG_DEBUG(paths[path]);    
};
void Window::error_callback(int code, char const*description) {
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

void Window::cursorPos_callback(GLFWwindow* window, double xpos, double ypos)
{
    LOG_DEBUG("[WEVENT] Window", windowFromPtr(window).title(), "cursurpos", xpos, ypos);
}

void Window::open() 
{
    // If already open.
    if (!glfwWindowShouldClose(d_window)) {
        LOG_WARNING("Window", title(), "already open!");
        return;
    }

    // If window already exists but is hidden. 
    glfwShowWindow(d_window);
    glfwSetWindowShouldClose(d_window, GLFW_FALSE);
    LOG_DEBUG("Showing window", title());
};



Window::~Window() {
    LOG_DEBUG("Window", title(), "destruction!");
    glfwDestroyWindow(d_window);
    d_window = nullptr;
    if (--Window::s_windowsactive == 0) {
        glfwTerminate();
        LOG_DEBUG("Destruction of GLFW library");
    }
};

void Window::clearColor(Color const &color)
{
    LOG_DEBUG("Setting clear color:", color);
    d_data.clearColor = color;
}
void Window::requestAttention() const {
    ASSERT(d_window, "Window not init!");
    glfwRequestWindowAttention(d_window);
}

void Window::startScene() {
    if (!d_window || glfwWindowShouldClose(d_window)) {
        LOG_WARNING("Starting scene on unopened Window", title());
        return;
    }
    // Render parameters.
    glfwMakeContextCurrent(d_window);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("GLAD initialization failed!");
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapInterval(1);

    float R, G, B, A;
    d_data.clearColor.RGBAf(&R, &G, &B, &A);
    glClearColor(R, G, B, A);
};

void Window::endScene() {
    glfwSwapBuffers(d_window);
};

void Window::updateidletasks() {
    glfwWaitEvents();
};

void Window::update() {
    glfwPollEvents();
};

void Window::close() {
    //if (glfwWindowShouldClose(d_window)) return;
    glfwSetWindowShouldClose(d_window, GLFW_TRUE);
    glfwHideWindow(d_window);
    LOG_DEBUG() << "Closed window.\n";
};


bool Window::opened() const {
    return !glfwWindowShouldClose(d_window);
};


void Window::bindkey(Callback const &bindfun, int key, int action, int mods)
{
    // Convert scancode to non-platform specific.   
    int scancode = glfwGetKeyScancode(key);
    LOG_DEBUG("[WEVENT] Binding key", key, "scancode", scancode, "mods", mods);
    KeyEvent keyevent = {key, scancode, action, mods};
    d_data.keybindings[keyevent] = bindfun;
}


// Required function by the glfw render library. Handles user key input. 
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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


void Window::drawArc(float const x1, float const y1, float const x2, float const y2, float const linewidth) const
{
    
};