//
// Created by pc on 05-12-21.
//

#include "AudioWidgetBase.h"

namespace app::disp {

    AudioWidgetBase::AudioWidgetBase(QWidget* parent) : QWidget(parent) {
        setFixedHeight(140);
        QPalette pal = QPalette();
        pal.setColor(QPalette::Window, QColor{158, 153, 175});
        setAutoFillBackground(true);
        setPalette(pal);
    }

    void AudioWidgetBase::set_bounds_type(AudioWidgetBase::BOUNDS_TYPE type) {
        m_type = type;
    }

    void AudioWidgetBase::set_draw_mode(AudioWidgetBase::DRAW_MODE draw_mode) {
        m_draw_mode = draw_mode;
    }
} // namespace app::disp