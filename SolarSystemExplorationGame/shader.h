/* Shader Class Header, Author: Dr. Tavakkoli*/

#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType);
    bool AddShader(GLenum ShaderType, const char* shaderFPath);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
    GLint GetAttribLocation(const char* pAttribName);


  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;

    std::string readShaderSource(const char* shaderFPath);
};

#endif  /* SHADER_H */
