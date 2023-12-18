#pragma once
#include "graphics_headers.h"

class Light
{
public:
	Light();
	Light(glm::mat4 viewMatrix, glm::vec3 lightPos);
	Light(glm::mat4 viewMatrix, glm::vec3 lightPos, std::vector<float> gAmb, 
	std::vector<float> lightAmb, std::vector<float> lightDiff, std::vector<float> lightSpec);

	void Enable(GLint ambLoc, GLint diffLoc, GLint specLoc, GLint lightPosLoc);

private:
	std::vector<float> m_globalAmbient;
	std::vector<float> m_lightAmbient;
	std::vector<float> m_lightDiffuse;
	std::vector<float> m_lightPosition;
	glm::mat3 m_lightPositionViewSpace;
	std::vector<float> m_lightSpecular;
};

