/* Graphics Class Header, Author: Dr. Tavakkoli*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "CubemapTexture.h"
#include "Light.h"
#include "starship.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }
    Starship* getStarship() const { return m_starship; }

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat); 

    
    glm::vec3 sunPosition;
    glm::vec4 sunAmbient;
    glm::vec4 sunDiffuse;
    glm::vec4 sunSpecular;

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader *m_shader;
    Shader* m_cubemap_shader;
    Shader* m_light_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_normMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;

    GLint m_hasTexture;
    GLint m_hasNormal;

    GLint m_cubeProjectionMatrix;
    GLint m_cubeMVmatrix;
    GLint m_cubePositionAttrib;

    GLint m_lightProjectionMatrix;
    GLint m_lightViewMatrix;
    GLint m_lightModelMatrix;
    GLint m_lightHasTexture;
    GLint m_lightPositionAttrib;
    GLint m_lightColorAttrib;
    GLint m_lightTCAttrib;

    GLint globalAmbLoc;
    GLint ambLoc;
    GLint diffLoc;
    GLint specLoc;
    GLint lightPosLoc;

    GLint mAmbLoc;
    GLint mDiffLoc;
    GLint mSpecLoc;
    GLint mShineLoc;

    std::vector<GLint> ambientLocs, diffuseLocs, specularLocs, positionLocs;

    CubemapTexture* m_cubemapTex;

    Sphere* m_sun;

    Sphere* m_mercury;
    Sphere* m_venus;
    Sphere* m_earth;
    Sphere* m_mars;
    Sphere* m_jupiter;
    Sphere* m_saturn;
    Sphere* m_uranus;
    Sphere* m_neptune;

    Sphere* m_moon;

    Mesh* m_mesh;

    Light* m_sunlight;

    std::unique_ptr<Light> m_lightManager;

    Starship* m_starship;

};

#endif /* GRAPHICS_H */
