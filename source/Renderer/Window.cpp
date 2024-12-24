//Own headers
#include "Nilib/Renderer/Window.hpp"

Window::Window(size_t width, size_t height, char const *title) {
    /* Initialize the glfw library */
    if (!glfwInit()) 
        throw std::runtime_error("Failed to initialize the glfw libary!");
    LOG_DEBUG("Constructing window", title, width, 'x', height);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_DECORATED, true);

    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &d_data.screenposx, &d_data.screenposy, &d_data.width, &d_data.height);
    d_data.width = width;
    d_data.height = height;
    d_data.title = title;
    d_data.open = false;
    d_data.current = false;
    d_data.owner = this;

    glfwSwapInterval(1);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(2.0);
    glOrtho(0.0, width, 0.0, height, 0.0, 1.0); 
    
    // Enable Alpha blending. 
    glBlendFunc  (GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glEnable     (GL_BLEND);
    glEnable     (GL_COLOR_MATERIAL);
    
    LOG_DEBUG() << "Created window context.\n";


    LOG_DEBUG() << "Completed window construction.\n";
}

void Window::setCallbacks() const
{
    // Callbacks. 
    glfwSetKeyCallback(d_window, Window::key_callback);
    glfwSetFramebufferSizeCallback(d_window, Window::framebuffer_size_callback);
    
    //glfwSetCharCallback()
    //glfwSetCharModsCallback();
    glfwSetDropCallback();
    glfwSetErrorCallback(Window::error_callback);
    glfwSetScrollCallback()
    glfwSetJoystickCallback();
    glfwSetCursorPosCallback();
    glfwSetCursorEnterCallback();
    glfwSetWindowPosCallback();
    glfwSetWindowSizeCallback();
    glfwSetCursorEnterCallback();
    
    glfwSetMouseButtonCallback();
    glfwSetWindowFocusCallback();
    glfwSetWindowIconifyCallback();
    glfwSetWindowMaximizeCallback();
    glfwSetWindowContentScaleCallback();

}

void Window::open() {
    if (d_data.open) return;

    d_window = glfwCreateWindow(d_data.width, d_data.height, d_data.title, nullptr, nullptr);
    
    glfwMakeContextCurrent(d_window);
    glfwSetWindowUserPointer(d_window, this);

    // Create the window.
    if (!d_window)
    {
        glfwDestroyWindow(d_window);
        d_data.open = false;
        return;
    }
    // Set up the window in preparation of rendering.
    //glfwMakeContextCurrent(d_window);
    LOG_DEBUG("Opened window", d_data.title);
    d_data.open = true;
};



Window::~Window() {
    glfwDestroyWindow(this->d_window);
    glfwTerminate();
    LOG_DEBUG() << "Window destroyed!\n";
};

void Window::startScene() {
    // Render parameters.
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(GLclampf(0.9), GLclampf(0.9), GLclampf(0.9), GLclampf(0.9));
};

void Window::endScene() {
    glfwSwapBuffers(this->d_window);
};

void Window::updateidletasks() {
    glfwWaitEvents();
};

void Window::update() {
    glfwPollEvents();
};

void Window::close() {
    glfwSetWindowShouldClose(this->d_window, 1);
    LOG_DEBUG() << "Closed window.\n";
};


bool Window::opened() const {
    return d_data.open;
};

void Window::framebuffer_size_callback(GLFWwindow*, int width, int height){ 
    LOG_DEBUG() << "Resizing framebuffer new dimensions " << width << 'x' << height << '\n';
    glViewport(0, 0, width, height);
};
  
void Window::setTitle(char const *title) const{
    glfwSetWindowTitle(d_window, title);
}

void Window::bindkey(Callback const &bindfun, int key, int action, int mods)
{
    // Convert scancode to non-platform specific.   
    int scancode = glfwGetKeyScancode(key);
    LOG_DEBUG() << "Binding key " << key << " bindfun: " << &bindfun << " scancode " << scancode << " mods " << mods << '\n';
    KeyEvent keyevent = {key, scancode, action, mods};
    d_data.keybindings[keyevent] = bindfun;
}

