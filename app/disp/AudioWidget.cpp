//
// Created by pc on 08-11-21.
//

#include "AudioWidget.h"

#include "../audio/SoundWindow.h"
#include "BeatWidget.h"
#include "HistoryWidget.h"
#include "MainWindow.h"
#include "SpectrumWidget.h"

#include <QDebug>
#include <QKeyEvent>
#include <QLabel>

namespace app::disp {

    AudioWidget::AudioWidget(MainWindow* parent)
        : QWidget(nullptr), // This is a floating widget
          m_sound_window(parent->m_sound_window.get()),
          m_spectrum_widget(new SpectrumWidget(this)),
          m_wave_form_widget(new HistoryWidget(this, 1.0f, "Waveform")),
          m_low_beat_widget(new HistoryWidget(this, 50.0f, "low beat")),
          m_mid_beat_widget(new HistoryWidget(this, 50.0f, "mid beat")),
          m_high_beat_widget(new HistoryWidget(this, 50.0f, "high beat")),
          m_layout(new QFormLayout(this)) {

        m_wave_form_widget->set_bounds_type(HistoryWidget::BOUNDS_TYPE::POSITIVE_AND_NEGATIVE);

        m_layout->addRow(new QLabel{"Waveform", this}, m_wave_form_widget);
        m_layout->addRow(new QLabel{"low beat", this}, m_low_beat_widget);
        m_layout->addRow(new QLabel{"mid beat", this}, m_mid_beat_widget);
        m_layout->addRow(new QLabel{"high beat", this}, m_high_beat_widget);
        m_layout->addRow(new QLabel{"Spectrum", this}, m_spectrum_widget);

        setLayout(m_layout);
        resize(800, 200);
    }

    void AudioWidget::update_widgets() {
        m_wave_form_widget->set_buffer(m_sound_window->waveform());
        m_spectrum_widget->set_buffer(m_sound_window->dft_buffers().last_set());
        m_low_beat_widget->set_buffer(m_sound_window->low_beat_buffer());
        m_mid_beat_widget->set_buffer(m_sound_window->mid_beat_buffer());
        m_high_beat_widget->set_buffer(m_sound_window->high_beat_buffer());
        repaint();
    }

} // namespace app::disp