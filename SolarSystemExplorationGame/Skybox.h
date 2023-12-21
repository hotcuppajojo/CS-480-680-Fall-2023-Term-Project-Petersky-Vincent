// skybox.h 

#ifndef SKYBOX_H
#define SKYBOX_H

#include "cubemap.h"
#include "object.h"

class Skybox : 
	public Object
{
public:
	Skybox();
	Skybox(glm::mat4 cubeModel);
	Skybox(glm::mat4 cubeModel, const char* posXfname, const char* negXfname,
		const char* posYfname, const char* negYfname,
		const char* posZfname, const char* negZfname);

	~Skybox();

	void Update(glm::mat4 cubeModel);
	void Render(GLint cubeTCAttrib, GLint isCubeMap);

	glm::mat4 GetModel();

	bool InitBuffers();

	bool hasTex;
	GLuint getTextureID() { return m_cubemapTexture->getTextureID(); }

private:
	glm::vec3 pivotLocation;
	glm::mat4 model;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	GLuint VB;
	GLuint IB;

	GLuint vao;

	float angle;

	Cubemap* m_cubemapTexture;

	void setupVertices();
};

#endif /* SKYBOX_H */