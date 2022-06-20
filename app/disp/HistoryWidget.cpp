//
// Created by pc on 05-12-21.
//

#include "HistoryWidget.h"

#include <QPainter>
#include <iostream>

namespace app::disp {

    HistoryWidget::HistoryWidget(QWidget* parent, float scale, QString name) : AudioWidgetBase(parent), m_scale(scale), m_name(std::move(name)) {
    }

    void HistoryWidget::set_buffer(const audio::HistoryBuffer& buffer) {
        m_history_buffer = buffer;
        for (size_t i = 0; i != audio::HistoryBuffer::size; ++i)
            m_history_buffer[i] *= m_scale;
        repaint();
    }

    void HistoryWidget::paintEvent([[maybe_unused]] QPaintEvent* event) {
        QPainter painter(this);
        painter.setPen(QPen{QColor{100, 100, 100}});
        painter.drawLine(0, QWidget::height() / 2, QWidget::width(), QWidget::height() / 2);
        if (m_draw_mode == AudioWidgetBase::DRAW_MODE::BARS)
            drawAsBars(painter);
        else
            drawAsLines(painter);
        drawScale(painter);
        QWidget::paintEvent(event);
    }

    void HistoryWidget::drawAsBars(QPainter& painter) {
        const int width           = QWidget::width();
        const int height          = QWidget::height();
        const int buffer_length   = audio::HistoryBuffer::size;
        const int rectangle_width = (width + buffer_length - 1) / buffer_length;

        if (m_type == BOUNDS_TYPE::NON_NEGATIVE)
            for (int i = 0; i != buffer_length; ++i)
                painter.fillRect(i * width / (buffer_length + 1),
                                 height - static_cast<int>(m_history_buffer[i]),
                                 rectangle_width,
                                 static_cast<int>(m_history_buffer[i]),
                                 QBrush{QColor{0, 32, 128}});
        else
            for (int i = 0; i != buffer_length; ++i)
                painter.fillRect(i * width / (buffer_length + 1),
                                 height / 2 - static_cast<int>(m_history_buffer[i]),
                                 rectangle_width,
                                 static_cast<int>(m_history_buffer[i]),
                                 QBrush{QColor{0, 32, 128}});
    }

    void HistoryWidget::drawAsLines(QPainter& painter) {
        const int height        = QWidget::height();
        const int width         = QWidget::width();
        const int buffer_length = audio::HistoryBuffer ::size;

        if (m_type == BOUNDS_TYPE::NON_NEGATIVE)
            for (int i = 1; i != buffer_length; ++i)
                painter.drawLine(
                    (i - 1) * width / buffer_length, height - static_cast<int>(m_history_buffer[i - 1]), i * width / buffer_length, height - static_cast<int>(m_history_buffer[i]));
        else
            for (int i = 1; i != buffer_length; ++i)
                painter.drawLine((i - 1) * width / buffer_length,
                                 height / 2 - static_cast<int>(m_history_buffer[i - 1]),
                                 i * width / buffer_length,
                                 height / 2 - static_cast<int>(m_history_buffer[i]));
    }
    void HistoryWidget::drawScale(QPainter& painter) {
        painter.setPen(QPen{QColor{255, 255, 255}});
        if (m_type == AudioWidgetBase::BOUNDS_TYPE::NON_NEGATIVE) {
            painter.drawText(0, QWidget::height() - 5, "0");
            painter.drawText(0, 15, QString::number(QWidget::height() / m_scale));
        }
    }
} // namespace app::disp