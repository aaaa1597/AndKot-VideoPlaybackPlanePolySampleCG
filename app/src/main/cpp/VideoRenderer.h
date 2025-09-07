#pragma once
#include <GLES2/gl2.h>

class VideoRenderer {
public:
    void init();
    void resize(int width, int height);
    void draw(int textureId, const float *texMatrix);

private:
    GLuint program;
    GLuint vPos;
    GLuint vTexCoord;
    GLuint uTexMatrix;
};
