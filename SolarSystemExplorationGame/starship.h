#pragma once

#ifndef STARSHIP_H
#define STARSHIP_H

#include "object.h"
#include "Texture.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Starship : public Object {
public:
    Starship(std::string objFilePath, const char* texturePath, const char* textureType);
    ~Starship();

    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc);

    // Movement methods
    void Update(float dt);
    void Roll(float angle);
    void Pitch(float angle);
    void Yaw(float angle);

    // Basic transformations
    void Rotate(float angle, glm::vec3 axis);
    void Accelerate(float speed);
    void Decelerate(float speed);

    // Load model and texture
    void LoadModel(std::string filePath);
    bool LoadTexture(const char* fname, const char* textureTyp);

    void UpdateModelMatrix(const glm::vec3& newPosition, const glm::vec3& newOrientation);

    // Getters
    int getNumVertices();
    int getNumIndices();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();

    GLuint getTextureID(const char* textureType);

    // Texture loading and material properties
    bool setMaterialProperties();
    bool setMaterialProperties(std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> spec, float shininess);

    // Texture and normal map flags
    bool hasTex;
    bool hasNorm;

    // Material properties
    std::vector<float> matAmbient;
    std::vector<float> matDiff;
    std::vector<float> matSpec;
    float matShininess;

private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    glm::vec3 pivotLocation;

    GLuint VB;
    GLuint IB;
    GLuint vao;

    Texture* m_texture;
    Texture* m_normTexture;

    int numVertices;
    int numIndices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> sTangents;
    std::vector<glm::vec3> tTangents;

    void setupVertices();
    void setupBuffers();

    float currentSpeed;
    glm::vec3 direction;
    GLuint textureID;
    float maxSpeed;
};
#endif // STARSHIP_H
