#ifndef CUBMAPSPHERE_H
#define CUBMAPSPHERE_H

#include "spherenode.h"
#include "fbocubesurface.h"

#include <QGLFramebufferObject>
#include <QGLFramebufferObjectSurface>
#include <QGLTextureCube>
#include <QGLCamera>
#include <QGLMaterial>

class FractalView;

class CubeMapSphereNode : public SphereNode
{
    Q_OBJECT

public:
    explicit CubeMapSphereNode(FractalView* view, float diameter, QObject* parent=0);
    virtual ~CubeMapSphereNode() { delete fboCubeSurface; }

    virtual void prepareLocations();
    virtual void draw(QGLPainter *painter);

private:
    QGLSceneNode* createCubeMapSphereNode();
    void updateCubeTexture(QGLPainter* painter);

    QGLCamera* innerCamera;
   // GLuint cubeTexID;

    int fbo_dim;  

    FboCubeSurface* fboCubeSurface;
    QGLMaterial* metal;
    int cameraPositionLocation;
    int cubeMapTexLocation;
    int reflectFactorLocation;
    int materialColourLocation;

    int again;
};

#endif // CUBMAPSPHERE_H
