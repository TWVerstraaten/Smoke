//
// Created by pc on 05-12-21.
//

#ifndef H_APP_DISP_HISTORYWIDGET_H
#define H_APP_DISP_HISTORYWIDGET_H

#include "../type/BufferTypes.h"
#include "../type/CircularBuffer.h"
#include "AudioWidgetBase.h"

#include <QWidget>

namespace app::disp {

    class HistoryWidget : public AudioWidgetBase {
        Q_OBJECT

      public:
        explicit HistoryWidget(QWidget* parent, float scale, QString name);

        void set_buffer(const type::HistoryBuffer& buffer);

      protected:
        void paintEvent(QPaintEvent* event) override;
        void draw_as_bars(QPainter& painter);
        void draw_as_lines(QPainter& painter);
        void draw_scale(QPainter& painter);

      private:
        float               m_scale;
        QString             m_name;
        type::HistoryBuffer m_history_buffer;
    };

} // namespace app::disp

#endif // H_APP_DISP_HISTORYWIDGET_H
