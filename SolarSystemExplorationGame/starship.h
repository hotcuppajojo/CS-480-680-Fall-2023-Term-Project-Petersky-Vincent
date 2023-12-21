#pragma once

#ifndef STARSHIP_H
#define STARSHIP_H

#include "Mesh.h"
#include "Texture.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Starship : public Mesh {
public:
    Starship(glm::vec3 pivot, const char* objFilePath, const char* texturePath);
    ~Starship();

    void UpdateModelMatrix(const glm::vec3& newPosition, const glm::vec3& newOrientation);

    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetOrientation() const { return orientation; }

    void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
    void SetOrientation(const glm::vec3& newOrientation) { orientation = newOrientation; }
    glm::vec3 GetForwardVector() const;

private:

    float currentSpeed;
    glm::vec3 direction;
    float maxSpeed;

    glm::vec3 position;
    glm::vec3 orientation;
};

#endif // STARSHIP_H