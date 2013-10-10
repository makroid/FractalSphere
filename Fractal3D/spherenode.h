#ifndef SPHERENODE_H
#define SPHERENODE_H

#include <QGLSceneNode>
#include <QVector3D>
#include <QGLShaderProgramEffect>

class FractalView;
class QGLPainter;

class SphereNode : public QGLSceneNode
{
    Q_OBJECT

public:
    explicit SphereNode(FractalView* view, float diameter=1, QObject* parent=0);

    float getRadius() const { return radius * scale.x(); }
    float getScaleX() const { return scale.x(); }
    float getScaleY() const { return scale.y(); }
    float getScaleZ() const { return scale.z(); }
    //void setScale(float s) { scale = s; }
    void multiplyScale(float f) { scale *= f; }
    void multiplyScaleX(float f) { scale.setX(scale.x() * f); }
    void multiplyScaleY(float f) { scale.setY(scale.y() * f); }
    void multiplyScaleZ(float f) { scale.setZ(scale.z() * f); }

    virtual void draw(QGLPainter *painter);
    virtual void prepareLocations() {}

    bool getDrawNode() const { return drawNode; }
    void setDrawNode(bool b) { drawNode = b; }

protected:   
    FractalView* view;
    float radius;
    bool drawNode;

    QVector3D scale;    
    QGLShaderProgramEffect* effect;
};

#endif // SPHERENODE_H
