#include "Light.h"

Light::Light()
{
	printf("No Light Data Provided.");
}

Light::Light(GLenum lightN, glm::mat4 viewMatrix, glm::vec3 lightPos)
{
	m_lightPosition = { 0.0, 0.0, 0.0 };

	m_lightPosition[0] = lightPos.x;
	m_lightPosition[1] = lightPos.y;
	m_lightPosition[2] = lightPos.z;

	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).x);
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).y);
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).z);

	lightNum = lightN;
}

Light::Light(GLenum lightN, glm::mat4 viewMatrix, glm::vec3 lightPos, std::vector<float> gAmb,
	std::vector<float> lightAmb, std::vector<float> lightDiff, std::vector<float> lightSpec)
{
	m_globalAmbient = { 0.0, 0.0, 0.0, 0.0 };
	m_lightAmbient = { 0.0, 0.0, 0.0, 0.0 };
	m_lightDiffuse = { 0.0, 0.0, 0.0, 0.0 };
	m_lightPosition = { 0.0, 0.0, 0.0 };
	m_lightSpecular = { 0.0, 0.0, 0.0, 0.0 };

	m_lightPosition[0] = lightPos.x;
	m_lightPosition[1] = lightPos.y;
	m_lightPosition[2] = lightPos.z;


	m_lightPositionViewSpace[0] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).x);
	m_lightPositionViewSpace[1] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).y);
	m_lightPositionViewSpace[2] = glm::vec4((viewMatrix * glm::vec4(lightPos, 1.0f)).z);

	m_globalAmbient[0] = gAmb[0];
	m_globalAmbient[1] = gAmb[1];
	m_globalAmbient[2] = gAmb[2];
	m_globalAmbient[3] = gAmb[3];

	m_lightAmbient[0] = lightAmb[0];
	m_lightAmbient[1] = lightAmb[1];
	m_lightAmbient[2] = lightAmb[2];
	m_lightAmbient[3] = lightAmb[3];

	m_lightDiffuse[0] = lightDiff[0];
	m_lightDiffuse[1] = lightDiff[1];
	m_lightDiffuse[2] = lightDiff[2];
	m_lightDiffuse[3] = lightDiff[3];

	m_lightSpecular[0] = lightSpec[0];
	m_lightSpecular[1] = lightSpec[1];
	m_lightSpecular[2] = lightSpec[2];
	m_lightSpecular[3] = lightSpec[3];

	lightNum = lightN;

}

void Light::Init()
{
	glEnable(lightNum);

	glLightfv(lightNum, GL_AMBIENT, m_lightAmbient.data());
	glLightfv(lightNum, GL_DIFFUSE, m_lightDiffuse.data());
	glLightfv(lightNum, GL_SPECULAR, m_lightSpecular.data());
	glLightfv(lightNum, GL_POSITION, m_lightPosition.data());
}