#include "Skybox.h"

Skybox::Skybox()
{
	angle = 0.0f;
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = glm::translate(glm::mat4(1.0f), pivotLocation);

	setupVertices();

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}
}

Skybox::Skybox(glm::mat4 cubeModel)
{
	angle = 0.0f;
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = cubeModel;

	setupVertices();

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}
}

Skybox::Skybox(glm::mat4 cubeModel,
	const char* posXfname, const char* negXfname,
	const char* posYfname, const char* negYfname,
	const char* posZfname, const char* negZfname)
{
	// load cubemap texture
	m_cubemapTexture = new Cubemap(posXfname, negXfname, posYfname, negYfname, posZfname, negZfname);
	if (m_cubemapTexture)
		hasTex = true;
	else
		hasTex = false;

	angle = 0.0f;
	pivotLocation = glm::vec3(0.f, 0.f, 0.f);
	model = cubeModel;

	// Vertex Set Up
	setupVertices();

	// Buffer Set Up
	if (!InitBuffers()) {
		printf("Some buffers not initialized.\n");
	}
}

Skybox::~Skybox()
{
	Vertices.clear();
	Indices.clear();
}

void Skybox::Update(glm::mat4 cubeModel)
{
	model = cubeModel;
}

glm::mat4 Skybox::GetModel()
{
	return model;
}

void Skybox::Render(GLint cubeTCAttrib, GLint isCubeMap)
{
	glBindVertexArray(vao);

	// Bind your VBO
	glBindBuffer(GL_ARRAY_BUFFER, VB);

	// Bind your Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

	glVertexAttribPointer(cubeTCAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// If has texture, set up texture unit(s): update here for texture rendering
	// If has texture, set up texture unit(s) Update here to activate and assign texture unit
	if (m_cubemapTexture != NULL) {
		glUniform1i(isCubeMap, true);
		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture->getTextureID());
	}
	else
		glUniform1i(isCubeMap, false);

	// Render
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(cubeTCAttrib);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Skybox::InitBuffers()
{
	// For OpenGL 3
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return true;
}

void Skybox::setupVertices()
{
	Vertices = {
		{{-1.0f,  -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1., 0.}},
		{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1., 0.}},
		{{-1.0f, 1.0f, -1.0f}, { 0.0f, 0.0f, 0.0f }, { 1., 0.}},
		{{1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f, 0.0f }, { 1., 0. }},
		{{1.0f,  1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f }, { 1., 0. }},
		{{1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1., 0.}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1., 0.}},
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1., 0.}}
    };

	Indices = {
		3, 1, 4,
		4, 6, 3,
		2, 1, 3,
		3, 8, 2,
		4, 7, 5,
		5, 6, 4,
		2, 8, 5,
		5, 7, 2,
		3, 6, 5,
		5, 8, 3,
		1, 2, 4,
		4, 2, 7
	};

	// The index works at a 0th index
	for (unsigned int i = 0; i < Indices.size(); i++)
	{
		Indices[i] = Indices[i] - 1;
	}
}