void Window::error_callback(int code, char const*description) {
    LOG_ERROR("GLFW Error callback", code, description);
}
// Required function by the glfw render library. Handles user key input. 
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Get the window from the glfw state machine. 
    Window* win_ptr = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    std::vector<char const *> const actionnames = {"release", "press", "hold"};

    //LOG_DEBUG() << "Key " << key << " action " << actionnames[action] << " scancode " << scancode << " mods " << mods << '\n';

    std::map<KeyEvent, Callback> keymap = win_ptr->d_data.keybindings;
    KeyEvent keyev = {key, scancode, action, mods};

    // Translate the view frustum on WASD.
    //float constexpr decrease = 0.9999;
    float constexpr shift = 0.01;
    //float constexpr increase = 1 / decrease; //1.0001;
    if (key == GLFW_KEY_A) {
        //win_ptr->d_windowData.Fmin.x() -= shift / win_ptr->d_windowData.zoomlevel; 
        //win_ptr->d_windowData.Fmax.x() -= shift / win_ptr->d_windowData.zoomlevel;
    }
    if (key == GLFW_KEY_D) {
        //win_ptr->d_windowData.Fmin.x() += shift / win_ptr->d_windowData.zoomlevel;
        //win_ptr->d_windowData.Fmax.x() += shift / win_ptr->d_windowData.zoomlevel;
    }
    if (key == GLFW_KEY_W) {
        //win_ptr->d_windowData.Fmin.y() += shift / win_ptr->d_windowData.zoomlevel; 
        //win_ptr->d_windowData.Fmax.y() += shift / win_ptr->d_windowData.zoomlevel;
    }
    if (key == GLFW_KEY_S) {
        //win_ptr->d_windowData.Fmin.y() -= shift / win_ptr->d_windowData.zoomlevel;
        //win_ptr->d_windowData.Fmax.y() -= shift / win_ptr->d_windowData.zoomlevel;
    }
    // Zoom on X-axis or Y-axis.
    if (key == GLFW_KEY_MINUS) {
        //auto c_size = win_ptr->d_windowData.Fmax - win_ptr->d_windowData.Fmin;
        //win_ptr->d_windowData.zoomlevel -= shift;
        //LOG_DEBUG() << "Current width:" << c_size.x() << " current height:" << c_size.y() << " current zoom: " << win_ptr->d_windowData.zoomlevel << '\n';
        
        //win_ptr->d_windowData.Fmin -= 0.5 * c_size * 0.01;
        //win_ptr->d_windowData.Fmax += 0.5 * c_size * 0.01;
    }
    if (key == GLFW_KEY_EQUAL) {      
        //auto c_size =  win_ptr->d_windowData.Fmax - win_ptr->d_windowData.Fmin; 
        // auto cw = std::max(win_ptr->d_windowData.f2X - win_ptr->d_windowData.f1X, 0.0f);
        // auto ch = std::max(win_ptr->d_windowData.f2Y - win_ptr->d_windowData.f1Y, 0.0f);
        //LOG_DEBUG() << "Current width:" << c_size.x() << " current height:" << c_size.y() << " current zoom: " << win_ptr->d_windowData.zoomlevel << '\n';
        //win_ptr->d_windowData.Fmin += 0.5 * c_size * (0.01);
        //win_ptr->d_windowData.Fmax -= 0.5 * c_size * (0.01);
        
        //win_ptr->d_windowData.zoomlevel += shift;     
    }
    if (keymap.find(keyev) != keymap.end()) 
        win_ptr->d_data.keybindings.at(keyev)();
    
};


void Window::xlim(float const min, float const max)
{
    //d_windowData.f1X = min;
    //d_windowData.f2X = max;

    //d_windowData.Fmin.x() = min;
    //d_windowData.Fmax.x() = max;
}

void Window::ylim(float const min, float const max)
{
    //d_windowData.f1Y = min;
    //d_windowData.f2Y = max;
    
    //d_windowData.Fmin.y() = min;
    //d_windowData.Fmax.y() = max;
}


