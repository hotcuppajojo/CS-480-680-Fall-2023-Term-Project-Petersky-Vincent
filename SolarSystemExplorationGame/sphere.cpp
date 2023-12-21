/* Sphere Class, Author: Dr. Tavakkoli */

#include "sphere.h"

Sphere::Sphere()
{
    init(48);
    setupVertices();
    setupBuffers();
    //setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);

    setMaterialProperties();
}

Sphere::Sphere(int prec) { // prec is precision, or number of slices

    init(prec);
    setupVertices();
    setupBuffers();
    //setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);
    hasTex = false;
    hasNorm = false;

    setMaterialProperties();
}

Sphere::Sphere(int prec, const char* fname, const char* textureType) { // prec is precision, or number of slices

    init(prec);

    setupVertices();

    setupBuffers();
    //setupModelMatrix(glm::vec3(0., 0., 0.), 0., 1.);

    hasNorm = false;
    hasTex = false;

    // load texture from file
    loadTexture(fname, textureType);

    setMaterialProperties();
}

bool Sphere::loadTexture(const char* fname, const char* textureType)
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


void Sphere::Render(GLint positionAttribLoc, GLint colorAttribLoc)
{

    glBindVertexArray(vao);

    // Enable vertex attibute arrays for each vertex attrib
    glEnableVertexAttribArray(positionAttribLoc);
    glEnableVertexAttribArray(colorAttribLoc);

    // Bind your VBO
    glBindBuffer(GL_ARRAY_BUFFER, VB);

    // Set vertex attribute pointers to the load correct data
    glVertexAttribPointer(positionAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(colorAttribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Bind your Element Array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

    // Render
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    // Disable vertex arrays
    glDisableVertexAttribArray(positionAttribLoc);
    glDisableVertexAttribArray(colorAttribLoc);
}

void Sphere::Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc, GLint hasTextureLoc)
{

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

void Sphere::Render(GLint posAttribLoc, GLint colAttribLoc, GLint tcAttribLoc,
    GLint hasTextureLoc, GLint hasNormLoc)
{
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
    /*if (m_texture != NULL) {
        glUniform1i(hasTextureLoc, true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->getTextureID());
    }
    else
        glUniform1i(hasTextureLoc, false);

    if (m_normTexture != NULL) {
        glUniform1i(hasNormLoc, true);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_normTexture->getTextureID());
    }
    else
        glUniform1i(hasNormLoc, false);

    */

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


void Sphere::setupVertices() {
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


void Sphere::setupBuffers() {
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

void Sphere::setupModelMatrix(glm::vec3 pivot, float angle, float scale) {
    pivotLocation = pivot;
    model = glm::translate(glm::mat4(1.0f), pivotLocation);
    model *= glm::rotate(glm::mat4(1.f), angle, glm::vec3(0., 1., 0));
    model *= glm::scale(glm::vec3(scale, scale, scale));
}

void Sphere::Update(glm::mat4 matModel) {

    model = matModel;

}

float Sphere::toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

void Sphere::init(int prec) {
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

GLuint Sphere::getTextureID(const char* textureType)
{
    if (textureType == "NORMAL_TEXTURE")
        return m_normTexture->getTextureID();
    else if (textureType == "IMG_TEXTURE")
        return m_texture->getTextureID();
    else
        return -1;
}

// accessors
int Sphere::getNumVertices() { return numVertices; }
int Sphere::getNumIndices() { return numIndices; }
std::vector<int> Sphere::getIndices() { return indices; }
std::vector<glm::vec3> Sphere::getVertices() { return vertices; }
std::vector<glm::vec2> Sphere::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Sphere::getNormals() { return normals; }

bool Sphere::setMaterialProperties()
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

bool Sphere::setMaterialProperties(std::vector<float> ambient, std::vector<float> diffuse,
    std::vector<float> spec, float shininess)
{
    matAmbient = ambient;
    matDiff = diffuse;
    matSpec = spec;
    matShininess = shininess;
    return true;
}