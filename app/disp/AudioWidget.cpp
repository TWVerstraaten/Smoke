//
// Created by pc on 08-11-21.
//

#include "AudioWidget.h"

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
          m_sound_window(&parent->m_sound_window),
          m_player(&parent->m_player),
          m_spectrum_widget(new SpectrumWidget(this)),
          m_wave_form_widget(new HistoryWidget(this, 200.0f, "Waveform")),
          m_energy_widget(new HistoryWidget(this, 0.001, "Energy")),
          m_energy_dif_widget(new HistoryWidget(this, 20.f, "d Energy")),
          m_beat_widget(new BeatWidget(this)),
          m_layout(new QFormLayout(this)) {

        m_wave_form_widget->set_bounds_type(HistoryWidget::BOUNDS_TYPE::POSITIVE_AND_NEGATIVE);

        m_layout->addRow(new QLabel{"Waveform", this}, m_wave_form_widget);
        m_layout->addRow(new QLabel{"Energy", this}, m_energy_widget);
        m_layout->addRow(new QLabel{"Energy Dif", this}, m_energy_dif_widget);
        m_layout->addRow(new QLabel{"Spectrum", this}, m_spectrum_widget);
        m_layout->addRow(new QLabel{"Energy Beat", this}, m_beat_widget);

        setLayout(m_layout);
        resize(800, 200);
    }

    void AudioWidget::update_widgets() {
        m_wave_form_widget->set_buffer(m_sound_window->waveform());
        m_spectrum_widget->set_buffer(m_sound_window->smoothed_dft_buffers().last_set());
        m_energy_widget->set_buffer(m_sound_window->energy_buffer());
        m_energy_dif_widget->set_buffer(m_sound_window->energy_dif_buffer());
        m_beat_widget->set_intensity(m_sound_window->current_beat_intensity());
        repaint();
    }

} // namespace app::disp