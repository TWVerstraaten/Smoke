//
// Created by pc on 05-12-21.
//

#include "SpectrumWidget.h"

#include <QDebug>
#include <QLine>
#include <QPainter>

namespace app::disp {

    SpectrumWidget::SpectrumWidget(QWidget* parent) : AudioWidgetBase(parent) {
    }

    void SpectrumWidget::set_buffer(const type::DftBuffer& buffer) {
        m_dft_buffer = buffer;
        repaint();
    }

    void SpectrumWidget::paintEvent(QPaintEvent* event) {
        const int width           = QWidget::width();
        const int height          = QWidget::height();
        const int buffer_length   = type::DftBuffer::s_size;
        const int rectangle_width = (width + buffer_length - 1) / buffer_length;
        QPainter  painter(this);

        QColor color{22, 42, 68};
        painter.setPen(color);
        for (size_t i = 1; i != buffer_length; ++i) {
            int norm_value = static_cast<int>(m_dft_buffer[i]);
            painter.fillRect(i * width / (buffer_length + 1), height - norm_value, rectangle_width, norm_value, QBrush{QColor{0, 32, 128}});
        }
        QWidget::paintEvent(event);
    }

} // namespace app::disp
