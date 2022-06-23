//
// Created by pc on 05-12-21.
//

#ifndef H_APP_DISP_SPECTRUMWIDGET_H
#define H_APP_DISP_SPECTRUMWIDGET_H

#include "../type/BufferTypes.h"
#include "../type/LinearBuffer.h"
#include "AudioWidgetBase.h"

#include <QWidget>

namespace app::disp {
    class SpectrumWidget : public AudioWidgetBase {
        Q_OBJECT

      public:
        explicit SpectrumWidget(QWidget* parent = nullptr);

        void set_buffer(const type::DftBuffer& buffer);

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        type::DftBuffer m_dft_buffer;
    };

} // namespace app::disp

#endif // H_APP_DISP_SPECTRUMWIDGET_H
