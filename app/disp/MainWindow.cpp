//
// Created by pc on 08-11-21.
//

#include "MainWindow.h"

#include "MainWidget.h"

namespace app::disp {

    MainWindow::MainWindow() : m_main_widget(new MainWidget(this)) {
        setCentralWidget(m_main_widget);

        resize(1000, 1000);
    }

} // namespace app::disp