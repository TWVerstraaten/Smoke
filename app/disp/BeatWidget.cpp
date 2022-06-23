//
// Created by pc on 05-12-21.
//

#include "BeatWidget.h"

#include <QDebug>
#include <QTimer>

namespace app::disp {

    BeatWidget::BeatWidget(QWidget* parent) : QWidget(parent) {
        setFixedHeight(90);
    }

    void BeatWidget::paintEvent(QPaintEvent* event) {
        QPalette pal = QPalette();
        pal.setColor(QPalette::Window, QColor{static_cast<int>(m_intensity * 255.0f), static_cast<int>(m_intensity * 255.0f), static_cast<int>(m_intensity * 255.0f)});
        setAutoFillBackground(true);
        setPalette(pal);
        m_intensity *= 0.86;
        QWidget::paintEvent(event);
    }

    void BeatWidget::set_intensity(float intensity) {
        m_intensity = std::max(intensity, m_intensity);
    }

} // namespace app::disp
