//
// Created by pc on 08-11-21.
//

#ifndef H_APP_DISP_MAINWINDOW_H
#define H_APP_DISP_MAINWINDOW_H

#include <QMainWindow>

namespace app::disp {

    class SettingsWidget;
    class SmokeWidget;

    class MainWindow : public QMainWindow {
        Q_OBJECT

      public:
        MainWindow();

        void keyPressEvent(QKeyEvent* e) override;

      protected:
        void closeEvent(QCloseEvent* event) override;

      private:
        SmokeWidget*    m_smoke_widget;
        SettingsWidget* m_settings_widget;
    };

} // namespace app::disp

#endif // H_APP_DISP_MAINWINDOW_H
