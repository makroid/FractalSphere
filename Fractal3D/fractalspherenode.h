#ifndef FRACTALSPHERENODE_H
#define FRACTALSPHERENODE_H

#include "spherenode.h"

#include <QTime>
#include <QTimer>


class FractalView;
class QGLSceneNode;

class FractalSphereNode : public SphereNode
{
    Q_OBJECT

public:
    explicit FractalSphereNode(FractalView* view, float diameter=1, int color=1, QObject* parent=0);

    virtual void prepareLocations();
    virtual void draw(QGLPainter *painter);

public slots:
    void setNewCoffset();

private:
    QGLSceneNode* createFractalSphereNode(int color);

    // locations for glsl shader bindings
    int coffsetLocation;
    int iterLocation;
    int scaleXLocation;
    int scaleYLocation;
    int scaleZLocation;

    // timer
    QTimer* coffsetTimer;
    QVector2D coffset;
    QTime startTime;
};

#endif // FRACTALSPHERENODE_H
