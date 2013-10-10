#ifndef FRACTAL3DVIEW_H
#define FRACTAL3DVIEW_H

#include <QGLView>
#include <QGLSceneNode>
#include <QMouseEvent>
#include <QRay3D>
#include <QGLLightModel>
#include <QGLLightParameters>

#include "selectedspherenode.h"

class SphereNode;
class FractalSphereNode;

class FractalView : public QGLView
{
    Q_OBJECT

public:
    FractalView(QWindow *parent = 0);
    ~FractalView() {}

    QGLPainter* getGLPainter() { return myPainter; }

    enum Mode {
        WatchMode  = 1,
        MoveMode   = 2,
        ScaleMode  = 3
    };

    QRay3D rayThruNearPlane(const QVector3D& pick) const;
    float distEyeToNode(const SphereNode* node) const;
    void render(QGLPainter* painter) { paintGL(painter); }


protected:
    void initializeGL(QGLPainter *painter);
    void paintGL(QGLPainter *painter);

    void keyPressEvent(QKeyEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    QGLSceneNode* sceneNode;
    QList<SphereNode* > nodes;

    QGLLightModel* lightModel;
    QGLLightParameters* lightParams;

    QGLPainter* myPainter;
    SelectedSphereNode* selectedNode;

    QColor selectionColor;
    Mode mode;
};

#endif // FRACTAL3DVIEW_H
