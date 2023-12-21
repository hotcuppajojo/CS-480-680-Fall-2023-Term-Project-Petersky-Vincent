#include "starship.h"

Starship::Starship(glm::vec3 pivot, const char* objFilePath, const char* texturePath)
    : Mesh(pivot, objFilePath, texturePath) {
    // ... implementation ...
}

Starship::~Starship() {
    // Clean up Starship-specific resources (if any)
    // ...
}

void Starship::UpdateModelMatrix(const glm::vec3& newPosition, const glm::vec3& newOrientation) {
    model = glm::mat4(1.0f); // Reset to identity matrix
    model = glm::translate(model, newPosition); // Move to new position
    model = glm::rotate(model, glm::radians(newOrientation.x), glm::vec3(1, 0, 0)); // Pitch
    model = glm::rotate(model, glm::radians(newOrientation.y), glm::vec3(0, 1, 0)); // Yaw
    model = glm::rotate(model, glm::radians(newOrientation.z), glm::vec3(0, 0, 1)); // Roll

    orientation = newOrientation;
    position = newPosition;
}

glm::vec3 Starship::GetForwardVector() const {
    // The forward vector is the third column of the rotation matrix
    // Since glm::mat4 stores columns in a one-dimensional array, the indices are 8, 9, and 10 for the third column
    // We negate it because in OpenGL, the camera looks down the negative Z-axis by default
    return -glm::vec3(model[2][0], model[2][1], model[2][2]);
}
