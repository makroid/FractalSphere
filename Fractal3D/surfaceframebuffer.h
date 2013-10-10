#ifndef SURFACEFRAMEBUFFER_H
#define SURFACEFRAMEBUFFER_H

#include <QOpenGLFramebufferObject>

class Surfaceframebuffer
{
public:
    Surfaceframebuffer(QSize size, GLenum target = GL_TEXTURE_CUBE_MAP);



private:
    QOpenGLFramebufferObject* fbo;
};

#endif // SURFACEFRAMEBUFFER_H
