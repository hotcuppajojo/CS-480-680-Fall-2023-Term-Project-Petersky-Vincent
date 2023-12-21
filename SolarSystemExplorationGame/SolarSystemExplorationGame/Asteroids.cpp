#include "Asteroids.h"
#include <stdlib.h>
#include <time.h>

Asteroids::Asteroids()
{
    init(48, 100);
    setupInstances();
    setupVertices();
    setupBuffers();
    setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);

    setMaterialProperties();
}

Asteroids::Asteroids(int prec)
{
    init(prec, 100);
    setupInstances();

    setupVertices();
    setupBuffers();
    setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);
    hasTex = false;
    hasNorm = false;

    setMaterialProperties();
}

Asteroids::Asteroids(int prec, const char* fname, const char* textureType)
{
    init(prec, 100);
    setupInstances();

    setupVertices();

    setupBuffers();
    setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);

    hasNorm = false;
    hasTex = false;

    // load texture from file
    loadTexture(fname, textureType);

    setMaterialProperties();
}

Asteroids::Asteroids(int prec, const char* fname, const char* textureType, int instanceNum)
{
    init(prec, numInstances);
    setupInstances();

    setupVertices();

    setupBuffers();
    setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);

    hasNorm = false;
    hasTex = false;

    // load texture from file
    loadTexture(fname, textureType);

    setMaterialProperties();
}

void Asteroids::Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc, GLint instanceAttribLoc, GLint hasTextureLoc, GLint hasNormLoc)
{
    glBindVertexArray(vao);
    // Enable vertex attibute arrays for each vertex attrib
    glEnableVertexAttribArray(posAttribLoc);
    glEnableVertexAttribArray(colAttribLoc);
    glEnableVertexAttribArray(tcAttribLoc);
    glEnableVertexAttribArray(instanceAttribLoc);

    // Bind your VBO
    glBindBuffer(GL_ARRAY_BUFFER, VB);

    // Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.
    glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(tcAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));


        // atribute pointer for mat4 is 4x vec4 buffers
    glVertexAttribPointer(instanceAttribLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texcoord) + sizeof(glm::vec2)));
    glVertexAttribPointer(instanceAttribLoc + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(offsetof(Vertex, texcoord) + sizeof(glm::vec2) + sizeof(glm::vec4)));
    glVertexAttribPointer(instanceAttribLoc + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(offsetof(Vertex, texcoord) + sizeof(glm::vec2) + 2 * sizeof(glm::vec4)));
    glVertexAttribPointer(instanceAttribLoc + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(offsetof(Vertex, texcoord) + sizeof(glm::vec2) + 3 * sizeof(glm::vec4)));


    glVertexAttribDivisor(instanceAttribLoc, 1);
    glVertexAttribDivisor(instanceAttribLoc + 1, 1);
    glVertexAttribDivisor(instanceAttribLoc + 2, 1);
    glVertexAttribDivisor(instanceAttribLoc + 3, 1);

    // Bind your Element Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    // Render
    glDrawElementsInstanced(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0, numInstances);

    // Disable vertex arrays
    glDisableVertexAttribArray(posAttribLoc);
    glDisableVertexAttribArray(colAttribLoc);
    glDisableVertexAttribArray(tcAttribLoc);
    glDisableVertexAttribArray(instanceAttribLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Asteroids::setupVertices()
{
    // same as Sphere class
    std::vector<int> ind = getIndices();
    std::vector<glm::vec3> vert = getVertices();
    std::vector<glm::vec2> tex = getTexCoords();
    std::vector<glm::vec3> norm = getNormals();


    int numIndices = getNumIndices();

    for (int i = 0; i < numIndices; i++) {
        Vertices.push_back(Vertex(vert[ind[i]], norm[ind[i]], tex[ind[i]]));
        Indices.push_back(i);
    }
}

void Asteroids::setupInstances()
{
    for (int i = 0; i < numInstances; i++) { instances.push_back(glm::mat4()); }

    // generates a list of randomized model matrices within ranges

    float dist = 10.;   // dist from sun
    float offset = 2.;  // ateroid belt range = 2*offset
    
    srand(time(NULL));

    for (int i = 0; i < numInstances; i++) {
        
        glm::mat4 localTransform = glm::mat4(1.0f);

        // generate x,y,z with random factors, but within circle within asteriod belt range (specified by dist and offset)
        
        // spread asteroids out around circle
       float angle = (float)i / (float)numInstances * 360.0f;

        // randomly generate displacement value within range -offset to offset
        float displacement = (rand() % (int)(2 * offset * 100.0f)) / 100.0f - offset;

        float x = sin(angle) * dist + displacement;
        displacement = (rand() % (int)(2 * offset * 100.0f)) / 100.0f - offset;

        float y = displacement * 0.4f; // height spread of asteroid field

        displacement = (rand() % (int)(2*offset * 100.0f)) / 100.0f - offset;
        
        float z = cos(angle) * dist + displacement;
        
        localTransform *= glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));


        // Add random rotation to randomize location further
        glm::vec3 rotVector = { 0., 1., 0. };
        float rotAmount = (float)((rand() % 360));
        localTransform *= glm::rotate(glm::mat4(1.f), rotAmount, rotVector);


        // set asteroid size between 0.05 and 0.25f
        float scale = (float)((rand() % 20) / 100.0 + 0.05);
        localTransform *= glm::scale(glm::mat4(1.f), glm::vec3(scale));

        instances[i] = localTransform;
    }
}

