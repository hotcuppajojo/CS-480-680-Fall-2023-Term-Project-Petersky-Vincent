#include "Light.h"

Light::Light()
{
	printf("No Light Data Provided.");
}

Light::Light(glm::mat4 viewMatrix, glm::vec3 lightPos)
{
	m_lightPosition = { 0.0, 0.0, 0.0 };

	m_lightPosition[0] = lightPos.x;
	m_lightPosition[1] = lightPos.y;
	m_lightPosition[2] = lightPos.z;

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).x);
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).y);
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).z);
}

Light::Light(glm::mat4 viewMatrix, glm::vec3 lightPos, std::vector<float> gAmb,
	std::vector<float> lightAmb, std::vector<float> lightDiff, std::vector<float> lightSpec)
{
	m_lightPosition = { 0.0, 0.0, 0.0 };

	m_lightPosition[0] = lightPos.x;
	m_lightPosition[1] = lightPos.y;
	m_lightPosition[2] = lightPos.z;

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).x);
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).y);
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).z);

	m_globalAmbient = gAmb;

	m_lightAmbient = lightAmb;

	m_lightDiffuse = lightDiff;

	m_lightSpecular = lightSpec;
}

void Light::Enable(GLint ambLoc, GLint diffLoc, GLint specLoc, GLint lightPosLoc)
{
	glUniform4fv(ambLoc, 1, m_lightAmbient.data());
	glUniform4fv(diffLoc, 1, m_lightDiffuse.data());
	glUniform4fv(specLoc, 1, m_lightSpecular.data());
	glUniform3fv(lightPosLoc, 1, m_lightPosition.data());
}