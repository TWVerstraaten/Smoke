//
// Created by pc on 05-12-21.
//

#ifndef H_APP_DISP_BEATWIDGET_H
#define H_APP_DISP_BEATWIDGET_H

#include <QWidget>

namespace app::disp {

    class BeatWidget : public QWidget {
        Q_OBJECT

      public:
        explicit BeatWidget(QWidget* parent);

        void set_intensity(float intensity);

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        float m_intensity = 0.0f;
    };

} // namespace app::disp

#endif // H_APP_DISP_BEATWIDGET_H
