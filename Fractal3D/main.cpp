#include "mainwindow.h"
#include <QApplication>

#include "fractal3dview.h"
#include "windowwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FractalView* fv = new FractalView;
    WindowWidget* ww = new WindowWidget(fv);
    ww->resize(1000, 800);
    ww->show();



    return app.exec();
}
