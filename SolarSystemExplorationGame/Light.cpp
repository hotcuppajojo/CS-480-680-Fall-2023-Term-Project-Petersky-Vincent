// light.cpp

#include "Light.h"

Light::Light() {
	printf("No Light Data Provided.");
}

void Light::SetGlobalAmbient(const glm::vec4& ambient) {
	globalAmbient = ambient;
}

Light::Light(glm::mat4 viewMatrix, glm::vec3 lightPos) {
    // Default light properties if needed
    glm::vec3 defaultPosition(0.0f, 0.0f, 0.0f);
    glm::vec4 defaultAmbient(0.1f, 0.1f, 0.1f, 1.0f);
    glm::vec4 defaultDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 defaultSpecular(1.0f, 1.0f, 1.0f, 1.0f);

    AddLight(viewMatrix, defaultPosition, defaultAmbient, defaultDiffuse, defaultSpecular);
}

Light::Light(glm::mat4 viewMatrix, glm::vec3 lightPos, std::vector<float> gAmb, std::vector<float> lightAmb, std::vector<float> lightDiff, std::vector<float> lightSpec) {
    // Ensure the vectors have the correct size
    if (gAmb.size() != 4 || lightAmb.size() != 4 || lightDiff.size() != 4 || lightSpec.size() != 4) {
        std::cerr << "Light property vectors must have exactly 4 elements." << std::endl;
        return;
    }
    
    glm::vec4 ambientVec(gAmb[0], gAmb[1], gAmb[2], gAmb[3]);
    glm::vec4 diffuseVec(lightAmb[0], lightAmb[1], lightAmb[2], lightAmb[3]);
    glm::vec4 specularVec(lightDiff[0], lightDiff[1], lightDiff[2], lightDiff[3]);
    glm::vec4 positionVec(lightSpec[0], lightSpec[1], lightSpec[2], lightSpec[3]);

    SetGlobalAmbient(ambientVec);
    AddLight(viewMatrix, lightPos, diffuseVec, specularVec, positionVec);
}

void Light::Enable(GLint numLightsLoc, GLint lightsLoc, GLint globalAmbientLoc) {
    glUniform1i(numLightsLoc, static_cast<GLint>(lights.size()));
    glUniform4fv(globalAmbientLoc, 1, glm::value_ptr(globalAmbient));

    for (size_t i = 0; i < lights.size(); ++i) {
        glUniform4fv(lightsLoc + i * 4, 1, glm::value_ptr(lights[i].ambient));
        glUniform4fv(lightsLoc + i * 4 + 1, 1, glm::value_ptr(lights[i].diffuse));
        glUniform4fv(lightsLoc + i * 4 + 2, 1, glm::value_ptr(lights[i].specular));
        glUniform3fv(lightsLoc + i * 4 + 3, 1, glm::value_ptr(lights[i].positionViewSpace));
    }

    // Set the global ambient light
    glUniform4fv(globalAmbientLoc, 1, glm::value_ptr(globalAmbient));
}

void Light::ClearLights() {
    lights.clear();
}

void Light::AddLight(const glm::mat4& viewMatrix, const glm::vec3& position, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular) {
    Lights light;
    light.position = position;
    light.ambient = ambient;
    light.diffuse = diffuse;
    light.specular = specular;
    // Transform the position to view space
    light.positionViewSpace = glm::vec3(viewMatrix * glm::vec4(position, 1.0f));
    lights.push_back(light);
}