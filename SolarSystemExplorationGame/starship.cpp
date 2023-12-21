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
}