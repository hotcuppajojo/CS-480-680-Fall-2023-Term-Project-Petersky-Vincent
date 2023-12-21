#ifndef CUBEMAP_H_
#define CUBEMAP_H_

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"

class Cubemap
{
public:
    Cubemap();
    Cubemap(const char* posXfname, const char* negXfname, 
            const char* posYfname, const char* negYfname,
            const char* posZfname, const char* negZfname);

    bool loadCubemapTexture(const char* posXfname, const char* negXfname,
        const char* posYfname, const char* negYfname,
        const char* posZfname, const char* negZfname);

    GLuint getTextureID() { return m_TextureID; }

private:
    GLuint m_TextureID;

    bool initializeTexture();
};

#endif