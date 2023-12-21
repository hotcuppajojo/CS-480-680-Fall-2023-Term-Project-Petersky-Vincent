#include "camera.h"

Camera::Camera() : mode(CameraMode::Exploration) {
    // Initialize with default values
    cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
    cameraFront = glm::vec3(0.0, 1.0, 1.0);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {
    // Cleanup if necessary
}

bool Camera::Initialize(int w, int h) {
    // Cache the original positions for both modes
    originalPosExploration = cameraPos;
    originalFrontExploration = cameraFront;

    originalPosObservation = cameraPos; // Adjust this based on your initial setup
    originalFrontObservation = cameraFront; // Adjust this based on your initial setup

    UpdateProjection(w, h);

    return true;
}

glm::mat4 Camera::GetProjection() {
    return projection;
}

glm::mat4 Camera::GetView() {
    return view;
}

void Camera::UpdateView(glm::vec3 cameraUpdate) {
    cameraFront = cameraUpdate;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::MoveVertical(float speed) {
    cameraPos += speed * cameraFront;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::MoveHorizontal(float speed) {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::Zoom(float fov) {
    projection = glm::perspective(glm::radians(fov), float(800) / float(600), 0.01f, 100.0f);
}

void Camera::ToggleMode() {
    if (mode == CameraMode::Exploration) {
        // Cache current position and front
        originalPosExploration = cameraPos;
        originalFrontExploration = cameraFront;

        // Switch to Planetary Observation Mode
        cameraPos = originalPosObservation;
        cameraFront = originalFrontObservation;
        mode = CameraMode::PlanetaryObservation;
    }
    else {
        // Cache current position and front
        originalPosObservation = cameraPos;
        originalFrontObservation = cameraFront;

        // Switch to Exploration Mode
        cameraPos = originalPosExploration;
        cameraFront = originalFrontExploration;
        mode = CameraMode::Exploration;
    }

    // Update view to reflect the mode change
    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::ResetToOriginal() {
    if (mode == CameraMode::Exploration) {
        cameraPos = originalPosExploration;
        cameraFront = originalFrontExploration;
    }
    else {
        cameraPos = originalPosObservation;
        cameraFront = originalFrontObservation;
    }

    view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
}

void Camera::UpdateProjection(int w, int h) {
    projection = glm::perspective(glm::radians(40.0f), float(w) / float(h), 0.01f, 100.0f);
}
