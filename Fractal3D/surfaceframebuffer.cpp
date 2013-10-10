#include "surfaceframebuffer.h"

Surfaceframebuffer::Surfaceframebuffer(QSize size, GLenum target)
{
    fbo = new QOpenGLFramebufferObject(size, target);
    fbo->setAttachment(QOpenGLFramebufferObject::Depth);

    fbo->bind();
    for (int i=0; i<6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, size.width(), size.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    }
    fbo->release();
}


void Surfaceframebuffer::begin(int face) {
    fbo->bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, fbo->texture(), 0);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, )
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


void Surfaceframebuffer::end() {
    fbo->release();
}
