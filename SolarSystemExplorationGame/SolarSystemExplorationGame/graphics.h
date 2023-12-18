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

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat); 

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
    GLint lightLoc;
    GLint lightDLoc;
    GLint lightSLoc;
    GLint lightPosLoc;

    GLint mAmbLoc;
    GLint mDiffLoc;
    GLint mSpecLoc;
    GLint mShineLoc;

    CubemapTexture* m_cubemapTex;

    Sphere* m_sun;
    Sphere* m_mercury;
    Sphere* m_moon;

    Mesh* m_mesh;

    Light* m_sunlight;

};

#endif /* GRAPHICS_H */
