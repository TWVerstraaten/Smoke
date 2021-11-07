//
// Created by pc on 03-11-21.
//

#ifndef H_APP_FL_MATRIX_H
#define H_APP_FL_MATRIX_H

#include "Settings.h"

#include <array>

namespace app::fl {

    class Matrix {

      public:
        template <class T>
        struct Row {
            T& operator[](size_t index) const {
                return *(m_p + index);
            }
            T* m_p;
        };
        Row<float>       operator[](size_t row);
        Row<const float> operator[](size_t row) const;

        void set_zero();

      private:
        std::array<float, g_point_count * g_point_count> m_array{};
    };

} // namespace app

#endif // H_APP_FL_MATRIX_H