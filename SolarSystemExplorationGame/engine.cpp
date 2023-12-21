#include "engine.h"
#include "glm/ext.hpp"

bool firstMouse = true;
float yaw = -90.0f;  // yaw is initialized to -90.0 degrees
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

Engine::Engine(const char* name, int width, int height) {
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;
}

Engine::~Engine() {
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize() {
    // Initialize window
    m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
    if (!m_window->Initialize()) {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Initialize graphics
    m_graphics = new Graphics();
    if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT)) {
        printf("The graphics failed to initialize.\n");
        return false;
    }

    glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);
    glfwSetScrollCallback(m_window->getWindow(), scroll_callback);

    return true;
}

void Engine::Run() {
    m_running = true;
    while (!glfwWindowShouldClose(m_window->getWindow())) {
        ProcessInput();
        Display(m_window->getWindow(), glfwGetTime());
        glfwPollEvents();
    }
    m_running = false;
}

void Engine::ProcessInput() {
    // Handle ESC key
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);
    
    // Toggle camera modes with spacebar
    static bool spaceKeyWasPressed = false;
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spaceKeyWasPressed) {
            m_graphics->getCamera()->ToggleMode();
            spaceKeyWasPressed = true;
        }
    }
    else {
        spaceKeyWasPressed = false;
    }

    // Handle camera movements
    double xpos, ypos;
    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(m_window->getWindow(), &xpos, &ypos);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constraining the pitch values
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::vec3 cameraFront = glm::normalize(front);

    // Update camera view
    m_graphics->getCamera()->UpdateView(cameraFront);
    m_graphics->getCamera()->Zoom(fov);

    // Handle camera position changes
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_graphics->getCamera()->MoveHorizontal(0.5f);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_graphics->getCamera()->MoveHorizontal(-0.5f);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_graphics->getCamera()->MoveVertical(0.5f);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_graphics->getCamera()->MoveVertical(-0.5f);

    // Add additional input processing here as needed
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}

unsigned int Engine::getDT() {
    // Time delta calculation (if needed)
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis() {
    // Time in milliseconds (if needed)
    return (long long)glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {
    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    // Cursor position callback logic (if needed)
}