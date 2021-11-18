//
// Created by pc on 11-11-21.
//

#ifndef H_APP_DISP_SETTINGSWIDGET_H
#define H_APP_DISP_SETTINGSWIDGET_H

#include <QComboBox>
#include <QFormLayout>
#include <QMap>
#include <QVBoxLayout>
#include <QWidget>
#include <climits>

namespace app::disp {

    class SettingsWidget : public QWidget {

        Q_OBJECT

      public:
        SettingsWidget();

        void add_section(const QString& name);
        void add(const QString& name, size_t& value, int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max());
        void add(const QString& name, float& value, float min = std::numeric_limits<float>::lowest(), float max = std::numeric_limits<float>::max());
        void add(const QString& name, bool& value);
        void add(const QString& name, const std::function<void()>& call_back);
        template <class Enum>
        void add(const QString& name, Enum& the_enum, const QMap<QString, Enum> names) {
            auto* combo_box = new QComboBox(this);
            for (auto it = names.begin(); it != names.end(); ++it) {
                combo_box->addItem(it.key());
            }
            connect(combo_box, QOverload<const QString&>::of(&QComboBox::activated), [&, names](const QString& selected) { the_enum = names[selected]; });
            m_current_box_layout->addRow(name, combo_box);
        }

      private:
        QVBoxLayout* m_layout;
        QFormLayout* m_current_box_layout = nullptr;
    };

} // namespace app::disp

#endif // H_APP_DISP_SETTINGSWIDGET_H
