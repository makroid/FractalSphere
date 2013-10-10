#include "fractal3dview.h"
#include "fractalspherenode.h"
#include "cubemapspherenode.h"

#include <QList>
#include <QSphere3D>
#include <QOpenGLFramebufferObject>
#include <QtOpenGL/qglfunctions.h>
#include <QGLContext>

FractalView::FractalView(QWindow *parent)
    : QGLView(parent)
{
    qsrand(QTime::currentTime().msec());

//    nodes.append(new FractalSphereNode(this, 2, 2));
//    nodes.last()->setObjectName("node 2");
//    nodes.last()->setPosition(QVector3D(0.5, 0.5, -1.5));

//    nodes.append(new FractalSphereNode(this, 1.5, 2));
//    nodes.last()->setObjectName("node 3");
//    nodes.last()->setPosition(QVector3D(1.5, 1.5, -1.5));
   // nodes[0]->setParent(this);

    camera()->setFarPlane(10000.0);
    camera()->setNearPlane(1.0);
    camera()->setProjectionType(QGLCamera::Perspective);
    camera()->setCenter(QVector3D(0.0, 0.0, 0.0));
    //camera()->setEye(QVector3D(-1.41218, -6.97675, 7.02358));
    //camera()->setEye(QVector3D(6.0, 6.0, 6.0));
    camera()->setEye(QVector3D(0.0, 0.0, -10.0));
    camera()->setUpVector(QVector3D::crossProduct(camera()->eye()-camera()->center(), QVector3D(1,0,0)).normalized());
    camera()->setFieldOfView(45.0);

    selectionColor = QColor(255, 0, 0, 255);

    lightModel = new QGLLightModel(this);
    lightModel->setAmbientSceneColor(Qt::white);
    //lightModel->setViewerPosition(QGLLightModel::LocalViewer);

    lightParams = new QGLLightParameters(this);
    lightParams->setPosition(QVector3D(0.0f, 0.0f, -10.0f));
    lightParams->setDirection(QVector3D(-2,0,10));
    lightParams->setAmbientColor(Qt::white);
}


void FractalView::initializeGL(QGLPainter *painter) {
     //painter->setClearColor(Qt::gray);
     glEnable(GL_BLEND);
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glEnable(GL_LIGHT0);

     const QGLContext* ctxt = QGLContext::currentContext();
     qDebug() << "initializeGL context=" << ctxt;
    CubeMapSphereNode* cmsn = new CubeMapSphereNode(this, 1);
    nodes.append(cmsn);
    nodes.last()->setPosition(QVector3D(0.0,0.0, 0));
    nodes.append(new FractalSphereNode(this, 1));
    nodes.last()->setObjectName("node 1");
    nodes.last()->setPosition(QVector3D(-2,0,-3));

//    nodes.append(new FractalSphereNode(this, 1));
//    nodes.last()->setObjectName("node 2");
//    nodes.last()->setPosition(QVector3D(-1,0.5,0));

    qDebug() << "node->position=" << nodes.last()->position();
    selectedNode = new SelectedSphereNode();
    mode = WatchMode;

    myPainter = painter;
   // QGLLightParameters* mainLightParameters = painter->mainLight();
    painter->setMainLight(lightParams);
}


void FractalView::paintGL(QGLPainter *painter) {
    painter->setLightModel(lightModel);
    painter->setStandardEffect(QGL::LitMaterial);
    painter->setMainLight(lightParams);
    for(int i=0; i<nodes.size(); ++i) {
        if ( ! nodes[i]->getDrawNode()) continue;
        if (nodes[i] == selectedNode->getNode()) {
            selectionColor.setAlpha(150);
            painter->setColor(selectionColor);
        }
        nodes[i]->draw(painter);
        if (nodes[i] == selectedNode->getNode()) {
            selectionColor.setAlpha(255);
            painter->setColor(selectionColor);
        }
    }
}


