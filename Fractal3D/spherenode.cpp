#include "spherenode.h"

SphereNode::SphereNode(FractalView* aview, float diameter, QObject* parent)
    : QGLSceneNode(parent),
      view(aview),
      radius(diameter/2),
      drawNode(true)
{
    scale = QVector3D(1.0f, 1.0f, 1.0f);
}

void SphereNode::draw(QGLPainter *painter) {
    QGLSceneNode::draw(painter);
}
