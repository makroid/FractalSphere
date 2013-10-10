#include "fractalspherenode.h"
#include "fractal3dview.h"

#include <QVector3D>
#include <QGLBuilder>
#include <QGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QGLTexture2D>
#include <QVector2D>
#include <QGLSphere>
#include <cmath>
#include <cstdlib>

FractalSphereNode::FractalSphereNode(FractalView* aview, float diameter, int color, QObject* parent)
       : SphereNode(aview, diameter, parent)
{
    addNode(createFractalSphereNode(color));
}

QGLSceneNode* FractalSphereNode::createFractalSphereNode(int color) {
    scale = QVector3D(1.0, 1.0, 1.0);
    QGLBuilder* builder = new QGLBuilder;
    *builder << QGL::Faceted;
    *builder << QGLSphere(2*radius);

    // add shader
    effect = new QGLShaderProgramEffect();
    effect->setVertexShaderFromFile("julia.vert");
    effect->setFragmentShaderFromFile("julia.frag");

    // load and set texture
    QUrl url;
    char* pathToTexture = "/home/armin/source/fractal3D/Fractal3D/pal.ppm";
    if (color==2) {
        pathToTexture = "/home/armin/source/fractal3D/Fractal3D/pal1.ppm";
    }
    url.setPath(QLatin1String(pathToTexture));
    url.setScheme(QLatin1String("file"));
    QGLMaterial* mat1 = new QGLMaterial;
    mat1->setTextureUrl(url, 0);

    int matSphere = builder->sceneNode()->palette()->addMaterial(mat1);
    builder->currentNode()->setMaterialIndex(matSphere);

    // finalize node
    QGLSceneNode* sphereSceneNode = builder->finalizedSceneNode();
    sphereSceneNode->setUserEffect(effect);

    coffsetTimer = new QTimer(this);
    connect(coffsetTimer, SIGNAL(timeout()), this, SLOT(setNewCoffset()));
    coffsetTimer->start(150);
    startTime = QTime::currentTime();
    int shift = qrand() % 100000;
    startTime = startTime.addMSecs(shift);

    return sphereSceneNode;
}


void FractalSphereNode::prepareLocations() {
    // prepare locations
    //qDebug() << "effect=" << effect;
    //qDebug() << "program=" << effect->program();
    // TODO: move this to initialize or constructor?
    coffsetLocation = effect->program()->uniformLocation("c");
    iterLocation = effect->program()->uniformLocation("iter");
    scaleXLocation = effect->program()->uniformLocation("scaleX");
    scaleYLocation = effect->program()->uniformLocation("scaleY");
    scaleZLocation = effect->program()->uniformLocation("scaleZ");

    effect->program()->setUniformValue(coffsetLocation, QVector2D(coffset.x(), coffset.y()));
    effect->program()->setUniformValue(iterLocation, 40);
    effect->program()->setUniformValue(scaleXLocation, scale.x());
    effect->program()->setUniformValue(scaleYLocation, scale.y());
    effect->program()->setUniformValue(scaleZLocation, scale.z());
}


void FractalSphereNode::draw(QGLPainter *painter) {
    effect->setActive(painter, true);
    prepareLocations();    
    QGLSceneNode::draw(painter);
}


void FractalSphereNode::setNewCoffset() {
    float t = startTime.msecsTo(QTime::currentTime()) / 3000.0;
    coffset.setX( (sin(cos(t / 10) * 10) + cos(t * 2.0) / 4.0 + sin(t * 3.0) / 6.0) * 0.8 );
    coffset.setY( (cos(sin(t / 10) * 10) + sin(t * 2.0) / 4.0 + cos(t * 3.0) / 6.0) * 0.8 );
    //effect->program()->setUniformValue(coffsetLocation, coffset);
    view->update();
}
