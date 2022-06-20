//
// Created by pc on 05-12-21.
//

#ifndef H_SRC_DISP_SPECTRUMWIDGET_H
#define H_SRC_DISP_SPECTRUMWIDGET_H

#include "../audio/LinearBuffer.h"
#include "../audio/Types.h"
#include "AudioWidgetBase.h"

#include <QWidget>

namespace app::disp {
    class SpectrumWidget : public AudioWidgetBase {
        Q_OBJECT

      public:
        explicit SpectrumWidget(QWidget* parent = nullptr);

        void set_buffer(const audio::DftBuffer& buffer);

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        audio::DftBuffer m_dft_buffer;
    };

} // namespace app::disp

#endif // H_SRC_DISP_SPECTRUMWIDGET_H
