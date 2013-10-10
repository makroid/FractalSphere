#ifndef FBOCUBESURFACE_H
#define FBOCUBESURFACE_H

#include <QGLAbstractSurface>
#include <QtGui/qopengl.h>
#include <QOpenGLFunctions>
#include <QOpenGLContext>

class FboCubeSurface : public QGLAbstractSurface, protected QOpenGLFunctions
{
public:
    FboCubeSurface(int width, int height, QOpenGLContext *context = 0);

    virtual ~FboCubeSurface();

    bool activate(QGLAbstractSurface *prevSurface = 0);
    void deactivate(QGLAbstractSurface *nextSurface = 0);
    QRect viewportGL() const;
    QRect viewportRect() const;
    bool isValid() const;

    bool begin(int face);
    void end();
    void bind() { glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); }
    void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    void bindTexture();
    void unbindTexture();
    GLuint id() const { return cubemap; }

private:
    void init();
    bool checkFramebufferStatus(QOpenGLContext *ctx) const;

    int width;
    int height;
    bool active;
    Q_DISABLE_COPY(FboCubeSurface)
    GLuint cubemap;
    GLuint framebuffer;
    GLuint depthbuffer;
};

#endif // FBOCUBESURFACE_H