void FractalView::keyPressEvent(QKeyEvent *e) {
    qDebug() << "eye=" << camera()->eye();
    switch(e->key()) {
    case Qt::Key_N:
        nodes.append(new FractalSphereNode(this, 1));
        selectedNode->setNode(nodes.last());
        selectedNode->setDist(distEyeToNode(selectedNode->getNode()));
        mode = MoveMode;
        //nodes.last()->setPosition(pick);
        break;
    case Qt::Key_B:
        nodes.append(new FractalSphereNode(this, 1, 2));
        selectedNode->setNode(nodes.last());
        nodes.last()->setPosition(QVector3D(2, 0, -3));
        selectedNode->setDist(distEyeToNode(selectedNode->getNode()));
        mode = MoveMode;
        //nodes.last()->setPosition(pick);
        break;
    case Qt::Key_M:
        if (selectedNode->valid()) {
            selectedNode->setDist(distEyeToNode(selectedNode->getNode()));
            qDebug() << "selected Node distance=" << selectedNode->getDist();
            mode = MoveMode;
        }
        break;
    case Qt::Key_Plus:
        if (selectedNode->valid()) {
            mode = ScaleMode;
            selectedNode->getNode()->multiplyScale(1.2);
            update();
        }
        break;
    case Qt::Key_Minus:
        if (selectedNode->valid()) {
            mode = ScaleMode;
            selectedNode->getNode()->multiplyScale(0.8);
            update();
        }
        break;

    case Qt::Key_X:
        if (selectedNode->valid()) {
            mode = ScaleMode;
            selectedNode->getNode()->multiplyScaleX(1.2);
            update();
        }
        break;

    case Qt::Key_Y:
        if (selectedNode->valid()) {
            mode = ScaleMode;
            selectedNode->getNode()->multiplyScaleY(1.2);
            update();
        }
        break;
    }
}


float FractalView::distEyeToNode(const SphereNode* node) const {
    return (camera()->eye() - node->position()).length();
}

QRay3D FractalView::rayThruNearPlane(const QVector3D& pick) const {
    // find the origin of the near plane
    QRay3D eyeline(camera()->eye(), (camera()->center() - camera()->eye()).normalized());
    QVector3D nearPlaneOrigin = eyeline.point(camera()->nearPlane());

    // from the near plane origin move up and across by the pick's XY to find the point
    // on the near plane
    QRay3D up(nearPlaneOrigin, camera()->upVector());
    QVector3D sideDir = QVector3D::crossProduct(camera()->upVector(), -eyeline.direction());
    QRay3D side(up.point(pick.y()), sideDir.normalized());
    QVector3D v = side.point(pick.x());

    // intersect the ray thru the picked point on the near plane with the floor
    QRay3D ray(camera()->eye(), (v - camera()->eye()).normalized());
    return ray;
}


void FractalView::mouseMoveEvent(QMouseEvent *e) {

    if (selectedNode->valid()) {
        if (mode == MoveMode) {
            QVector3D pick = mapPoint(e->pos());
            QRay3D ray = rayThruNearPlane(pick);
            qDebug() << "ray.direction=" << ray.direction();

            QVector3D movePosition = ray.point(selectedNode->getDist());
            selectedNode->getNode()->setPosition(movePosition);
            qDebug() << "selected node position=" << movePosition;
            qDebug() << "selected node distance=" << selectedNode->getDist();
            qDebug() << "selected node real dist=" << distEyeToNode(selectedNode->getNode());
            update();
        }
    } else {
        QGLView::mouseMoveEvent(e);
    }
}


void FractalView::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::RightButton) {
        mode = WatchMode;
        selectedNode->setInvalid();
    }
    else {
        QGLView::mousePressEvent(e);
    }
}


void FractalView::mouseDoubleClickEvent(QMouseEvent *e) {
    QVector3D pick = mapPoint(e->pos());

    QRay3D ray = rayThruNearPlane(pick);
    bool intersection = false;
    double minDist = 99999;
    int minDistIdx;
    for (int i=0; i<nodes.size(); ++i) {
        QSphere3D s3d(nodes[i]->position(), nodes[i]->getRadius());
        if (s3d.intersects(ray)) {
            qreal dist = (camera()->eye() - ray.point(s3d.intersection(ray))).length();
            if (dist < minDist) {
                minDist = dist;
                minDistIdx = i;
            }
            intersection = true;
        }
    }
    if ( ! intersection) {
        qDebug() << "no intersection!";
        selectedNode->setInvalid();
    } else {
        selectedNode->setNode(nodes[minDistIdx]);
        qDebug() << "Intersection at distance: " << minDist;
        qDebug() << "Position of intersecting node: " << nodes[minDistIdx]->position();
    }
}
