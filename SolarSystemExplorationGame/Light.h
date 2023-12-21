// light.h

#pragma once
#include "graphics_headers.h"
#include <vector>

struct Lights {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec3 position; // World space position
	glm::vec3 positionViewSpace; // View space position
};

class Light {
public:
	Light();
	Light(glm::mat4 viewMatrix, glm::vec3 lightPos);
	Light(glm::mat4 viewMatrix, glm::vec3 lightPos, std::vector<float> gAmb, std::vector<float> lightAmb, std::vector<float> lightDiff, std::vector<float> lightSpec);

	void SetGlobalAmbient(const glm::vec4& ambient);
	void Enable(GLint numLightsLoc, GLint lightsLoc, GLint globalAmbientLoc);
	void AddLight(const glm::mat4& viewMatrix, const glm::vec3& position, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular);
	void ClearLights(); // Method to clear existing lights

private:
	std::vector<Lights> lights;
	glm::vec4 globalAmbient; // New property for global ambient light
};

