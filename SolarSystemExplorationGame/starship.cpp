#include "starship.h"
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Starship::Starship(std::string objFilePath, const char* texturePath, const char* textureType)
    : Object(), currentSpeed(0.0f), direction(glm::vec3(0.0f, 0.0f, 1.0f)) {
    setupVertices();
    LoadModel(objFilePath);

    LoadTexture(texturePath, textureType);
    setupBuffers(); // Setup the buffers similar to the Sphere class
}

Starship::~Starship() {
    // Properly delete the created buffers and textures
    glDeleteBuffers(1, &VB);
    glDeleteBuffers(1, &IB);
    glDeleteVertexArrays(1, &vao);
    if (m_texture) {
        delete m_texture;
        m_texture = nullptr;
    }
    if (m_normTexture) {
        delete m_normTexture;
        m_normTexture = nullptr;
    }
}

void Starship::setupBuffers() {
    // For OpenGL 3
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

void Starship::Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc) {

    glBindVertexArray(vao);
    // Enable vertex attibute arrays for each vertex attrib
    glEnableVertexAttribArray(posAttribLoc);
    glEnableVertexAttribArray(colAttribLoc);
    glEnableVertexAttribArray(tcAttribLoc);

    // Bind your VBO
    glBindBuffer(GL_ARRAY_BUFFER, VB);

    // Set vertex attribute pointers to the load correct data. Update here to load the correct attributes.
    glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(tcAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));


    // If has texture, set up texture unit(s): update here for texture rendering
    // If has texture, set up texture unit(s) Update here to activate and assign texture unit
    if (m_texture != NULL) {
        glUniform1i(hasTextureLoc, true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->getTextureID());
    }
    else
        glUniform1i(hasTextureLoc, false);



    // Bind your Element Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    // Render
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Disable vertex arrays
    glDisableVertexAttribArray(posAttribLoc);
    glDisableVertexAttribArray(colAttribLoc);
    glDisableVertexAttribArray(tcAttribLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Starship::Update(float dt) {
    glm::vec3 delta = currentSpeed * direction * dt; // Calculate change in position
    model = glm::translate(model, delta); // Update the model matrix with the new position
}

void Starship::Roll(float angle) {
    Rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis
}

void Starship::Pitch(float angle) {
    Rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
}

void Starship::Yaw(float angle) {
    Rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
}

void Starship::Rotate(float angle, glm::vec3 axis) {
    model = glm::rotate(model, glm::radians(angle), axis);
    direction = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis)) * direction;
}

void Starship::Accelerate(float speed) {
    currentSpeed += speed;
    if (currentSpeed > maxSpeed) currentSpeed = maxSpeed; // Assume maxSpeed is defined
}

void Starship::Decelerate(float speed) {
    currentSpeed -= speed;
    if (currentSpeed < 0) currentSpeed = 0;
}

void Starship::LoadModel(std::string filePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    // Check if the file was loaded
    if (!scene) {
        std::cerr << "ERROR::ASSIMP:: Could not load file: " << filePath << std::endl;
        return;
    }

    // Check if the scene is incomplete
    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: The scene is incomplete or the root node is null: " << importer.GetErrorString() << std::endl;
        return;
    }

    // Check if the scene has meshes
    if (scene->mNumMeshes <= 0) {
        std::cerr << "ERROR::ASSIMP:: No meshes found in scene: " << filePath << std::endl;
        return;
    }

    aiMesh* mesh = scene->mMeshes[0]; // Get the first mesh

    // Clear previous data
    Vertices.clear();
    Indices.clear();

    // Ensure the mesh has vertices
    if (mesh->mNumVertices <= 0) {
        std::cerr << "ERROR::ASSIMP:: Mesh has no vertices: " << filePath << std::endl;
        return;
    }

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;

        // Process vertex positions, normals and texture coordinates
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        Vertices.push_back(Vertex(position, normal, texCoords));
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // Ensure each face has the expected number of indices (3 for triangles)
        if (face.mNumIndices != 3) {
            std::cerr << "ERROR::ASSIMP:: Face does not have 3 indices, found: " << face.mNumIndices << " at face " << i << std::endl;
            continue; // Skip this face or handle it according to your needs
        }
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            Indices.push_back(face.mIndices[j]);
        }
    }
}

bool Starship::LoadTexture(const char* fname, const char* textureType) {
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

GLuint Starship::getTextureID(const char* textureType) {
    if (textureType == "NORMAL_TEXTURE" && m_normTexture) {
        return m_normTexture->getTextureID();
    }
    else if (textureType == "IMG_TEXTURE" && m_texture) {
        return m_texture->getTextureID();
    }
    else {
        return -1;
    }
}

int Starship::getNumVertices() {
    return Vertices.size();
}

int Starship::getNumIndices() {
    return Indices.size();
}

std::vector<int> Starship::getIndices() {
    return indices;
}

std::vector<glm::vec3> Starship::getVertices() {
    return vertices;
}

std::vector<glm::vec2> Starship::getTexCoords() {
    return texCoords;
}

std::vector<glm::vec3> Starship::getNormals() {
    return normals;
}


bool Starship::setMaterialProperties()
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

bool Starship::setMaterialProperties(std::vector<float> ambient, std::vector<float> diffuse,
    std::vector<float> spec, float shininess)
{
    matAmbient = ambient;
    matDiff = diffuse;
    matSpec = spec;
    matShininess = shininess;
    return true;
}

void Starship::UpdateModelMatrix(const glm::vec3& newPosition, const glm::vec3& newOrientation) {
    model = glm::mat4(1.0f); // Reset to identity matrix
    model = glm::translate(model, newPosition); // Move to new position
    // Assuming newOrientation is Euler angles (pitch, yaw, roll)
    model = glm::rotate(model, glm::radians(newOrientation.x), glm::vec3(1, 0, 0)); // Pitch
    model = glm::rotate(model, glm::radians(newOrientation.y), glm::vec3(0, 1, 0)); // Yaw
    model = glm::rotate(model, glm::radians(newOrientation.z), glm::vec3(0, 0, 1)); // Roll
}

void Starship::setupVertices() {
    std::vector<int> ind = getIndices();
    std::vector<glm::vec3> vert = getVertices();
    std::vector<glm::vec2> tex = getTexCoords();
    std::vector<glm::vec3> norm = getNormals();


    int numIndices = getNumIndices();

    Indices.clear(); // Clear any previous data
    for (int i = 0; i < getNumVertices(); ++i) {
        Indices.push_back(i); // Fill with consecutive numbers
    }
}