/*
Vec2D<float> const Window::transform2d(Vec2D<float> const x) const
{
    auto const size = d_windowData.Fmax - d_windowData.Fmin; 
    Mat2x2 const transform = Mat2x2<float>({static_cast<float>(d_windowData.width), 0, 
                                            0, static_cast<float>(d_windowData.height)});
                                            
    Mat2x2 const transform2 = Mat2x2<float>({1 / size.x(), 0, 
                                            0, 1 / size.y()});
    return  transform2 * transform * (x - d_windowData.Fmin);

}

void Window::drawColor(Color const &col) const
{
    glColor4f(col.R, col.G, col.B, 0.2);
}

void Window::drawArc(Vec2D<float> const x1, Vec2D<float> const x2, float const linewidth) const
{
    auto x = transform2d(x1);
    auto y = transform2d(x2);
    // Line segment from this to adjacent. 
    glLineWidth(linewidth);
    glBegin(GL_LINES);
    
    glVertex2fv(x.get());
	glVertex2fv(y.get());
	glEnd();
};

// Draw shapes.
void Window::drawSquare(Vec2D<float> const x1, float const size) const {
    glBegin(GL_QUADS);
    auto x1p = transform2d(x1);

    auto x = x1p.x();
    auto y = x1p.y();
	glVertex3f(x - size, y - size, 0.0f);
	glVertex3f(x - size, y + size, 0.0f);
	// North
	glVertex3f(x - size, y + size, 0.0f);
	glVertex3f(x + size, y + size, 0.0f);
	// West
	glVertex3f(x + size, y + size, 0.0f);
	glVertex3f(x + size, y - size, 0.0f);
	// South
	glVertex3f(x + size, y - size, 0.0f);
	glVertex3f(x - size, y - size, 0.0f);
    glEnd();
};


void Window::drawCircle(Vec2D<float> const x1, float const radius) const {
    glBegin(GL_TRIANGLE_FAN);
    int arc = 0;
    int sides = 4;
    
    auto x11 = transform2d(x1);
    auto x = x11.x(), y = x11.y();

	float arcx = x + radius;
	float arcy = y;
	while (arc <= 360)
	{
		glVertex3f(arcx, arcy, 0.0f);
		arc += 360 / sides;
		arcx = x + radius * cos(arc * 3.14 / 180);
		arcy = y + radius * sin(arc * 3.14 / 180);
		glVertex3f(arcx, arcy, 0.0f);
	}
    glEnd();
};

void Window::drawCross(Vec2D<float> const x1, float const size) const {
    glBegin(GL_LINES);
    
    auto x11 = transform2d(x1);
    auto x = x11.x(), y = x11.y();
	glVertex3f(x - size, y - size, 0.0f);
	glVertex3f(x + size, y + size, 0.0f);
	glVertex3f(x - size, y + size, 0.0f);
	glVertex3f(x + size, y - size, 0.0f);
    glEnd();
};

void Window::drawUpTriangle(Vec2D<float> const x1, float const size) const {
    glBegin(GL_TRIANGLES);
    auto x11 = transform2d(x1);
    auto x = x11.x(), y = x11.y();
    glVertex3f(x       , y + size, 0.0f);   // Top
    glVertex3f(x + size, y - size, 0.0f);   // Bottom right
    glVertex3f(x - size, y - size, 0.0f);   // Bottom left
    glEnd();
};

void Window::drawDownTriangle(Vec2D<float> const x1, float const size) const {
    glBegin(GL_TRIANGLES);

    auto x11 = transform2d(x1);
    auto x = x11.x(), y = x11.y();
    glVertex3f(x       , y - size, 0.0f);   // Top
    glVertex3f(x + size, y + size, 0.0f);   // Bottom right
    glVertex3f(x - size, y + size, 0.0f);   // Bottom left
    glEnd();
};

void Window::drawCubicBezier(Vec2D<float> const , 
                             Vec2D<float> const , 
                             Vec2D<float> const ,
                             Vec2D<float> const ) const 
{
    ASSERT(false, "Not implemented")
}
*/
