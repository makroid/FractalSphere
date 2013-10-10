#include "fbocubesurface.h"

#include <QDebug>
#include <QImage>

FboCubeSurface::FboCubeSurface(int awidth, int aheight, QOpenGLContext* context)
    : QGLAbstractSurface(QGLAbstractSurface::User),
      QOpenGLFunctions(context==0 ? QOpenGLContext::currentContext() : context),
      width(awidth),
      height(aheight) {
    if (context != 0)
        setContext(context);
    init();
}


void FboCubeSurface::init() {
    // check context
    QOpenGLContext* ctxt = QOpenGLContext::currentContext();
    if ( ! ctxt) {
        qDebug() << "FboCubeSurface: no current context";
        return;
    }

    initializeOpenGLFunctions();
    //glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    // set textures
//    QImage one(QString("/home/armin/Pictures/one.png"));
//    QImage two(QString("/home/armin/Pictures/two.png"));
//    QImage three(QString("/home/armin/Pictures/three.png"));
//    QImage four(QString("/home/armin/Pictures/four.png"));
//    QImage five(QString("/home/armin/Pictures/five.png"));
//    QImage six(QString("/home/armin/Pictures/six.png"));

//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, one.bits());
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, two.bits());
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, three.bits());
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, four.bits());
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, five.bits());
//    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, six.bits());

    for (int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    // create the fbo
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // create the uniform depth buffer
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    //glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach it
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    // attach only the +X cubemap texture (for completeness)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cubemap, 0);

    // this function just checks for framebuffer completeness and throws and exception if it’s not happy
    checkFramebufferStatus(ctxt);

    // disable
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


FboCubeSurface::~FboCubeSurface() {
    if (context() && framebuffer) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glDeleteFramebuffers(1, &framebuffer);
    }
}

bool FboCubeSurface::isValid() const {
     return QGLAbstractSurface::isValid() && cubemap && framebuffer && context();
}


bool FboCubeSurface::activate(QGLAbstractSurface *prevSurface) {
    //qDebug() << "calling activate";
    //if (active) return true;
    Q_UNUSED(prevSurface);
    bool success = true;
//    if (context()) {
//        if (!QOpenGLContext::areSharing(QOpenGLContext::currentContext(), context())) {
//            context()->makeCurrent(context()->surface());
//        }
//    } else {
//        setContext(QOpenGLContext::currentContext());
//    }

//    if (isValid()) {
//        //glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
//        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//        //glPushAttrib(GL_VIEWPORT_BIT);
//        success = checkFramebufferStatus(context());
//    }
//#ifndef QT_NO_DEBUG_STREAM
//    else {
//        qWarning() << "Attempt to activate invalid fbo surface";
//    }
//#endif
//    if ( ! success) {
//        qWarning() << "FboCubeSurface activate failed";
//    }
//    active = true;
    return success;
}


void FboCubeSurface::deactivate(QGLAbstractSurface *nextSurface) {
    //qDebug() << "calling deactivate";
    //if ( ! active) return;
    Q_UNUSED(nextSurface);
//    if (framebuffer && context()) {
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        //glPopAttrib();
//       // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//    }
//    active = false;
}


void FboCubeSurface::bindTexture() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glEnable(GL_TEXTURE_CUBE_MAP);
}


void FboCubeSurface::unbindTexture() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}

bool FboCubeSurface::begin(int face) {
    //qDebug() << "calling bindcubemapface for face" << face;
    if (checkFramebufferStatus(context())) return false;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemap, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    return true;
}

void FboCubeSurface::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


QRect FboCubeSurface::viewportGL() const {
    if (framebuffer) {
        return QRect(0, 0, width, height);
    } else
        return QRect();
}


QRect FboCubeSurface::viewportRect() const {
    QRect view = viewportGL();
    return QRect(view.x(), height - (view.y() + view.height()), view.width(), view.height());
}


bool FboCubeSurface::checkFramebufferStatus(QOpenGLContext *ctx) const
{
    if (!ctx)
        return false;   // Context no longer exists.
    GLenum status = ctx->functions()->glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
    case GL_NO_ERROR:
    case GL_FRAMEBUFFER_COMPLETE:
        return true;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        qDebug("QOpenGLFramebufferObject: Unsupported framebuffer format.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete attachment.");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, missing attachment.");
        break;
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
    case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, duplicate attachment.");
        break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, attached images must have same dimensions.");
        break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_FORMATS
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, attached images must have same format.");
        break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, missing draw buffer.");
        break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, missing read buffer.");
        break;
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        qDebug("QOpenGLFramebufferObject: Framebuffer incomplete, attachments must have same number of samples per pixel.");
        break;
#endif
    default:
        qDebug() <<"QOpenGLFramebufferObject: An undefined error has occurred: "<< status;
        break;
    }
    return false;
}
