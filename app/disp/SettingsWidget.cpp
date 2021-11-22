//
// Created by pc on 11-11-21.
//

#include "SettingsWidget.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>
#include <iostream>

namespace app::disp {

    SettingsWidget::SettingsWidget() {
        m_layout = new QVBoxLayout(this);
        setLayout(m_layout);
    }

    void SettingsWidget::add_section(const QString& name) {
        auto* group_box      = new QGroupBox(name, this);
        m_current_box_layout = new QFormLayout(group_box);
        group_box->setLayout(m_current_box_layout);
        m_layout->addWidget(group_box);
    }

    void SettingsWidget::add(const QString& name, size_t& value, size_t min, size_t max, const std::function<void(size_t)>& call_back) {
        auto* spin_box = new QSpinBox(this);
        spin_box->setValue(static_cast<int>(value));
        spin_box->setMinimum(static_cast<int>(min));
        spin_box->setMaximum(static_cast<int>(max));
        connect(spin_box, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&, call_back](int v) {
            value = v;
            call_back(value);
        });
        m_current_box_layout->addRow(name, spin_box);
    }

    void SettingsWidget::add(const QString& name, bool& value) {
        auto* check_box = new QCheckBox(this);
        check_box->setChecked(value);
        connect(check_box, &QCheckBox::toggled, [&](bool b) { value = b; });
        m_current_box_layout->addRow(name, check_box);
    }

    void SettingsWidget::add(const QString& name, float& value, float min, float max, bool as_slider) {
        if (as_slider) {
            std::cout << min << '\t' << max << "\n";
            auto* slider = new QSlider(Qt::Horizontal, this);
            slider->setMinimum(0);
            slider->setMaximum(100);
            slider->setValue(static_cast<int>(100.0f * (value - min) / (max - min)));
            connect(slider, &QSlider::valueChanged, [&, min, max](int i) { value = min + (max - min) * static_cast<float>(i) / 100.0f; });
            m_current_box_layout->addRow(name, slider);
        } else {
            auto* double_spin_box = new QDoubleSpinBox(this);
            double_spin_box->setDecimals(6);
            double_spin_box->setMinimum(min);
            double_spin_box->setMaximum(max);
            double_spin_box->setValue(value);
            connect(double_spin_box, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double d) { value = static_cast<float>(d); });
            m_current_box_layout->addRow(name, double_spin_box);
        }
    }

    void SettingsWidget::add(const QString& name, const std::function<void()>& call_back) {
        auto* push_button = new QPushButton("GO " + name, this);
        connect(push_button, &QPushButton::pressed, call_back);
        m_current_box_layout->addRow(name, push_button);
    }

} // namespace app::disp