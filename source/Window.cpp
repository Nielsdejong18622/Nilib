//Own headers
#include "Window.hpp"

Window::Window(size_t width, size_t height, char const *title, bool eventbased=true) {
    /* Initialize the glfw library */
    if (!glfwInit()) 
        throw std::runtime_error("Failed to initialize the glfw libary!");
    Log::debug("Initialized GLFW library.\n");

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_DECORATED, true);

    // Create the window.
    d_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!d_window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to open the window!");
    }
    
    // Set up the window in preparation of rendering.
    glfwMakeContextCurrent(d_window);
    glfwSwapInterval(1);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glDisable(GL_LINE_SMOOTH);
    glLineWidth(4.0);
    glOrtho(0.0, width, 0.0, height, 0.0, 1.0); 
    glEnable(GL_COLOR_MATERIAL);
    
    Log::debug("Created window context.\n");

    d_windowData.width = width;
    d_windowData.height = height;
    d_windowData.owner = this;
    d_windowData.eventbased = eventbased;

    glfwSetKeyCallback(d_window, Window::key_callback);
    glfwSetFramebufferSizeCallback(d_window, Window::framebuffer_size_callback);

    glfwSetWindowUserPointer(d_window, this);
    Log::debug("Completed window construction.\n");
};


Window::~Window() {
    glfwDestroyWindow(this->d_window);
    glfwTerminate();
    Log::debug("Window destroyed!\n");
};

void Window::startScene() {
    // Render parameters.
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(GLclampf(0.9), GLclampf(0.9), GLclampf(0.9), GLclampf(0.9));
};

void Window::endScene() {
    glfwSwapBuffers(this->d_window);
    if (!d_windowData.eventbased)
        updateidletasks();
    else
        update();
};

/// @brief Poll the event queue. 
void Window::updateidletasks() {
    glfwWaitEvents();
};

void Window::update() {
    glfwPollEvents();
};

void Window::close() {
    glfwSetWindowShouldClose(this->d_window, 1);
    Log::debug("Closed window.\n");
};

void Window::open() {
    glfwSetWindowShouldClose(this->d_window, 0);
    Log::debug("Opened window.\n");
};

bool Window::closed() const {
    return glfwWindowShouldClose(this->d_window);
};
bool Window::opened() const {
    return !glfwWindowShouldClose(this->d_window);
};


void Window::drawColor(Color const &col) const
{
    glColor3f(col.R, col.G, col.B);
}

void Window::drawArc(float const x1, float const y1, float const x2, float const y2, float const linewidth) const
{
    // Line segment from this to adjacent. 
    glLineWidth(linewidth);
    glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0f);
	glVertex3f(x2, y2, 0.0f);
    glEnd();
};

// Draw shapes.
void Window::drawSquare(float const x, float const y, float size) const {
    glBegin(GL_QUADS);
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


void Window::drawCircle(float const x, float const y, float const radius) const {
    glBegin(GL_TRIANGLE_FAN);
    int arc = 0;
    int sides = 12;
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

void Window::drawCross(float const x, float const y, float const size) const {
    glBegin(GL_LINES);
	glVertex3f(x - size, y - size, 0.0f);
	glVertex3f(x + size, y + size, 0.0f);
	glVertex3f(x - size, y + size, 0.0f);
	glVertex3f(x + size, y - size, 0.0f);
    glEnd();
};

void Window::drawUpTriangle(float const x, float const y, float const size) const {
    glBegin(GL_TRIANGLES);
    glVertex3f(x       , y + size, 0.0f);   // Top
    glVertex3f(x + size, y - size, 0.0f);   // Bottom right
    glVertex3f(x - size, y - size, 0.0f);   // Bottom left
    glEnd();
};

void Window::drawDownTriangle(float const x, float const y, float const size) const {
    glBegin(GL_TRIANGLES);
    glVertex3f(x       , y - size, 0.0f);   // Top
    glVertex3f(x + size, y + size, 0.0f);   // Bottom right
    glVertex3f(x - size, y + size, 0.0f);   // Bottom left
    glEnd();
};

void Window::drawCubicBezier(float const x1, float const y1, float const x2, float const y2, float const x3, float const y3, float const x4, float const y4) const {
    GLfloat ctrlpoints[4][3]
        = { { x1, y1, 0.0 },
            { x2, y2, 0.0 },
            { x3, y3, 0.0 },
            { x4, y4, 0.0 } };
    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4,
            &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
 
    // Fill the color
    glBegin(GL_LINE_STRIP);
    // Find the coordinates
    for (int i = 0; i <= 30; i++)
        glEvalCoord1f((GLfloat)i / 30.0);
    glEnd();
}

void Window::framebuffer_size_callback(GLFWwindow*, int width, int height){ 
    Log::debug("Resizing framebuffer new dimensions ") << width << 'x' << height << '\n';
    glViewport(0, 0, width, height);
};
  
void Window::setTitle(char const *title) const{
    glfwSetWindowTitle(d_window, title);
}

void Window::bindkey(Callback const &bindfun, int key, int action, int mods)
{
    // Convert scancode to non-platform specific.   
    int scancode = glfwGetKeyScancode(key);
    Log::info("Binding key ") << key << " bindfun: " << &bindfun << " scancode " << scancode << " mods " << mods << '\n';
    KeyEvent keyevent = {key, scancode, action, mods};
    d_windowData.keybindings[keyevent] = bindfun;
}

// Required function by the glfw render library. Handles user key input. 
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Get the window from the glfw state machine. 
    Window* win_ptr = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    std::vector<char const *> const actionnames = {"release", "press", "hold"};

    Log::debug("Key ") << key << " action " << actionnames[action] << " scancode " << scancode << " mods " << mods << '\n';

    std::map<KeyEvent, Callback> keymap = win_ptr->d_windowData.keybindings;
    KeyEvent keyev = {key, scancode, action, mods};

    if (keymap.find(keyev) != keymap.end()) 
        win_ptr->d_windowData.keybindings.at(keyev)();
};

