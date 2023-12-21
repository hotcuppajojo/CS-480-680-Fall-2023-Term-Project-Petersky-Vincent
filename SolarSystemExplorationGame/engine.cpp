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
    // Handle ESC key for closing the window
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window->getWindow(), true);
    }

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

    // Handle camera movement
    Camera* camera = m_graphics->getCamera();
    float cameraSpeed = 0.05f; // adjust as necessary

    // WASD keys move the camera in both modes
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        camera->MoveForward(cameraSpeed);
    }
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        camera->MoveForward(-cameraSpeed);
    }
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        camera->MoveRight(-cameraSpeed);
    }
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        camera->MoveRight(cameraSpeed);
    }

    // Handle inputs based on the current camera mode
    CameraMode currentMode = m_graphics->getCamera()->GetMode();
    if (currentMode == CameraMode::Exploration) {
        // In Exploration mode, update the starship position and orientation based on camera controls
        UpdateStarshipFromCamera();
    }
    else {
        // In Planetary Observation mode, enable free look-around
        ProcessFreeLook();
    }

}

void Engine::UpdateStarshipFromCamera() {
    Starship* starship = m_graphics->getStarship();
    Camera* camera = m_graphics->getCamera();

    if (starship && camera) {
        // Here, we calculate the new position for the starship based on the camera's position and orientation
        glm::vec3 starshipPosition = camera->GetPosition() + camera->GetFront() * fixedOffset;

        // You can adjust the orientation vector as needed to keep the starship facing the right way
        glm::vec3 starshipOrientation = -camera->GetFront(); // This is a simple example; adjust as necessary

        // Update the starship's model matrix to reflect its new position and orientation
        starship->UpdateModelMatrix(starshipPosition, starshipOrientation);
    }
}

void Engine::ProcessFreeLook() {
    double xpos, ypos;
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

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::vec3 cameraFront = glm::normalize(front);

    m_graphics->getCamera()->UpdateView(cameraFront);
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

void Engine::ToggleCameraMode() {
    Camera* camera = m_graphics->getCamera();
    camera->ToggleMode();
    HandleCameraModeChange();
}

void Engine::HandleCameraModeChange() {
    CameraMode currentMode = m_graphics->getCamera()->GetMode();
    if (currentMode == CameraMode::Exploration) {
        ConfigureCameraForExploration();
    }
    else {
        ConfigureCameraForObservation();
    }
}

void Engine::ConfigureCameraForExploration() {
    // Assuming 'starship' is a pointer to your Starship object and has methods to get its position and orientation
    Starship* starship = m_graphics->getStarship();
    Camera* camera = m_graphics->getCamera();

    if (starship && camera) {
        // Determine the offset from the starship where the camera should be positioned
        glm::vec3 starshipPosition = starship->GetPosition();
        glm::vec3 starshipOrientation = starship->GetOrientation(); // Assume this returns a forward vector

        glm::vec3 cameraOffset = glm::vec3(0.0f, 5.0f, 10.0f); // Example offset: 5 units up and 10 units back
        glm::vec3 cameraPosition = starshipPosition - (starshipOrientation * cameraOffset.z) + glm::vec3(0, cameraOffset.y, 0);

        // Point the camera towards the starship
        glm::vec3 cameraLookAt = starshipPosition;

        // Update camera position and orientation
        camera->SetPosition(cameraPosition);
        camera->SetLookAt(cameraLookAt);
        camera->UpdateView();
    }
}

void Engine::ConfigureCameraForObservation() {
    Camera* camera = m_graphics->getCamera();
    Starship* starship = m_graphics->getStarship();

    if (camera && starship) {
        // Position the camera at a fixed distance in front of the starship
        glm::vec3 starshipPosition = starship->GetPosition();
        glm::vec3 starshipForward = starship->GetForwardVector(); // Method to get the forward vector of the starship
        float observationDistance = 50.0f; // Distance in front of the starship for observation

        // Calculate the observation position in front of the starship
        glm::vec3 observationPosition = starshipPosition + (starshipForward * observationDistance);

        // Assume the planet is always in front of the starship in observation mode
        // Point the camera towards the point in space where we "know" the planet should be
        glm::vec3 cameraLookAt = starshipPosition + (starshipForward * (observationDistance + 10.0f)); // Slightly further than the camera position

        // Update the camera's position and orientation
        camera->SetPosition(observationPosition);
        camera->SetLookAt(cameraLookAt);
        camera->UpdateView();

        // Cache this position as the original position for Planetary Observation mode
        camera->SetObservationModePosition(observationPosition, starshipForward);
    }
}