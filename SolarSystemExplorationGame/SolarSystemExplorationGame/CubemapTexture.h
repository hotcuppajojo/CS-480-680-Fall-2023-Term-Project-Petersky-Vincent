#pragma once
#include "graphics_headers.h"
#include "Texture.h"
#include <SOIL2/SOIL2.h>

class CubemapTexture
{
public:
	CubemapTexture();
	CubemapTexture(const char* posXfname, const char* negXfname,
		const char* posYfname, const char* negYfname,
		const char* posZfname, const char* negZfname);

	bool loadTexture(const char* posXfname, const char* negXfname,
		const char* posYfname, const char* negYfname,
		const char* posZfname, const char* negZfname);

	void Update(glm::mat4 cubeModelView);
	void Render(GLint cubePosAttribLoc);

	GLuint getTextureID() { return m_TextureID; }

private:
	GLuint m_TextureID;

	glm::mat4 modelView;
	float* vertexPositions;
	GLuint VB;

	GLuint vao;

	bool initializeTexture();
	void setupVertices();
	void setupBuffers();
};

