#include "app/MainWidget.h"
#include "app/Math.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <iostream>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("smoke");
    app::disp::MainWidget widget;
    widget.show();
    return app.exec();
}