void Asteroids::setupBuffers() 
{
    // same as Sphere class

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned long int size = sizeof(Vertex) * Vertices.size() + sizeof(glm::mat4) * numInstances;

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, size, &Vertices[0], GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * Vertices.size(), &Vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), sizeof(glm::mat4) * numInstances, &instances);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

// same as Sphere class
void Asteroids::setupModelMatrix(glm::vec3 pivot, float angle, float scale) {
    pivotLocation = pivot;
    model = glm::translate(glm::mat4(1.0f), pivotLocation);
    model *= glm::rotate(glm::mat4(1.f), angle, glm::vec3(0., 1., 0));
    model *= glm::scale(glm::vec3(scale, scale, scale));
}

// slightly modified Sphere class
void Asteroids::init(int prec, int instanceNum)
{
    numInstances = instanceNum;
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
    for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { sTangents.push_back(glm::vec3()); }
    for (int i = 0; i < numVertices; i++) { tTangents.push_back(glm::vec3()); }
    for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

    for (int i = 0; i <= prec; i++) {
        for (int j = 0; j <= prec; j++) {
            float y = (float)cos(toRadians(180.f - i * 180.f / prec));
            float x = -(float)cos(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.f / prec)) * (float)abs(cos(asin(y)));
            vertices[i * (prec + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (prec + 1) + j] = glm::vec2(((float)j / prec), ((float)i / prec));
            normals[i * (prec + 1) + j] = glm::vec3(x, y, z);
        }
    }

    // calculate triangles indices
    for (int i = 0; i < prec; i++) {
        for (int j = 0; j < prec; j++) {
            indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
            indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
            indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
            indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
        }
    }
}


float Asteroids::toRadians(float degrees)
{
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

// same as Sphere class
int Asteroids::getNumVertices() { return numVertices; }
int Asteroids::getNumIndices() { return numIndices; }
std::vector<int> Asteroids::getIndices() { return indices; }
std::vector<glm::vec3> Asteroids::getVertices() { return vertices; }
std::vector<glm::vec2> Asteroids::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Asteroids::getNormals() { return normals; }


GLuint Asteroids::getTextureID(const char* textureType) {
    if (textureType == "NORMAL_TEXTURE")
        return m_normTexture->getTextureID();
    else if (textureType == "IMG_TEXTURE")
        return m_texture->getTextureID();
    else
        return -1;
}

bool Asteroids::loadTexture(const char* fname, const char* textureType)
{
    if (textureType == "NORMAL_TEXTURE") {
        m_normTexture = new Texture(fname);

        if (m_normTexture)
            hasNorm = true;
        else
            hasNorm = false;

        return hasNorm;
    }
    else if (textureType == "IMG_TEXTURE") {
        // load texture from file
        m_texture = new Texture(fname);
        if (m_texture)
            hasTex = true;
        else
            hasTex = false;

        return hasTex;
    }

    return false;
}

bool Asteroids::setMaterialProperties()
{
    std::vector<float> ambient = { 0.2, 0.2, 0.2, 1.0 };
    std::vector<float> diffuse = { 1.0, 1.0, 1.0, 1.0 };
    std::vector<float> spec = { 1.0, 1.0, 1.0, 1.0 };
    float shininess = 20.0;

    matAmbient = ambient;
    matDiff = diffuse;
    matSpec = spec;
    matShininess = shininess;

    return true;
}
bool Asteroids::setMaterialProperties(std::vector<float> ambient, std::vector<float> diffuse, std::vector<float> spec, float shininess)
{
    matAmbient = ambient;
    matDiff = diffuse;
    matSpec = spec;
    matShininess = shininess;
    return true;
}
