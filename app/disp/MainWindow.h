//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_MAINWINDOW_H
#define H_APP_DISP_MAINWINDOW_H

#include <QMainWindow>

namespace app::disp {

    class MainWidget;

    class MainWindow : public QMainWindow {
        Q_OBJECT

      public:

        MainWindow();

      private:
        MainWidget* m_main_widget;
    };

} // namespace app::disp

#endif // H_APP_DISP_MAINWINDOW_H
