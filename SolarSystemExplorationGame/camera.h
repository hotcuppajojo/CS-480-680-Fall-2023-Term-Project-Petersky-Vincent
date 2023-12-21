#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

enum class CameraMode { Exploration, PlanetaryObservation };

class Camera {
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    CameraMode GetMode() { return mode; };

    void UpdateView(glm::vec3 cameraUpdate);
    void MoveVertical(float speed);
    void MoveHorizontal(float speed);
    void Zoom(float fov);
    void ToggleMode();

    void MoveForward(float speed);
    void MoveRight(float speed);
    void SetObservationModePosition(const glm::vec3& position, const glm::vec3& front);

    // New accessor methods
    glm::vec3 GetPosition() const { return cameraPos; }
    glm::vec3 GetFront() const { return cameraFront; }

    void SetPosition(const glm::vec3& position);
    void SetLookAt(const glm::vec3& target);

    void UpdateView();

private:
    void UpdateProjection(int w, int h);


    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::mat4 projection;
    glm::mat4 view;

    CameraMode mode;

    // Caching original positions and orientations for mode switching
    glm::vec3 originalPosExploration;
    glm::vec3 originalFrontExploration;
    glm::vec3 originalPosObservation;
    glm::vec3 originalFrontObservation;
};

#endif /* CAMERA_H */
