#pragma once
#include "object.h"
#include "Texture.h"

class Asteroids :
    public Object
{
public:
    Asteroids();
    Asteroids(int prec);
    Asteroids(int prec, const char* fname, const char* textureType);
    Asteroids(int prec, const char* fname, const char* textureType, int numInstances);
    
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint instanceAttribLoc, GLint hasTextureLoc, GLint hasNormLoc);

    int getNumVertices();
    int getNumIndices();
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();

    GLuint getTextureID(const char* textureType);

    bool loadTexture(const char* fname, const char* textureType);

    bool setMaterialProperties();
    bool setMaterialProperties(std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> spec, float shininess);

    bool hasTex;
    bool hasNorm;

    std::vector<float> matAmbient;
    std::vector<float> matDiff;
    std::vector<float> matSpec;
    float matShininess;

private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    Texture* m_texture;
    Texture* m_normTexture;


    GLuint vao;

    float angle;

    void setupVertices();
    void setupInstances();
    void setupBuffers();
    void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);

    int numVertices;
    int numIndices;
    int numInstances;

    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::mat4> instances;

    std::vector<glm::vec3> sTangents;
    std::vector<glm::vec3> tTangents;

    void init(int prec, int instanceNum);
    float toRadians(float degrees);

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // texture coordinates
    std::vector<float> nvalues; // normal vectors
};

