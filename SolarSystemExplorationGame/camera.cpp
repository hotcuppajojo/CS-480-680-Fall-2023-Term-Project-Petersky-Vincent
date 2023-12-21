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

void Camera::MoveForward(float speed) {
    // Camera moves along the view direction in both modes.
    cameraPos += cameraFront * speed;
    UpdateView();
}

void Camera::MoveRight(float speed) {
    // Camera strafes relative to the view direction in both modes.
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    cameraPos += right * speed;
    UpdateView();
}

void Camera::Zoom(float fov) {
    projection = glm::perspective(glm::radians(fov), float(800) / float(600), 0.01f, 100.0f);
}

void Camera::ToggleMode() {
    if (mode == CameraMode::Exploration) {
        // Save current position and front for Exploration mode
        originalPosExploration = cameraPos;
        originalFrontExploration = cameraFront;

        // Switch to Planetary Observation Mode
        mode = CameraMode::PlanetaryObservation;

        // Set camera to a predefined position and orientation suitable for Planetary Observation
        cameraPos = glm::vec3(0.0f, 0.0f, 10.0f); // Example position
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // Example orientation towards the planet
    }
    else {
        // Save current position and front for Planetary Observation mode
        originalPosObservation = cameraPos;
        originalFrontObservation = cameraFront;

        // Switch to Exploration Mode
        mode = CameraMode::Exploration;

        // Set camera to a predefined position and orientation suitable for Exploration
        cameraPos = originalPosExploration;
        cameraFront = originalFrontExploration;
    }

    // Update view to reflect the mode change
    UpdateView();
}

void Camera::SetObservationModePosition(const glm::vec3& position, const glm::vec3& front) {
    originalPosObservation = position;
    originalFrontObservation = front;
}

void Camera::UpdateProjection(int w, int h) {
    projection = glm::perspective(glm::radians(40.0f), float(w) / float(h), 0.01f, 100.0f);
}
void Camera::UpdateView() {
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::SetPosition(const glm::vec3& position) {
    cameraPos = position;
    UpdateView(); // Make sure to update the view matrix after changing the position
}

void Camera::SetLookAt(const glm::vec3& target) {
    cameraFront = glm::normalize(target - cameraPos);
    UpdateView(); // Make sure to update the view matrix after changing the orientation
}