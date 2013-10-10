#include "cubemapspherenode.h"
#include "fractal3dview.h"

#include <QGLPainter>
#include <QVector3D>
#include <QGLBuilder>
#include <QGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QGLSphere>
#include <cmath>
#include <cstdlib>
#include <QUrl>

#include <QGLContext>

#include <QOpenGLContext>

CubeMapSphereNode::CubeMapSphereNode(FractalView* view, float diameter=1, QObject* parent)
    :SphereNode(view, diameter, parent) {
    addNode(createCubeMapSphereNode());
    again = 0;
}

QGLSceneNode* CubeMapSphereNode::createCubeMapSphereNode() {
    scale = QVector3D(1.0, 1.0, 1.0);
    QGLBuilder* builder = new QGLBuilder;
    *builder << QGL::Faceted;
    *builder << QGLSphere(2*radius);

    //QGLMaterial *mat = new QGLMaterial;
   // mat->setColor(QColor(170, 202, 0));
    //mat->setColor(QColor(0, 255, 0));
    //mat->setAmbientColor(Qt::darkGray);
    //mat->setDiffuseColor(Qt::darkGray);

//    QUrl url;
//    url.setPath(QLatin1String("/home/armin/Documents/alignment_ex.png"));
//    url.setScheme(QLatin1String("file"));
//    mat->setTextureUrl(url, 0);


    fbo_dim = 512;
    innerCamera = new QGLCamera(this);

    fboCubeSurface = new FboCubeSurface(fbo_dim, fbo_dim);

    effect = new QGLShaderProgramEffect();
    effect->setVertexShaderFromFile("cubemap.vert");
    effect->setFragmentShaderFromFile("cubemap.frag");

    metal = new QGLMaterial(this);
    metal->setAmbientColor(QColor(255, 192, 0));
    metal->setDiffuseColor(QColor(60, 60, 60));
    metal->setSpecularColor(QColor(255, 255, 255));
   // metal->setColor(Qt::blue);
    metal->setShininess(128);

    QGLSceneNode* sphereSceneNode = builder->finalizedSceneNode();
    //sphereSceneNode->setMaterial(mat);
    //sphereSceneNode->setEffect(QGL::FlatColor);
    sphereSceneNode->setUserEffect(effect);
    sphereSceneNode->setMaterial(metal);
    sphereSceneNode->setEffect(QGL::LitMaterial);

    qDebug() << "finished createCubeMapSphere";
    return sphereSceneNode;
}


void CubeMapSphereNode::prepareLocations() {
    cameraPositionLocation = effect->program()->uniformLocation("WorldCameraPosition");
    cubeMapTexLocation = effect->program()->uniformLocation("cubeMapTex");
    reflectFactorLocation = effect->program()->uniformLocation("ReflectFactor");
    materialColourLocation = effect->program()->uniformLocation("MaterialColor");

    effect->program()->setUniformValue(cameraPositionLocation, view->camera()->eye());
    //glActiveTexture(GL_TEXTURE0);
    effect->program()->setUniformValue(cubeMapTexLocation, 0);
    effect->program()->setUniformValue(reflectFactorLocation, 0.25f);
    effect->program()->setUniformValue(materialColourLocation, QVector4D(0.5f, 0.5f, 0.5f, 1.0f));

}

void CubeMapSphereNode::draw(QGLPainter *painter) {
    //effect->setActive(painter, true);
    //qDebug() << "calling CubeMapSphereNode::draw";

    updateCubeTexture(painter);

    effect->setActive(painter, true);

    fboCubeSurface->bindTexture();
    prepareLocations();    

    //painter->setColor(metal->diffuseColor());
    painter->setFaceMaterial(QGL::AllFaces, metal);
    //painter->setStandardEffect(QGL::LitMaterial);

    children().at(0)->draw(painter);
    fboCubeSurface->unbindTexture();
}


void CubeMapSphereNode::updateCubeTexture(QGLPainter* painter) {          
    this->setDrawNode(false);

    QGLCamera* cameraSave = view->camera();

    // setup camera
    innerCamera->setEye(this->position());
    innerCamera->setNearPlane(0.1);
    innerCamera->setFarPlane(10000);
    innerCamera->setFieldOfView(90);
    innerCamera->setAdjustForAspectRatio(false);

    // POSITIVE_X
    fboCubeSurface->begin(0);
    innerCamera->setCenter(innerCamera->eye() + QVector3D(-1,0,0));
    innerCamera->setUpVector(QVector3D(0,1,0));
    painter->setCamera(innerCamera);
    painter->pushSurface(fboCubeSurface);
    view->render(painter);
    painter->popSurface();
    fboCubeSurface->end();

    // NEGATIVE_X
    fboCubeSurface->begin(1);
    innerCamera->setCenter(innerCamera->eye() + QVector3D(1,0,0));
    innerCamera->setUpVector(QVector3D(0,1,0));
    painter->setCamera(innerCamera);
    painter->pushSurface(fboCubeSurface);
    view->render(painter);
    painter->popSurface();
    fboCubeSurface->end();

    // POSITIVE_Y
    fboCubeSurface->begin(2);
    innerCamera->setCenter(innerCamera->eye() + QVector3D(0,-1,0));
    innerCamera->setUpVector(QVector3D(0,0,1));
    painter->setCamera(innerCamera);
    painter->pushSurface(fboCubeSurface);
    view->render(painter);
    painter->popSurface();
    fboCubeSurface->end();

    // NEGATIVE_Y
    fboCubeSurface->begin(3);
    innerCamera->setCenter(innerCamera->eye() + QVector3D(0,1,0));
    innerCamera->setUpVector(QVector3D(0,0,-1));
    painter->setCamera(innerCamera);
    painter->pushSurface(fboCubeSurface);
    view->render(painter);
    painter->popSurface();
    fboCubeSurface->end();

    // POSITIVE_Z
    fboCubeSurface->begin(4);
    innerCamera->setCenter(this->position() + QVector3D(0,0,1));
    innerCamera->setUpVector(QVector3D(0,1,0));
    //innerCamera->rotateEye(QQuaternion(180, 1,0,0));
    painter->setCamera(innerCamera);
    painter->pushSurface(fboCubeSurface);
    view->render(painter);
    painter->popSurface();
    fboCubeSurface->end();

    // NEGATIVE_Z
    fboCubeSurface->begin(5);
    innerCamera->setCenter(this->position() + QVector3D(0,0,-1));
    innerCamera->setUpVector(QVector3D(0,1,0));
    painter->setCamera(innerCamera);
    painter->pushSurface(fboCubeSurface);
    view->render(painter);
    painter->popSurface();
    fboCubeSurface->end();

    this->setDrawNode(true);
    painter->setCamera(cameraSave);

}
