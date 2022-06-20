//
// Created by pc on 05-12-21.
//

#ifndef H_APP_DISP_AUDIOWIDGETBASE_H
#define H_APP_DISP_AUDIOWIDGETBASE_H

#include <QWidget>

namespace app::disp {

    class AudioWidgetBase : public QWidget {
        Q_OBJECT

      public:
        enum class BOUNDS_TYPE { NON_NEGATIVE, POSITIVE_AND_NEGATIVE };
        enum class DRAW_MODE { BARS, LINE };

        explicit AudioWidgetBase(QWidget* parent);

        void set_bounds_type(BOUNDS_TYPE type);
        void set_draw_mode(DRAW_MODE draw_mode);

      protected:
        BOUNDS_TYPE m_type      = BOUNDS_TYPE::NON_NEGATIVE;
        DRAW_MODE   m_draw_mode = DRAW_MODE::BARS;
    };

} // namespace app::disp

#endif // H_APP_DISP_AUDIOWIDGETBASE_H
