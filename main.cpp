#include "app/disp/MainWidget.h"
#include "app/disp/MainWindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("smoke");
    app::disp::MainWindow widget;
    widget.show();
    return app.exec();
}
