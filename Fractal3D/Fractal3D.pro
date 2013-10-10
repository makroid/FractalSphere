#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T20:22:42
#
#-------------------------------------------------

QT       += core gui 3d opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fractal3D
TEMPLATE = app


SOURCES += main.cpp \
    fractalspherenode.cpp \
    fractal3dview.cpp \
    selectedspherenode.cpp \
    windowwidget.cpp \
    settingswidget.cpp \
    spherenode.cpp \
    cubemapspherenode.cpp \
    fbocubesurface.cpp

HEADERS  += \
    fractalspherenode.h \
    fractal3dview.h \
    selectedspherenode.h \
    windowwidget.h \
    settingswidget.h \
    spherenode.h \
    cubemapspherenode.h \
    fbocubesurface.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../Fractal3D-build-Desktop-Debug/julia.frag \
    ../Fractal3D-build-Desktop-Debug/julia.vert \
    ../Fractal3D-build-Desktop-Debug/cubemap.vert \
    ../Fractal3D-build-Desktop-Debug/cubemap.frag
