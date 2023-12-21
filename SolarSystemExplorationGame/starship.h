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

private:

    float currentSpeed;
    glm::vec3 direction;
    float maxSpeed;
};

#endif // STARSHIP_H