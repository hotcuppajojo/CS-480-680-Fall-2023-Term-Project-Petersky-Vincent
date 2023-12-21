#include "cubemap.h"

Cubemap::Cubemap()
{
	m_TextureID = -1;
	printf("No Texture Data Provided.");
}

Cubemap::Cubemap(const char* posXfname, const char* negXfname, const char* posYfname, const char* negYfname,
	const char* posZfname, const char* negZfname)
{
	loadCubemapTexture(posXfname, negXfname, posYfname, negYfname, posZfname, negZfname);
	initializeTexture();
}

bool Cubemap::loadCubemapTexture(const char* posXfname, const char* negXfname, const char* posYfname, const char* negYfname,
	const char* posZfname, const char* negZfname)
{
	m_TextureID = SOIL_load_OGL_cubemap(posXfname, negXfname, posYfname, negYfname,
		posZfname, negZfname, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);


	if (!m_TextureID) {
		printf("Failed: Could not open texture file!\n");
		return false;
	}
	return true;
}

bool Cubemap::initializeTexture()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	return true;
}